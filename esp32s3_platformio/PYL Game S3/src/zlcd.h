#ifndef ZLCD_H
#define ZLCD_H
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <map>
#include <string>

// Define colors similar to the TFT colors in MicroPython
#define ST77XX_MAROON 32768
#define ST77XX_DARKGREEN 1040
#define ST77XX_NAVY 16
#define ST77XX_PURPLE 63519
#define ST77XX_GRAY 33808
extern std::map<int, uint16_t> tftcolors;

// Define zlcd class

class zlcd : public Adafruit_ST7735 {
    public:
        // Constructor
        zlcd(SPIClass *spiClass, int8_t cs, int8_t dc, int8_t rst, int8_t bl);
        ~zlcd();

        // Overrides to add semaphore
        //void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) override;
        //void fillRect(int16_t startX, int16_t startY, int16_t width, int16_t height, uint16_t color) override;
        //void fillScreen(uint16_t color) override;
        //void setSPISpeed(uint32_t freq);
        //void initR(uint8_t options);
        //void setRotation(uint8_t m) override;

        // Custom functions
        void clearGameArea(bool mini = false);
        void lcdezstr(int offx, int offy, const std::string &str, const GFXfont *font = nullptr, int size = 1);
        void setBacklight(bool status);
        bool getBacklight();
        int takeMutex();
        int freeMutex();

        // Custom definitions
        std::map<int, uint16_t> tftcolors = {
            {0, ST77XX_WHITE}, {1, ST77XX_RED}, {2, ST77XX_MAROON},
            {3, ST77XX_GREEN}, {4, ST77XX_DARKGREEN}, {5, ST77XX_BLUE},
            {6, ST77XX_NAVY}, {7, ST77XX_CYAN}, {8, ST77XX_YELLOW},
            {9, ST77XX_PURPLE}, {10, ST77XX_GRAY}, {11, ST77XX_BLACK}
        };

        uint32_t timeout = pdMS_TO_TICKS(1000);

    private:
        // Member variables
        SemaphoreHandle_t lcdMutex;
        int8_t backlight_pin;
        int hexChar2Dec(char hex);
        
};

#endif // ZLCD_H