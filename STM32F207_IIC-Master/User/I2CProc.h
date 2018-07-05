
#ifndef I2CProc_H
#define I2CProc_H

#include "stm32f2xx.h"



void I2CWork(int mode, int slaveNum, uint8_t *slaveAddrBuffer);

#endif

