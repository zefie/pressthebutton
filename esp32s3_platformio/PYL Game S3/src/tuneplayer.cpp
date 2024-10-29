#include "tuneplayer.h"

TunePlayer::TunePlayer(Buzzer& buzzer)
    : buzzer_(buzzer), cancelFlag_(false), tuneTaskHandle_(nullptr), isPlaying_(false) {}

TunePlayer::~TunePlayer() {
    stop();
}

void TunePlayer::play(const Tune& tune) {
    stop();  // Stop any existing playback
    cancelFlag_.store(false);
    isPlaying_.store(true);
    tune_ = tune;  // Copy the tune to the class member

    // Create a FreeRTOS task
    xTaskCreate(
        playTuneTask,       // Task function
        "TunePlayerTask",   // Name of the task
        4096,               // Stack size in words (adjust as needed)
        this,               // Task input parameter
        tskIDLE_PRIORITY,   // Priority (adjust as needed)
        &tuneTaskHandle_    // Task handle
    );
}

void TunePlayer::stop() {
    if (isPlaying()) {
        cancelFlag_.store(true);
        if (tuneTaskHandle_ != nullptr) {
            // Wait for the task to finish
            vTaskDelete(tuneTaskHandle_);
            tuneTaskHandle_ = nullptr;
        }
        isPlaying_.store(false);
    }
    buzzer_.stop();
}

bool TunePlayer::isPlaying() const {
    return isPlaying_.load();
}

void TunePlayer::playTuneTask(void* pvParameters) {
    TunePlayer* player = static_cast<TunePlayer*>(pvParameters);
    player->playTune();
    vTaskDelete(nullptr);  // Delete this task when done
}

void TunePlayer::playTuneShared() {
    int j = 0;
    int first = true;
    float multiplier = tune_.multiplier != 0 ? tune_.multiplier : 1.0f;
    for (int i = 0; i < tune_.notes.size(); ++i) {
        if (cancelFlag_.load()) {
            break;
        }

        int freq = tune_.notes[i].frequency;
        int duration = tune_.notes[i].duration;

        if (freq == 0 || freq == 8) {
            buzzer_.stop();
        } else {
            buzzer_.init(freq, 255);
        }

        if (tune_.debug) {
            printf("Note %d: Freq=%d, Duration=%dms\n", i, freq, duration);
        }

        // Use vTaskDelay instead of sleep
        sleepWithCancellation(static_cast<int>(duration / multiplier));
        if (tune_.callback) {
            if (tune_.callback_at_start && first && tune_.callback_interval != 1) {
                // don't double callback if interval is 1
                tune_.callback();
                first = false;
            } else {
                j++; // bump to 1 before interval comparison
                if (j == tune_.callback_interval) {
                    tune_.callback();
                    j = 0;
                }
            }
        }
    }
}

void TunePlayer::playTune() {
    try {
        if (tune_.loopable) {
            while (!cancelFlag_.load()) {
                playTuneShared();
            }
        } else {
            playTuneShared();
            buzzer_.stop();
        }
    } catch (...) {
        buzzer_.stop();
        isPlaying_.store(false);
        throw;
    }
    buzzer_.stop();
    isPlaying_.store(false);
}

void TunePlayer::sleepWithCancellation(int duration_ms) {
    const int interval = 50;
    int elapsed = 0;
    while (elapsed < duration_ms) {
        if (cancelFlag_.load()) {
            break;
        }
        int sleep_time = std::min(interval, duration_ms - elapsed);
        vTaskDelay(pdMS_TO_TICKS(sleep_time));
        elapsed += sleep_time;
    }
}

