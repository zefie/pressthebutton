stop_oopsie = {
    'freqs': [[164, 500], [69, 750], [8, 0]],
    'loopable': False
}

if __name__ == '__main__':
    print('playing stop_oopsie')
    from sounds.tuneplayer import playtune
    from sounds.buzzercfg import buzzer
    playtune(buzzer, stop_oopsie)
