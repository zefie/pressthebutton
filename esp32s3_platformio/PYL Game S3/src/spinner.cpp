#include "config.h"
#include "spinner.h"
#include "zlcd.h"
#include "functions.h"
#include "game.h"

Spinner::Spinner()
    : spinnerTaskHandle_(nullptr),
      res_(0),
      pmode_(false),
      mode_(Automatic),
      stopFlag_(false),
      resultMutex_(nullptr),
      spinning_(false),
      taskCompleteSemaphore_(nullptr) {
    // Initialize the result mutex
    resultMutex_ = xSemaphoreCreateMutex();
    spinningMutex_ =  xSemaphoreCreateMutex();
}

Spinner::~Spinner() {
    stop();
    if (resultMutex_ != nullptr) {
        vSemaphoreDelete(resultMutex_);
        resultMutex_ = nullptr;
    }
}

void Spinner::start(int res, bool pmode, Mode mode) {
    stop();  // Stop any existing task
    res_ = res;
    pmode_ = pmode;
    mode_ = mode;

    stopFlag_.store(false);  // Reset the stop flag
    debug("Starting spinner task with res=%d, pmode=%d", res_, pmode_);

    if (mode_ == Automatic) {
        // Create the task completion semaphore
        taskCompleteSemaphore_ = xSemaphoreCreateBinary();

        if (taskCompleteSemaphore_ == nullptr) {
            debug("Failed to create taskCompleteSemaphore_");
        } else {
            debug("taskCompleteSemaphore_ created successfully");
        }    

        BaseType_t result = xTaskCreate(
            spinnerTask,
            "SpinnerTask",
            4096,
            this,
            tskIDLE_PRIORITY + 1,
            &spinnerTaskHandle_
        );

        if (result != pdPASS) {
            debug("Failed to create spinner task");
            spinnerTaskHandle_ = nullptr;
            vSemaphoreDelete(taskCompleteSemaphore_);
            taskCompleteSemaphore_ = nullptr;
        }
    }
}

void Spinner::stop() {
    if (spinnerTaskHandle_ != nullptr) {
        stopFlag_.store(true);  // Signal the task to stop

        // Wait for the task to signal completion
        if (taskCompleteSemaphore_ != nullptr) {
            debug("Waiting for taskCompleteSemaphore_");
            // Wait for the semaphore to be given by the task
            if (xSemaphoreTake(taskCompleteSemaphore_, pdMS_TO_TICKS(1000)) != pdTRUE) {
                // Handle timeout or error
                debug("Timeout waiting for spinner task to complete");
            }
            // Delete the semaphore
            vSemaphoreDelete(taskCompleteSemaphore_);
            taskCompleteSemaphore_ = nullptr;
        } else {
            debug("Error: taskCompleteSemaphore_ is null in", __func__);
        }

        // The task handle is no longer valid
        spinnerTaskHandle_ = nullptr;
    }
}

bool Spinner::getResult(SpinnerResult& result) {
    if (resultMutex_ != nullptr) {
        if (xSemaphoreTake(resultMutex_, pdMS_TO_TICKS(1000)) == pdTRUE) {
            result = result_;  // Copy the result
            xSemaphoreGive(resultMutex_);
            return true;
        } else {
            debug("Failed to obtain mutex in %s", __func__);
            return false;
        }
    }
    return false;
}

void Spinner::clearResult() {
    if (resultMutex_ != nullptr) {
        if (xSemaphoreTake(resultMutex_, pdMS_TO_TICKS(1000)) == pdTRUE) {
            result_ = SpinnerResult();  // Reset the result
            xSemaphoreGive(resultMutex_);
        } else {
            debug("Failed to obtain mutex in %s", __func__);
        }
    }
}

void Spinner::singleSpin() {
    // Spinner logic

    if (spinningMutex_ != nullptr) {
        if (xSemaphoreTake(spinningMutex_, pdMS_TO_TICKS(100)) == pdTRUE) {
            spinning_ = true;
            xSemaphoreGive(spinningMutex_);
        } else {
            printf("Failed to obtain mutex to update spin status (true)\n");
        }
    }
    SpinnerResult result;
    tft.clearGameArea(true);
    if (resultMutex_ != nullptr) {
        if (xSemaphoreTake(resultMutex_, pdMS_TO_TICKS(100)) == pdTRUE) {
            result = result_;  // Update the result
            xSemaphoreGive(resultMutex_);
        } else {
            debug("Failed to obtain mutex to fetch result");
        }
    }
    if (!pmode_) {
        // Random color selection mode
        do {
            result.result = rng(sizeof(gcolors) / sizeof(gcolors[0]));
        } while (result.result == res_);  // Ensure the color is different from the last

        tft.lcdezstr(-1, 80, "|" + std::to_string(gcolors[result.result].lcdezcolor) + std::to_string(gcolors[result.result].scorevalue), NULL, 3);
        debug("Spinner rolling: %i", result.result);
        this->res_ = result.result;
        result.pnum = -1;
        result.poffset = -1;
    } else {
        // Pattern mode
        if (result.pnum == -1) {
            result.pnum = rng(sizeof(patterns) / sizeof(patterns[0]));
            result.poffset = 0;
        }

        if (result.poffset >= (sizeof(patterns[result.pnum]) / sizeof(patterns[result.pnum][0]))) {
            result.poffset = 0;  // Loop back to the start of the pattern
        }

        result.result = patterns[result.pnum][result.poffset];
        tft.lcdezstr(-1, 80, "|" + std::to_string(gcolors[result.result].lcdezcolor) + std::to_string(gcolors[result.result].scorevalue), NULL, 3);
        debug("Spinner rolling: %i Pattern: %i Offset: %i", result, result.pnum, result.poffset);
        result.poffset++;
    }

    np.setPixelColor(0, gcolors[result.result].npcolor[0], gcolors[result.result].npcolor[1], gcolors[result.result].npcolor[2]);
    np.show();

    // Clear and display text
    tft.lcdezstr(-1, 80, "|" + std::to_string(result.result), nullptr, 3);

    // When the result is ready, send it to the queue
    if (resultMutex_ != nullptr) {
        if (xSemaphoreTake(resultMutex_, pdMS_TO_TICKS(100)) == pdTRUE) {
            result_ = result;  // Update the result
            xSemaphoreGive(resultMutex_);
        } else {
            printf("Failed to obtain mutex to update result\n");
        }
    }
    if (spinningMutex_ != nullptr) {
        if (xSemaphoreTake(spinningMutex_, pdMS_TO_TICKS(100)) == pdTRUE) {
            spinning_ = false;
            xSemaphoreGive(spinningMutex_);
        } else {
            printf("Failed to obtain mutex to spin status (false)\n");
        }
    }

}

void Spinner::update() {
    // Only perform update if in Manual mode
    if (mode_ == Manual && !stopFlag_) {
        // Spinner logic for a single update
        SpinnerResult result;
        singleSpin();
    }
}

bool Spinner::isSpinning() {
    bool isspinning = true;
    if (spinningMutex_ != nullptr) {
        if (xSemaphoreTake(spinningMutex_, pdMS_TO_TICKS(1000)) == pdTRUE) {
            isspinning = spinning_;  // Copy the result
            xSemaphoreGive(spinningMutex_);
        } else {
            debug("Failed to obtain mutex in %s", __func__);
        }
    }
    return isspinning;
}

void Spinner::spinnerTask(void* pvParameters) {
    Spinner* spinner = static_cast<Spinner*>(pvParameters);
    debug("automatic spinner start");

    while (!spinner->stopFlag_.load()) {
        spinner->singleSpin();
        vTaskDelay(pdMS_TO_TICKS(300));
    }
    // Task completes its work
    printf("automatic spinner stopping\n");

    // Signal that the task is about to delete itself
    if (spinner->taskCompleteSemaphore_ != nullptr) {
        printf("Giving taskCompleteSemaphore_\n");
        xSemaphoreGive(spinner->taskCompleteSemaphore_);
    } else {
        printf("Error: taskCompleteSemaphore_ is null\n");
    }

    vTaskDelete(NULL);
}
