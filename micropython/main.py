import neopixel, random, time, asyncio, sys, os
from machine import SoftSPI, SPI, Pin, PWM, freq

from ST7735 import TFT
from fonts.sysfont import sysfont

from sounds.buzzercfg import buzzer
from sounds.tuneplayer import playtune_async
from sounds.game.spinner_tune import spinner_tune
from sounds.game.stop_tune import stop_tune
from sounds.game.stop_oopsie import stop_oopsie
from sounds.extras.webtv_modem import modem

LCD_SCK=12 # SPI CLK Pin
LCD_MOSI=11 # LCD MOSI/DIN Pin
LCD_DC=18 # LCD DC Pin
LCD_RST=16 # LCD Reset Pin
LCD_CS=10 # LCD CS Pin
LCD_BL=14 # LCD Backlight Pin

BUTTON=45 # Button GPIO Pin
RGBLED=48 # RGB LCD Pin
OPT1=39 # Option 1 GPIO Pin

max_oopsies = 3

## ST7735
LCD = TFT(
    SPI(1, baudrate=40000000, polarity=0, phase=0, sck=Pin(LCD_SCK), mosi=Pin(LCD_MOSI)),
    aDC=LCD_DC,
    aReset=LCD_RST,
    aCS=LCD_CS   
)

tftclrs = [
    TFT.WHITE,
    TFT.RED,
    TFT.MAROON,
    TFT.GREEN,
    TFT.FOREST,
    TFT.BLUE,
    TFT.NAVY,
    TFT.CYAN,
    TFT.YELLOW,
    TFT.PURPLE,
    TFT.GRAY,
    TFT.BLACK,
]

# any kind of button
button = Pin(BUTTON, Pin.IN, Pin.PULL_DOWN)
opt1 = Pin(OPT1, Pin.IN, Pin.PULL_UP)

np = neopixel.NeoPixel(Pin(RGBLED), 1)

r = 0
i = 8 # default RGB LED brightness intensity

colors = [
    # [(r,g,b), scorevalue, ezlcdcolor]
    [(i,0,0),  0, 1], # red, must be first entry
    [(0,0,i),  3, 5], # blue
    [(i,i,0),  5, 8], # yellow
    [(0,i,i),  7, 7], # cyan
    [(i,i,i), 10, 0], # white
    [(i,0,i), 15, 9], # purple
    [(0,i,0), 25, 3], # green
]

patterns = [
    [3, 6, 3, 4, 6, 3, 1, 2, 6, 1, 5, 3, 5, 2, 1, 2],
    [5, 0, 4, 2, 3, 2, 0, 6, 4, 2, 6, 4, 5, 6, 5, 0],
    [6, 2, 0, 6, 3, 1, 3, 5, 3, 4, 3, 2, 0, 1, 0, 1],
    [5, 0, 6, 0, 6, 5, 4, 0, 6, 3, 4, 0, 2, 0, 3, 1],
    [2, 0, 4, 2, 5, 0, 6, 0, 2, 3, 0, 5, 4, 0, 5, 1],
    [1, 0, 6, 2, 4, 2, 1, 3, 2, 6, 3, 1, 3, 1, 4, 2],
    [5, 4, 1, 3, 4, 3, 0, 1, 6, 1, 3, 2, 0, 3, 6, 3],
    [3, 5, 0, 5, 1, 2, 6, 4, 0, 2, 4, 1, 6, 2, 1, 5],
    [2, 0, 5, 6, 3, 4, 0, 3, 6, 4, 6, 3, 1, 6, 0, 1],
    [6, 0, 3, 1, 5, 2, 5, 0, 2, 5, 2, 0, 1, 0, 3, 2]
]

def debug(*args):
    for a in args:
        print(str(a), end=' ')
    print('')

def init_sys():
    freq(240000000)
    debug("SYS:",os.uname().machine,"running at",int(freq() / 1000000),"MHz")
    debug("RAM:",gc.mem_free(),"bytes available")
    debug("FSH:",str(os.statvfs('/')[0] * os.statvfs('/')[3])+" bytes available")

def init_lcd():
    LCD.initg()
    LCD.rotation(2)
    LCD.fill(TFT.BLACK)
    LCD.backlight = Pin(LCD_BL, Pin.OUT)
    LCD.backlight.on()

def mkscore(score):
    score_str = str(score)
    while len(score_str) < 8:
        score_str = "0" + score_str
    return score_str

def getHiScore(classic_mode = False, pattern_mode = False):
    try:
        if classic_mode:
            if pattern_mode:
                f = open('/data/highscore_classic_patterns.dat', 'r')
            else:
                f = open('/data/highscore_classic_rng.dat', 'r')
        else:
            if pattern_mode:
                f = open('/data/highscore_patterns.dat', 'r')
            else:
                f = open('/data/highscore_rng.dat', 'r')
        highscore = int(f.read())
        f.close()
    except:
        highscore = 0
    return highscore

def saveHiScore(score, classic_mode = False, pattern_mode = False):
    if classic_mode:
        if pattern_mode:
            f = open('/data/highscore_classic_patterns.dat', 'w')
        else:
            f = open('/data/highscore_classic_rng.dat', 'w')
    else:
        if pattern_mode:
            f = open('/data/highscore_patterns.dat', 'w')
        else:
            f = open('/data/highscore_rng.dat', 'w')
    f.write(str(score))
    f.close()    

async def lcdezstr(off, string, font=sysfont, size=1):
    # Supports easy color with |#
    cw = font['Width'] + 1 # otherwise its cramped
    ch = font['Height']
    last = ""
    clr = tftclrs[0]
    offx, offy = off
    if offx == -1:
        offx = ((LCD._size[0] - ((len(string) - (string.count('|') * 2))) * (cw * size)) / 2)
        if offx < 0:
            offx = 0
            
    for c in range(0,len(string)):
        s = string[c]
        if last == "|":
            clr = tftclrs[int(s, 16)]
            last = s
            continue
        if s == "|":
            last = s
            continue
        LCD.char((offx, offy), s, clr, font, (size, size))
        offx = offx + (cw * size);
        if (offx >= LCD._size[0] - (cw * size)):
            # word wrap if calculated offset > LCD width
            offx = 0
            offy = offy + ch
        if (offy > LCD._size[1] - (ch * size)):
            # reset to (0,0) if we overrun the LCD height
            # !! idk probably doesn't work either
            offx = 0
            offy = 0

async def spin(r, pattern_mode, patnum=-1, pattern_offset=-1):
    if not pattern_mode:
        res = random.randrange(0,len(colors))
        while res == r:
            res = random.randrange(0,len(colors))
        np[0] = colors[res][0]
        np.write()
        clearGameArea(mini=True)
        asyncio.create_task(lcdezstr((-1, 80), "|"+str(colors[res][2])+str(colors[res][1]), size=3))
        print('spinner rolling', res)
        return res, -1, -1
    else:
        # use pre-defined patterns, but pick the pattern by random
        if patnum == -1:            
            patnum = random.randrange(0,len(patterns))
            pattern_offset = 0
        pat = patterns[patnum]
        if pattern_offset >= len(pat):
            pattern_offset = 0            
        res = pat[pattern_offset]
        np[0] = colors[res][0]
        np.write()
        clearGameArea(mini=True)
        asyncio.create_task(lcdezstr((-1, 80), "|"+str(colors[res][2])+str(colors[res][1]), size=3))
        print('spinner rolling', res, 'pattern', patnum, 'offset', pattern_offset)
        return res, patnum, pattern_offset+1

def shutdown():
    buzzer.init(duty_u16=0)
    np[0] = (0,0,0)
    np.write()
    return

async def showGameHelp(classic_mode = False):
    asyncio.create_task(lcdezstr((2,40),"|4How To Play:"))
    asyncio.create_task(lcdezstr((2,48), "Press the |9button"))
    asyncio.create_task(lcdezstr((2,56), "to stop the |1L|3E|5D|0."))
    asyncio.create_task(lcdezstr((2,64), "Don't stop on |1red|0!"))
    loff = 72
    if classic_mode:
        asyncio.create_task(lcdezstr((2,72), "If you do, your"))
        asyncio.create_task(lcdezstr((2,80), "score resets to |10|0!"))
        loff = loff + 16
    asyncio.create_task(lcdezstr((2,loff), "Stop on |1red |5"+str(max_oopsies)+" |0times"))
    asyncio.create_task(lcdezstr((2,loff+8), "and its |1Game Over|0!"))
    asyncio.create_task(lcdezstr((-1,loff+24),"Press button"))
    asyncio.create_task(lcdezstr((-1,loff+32),"to begin!"))
    
def clearGameArea(full=False, mini=False):
    if full:
        LCD.fill(TFT.BLACK)
    elif mini:
        LCD.fillrect((32,80), (96, 32), TFT.BLACK)
    else:
        LCD.fillrect((0,40), (128, 80), TFT.BLACK)

async def showGameMode(mode):
    LCD.fillrect((0,152), (128, 8), TFT.BLACK)
    asyncio.create_task(lcdezstr((54,152), "|AGame Mode: "+str(mode)))

async def runSpinner(pattern_mode):
    try:
        res, pattern, pattern_offset = -1, -1, -1
        while True:
            res, pattern, pattern_offset = await spin(res, pattern_mode, pattern, pattern_offset)         
            await asyncio.sleep_ms(300)
    except Exception as e:
        print('runSpinner() exception:', e)
        return res
    finally:
        return res

async def main():
    classic_mode = False
    pattern_mode = False
    shutdown()
    
    if opt1.value() == 0:
        pattern_mode = True
        
    while button.value() == 1:
        if not classic_mode:
            asyncio.create_task(lcdezstr((-1,64),"|7Classic Mode", size=1.5))
            asyncio.create_task(lcdezstr((-1,80),"Please release"))
            asyncio.create_task(lcdezstr((-1,88),"button to continue"))
            classic_mode = True
        await asyncio.sleep_ms(100)


    if classic_mode and pattern_mode:
        game_mode = 4
    elif classic_mode and not pattern_mode:
        game_mode = 3
    elif not pattern_mode and not classic_mode:
        game_mode = 2
    elif pattern_mode and not classic_mode:
        game_mode = 1
    else:
        # unknown???
        game_mode = 0
        
    print("Game Mode:", game_mode)
    
    clearGameArea()
    if classic_mode:
        from sounds.extras.thriller import thriller
        titletune = asyncio.create_task(playtune_async(buzzer, thriller))
    else:
        from sounds.extras.terranigma import terranigma
        terranigma['multiplier'] = 0.90
        titletune = asyncio.create_task(playtune_async(buzzer, terranigma))

    asyncio.create_task(lcdezstr((-1,8),"|9zefie |7presents|0:"))
    asyncio.create_task(lcdezstr((-1,16),"|1Press |8The |3Button|0!"))
    LCD.line((2,34), (124, 34), TFT.WHITE)        
    
    asyncio.create_task(showGameHelp(classic_mode))
    while button.value() == 0:
        await asyncio.sleep_ms(10)
    titletune.cancel()
        
    await showGameMode(game_mode)
    
    gameover = False
    highscore = getHiScore(classic_mode, pattern_mode)

    clearGameArea(full=True)
    asyncio.create_task(lcdezstr((20,8),"|5Score: |0"+mkscore(0)))
    asyncio.create_task(lcdezstr((8,16),"|5HiScore: |0"+mkscore(highscore)))
    asyncio.create_task(lcdezstr((7,24),"|1Oopsies:"))
    asyncio.create_task(lcdezstr((62,24),"0 / "+str(max_oopsies)))
    LCD.line((2,34), (124, 34), TFT.WHITE)        
        
    
    score = 0
    res = 0
    oopsies = 0
    spintune = None
    await asyncio.sleep_ms(500)
    await playtune_async(buzzer, modem)
    await asyncio.sleep_ms(500)
    print("Game Start")
    while gameover == False:
        spinner = asyncio.create_task(runSpinner(pattern_mode))
        spintune = asyncio.create_task(playtune_async(buzzer, spinner_tune))
        await asyncio.sleep_ms(500)
        while button.value() == 0:
            await asyncio.sleep_ms(10)

        spinner.cancel()
        res = await asyncio.wait_for(spinner,10)
        
        print('spinner stop', res)
        spintune.cancel()
        spintune = None
        clearGameArea(mini=True)
         
        if (res != 0):
            asyncio.create_task(playtune_async(buzzer, stop_tune))
            asyncio.create_task(lcdezstr((-1, 80), "|"+str(colors[res][2])+"+"+str(colors[res][1]), size=4))
            score = score + colors[res][1]
        else:
            asyncio.create_task(playtune_async(buzzer, stop_oopsie))
            if classic_mode:
                score = 0
            oopsies = oopsies + 1
            if oopsies == 1:
                ext = "st"
            elif oopsies == 2:
                ext = "nd"
            elif oopsies == 3:
                ext = "rd"
            else:
                ext = "th"
                     
            asyncio.create_task(lcdezstr((-1, 80), "|1"+str(oopsies)+ext, size=2))
            asyncio.create_task(lcdezstr((-1, 96), "|1oopsie!", size=2))
            asyncio.create_task(lcdezstr((7,24),"|1Oopsies:"))
            asyncio.create_task(lcdezstr((62,24),str(oopsies)+" / "+str(max_oopsies)))
            
        asyncio.create_task(lcdezstr((20,8),"|5Score: |0"+mkscore(score)))
        if (score > highscore):
            highscore = score
            saveHiScore(highscore, classic_mode, pattern_mode)
            asyncio.create_task(lcdezstr((8,16),"|5HiScore: |0"+mkscore(highscore)))
         
        if (oopsies < max_oopsies):
            await asyncio.sleep(2)
            clearGameArea()
            buttonwarn = False
            while button.value() == 1:
                if not buttonwarn:
                    clearGameArea()
                    asyncio.create_task(lcdezstr((-1,72),"Please release the"))
                    asyncio.create_task(lcdezstr((-1,80),"button to continue"))
                    buttonwarn = True
                await asyncio.sleep(0.01)
            buttonwarn = False
            clearGameArea()                    
        else:
            gameover = True
            await asyncio.sleep(2)
            clearGameArea()
            asyncio.create_task(lcdezstr((-1,64),"|1Game Over!", size=2))
            asyncio.create_task(lcdezstr((-1,88),"Press RST to"))
            asyncio.create_task(lcdezstr((-1,96),"play again!"))
        await asyncio.sleep(0.30)
    return shutdown()


if __name__ == '__main__':
    try:
        init_sys()
        init_lcd()
        asyncio.run(main())
    except KeyboardInterrupt:
        debug('Interrupted')
        shutdown()
        LCD.fill(TFT.BLUE)
        sys.exit(0)
    except Exception as e:
        raise e
    
    sys.exit(0)

