import sys, pprint
from mido import MidiFile


old_mode = False

# Load the MIDI file
midi_file_path = sys.argv[1]
if len(sys.argv) > 2:
    channels = [int(channel) for channel in sys.argv[2].split(',')]
else:
    channels = [0]
    
if len(sys.argv) > 4:
    old_mode = True
    
midi_data = MidiFile(midi_file_path)
notes = []

for track in midi_data.tracks:
    for msg in track:
        if msg.type == "set_tempo":
            midi_data.ticks_per_beat = msg.tempo / 1000
            break
    break


if len(sys.argv) > 3:
    bpm = int(sys.argv[3])
else:
    bpm = int((60000000 / midi_data.ticks_per_beat) / 1000)
    print("# detected bpm:", bpm)
    
ms_per_tick = int((60000000 / bpm) / 1000) / midi_data.ticks_per_beat

def delta_time_to_ms(delta_ticks):
    return int(delta_ticks * ms_per_tick)
  
if old_mode:
    for track in midi_data.tracks:
        for msg in track:
            if (msg.type == 'note_on' and msg.channel == channels[0]) or msg.type == 'note_off':    
                    notes.append((msg.note, delta_time_to_ms(msg.time)))  # Store note number and event time          
                    
else:
    last_note = -1
    for track in midi_data.tracks:
        for msg in track:
            if (msg.type == 'note_on' and msg.channel in channels) or msg.type == 'note_off':
                    if msg.type != 'note_on':
                        if msg.time > 0:
                            msg.note = 0
                        else:
                            continue
                    if last_note >= 0:
                        notes.append((last_note, delta_time_to_ms(msg.time)))  # Store note number and event time          
                    last_note = msg.note
                
    notes.append((0,0))
# Convert MIDI note numbers to frequencies
def midi_to_freq(midi_note):
    return 440.0 * (2 ** ((midi_note - 69) / 12.0))

#print(midi_data)
# Convert note numbers to frequencies and create an array of frequencies
frequencies = [[round(midi_to_freq(note[0])),note[1]] for note in notes]

# Print or use the frequency array
data = {
    'freqs': frequencies,
    'loopable': False,
    'debug': True
}

name = sys.argv[1].replace('.'+sys.argv[1].split('.')[-1],'').split("/")[-1].split("\\")[-1].replace("-","_")
if old_mode:
    print("# old broken mode enabled")
print(name+" = ", end='')
pprint.pp(data, compact=True)
print()
print("if __name__ == '__main__':")
print("    print('playing "+name+"')")
print("    from sounds.tuneplayer import playtune")
print("    from sounds.buzzercfg import buzzer")
print("    playtune(buzzer, "+name+")")

