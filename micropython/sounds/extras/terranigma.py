# detected bpm: 82
terranigma = {
    'freqs': [
        [587, 175], [587, 0], [392, 175], [392, 0], [466, 175], [466, 0],
        [587, 175], [587, 0], [392, 175], [392, 0], [466, 175], [466, 0],
        [587, 175], [587, 0], [392, 175], [392, 0], [698, 175], [698, 0],
        [392, 175], [392, 0], [466, 175], [466, 0], [587, 175], [587, 0],
        [392, 175], [392, 0], [466, 175], [466, 0], [587, 175], [587, 0],
        [392, 175], [392, 0], [587, 175], [587, 0], [349, 175], [349, 0],
        [466, 175], [466, 0], [587, 175], [587, 0], [349, 175], [349, 0],
        [466, 175], [466, 0], [587, 175], [587, 0], [349, 175], [349, 0],
        [698, 175], [698, 0], [349, 175], [349, 0], [466, 175], [466, 0],
        [587, 175], [587, 0], [349, 175], [349, 0], [466, 175], [466, 0],
        [587, 175], [587, 0], [349, 175], [349, 0], [587, 175], [587, 0],
        [392, 175], [392, 0], [466, 175], [466, 0], [587, 175], [587, 0],
        [392, 175], [392, 0], [466, 175], [466, 0], [587, 175], [587, 0],
        [392, 175], [392, 0], [622, 175], [622, 0], [392, 175], [392, 0],
        [466, 175], [466, 0], [587, 175], [587, 0], [392, 175], [392, 0],
        [587, 175], [587, 0], [466, 175], [466, 0], [392, 175], [392, 0],
        [523, 175], [523, 0], [349, 175], [349, 0], [440, 175], [440, 0],
        [523, 175], [523, 0], [349, 175], [349, 0], [440, 175], [440, 0],
        [523, 175], [523, 0], [349, 175], [349, 0], [698, 175], [698, 0],
        [349, 175], [349, 0], [440, 175], [440, 0], [523, 175], [523, 0],
        [349, 175], [349, 0], [440, 175], [440, 0], [523, 175], [523, 0],
        [349, 175], [349, 0]
    ],
    'loopable': True
}

if __name__ == '__main__':
    print('playing terranigma')
    from sounds.tuneplayer import playtune
    from sounds.buzzercfg import buzzer
    playtune(buzzer, terranigma)