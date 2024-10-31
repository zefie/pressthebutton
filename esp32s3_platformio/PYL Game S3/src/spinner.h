#ifndef SPINNER_H
#define SPINNER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "functions.h"
#include "config.h"
#include "zlcd.h"
#include <RGBLed.h>
#include <atomic>


extern zlcd tft;
extern RGBLed led;

struct SpinnerResult {
    int result = -1;
    int pnum = -1;
    int poffset = -1;
};

class Spinner {
public:
    enum Mode {
        Automatic,
        Manual
    };

    Spinner();
    ~Spinner();

    void start(int res, bool pmode, Mode mode = Automatic);
    void stop();
    void update();  // Used in Manual mode
    // Method to get the result

    bool isSpinning();
    bool getResult(SpinnerResult& result);
    void clearResult();

private:
    void singleSpin();
    static void spinnerTask(void* pvParameters);
    TaskHandle_t spinnerTaskHandle_;
    QueueHandle_t resultQueue_;

    int res_;
    bool pmode_;
    bool spinning_ = false;
    Mode mode_;
    std::atomic<bool> stopFlag_;  // Stop flag
    SpinnerResult result_;

    SemaphoreHandle_t resultMutex_;
    SemaphoreHandle_t spinningMutex_;
    SemaphoreHandle_t taskCompleteSemaphore_;
};

#endif // SPINNER_H