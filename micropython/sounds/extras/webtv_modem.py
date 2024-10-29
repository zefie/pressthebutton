modem = {
    'freqs': [
        [262, 109], [8, 36], [349, 109], [8, 39], [392, 109], [8, 39],
        [262, 109], [8, 39], [330, 109], [8, 39], [294, 109], [8, 39],
        [392, 121], [8, 0]
    ],
    'loopable': False,
}

if __name__ == '__main__':
    print('playing modem')
    from sounds.tuneplayer import playtune
    from sounds.buzzercfg import buzzer
    playtune(buzzer, modem)
