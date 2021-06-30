
#ifndef MINILIB_H_
#define MINILIB_H_

uint32_t div32(uint32_t dividend,uint32_t divisor,uint32_t* rem);
uint8_t div8(uint8_t dividend,uint8_t divisor,uint8_t* rem);
int printf(const char * data);
void sendChar(uint8_t data);
void toChar(uint8_t v,char* res);

uint32_t sineVal(uint32_t phase);
uint32_t sineTaylor(uint32_t p);

#endif
