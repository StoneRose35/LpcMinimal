#include "types.h"
#include "minilib.h"
#include "chip.h"

int printf(const char * data)
{
	int cnt = 0;
	while (data[cnt] != 0)
	{
		sendChar(data[cnt]);
		cnt++;
	}
	return cnt;
}

void sendChar(uint8_t data)
{
	while ((*USART_STAT0 & (0x1 << 2)) == 0) {}
	*TXDAT0 = data;
}

void asHex(uint32_t v,char* res)
{
	uint32_t bfr;
	uint32_t cnt = 0;
	res[cnt] = 0x30;
	cnt++;
	res[cnt] = 0x78;
	cnt++;
	while (v > 0)
	{
		bfr = v & 0xF0000000;
		bfr = bfr >> 32-4;
		bfr = bfr & 0xF;
		if (bfr > 9)
		{
			res[cnt] = bfr + 0x41 - 0xA;
		}
		else
		{
			res[cnt] = bfr + 0x30;
		}
		cnt++;
		v = v << 4;
	}
	if (cnt==2)
	{
		res[cnt]=0x30;
	}
}

void toChar(uint8_t v,char* res)
{

	uint8_t v_bfr;
	uint8_t rem2;
	res[0]=0x0;
	res[1]=0x0;
	res[2]=0x0;
	res[3]=0x0;
	char res_bfr[4];
	uint8_t idx;

	idx = 0;
	v_bfr = v;
	while(v_bfr > 9)
	{
		v_bfr = div8(v_bfr,10,&rem2);
		res_bfr[idx]=rem2+0x30;
		idx++;
	}
	res_bfr[idx] = v_bfr + 0x30;
	for(uint8_t c=0;c<=idx;c++)
	{
		res[c]=res_bfr[idx-c];
	}
}


uint32_t div32(uint32_t dividend,uint32_t divisor,uint32_t* rem)
{
	uint32_t bfr;
	uint32_t res=0;
	bfr=dividend;
	while(bfr > divisor && bfr <= dividend)
	{
		res++;
		bfr -=divisor;
	}
	if(rem!=0)
	{
		*rem=bfr;
	}
	return res;
}

uint8_t div8(uint8_t dividend,uint8_t divisor,uint8_t* rem)
{
	uint8_t bfr;
	uint8_t res=0;
	bfr=dividend;
	while(bfr >= divisor)
	{
		res++;
		bfr -=divisor;
	}
	if(rem!=0)
	{
		*rem=bfr;
	}
	return res;
}


uint32_t sineTaylor(uint32_t p)
{
	uint32_t bfr;
	uint32_t res;
	bfr=p*p;
	bfr = bfr >> 16 & 0xFFFF;
	bfr = bfr*p;
	bfr = div32(bfr,3170,0);
	//bfr /= 3170;
	res = (p*3 - bfr);
	return(res);
}

/*returns an approximated sine value with 16 bit resolution for both the phase value and and returned sine value
 * input and output are actually unsigned 16bit integer stored in a 32bit variable to save a few processing steps for typecasting
 * */
uint32_t sineVal(uint32_t phase)
{
	uint32_t res;
	if ((phase & 0x0000c000)  == 0)
	{
		res = sineTaylor(phase) + 0x7FFF;
	}
	else if ((phase & 0x0000c000) == 1 << 14)
	{
		uint32_t phase_rel = 0x7FFF - phase;
		res = 0x7FFF + sineTaylor(phase_rel);
	}
	else if ((phase & 0x0000c000) == 2 << 14)
	{
		uint32_t phase_rel = phase - 0x7FFF;
		res = 0x7FFF - sineTaylor(phase_rel);
	}
	else
	{
		uint32_t phase_rel = 0xFFFF - phase;
		res = 0x7FFF - sineTaylor(phase_rel);
	}

	return res;
	//taylor series: x - x^3/6
}


