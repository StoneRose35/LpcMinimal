#include "types.h"
#include "mrt.h"
#include "adagio_for_tron.h"
#include "minilib.h"

extern uint32_t p_i;
extern uint32_t* p_phaseIncrements;
extern uint8_t amplitude;

void play_adagio_for_tron()
{
	printf("playing adagio for tron\r\n");
	amplitude=0xFF;
	play_rel_note(0);
	play_rel_note(7);
	play_rel_note(12);
	play_rel_note(7);

	play_rel_note(3);
	play_rel_note(7);
	play_rel_note(12);
	play_rel_note(7);

	play_rel_note(0);
	play_rel_note(7);
	play_rel_note(12);
	play_rel_note(7);

	play_rel_note(3);
	play_rel_note(7);
	play_rel_note(12);
	play_rel_note(7);
	/* Bar 2 */
	play_rel_note(2);
	play_rel_note(7);
	play_rel_note(12);
	play_rel_note(7);

	play_rel_note(2);
	play_rel_note(7);
	play_rel_note(12);
	play_rel_note(7);

	play_rel_note(2);
	play_rel_note(7);
	play_rel_note(11);
	play_rel_note(7);

	play_rel_note(2);
	play_rel_note(7);
	play_rel_note(11);
	play_rel_note(7);
	/*bar 3*/
	play_rel_note(3-12);
	play_rel_note(7-12);
	play_rel_note(12-12);
	play_rel_note(7-12);

	play_rel_note(0-12);
	play_rel_note(7-12);
	play_rel_note(12-12);
	play_rel_note(7-12);

	play_rel_note(3-12);
	play_rel_note(7-12);
	play_rel_note(12-12);
	play_rel_note(7-12);

	play_rel_note(0-12);
	play_rel_note(7-12);
	play_rel_note(12-12);
	play_rel_note(7-12);
	/* bar 4*/
	play_rel_note(0-12);
	play_rel_note(7-12);
	play_rel_note(12-12);
	play_rel_note(7-12);

	play_rel_note(0-12);
	play_rel_note(7-12);
	play_rel_note(12-12);
	play_rel_note(7-12);

	play_rel_note(0-12);
	play_rel_note(8-12);
	play_rel_note(12-12);
	play_rel_note(8-12);

	play_rel_note(0-12);
	play_rel_note(8-12);
	play_rel_note(12-12);
	play_rel_note(8-12);
	/*bar 5*/
	play_rel_note(3-12);
	play_rel_note(7-12);
	play_rel_note(10-12);
	play_rel_note(7-12);

	play_rel_note(3-12);
	play_rel_note(7-12);
	play_rel_note(10-12);
	play_rel_note(7-12);

	play_rel_note(3-12);
	play_rel_note(7-12);
	play_rel_note(10-12);
	play_rel_note(7-12);

	play_rel_note(3-12);
	play_rel_note(7-12);
	play_rel_note(10-12);
	play_rel_note(7-12);
	/*bar 6*/
	play_rel_note(2-12);
	play_rel_note(7-12);
	play_rel_note(12-12);
	play_rel_note(7-12);

	play_rel_note(2-12);
	play_rel_note(7-12);
	play_rel_note(12-12);
	play_rel_note(7-12);

	play_rel_note(2-12);
	play_rel_note(7-12);
	play_rel_note(12-12);
	play_rel_note(7-12);

	play_rel_note(2-12);
	play_rel_note(7-12);
	play_rel_note(12-12);
	play_rel_note(7-12);
	/*bar 7*/

	play_rel_note(2-12);
	play_rel_note(7-12);
	play_rel_note(12-12);
	play_rel_note(7-12);

	play_rel_note(2-12);
	play_rel_note(7-12);
	play_rel_note(12-12);
	play_rel_note(7-12);

	//pause
	amplitude=0x00;
	runTimer(NOTE_DURATION*8);
}

void play_rel_note(int8_t nr)
{
	char display_bfr[11];
	uint8_t delta;
	delta = ((int8_t)BASE_NOTE + nr);
	for(uint8_t c=0;c<11;c++)
	{
		display_bfr[c] = 0;
	}
	p_i = *(p_phaseIncrements + delta);
	printf("phase increment: ");
	asHex((uint32_t)(p_phaseIncrements + delta),display_bfr);
	printf(display_bfr);
	printf(", note: ");
	for(uint8_t c=0;c<11;c++)
	{
		display_bfr[c] = 0;
	}
	toChar(nr,display_bfr);
	printf(display_bfr);
	printf("\r\n");
	runTimer(NOTE_DURATION);
}
