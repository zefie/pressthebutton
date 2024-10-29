#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <esp32-hal-psram.h>
#include "pins_arduino.h"
#include "functions.h"
#include "zlcd.h"
#include "esp_err.h"
#include "esp_spiffs.h"
#include "esp_log.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "tuneplayer.h"
#include "tunes/terranigma.h"
#include "tunes/thriller.h"
#include "tunes/gamestart.h"
#include "tunes/spinner_tune.h"
#include "tunes/spinner_stop.h"
#include "tunes/spinner_stop_oopsie.h"
#include "spinner.h"
#include "config.h"
#include "game.h"

SPIClass spiDMA(HSPI);
zlcd tft = zlcd(&spiDMA, SS, LCD_DC, LCD_RST, LCD_BL);
Adafruit_NeoPixel np = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
Tune spin_tune_cb;
bool gameover = false;
bool pattern_mode = false;
bool classic_mode = false;
bool buttonwarn = false;
std::string score_str;
std::string highscore_str;
int game_mode = 0; // gets updated at bootup
int res = -1;
Spinner spinner;    
SpinnerResult result;
Buzzer buzzer(BUZZER_PIN);
TunePlayer player(buzzer);


std::atomic<bool> spinner_active(false); // Flag to control the thread

void checkSPIFFSUsage() {
    size_t total = 0, used = 0;
    esp_err_t ret = esp_spiffs_info(data_partition, &total, &used);
    if (ret != ESP_OK) {
        printf("FFS: Failed to get SPIFFS partition information (%s)\n", esp_err_to_name(ret));
    }
    debug("FFS: Total space: %d bytes", total);
    debug("FFS: Used space: %d bytes", used);
    debug("FFS: Free space: %d bytes", total - used);
}

esp_err_t format_spiffs(esp_vfs_spiffs_conf_t *conf) {
    tft.lcdezstr(-1, 40,"|1Formatting", nullptr, 2);
    tft.lcdezstr(-1, 56,"|1Storage", nullptr, 2);
    tft.drawLine(2, 74, 126, 74, ST77XX_WHITE);
    tft.lcdezstr(-1, 80,"This may take a");
    tft.lcdezstr(-1, 88,"moment. Please");
    tft.lcdezstr(-1, 96,"be patient!");
    conf->format_if_mount_failed = true;
    return esp_vfs_spiffs_register(conf);
}

void init_spiffs() {
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/data",
        .partition_label = data_partition,
        .max_files = 5,
        .format_if_mount_failed = false
    };
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            debug("FFS: Failed to mount, attemping to format filesystem");
            ret = format_spiffs(&conf);
            if (ret != ESP_OK) {
                debug("FFS: Failed to format filesystem");
                return;
            } else {
                debug("FFS: Formatted successfully.");
            }
         } else if (ret == ESP_ERR_NOT_FOUND) {
            debug("FFS: Failed to find SPIFFS partition");
         } else {
            debug("FFS: Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
            return;
         }
    }
     // Get filesystem information
    checkSPIFFSUsage();
}



void memInfo(uint32_t cap) {
    size_t freemem = heap_caps_get_free_size(cap);
    size_t totalmem = heap_caps_get_total_size(cap);
    size_t usedmem = (totalmem - freemem);
    char ramtype[9];
    switch (cap) {
        case MALLOC_CAP_8BIT:
            strcpy(ramtype, "  Total");
            break;
        case MALLOC_CAP_DMA:
            strcpy(ramtype, "    DMA");
            break;
        case MALLOC_CAP_SPIRAM:
            strcpy(ramtype, "  PSRAM");
            break;
        case MALLOC_CAP_INTERNAL:
            strcpy(ramtype, "    SYS");
            break;
        default:
            strcpy(ramtype, "    ???");
            break;
    }
    debug("RAM: %s: %u/%u bytes (%.2f%%)", ramtype, usedmem, totalmem, ((float)usedmem / totalmem) * 100);
}



void init_sys() {
    setCpuFrequencyMhz(240); // Set CPU frequency to 240MHz
    debug("SYS: %s running at %d MHz", ESP.getChipModel(), getCpuFrequencyMhz());
    debug("RAM: %u bytes free", heap_caps_get_free_size(MALLOC_CAP_8BIT));      
    debug("FSH: %u bytes", ESP.getFlashChipSize());
    memInfo(MALLOC_CAP_INTERNAL);
    memInfo(MALLOC_CAP_SPIRAM);
    memInfo(MALLOC_CAP_8BIT);
}

void init_lcd() {
    // Initialize the display  
    spiDMA.begin(SCK, -1, MOSI, SS);
    tft.setSPISpeed(40000000);
    debug("SPI: DMA initialized.");
    tft.initR(INITR_BLACKTAB);
    tft.setRotation(0);
    tft.fillScreen(ST7735_BLACK);
    debug("LCD: Initialized.");
    tft.setBacklight(true);
}

#include <stdio.h>
#include <stdbool.h>

uint32_t loadHiScore(bool classic_mode, bool pattern_mode) {
    FILE *file;
    const char *filename;
    uint32_t score = 0;

    // Determine the correct filename based on the mode flags
    if (classic_mode) {
        if (pattern_mode) {
            filename = "/data/highscore_classic_patterns.dat";
        } else {
            filename = "/data/highscore_classic_rng.dat";
        }
    } else {
        if (pattern_mode) {
            filename = "/data/highscore_patterns.dat";
        } else {
            filename = "/data/highscore_rng.dat";
        }
    }

    // Open the file in read mode
    file = fopen(filename, "r");
    if (file == NULL) {
        // Handle the error appropriately
        perror("No high score data exists for this game mode");
        return 0; // Return 0 or an appropriate error code/value
    }

    // Read the score from the file
    if (fscanf(file, "%i", &score) != 1) {
        // Handle the error if reading fails
        perror("Error reading high score from saved data");
        score = 0; // Reset score to 0 or handle as needed
    }

    // Close the file
    fclose(file);
    return score;
}

void saveHiScore(uint32_t score, bool classic_mode, bool pattern_mode) {
    FILE *file;
    const char *filename;

    if (classic_mode) {
        if (pattern_mode) {
            filename = "/data/highscore_classic_patterns.dat";
        } else {
            filename = "/data/highscore_classic_rng.dat";
        }
    } else {
        if (pattern_mode) {
            filename = "/data/highscore_patterns.dat";
        } else {
            filename = "/data/highscore_rng.dat";
        }
    }

    file = fopen(filename, "w");
    if (file == NULL) {
        // Handle the error appropriately
        perror("Error opening file");
        return;
    }

    fprintf(file, "%i", score);
    fclose(file);
}

void shutdown() {
    buzzer.stop();
    np.setPixelColor(0, np.Color(0,0,0));
    np.show();
}

void showGameHelp(bool classic_mode) {
    tft.lcdezstr(2, 40,"|AHow To Play:");
    tft.lcdezstr(2, 48, "Press the |9button");
    tft.lcdezstr(2, 56, "to stop the |1L|3E|5D|0.");
    tft.lcdezstr(2, 64, "Don't stop on |1red|0!");
    int loff = 72;
    if (classic_mode == true) {
        tft.lcdezstr(2, 72, "If you do, your");
        tft.lcdezstr(2, 80, "score resets to |10|0!");
        loff = loff + 16;
    }
    tft.lcdezstr(2, loff, "Stop on |1red |5"+std::to_string(max_oopsies)+" |0times");
    tft.lcdezstr(2, loff + 8, "and its |1Game Over|0!");
    tft.lcdezstr(-1, loff + 24, "|1Press |8the |3button");
    tft.lcdezstr(-1, loff + 32, "to begin!");
}

void showGameMode(int mode) {
    debug("Game Mode: %i", game_mode);
    tft.fillRect(0, 152, 128, 8, ST7735_BLACK);
    tft.lcdezstr(54, 152, "|AGame Mode: "+std::to_string(mode));
}

auto spinnerUpdateCallback = []() {
    spinner.update();
};

void setup() {
    Serial.begin(115200);
    shutdown();    
    init_sys();
    init_lcd();
    init_spiffs();
    spin_tune_cb = spinner_tune();
    spin_tune_cb.callback = spinnerUpdateCallback; 
}

void loop() {
    // Configure Option1 (jumper/switch)
    gpio_pad_select_gpio(OPT1);
    gpio_set_direction((gpio_num_t) OPT1, GPIO_MODE_INPUT);
    gpio_set_pull_mode((gpio_num_t) OPT1, GPIO_PULLDOWN_ONLY);
    if (gpio_get_level((gpio_num_t) OPT1) == 1) {
        pattern_mode = true;
    }

    // Configure button and see if it was held on boot
    gpio_pad_select_gpio(BUTTON);
    gpio_set_direction((gpio_num_t) BUTTON, GPIO_MODE_INPUT);

    while(gpio_get_level((gpio_num_t) BUTTON) == 1) {
        if (!classic_mode) {
            tft.lcdezstr(-1, 64, "|7Classic Mode");
            tft.lcdezstr(-1, 80, "Please release");
            tft.lcdezstr(-1, 88, "button to continue");
            classic_mode = true;
        }
        delay(100);
    }

    // init game
    if (classic_mode && pattern_mode) {
        game_mode = 4;
    } 
    else if (classic_mode && !pattern_mode) {
        game_mode = 3;
    }
    else if (!pattern_mode && !classic_mode) {
        game_mode = 2;
    }
    else if (pattern_mode && !classic_mode) {
        game_mode = 1;
    } 

    tft.clearGameArea();

    // title screen press button loop
    if (classic_mode) {
        player.play(thriller());
    } else {
        player.play(terranigma());
    }

    tft.lcdezstr(-1, 8, "|9zefie |7presents|0:");
    tft.lcdezstr(-1, 16, "|1Press |8The |3Button|0!");
    tft.drawLine(2, 34, 124, 34, ST7735_WHITE);
    showGameHelp(classic_mode);
    showGameMode(game_mode);

    while (gpio_get_level((gpio_num_t) BUTTON) == 0) {
        delay(10);
    }

    player.stop();
    uint32_t highscore = loadHiScore(classic_mode, pattern_mode);

    mkscore(0, score_str);
    mkscore(highscore, highscore_str);

    tft.fillScreen(ST7735_BLACK);
    tft.lcdezstr(20,8,"|5Score: |0" + score_str);
    tft.lcdezstr(8,16,"|5HiScore: |0"+ highscore_str);
    tft.lcdezstr(7,24,"|1Oopsies:");
    tft.lcdezstr(62,24,"0 / " + std::to_string(max_oopsies));
    tft.drawLine(2, 34, 124, 34, ST7735_WHITE);
    uint32_t score = 0;
    int oopsies = 0;
    
    // game loop
    debug("Game start!");
    player.play(gamestart());
    delay(2000);
    result.result = -1;
    while (gameover == false) {
        // start spinner and spinner tune
        player.play(spin_tune_cb);
        spinner.start(result.result, pattern_mode, Spinner::Manual);

        // block button for 500 ms
        delay(500);

        // Wait for button
        while (gpio_get_level((gpio_num_t) BUTTON) == 0) {
            delay(10);
        }
        
        // Stop spinner
        player.stop();
        spinner.stop();
        // Retrieve result from the spinner
        if (spinner.getResult(result)) {
            // Use the result
            debug("Spinner result: %d", result.result);
        } else {
            debug("Unknown error getting spinner result");
        }
        while (spinner.isSpinning()) {
            delay(10);
        }
        spinner.clearResult();
        tft.clearGameArea(true);

        //handle result
        if (result.result != 0) {
            player.play(spinner_stop());
            tft.lcdezstr(-1, 80, "|"+std::to_string(gcolors[result.result].lcdezcolor) + "+" + std::to_string(gcolors[result.result].scorevalue), nullptr, 4);
            score = score + gcolors[result.result].scorevalue;
        } else {
            player.play(spinner_stop_oopsie());
            if (classic_mode) { score = 0; }
            oopsies = oopsies + 1;
            std::string ext;
            switch (oopsies) {
                case 1:
                ext = "st";
                break;
                case 2:
                ext = "nd";
                break;
                case 3:
                ext = "rd";
                break;
                default:
                ext = "th";
            } 

            // big oopsie
            tft.lcdezstr(-1, 80, "|1"+std::to_string(oopsies) + ext, nullptr, 2);
            tft.lcdezstr(-1, 96, "|1oopsie!", nullptr, 2);

            // small oopsie
            tft.fillRect(7, 24, 104, 8, ST7735_BLACK);
            tft.lcdezstr(7, 24, "|1Oopsies:");
            tft.lcdezstr(62, 24, std::to_string(oopsies)+" / "+std::to_string(max_oopsies));
        }

        // Current Score
        mkscore(score, score_str);
        tft.fillRect(20, 8, 108, 8, ST7735_BLACK);
        tft.lcdezstr(20,8,"|5Score: |0" + score_str);
        
        if (score > highscore) {
            // High Score
            highscore = score;
            saveHiScore(highscore, classic_mode, pattern_mode);
            mkscore(highscore, highscore_str);
            tft.fillRect(8, 16, 108, 8, ST7735_BLACK);
            tft.lcdezstr(8,16,"|5HiScore: |0"+ highscore_str);
        }

        if (oopsies < max_oopsies) {
            delay(2000);
            tft.clearGameArea();
            buttonwarn = false;
            while (gpio_get_level((gpio_num_t) BUTTON) == 1) {
                if (!buttonwarn) {
                    tft.clearGameArea();
                    tft.lcdezstr(-1, 72, "Please release the");
                    tft.lcdezstr(-1, 80, "button to continue");
                    buttonwarn = true;
                }
                delay(10);
            }
            buttonwarn = false;
            tft.clearGameArea();
        } else {
            // game over
            gameover = true;
            delay(2000);
            tft.clearGameArea();
            tft.lcdezstr(-1, 64, "|1Game Over!", nullptr, 2);
            tft.lcdezstr(-1, 88, "Press RST to");
            tft.lcdezstr(-1, 96, "play again!");
        }
    }
    shutdown();
    while (true) {
        // gently sleep until RST
        delay(5000);
    }
}
