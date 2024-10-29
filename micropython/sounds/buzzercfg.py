from machine import PWM, Pin
buzzer = PWM(Pin(4), duty_u16=0)