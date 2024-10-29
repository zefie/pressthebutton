#ifndef CONFIG_H
#define CONFIG_H

// Config
// Game
const int max_oopsies = 3;

// SPIFlash Mount Point
const char data_partition[] = "spiffs";
const char mount_point[] = "/data";
#define BLOCK_SIZE 512

// LCD Pins
#define LCD_RST    16
#define LCD_DC     18
#define LCD_BL     14

// Input Pins
#define BUTTON     45 
#define OPT1       39

// RGB LED
#define NUM_PIXELS 1 // Number of LEDs on strip, only 1 supported in this game
#define LED_PIN 48
#define I 16 // LED Intensity (Brightness) (max 255)

#define BUZZER_PIN 4
// End Config

#endif // CONFIG_H
