#ifndef CONFIG_H
#define CONFIG_H

// Config
// Game
const int max_oopsies = 3;

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

typedef struct {
    int npcolor[3];
    int scorevalue;
    int lcdezcolor;
} GameColors;

const GameColors gcolors[7] = {
    {{I, 0, 0},  0, 1},  // red
    {{0, 0, I},  3, 5},  // blue
    {{I, I, 0},  5, 8},  // yellow
    {{0, I, I},  7, 7},  // cyan
    {{I, I, I}, 10, 0},  // white
    {{I, 0, I}, 15, 9},  // purple
    {{0, I, 0}, 25, 3},  // green
};

const int patterns[10][16] = {
    {3, 6, 3, 4, 6, 3, 1, 2, 6, 1, 5, 3, 5, 2, 1, 2},
    {5, 0, 4, 2, 3, 2, 0, 6, 4, 2, 6, 4, 5, 6, 5, 0},
    {6, 2, 0, 6, 3, 1, 3, 5, 3, 4, 3, 2, 0, 1, 0, 1},
    {5, 0, 6, 0, 6, 5, 4, 0, 6, 3, 4, 0, 2, 0, 3, 1},
    {2, 0, 4, 2, 5, 0, 6, 0, 2, 3, 0, 5, 4, 0, 5, 1},
    {1, 0, 6, 2, 4, 2, 1, 3, 2, 6, 3, 1, 3, 1, 4, 2},
    {5, 4, 1, 3, 4, 3, 0, 1, 6, 1, 3, 2, 0, 3, 6, 3},
    {3, 5, 0, 5, 1, 2, 6, 4, 0, 2, 4, 1, 6, 2, 1, 5},
    {2, 0, 5, 6, 3, 4, 0, 3, 6, 4, 6, 3, 1, 6, 0, 1},
    {6, 0, 3, 1, 5, 2, 5, 0, 2, 5, 2, 0, 1, 0, 3, 2}
};


#endif // CONFIG_H
