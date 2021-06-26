
#ifndef MIDI_H_
#define MIDI_H_


#define MIDI_NOTEOFF 0x80
#define MIDI_NOTEON 0x90
void handleMidiNote(uint8_t midi_cmd,volatile uint8_t* note_nr,volatile uint8_t* amplitude);
void handleNoteOff(volatile uint8_t* note_nr,volatile uint8_t*amplitude);
uint8_t getCurrentNote(void);

#endif
