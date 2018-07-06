
#include "stm32f2xx.h"
#include "UsartCommandProc.h"
#include "stm32f2xx_hal_i2c.h"
#include "usart.h"

extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern struct WorkMode __workMode;

uint8_t  gUsartProcBuffer[USART_BUF_SIZE];
uint8_t  gUsartRxBuffer[USART_BUF_SIZE];
uint32_t gUsartRxBytes;


void UsartInit(void)
{
	HAL_UART_Receive_DMA(&huart2, gUsartRxBuffer, USART_BUF_SIZE);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
}


int I2CTestModeProc(uint8_t testStatus)
{
	__workMode.workMode = testStatus;
	return 1;
}


int I2CSlaveProc(uint8_t SlaveNum, uint8_t *SlaveAddrBuf)
{
	uint8_t bufIdx;
	
	if(SlaveNum < 5)
	{
		for(bufIdx = 0; bufIdx < SlaveNum; bufIdx++)
		{
			__workMode.I2CSlaveAddr[bufIdx] = SlaveAddrBuf[bufIdx];
		}
		__workMode.I2CSlaveNum = SlaveNum;
		return 1;
	}
	return -1;
}


int CommandFormatCheck(uint8_t *commandBuffer, uint32_t BufferBytes)
{
	int CheckStatus = ((0x5A == commandBuffer[0]) && (0xA5 == commandBuffer[1]) && (8 == BufferBytes)) ? 0 : -1;
	
	return CheckStatus;
}


void CommandProcUpdate(int procStatus, uint8_t commandCode)
{
	if(procStatus > 0)
	{
		printf("Command 0x%2x proc successful\r\n", commandCode);
	}
	else
	{
		printf("Command 0x%2x proc unsuccessful\r\n", commandCode);
	}
}


void UserCommandProc(uint8_t *commandBuffer, uint32_t BufferBytes)
{
	int procStatus = 0;
	
	if(CommandFormatCheck(commandBuffer, BufferBytes) < 0)
	{
		printf("CommandFormatCheck failed\r\n");
		return;
	}
	
	switch(commandBuffer[BufferBytes - 1])
	{
		case 0x01:
			procStatus = I2CTestModeProc(commandBuffer[BufferBytes - 2]);
			break;
		case 0x02:
			procStatus = I2CSlaveProc(commandBuffer[BufferBytes - 2], &commandBuffer[2]);
			break;
		default:
			break;
	}
	CommandProcUpdate(procStatus, commandBuffer[BufferBytes - 1]);
}


void UserCommandExitCheck(void)
{
	if(gUsartRxBytes > 0)
	{
		UserCommandProc(gUsartProcBuffer, gUsartRxBytes);
		gUsartRxBytes = 0;
	}
}


void UsartSaveRxDataToProcBuffer(uint32_t RxDataBytes)
{
	uint32_t bufIdx;

	for(bufIdx = 0; bufIdx < RxDataBytes; bufIdx++)
	{
		gUsartProcBuffer[bufIdx] = gUsartRxBuffer[bufIdx];
	}
}


void HAL_UART_IDLECallback(UART_HandleTypeDef *huart)
{
	uint32_t clearStatus = 0;

	if((__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != RESET))
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);

		clearStatus = huart2.Instance->SR;
		clearStatus = huart2.Instance->DR;

		HAL_UART_DMAStop(&huart2);
		gUsartRxBytes = USART_BUF_SIZE - hdma_usart2_rx.Instance->NDTR;
		HAL_UART_Receive_DMA(&huart2, gUsartRxBuffer, USART_BUF_SIZE);
		
		UsartSaveRxDataToProcBuffer(gUsartRxBytes);
	}
}



