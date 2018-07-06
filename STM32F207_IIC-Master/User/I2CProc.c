
#include "I2CProc.h"
#include "stm32f2xx.h"
#include "i2c.h"

#define WriteBufferBytes	8
#define ReadBufferBytes		8

uint8_t gI2CWriteBuffer[WriteBufferBytes] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
uint8_t gI2CReadBuff[4][ReadBufferBytes];




void I2CReadDataUpdate(uint8_t SlaveAddr, uint8_t *readDataBuffer, uint32_t bufBytes)
{
	uint32_t bufIdx;
	
	printf("0x%2x Data:", SlaveAddr);
	for(bufIdx = 0; bufIdx < bufBytes; bufIdx++)
	{
		printf("0x%2x ", readDataBuffer[bufIdx]);
	}
	printf("\r\n");
}

void I2CProcCheck(int writtenOperation, HAL_StatusTypeDef status)
{
	switch(status)
	{
		case HAL_OK:
			if(writtenOperation < 0)
			{
				printf("I2C Written Done\r\n");
			}
			break;
		case HAL_ERROR:
			printf("I2C Written/Read error\r\n");
			break;
		case HAL_BUSY:
			printf("I2C Written/Read Busy\r\n");
			break;
		case HAL_TIMEOUT:
			printf("I2C Written/Read timeout\r\n");
			break;
		default:
			printf("I2C unkown error\r\n");
			break;
	}
}
void I2CWriteToSlave(int slaveNum, uint8_t *slaveAddrBuffer)
{
	int slaveIdx;
	HAL_StatusTypeDef writeStatus;
	
	for(slaveIdx = 0; slaveIdx < slaveNum; slaveIdx++)
	{
		writeStatus = HAL_I2C_Master_Transmit_DMA(&hi2c1, slaveAddrBuffer[slaveIdx]<<1, gI2CWriteBuffer, WriteBufferBytes);
		I2CProcCheck(-1, writeStatus);
	}
}

void I2CReadFromSlave(int slaveNum, uint8_t *slaveAddrBuffer)
{
	int slaveIdx;
	HAL_StatusTypeDef readStatus;

	for(slaveIdx = 0; slaveIdx < slaveNum; slaveIdx++)
	{
		readStatus = HAL_I2C_Master_Receive(&hi2c1, slaveAddrBuffer[slaveIdx]<<1, &gI2CReadBuff[slaveIdx][0], ReadBufferBytes, 10);
		I2CProcCheck(0, readStatus);
		if(readStatus == HAL_OK)
		{
			I2CReadDataUpdate(slaveAddrBuffer[slaveIdx], &gI2CReadBuff[slaveIdx][0], ReadBufferBytes);
		}
	}
}

void I2CWork(int mode, int slaveNum, uint8_t *slaveAddrBuffer)
{
	if(0x01 == mode)
	{
		I2CReadFromSlave(slaveNum, slaveAddrBuffer);
	}
}	


