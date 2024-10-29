stop_tune = {
    'freqs': [
        [640,250],
        [8, 250],
        [640, 250],
        [8, 0]
    ],
    'loopable': False
}

if __name__ == '__main__':
    print('playing stop_tune')
    from sounds.tuneplayer import playtune
    from sounds.buzzercfg import buzzer
    playtune(buzzer, stop_tune)