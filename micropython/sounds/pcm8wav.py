from machine import Pin, PWM, Timer

class PWM8(PWM):
    def duty(self,value=None):
        if value is None:
            return self.duty_u16() >> 8
        if value > 65534:
            self.duty_u16(65535)
        else:
            self.duty_u16((value << 8) | value)
            
buzzer = PWM8(Pin(4))

def tick(timer):
    global samples, pos
    if pos >= len(samples):
        pos = 44
    buzzer.duty(samples[ pos ])
    pos += 1

buzzer.freq(65535)
pos = 44
f = open("/sounds/game/whammy.wav", 'rb')
samples = f.read()
f.close()
timer = Timer(1)
timer.init(freq=11025, mode=Timer.PERIODIC, callback=tick)

