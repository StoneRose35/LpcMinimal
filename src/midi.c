#include "types.h"
#include "midi.h"

/*midi controller variables*/
volatile uint8_t midiNotes = 0;
volatile uint8_t midiCntr = 0;
volatile uint8_t lastStatus = 0x80;
volatile uint8_t noteBfr;
volatile uint8_t velBfr;
volatile uint8_t notes[16];
volatile uint8_t playedNote = 0;




void handleMidiNote(uint8_t midi_cmd)
{
	if ((midi_cmd & 0x80) == 0x80) /*check for status byte*/
	{
		midiCntr=1;
		lastStatus = midi_cmd & 0xF0;
	}
	else
	{
		if (midiCntr==1)
		{
			noteBfr = midi_cmd;
			midiCntr++;
		}
		else if (midiCntr==2)
		{
			velBfr=midi_cmd;
			midiCntr++;
		}
	}
}


void processMidiCommand(volatile uint8_t* note_nr,volatile uint8_t* amplitude)
{

	switch (lastStatus)
	{
		case MIDI_NOTEOFF:

			if (midiNotes > 0)
			{
				handleNoteOff(note_nr,amplitude);
			}
			else
			{
				*note_nr=0;
				*amplitude=0;
			}
			break;
		case MIDI_NOTEON:
			if (velBfr == 0)
			{
				if (midiNotes > 0)
				{
					handleNoteOff(note_nr,amplitude);
				}
				else
				{
					*note_nr=0;
					*amplitude=0;
				}
			}
			else
			{
				*amplitude = 255;
				*(notes+midiNotes) = noteBfr;
				midiNotes++;
				//set note, currently implementing highest first
				if (noteBfr > playedNote)
				{
					playedNote = noteBfr;
					*note_nr=playedNote;
				}

			}
			break;
		default:
			break;
	}
	midiCntr=1;
}

uint8_t getCurrentNote(void)
{
	return playedNote;
}

uint8_t getMidiCntr()
{
	return midiCntr;
}

void setMidiCntr(uint8_t val)
{
	midiCntr = val;
}

void handleNoteOff(volatile uint8_t* note_nr,volatile uint8_t* amplitude)
{
	for (char c=0;c<midiNotes;c++)
	{
		if (*(notes+c)==noteBfr)
		{
			for(char cc=c;cc<15;cc++)
			{
				*(notes+cc) = *(notes + cc + 1);
			}

			*(notes+15) = 0;

			midiNotes--;
			if (midiNotes > 0)
			{
				*note_nr = *(notes + midiNotes-1);
			}
			else
			{
				*note_nr=0;
				*amplitude=0;
			}
		}
	}

}
