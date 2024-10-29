import time, asyncio

# non-async for listening
def playtune(buzzer, tune):
    i = 0
    if 'multiplier' in tune:
        multiplier = tune['multiplier']
    else:
        multiplier = 1
        
    if tune['loopable']:
        while True:
            for i in range(0, len(tune['freqs'])):
                if tune['freqs'][i][0] == 0 or tune['freqs'][i][0] == 8:
                    buzzer.init(duty_u16=0)
                else:
                    buzzer.init(freq=tune['freqs'][i][0], duty_u16=32768)
                    
                timing = tune['freqs'][i][1]
                if 'debug' in tune:
                    if tune['debug'] == True:
                        print(i, tune['freqs'][i][0], timing)
                        i = i + 1

                time.sleep_ms(int(timing / multiplier))
    else:
        for i in range(0, len(tune['freqs'])):
            if tune['freqs'][i][0] == 0 or tune['freqs'][i][0] == 8:
                buzzer.init(duty_u16=0)
            else:
                buzzer.init(freq=tune['freqs'][i][0], duty_u16=32768)
                
            timing = tune['freqs'][i][1]

            if 'debug' in tune:
                if tune['debug'] == True:
                    print(i, tune['freqs'][i][0], timing)
                    i = i + 1

            time.sleep_ms(int(timing / multiplier))
        buzzer.init(duty_u16=0)

# async for game
async def playtune_async(buzzer, tune):
    i = 0
    if 'multiplier' in tune:
        multiplier = tune['multiplier']
    else:
        multiplier = 1
    try:
        if tune['loopable']:
            while True:
                for i in range(0, len(tune['freqs'])):
                    if tune['freqs'][i][0] == 0 or tune['freqs'][i][0] == 8:
                        buzzer.init(duty_u16=0)
                    else:
                        buzzer.init(freq=tune['freqs'][i][0], duty_u16=32768)
                        
                    timing = tune['freqs'][i][1]

                    if 'debug' in tune:
                        if tune['debug'] == True:
                            print(i, tune['freqs'][i][0], timing)
                            i = i + 1
                        
                    await asyncio.sleep_ms(int(timing / multiplier))
        else:
            for i in range(0, len(tune['freqs'])):
                if tune['freqs'][i][0] == 0 or tune['freqs'][i][0] == 8:
                    buzzer.init(duty_u16=0)
                else:
                    buzzer.init(freq=tune['freqs'][i][0], duty_u16=32768)
                    
                timing = tune['freqs'][i][1]

                if 'debug' in tune:
                    if tune['debug'] == True:
                        print(i, tune['freqs'][i][0], timing)
                        i = i + 1

                await asyncio.sleep_ms(int(timing / multiplier))
        buzzer.init(duty_u16=0)
    finally:
        buzzer.init(duty_u16=0)
