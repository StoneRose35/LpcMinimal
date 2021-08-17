
#ifndef MIDI_H_
#define MIDI_H_


#define MIDI_NOTEOFF 0x80
#define MIDI_NOTEON 0x90
void handleMidiNote(uint8_t midi_cmd);
void handleNoteOff(volatile uint8_t* note_nr,volatile uint8_t*amplitude);
uint8_t getCurrentNote(void);
uint8_t getMidiCntr();
void setMidiCntr(uint8_t val);
void processMidiCommand(volatile uint8_t* note_nr,volatile uint8_t* amplitude);

#endif
