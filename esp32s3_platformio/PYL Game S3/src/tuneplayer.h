#ifndef TUNEPLAYER_H
#define TUNEPLAYER_H
#include <thread>
#include <atomic>
#include <vector>
#include "buzzer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tune.h"

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
