#ifndef CONFIG_H
#define CONFIG_H

// Config
// Game
const int max_oopsies = 3;

// SPIFlash Mount Point
const char data_partition[] = "spiffs";
const char mount_point[] = "/data";
#define BLOCK_SIZE 512

// Input Pins
#define BUTTON     45 

// RGB LED
#define I 16 // LED Intensity (Brightness) (max 255)

#define BUZZER_PIN SPEAK
// End Config

#endif // CONFIG_H
