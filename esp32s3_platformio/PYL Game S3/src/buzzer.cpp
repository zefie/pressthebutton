#include "buzzer.h"

Buzzer::Buzzer(int gpio_num) : gpio_num_(gpio_num), channel_(0) {
    // Attach the pin to a PWM channel (0 to 15)
    ledcAttachPin(gpio_num_, channel_);
}

void Buzzer::init(int freq, uint8_t duty) {
    // Set the frequency and duty cycle
    ledcWriteTone(channel_, freq);
    ledcWrite(channel_, duty);  // Duty cycle: 0 to 255
}

void Buzzer::stop() {
    // Stop the tone
    ledcWriteTone(channel_, 0);
    ledcWrite(channel_, 0);
}

