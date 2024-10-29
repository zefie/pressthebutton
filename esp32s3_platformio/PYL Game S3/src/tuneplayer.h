#ifndef TUNEPLAYER_H
#define TUNEPLAYER_H
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include "buzzer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

struct Note {
    int frequency;  // Frequency in Hz
    int duration;   // Duration in milliseconds
};

struct Tune {
    float multiplier = 1.0f;
    bool loopable = false;
    bool debug = false;
    std::function<void()> callback = nullptr;
    int callback_interval = 0;
    bool callback_at_start = false;
    std::vector<Note> notes;
};

class TunePlayer {
    public:
        TunePlayer(Buzzer& buzzer);
        ~TunePlayer();

        void play(const Tune& tune);
        void stop();
        bool isPlaying() const;

    private:
        static void playTuneTask(void* pvParameters);
        void playTune();
        void playTuneShared();
        void sleepWithCancellation(int duration_ms);

        Buzzer& buzzer_;
        std::atomic<bool> cancelFlag_;
        TaskHandle_t tuneTaskHandle_;
        std::atomic<bool> isPlaying_;
        Tune tune_;  // Store the tune to be played
};

#endif // TUNEPLAYER_H
