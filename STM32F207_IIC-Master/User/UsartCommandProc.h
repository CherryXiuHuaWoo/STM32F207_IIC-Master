
#ifndef User_Commands_H
#define User_Commands_H

#include "stm32f2xx.h"

#define USART_BUF_SIZE 		64

struct WorkMode
{
	int workMode;
	int I2CSlaveNum;
	uint8_t I2CSlaveAddr[4];
};


void UsartInit(void);
void UserCommandExitCheck(void);

#endif

