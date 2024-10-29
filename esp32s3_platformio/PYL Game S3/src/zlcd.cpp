#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <map>
#include <string>
#include "zlcd.h"
#include "functions.h"

// Constructor
zlcd::zlcd(SPIClass *spiClass, int8_t cs, int8_t dc, int8_t rst, int8_t bl)
    : Adafruit_ST7735(spiClass, cs, dc, rst) {
    this->lcdMutex = xSemaphoreCreateMutex();
    this->backlight_pin = bl;
}

zlcd::~zlcd() {
    if (this->lcdMutex) {
        vSemaphoreDelete(this->lcdMutex);
    }
}

BaseType_t zlcd::takeMutex() {
    if (this->lcdMutex) {
        return xSemaphoreTake(this->lcdMutex, this->timeout);
    }
    return pdFALSE;
}

BaseType_t zlcd::freeMutex() {
    if (this->lcdMutex) {
        BaseType_t res = xSemaphoreGive(this->lcdMutex);
        if (res != pdTRUE) {
            debug("Failed to release lcdMutex: %i", res);
        }
        return res;
    }
    return pdFALSE;
}

void zlcd::clearGameArea(bool mini) {
    int startX = 0;
    int startY = mini ? 60 : 40;
    int width = Adafruit_ST7735::width();    
    int height = mini ? 42 : Adafruit_ST7735::height() - 40;
    if (this->takeMutex() == pdTRUE) {
        this->fillRect(startX, startY, width, height, ST77XX_BLACK);
        this->freeMutex();
    } else {
        debug("Failed to take lcdMutex in %s", __func__);
    }    
    
}

void zlcd::setBacklight(bool status) {
    gpio_pad_select_gpio(this->backlight_pin);
    gpio_set_direction((gpio_num_t) this->backlight_pin, GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t) this->backlight_pin, (status) ? 1: 0);    
}

bool zlcd::getBacklight() {
    return (gpio_get_level((gpio_num_t) this->backlight_pin) == 1) ? true : false; 
}

int zlcd::hexChar2Dec(char hex) {
    hex = std::toupper(hex);  // Handle lowercase by converting to uppercase

    if (hex >= '0' && hex <= '9') {
        return hex - '0';  // '0'-'9' maps to 0-9
    } else if (hex >= 'A' && hex <= 'F') {
        return hex - 'A' + 10;  // 'A'-'F' maps to 10-15
    } else {
        throw std::invalid_argument("Invalid hexadecimal character");
    }
}

void zlcd::lcdezstr(int offx, int offy, const std::string &str, const GFXfont *font, int size) {
    int cw = 6 * size;
    int ch = 8 * size;
    int screenWidth = 0;
    int screenHeight = 0;

    uint16_t color = tftcolors[0];
    if (this->takeMutex() == pdTRUE) {
        screenWidth = Adafruit_ST7735::width();
        screenHeight = Adafruit_ST7735::height();
        this->freeMutex();
    } else {
        debug("Failed to take lcdMutex to get width and height in %s", __func__);
    }

    if (offx == -1) {
        offx = (screenWidth - (str.size() - std::count(str.begin(), str.end(), '|') * 2) * cw) / 2;
        if (offx < 0) offx = 0;
    }

    if (this->takeMutex() == pdTRUE) {
        Adafruit_ST7735::setTextSize(size);
        if (font) Adafruit_ST7735::setFont(font);
        this->freeMutex();
    } else {
        debug("Failed to take lcdMutex to setTextSize and setFont in %s", __func__);
    }

    for (size_t i = 0; i < str.size(); i++) {
        char s = str[i];

        if (s == '|') {
            if (i + 1 < str.size() && std::isxdigit(str[i + 1])) {
                i++;
                try {
                    int hexValue = hexChar2Dec(str[i]);  // Convert hex character to decimal
                    color = tftcolors[hexValue];
                } catch (const std::invalid_argument& e) {
                    // Handle error: invalid hex character
                    printf("Invalid hex character: %c\n", str[i]);
                }
            }
            continue;
        }

        if (this->takeMutex() == pdTRUE) {
            Adafruit_ST7735::setTextColor(color);
            Adafruit_ST7735::setCursor(offx, offy);
            Adafruit_ST7735::print(s);
            this->freeMutex();
        } else {
            debug("Failed to take lcdMutex to setTextColor, setCursor, and print character %s in %s", s, __func__);
        }
        offx += cw;
        if (offx >= screenWidth - cw) {
            offx = 0;
            offy += ch;
            if (offy >= screenHeight - ch) {
                offy = 0;
            }
        }
    }
}