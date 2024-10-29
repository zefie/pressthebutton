from machine import I2S, Pin
import time
import math
import ustruct

# Pin definitions
PDM_CLK_PIN = 5     # PDM clock pin (BCK)
PDM_DOUT_PIN = 4    # PDM data pin (DATA)

# Initialize I2S for pseudo-PDM output
i2s = I2S(0,                      # I2S channel 0
          sck=Pin(PDM_CLK_PIN),   # Clock pin
          ws=Pin(1),              # Not used in PDM mode
          sd=Pin(PDM_DOUT_PIN),   # Data pin
          mode=I2S.TX,            # Transmit only
          bits=16,                # Bits per sample
          format=I2S.MONO,        # Mono channel
          rate=16000,             # Sampling rate in Hz
          ibuf=1024)               # Buffer size

# Generate a sine wave as audio input for testing
def generate_sine_wave(freq, sample_rate, duration):
    # Generate audio samples for a sine wave of a given frequency and duration
    samples = bytearray()
    #amplitude = 32767  # Max amplitude for 16-bit audio
    amplitude = 25000

    for i in range(int(sample_rate * duration)):
        sample_value = int(amplitude * math.sin(2 * math.pi * freq * i / sample_rate))
        samples += sample_value.to_bytes(2, 'little')

    return samples

def moving_average_filter(buffer, window_size=4):
    filtered_buffer = bytearray(len(buffer))
    for i in range(len(buffer)):
        avg = 0
        count = 0
        for j in range(max(0, i - window_size + 1), i + 1):
            avg += buffer[j]
            count += 1
        filtered_buffer[i] = avg // count
    return filtered_buffer

def play_wav(filename):
    # Open WAV file
    with open(filename, "rb") as f:
        # Read and validate WAV file header
        header = f.read(44)
        if header[:4] != b'RIFF' or header[8:12] != b'WAVE':
            print("Invalid WAV file format.")
            return

        # Extract audio format details from the WAV header
        num_channels = ustruct.unpack("<H", header[22:24])[0]
        sample_rate = ustruct.unpack("<I", header[24:28])[0]
        bits_per_sample = ustruct.unpack("<H", header[34:36])[0]

        print(f"Playing {filename}:")
        print(f"Channels: {num_channels}, Sample Rate: {sample_rate} Hz, Bits: {bits_per_sample}")

        # Ensure compatibility with I2S setup
        if num_channels != 1 or bits_per_sample != 16:
            print("Only 16-bit mono WAV files are supported.")
            return

        # Adjust I2S sample rate to match the file
#        i2s.rate(sample_rate)

        # Start reading and playing the audio data
        buffer = bytearray(1024)  # Buffer for audio data
        while True:
            num_bytes_read = f.readinto(buffer)
            if num_bytes_read == 0:
                break  # End of file

            # Write audio data to I2S
            i2s.write(buffer[:num_bytes_read])

        print("Playback finished.")

# Prepare sine wave data for PDM
while True:
    play_wav("/sounds/game/whammy.wav")
    time.sleep(0.1)
    

