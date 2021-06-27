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
