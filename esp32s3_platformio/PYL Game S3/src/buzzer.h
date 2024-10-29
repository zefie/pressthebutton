#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
    public:
        Buzzer(int gpio_num);
        void init(int freq, uint8_t duty);
        void stop();

    private:
        int gpio_num_;
        uint8_t channel_;
};

#endif // BUZZER_H