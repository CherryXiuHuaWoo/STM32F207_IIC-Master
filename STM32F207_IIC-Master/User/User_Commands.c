
#include "stm32f2xx.h"
#include "User_Commands.h"
#include "stm32f2xx_hal_i2c.h"
#include "usart.h"
#include "i2c.h"

#define CommandBufLen	8
#define I2CTxBufLen		4

#define CommandBufHead0	0x42
#define CommandBufHead1	0x57
#define CommandBufHead2	0x02
#define CommandBufHead3	0x00

#define CommandRx_OK	0x01


/*
 * Function: Command  Management
 */
void UserCommand(uint8_t *CommandBuf, int BufLen)
{
	
	int i;
	uint8_t ReplyBuf[CommandBufLen] = {CommandBufHead0, CommandBufHead1, CommandBufHead2, CommandBufHead3};
	
	if(BufLen == CommandBufLen)
	{
		if((CommandBuf[0] == CommandBufHead0) && 
		   (CommandBuf[1] == CommandBufHead1) &&
		   (CommandBuf[2] == CommandBufHead2) &&
		   (CommandBuf[3] == CommandBufHead3))
		{
			
			for(i = 4; i < CommandBufLen; i++)
			{
				ReplyBuf[i] = CommandBuf[i];
			}
			
			switch(CommandBuf[BufLen - 1])
			{
				/***0x0n: System Command***/
				case SYSTEM_RESET:
					ReplyBuf[3] = 1;
					break;
					
				case CONFIG_MODE:
					ReplyBuf[3] = 1;
					break;
				case INTERFACE_MODE:
					ReplyBuf[3] = 1;					
					break;
				case OUTPUT_FORMAT:
					ReplyBuf[3] = 1;
					break;
				case MEASURE_PERIOD: 
					ReplyBuf[3] = 1;					
					break;					
				case OUTPUT_PERIOD:
					ReplyBuf[3] = 1;
					break;
				case UART_BOUNDRATE:
					ReplyBuf[3] = 1;
					break;

				/***0x1n:Algorthm Command***/
				case INTTIME:
					ReplyBuf[3] = 1;
					break;
				case INTTIME_MODE:
					ReplyBuf[3] = 1;
					break;
				case KALMAN_FILTER:
					ReplyBuf[3] = 1;
					break;
				case AMB_COMP_MODE:
					ReplyBuf[3] = 1;
					break;
				case REF_COMP_MODE:
					ReplyBuf[3] = 1;
					break;
				case MAXRANGE_MODE:
					ReplyBuf[3] = 1;
					break;
				case UNIT:
					ReplyBuf[3] = 1;
					break;
				case TILIT_TOOLINT_DEFAULTS:
					ReplyBuf[3] = 1;
					break;

				/***0x2n:Extend Algorthm Command***/
				case AMP_THRE_MIN:
					ReplyBuf[3] = 1;
					break;
				case AMP_THRE_MAX:
					ReplyBuf[3] = 1;
					break;
				case BLIND_THRE_MAX:
					ReplyBuf[3] = 1;
					break;
				case INT3TO0_TRANSAMMPTHRES:
					ReplyBuf[3] = 1;
					break;

				/***0x3n:Compensation Command***/
				case OFFSET:
					ReplyBuf[3] = 1;
					break;
				case KALAM_SYSTEM_COV_INT:
					ReplyBuf[3] = 1;
					break;
				case KALAM_SYSTEM_COV_NOR:
					ReplyBuf[3] = 1;
					break;
				case KALAM_DISTLIMIT_IDX:
					ReplyBuf[3] = 1;
					break;

				/***0x4n:Trigger Command***/
				case SET_IIC_ADDR:  
					ReplyBuf[3] = 1;
					break;
				case USER_CONFIG_DEFAULTS:
					ReplyBuf[3] = 1;
					break;
				default:
					ReplyBuf[3] = 0xff;
					break;
			}
			
			if(ReplyBuf[3] == CommandRx_OK)
			{
				if(HAL_I2C_Master_Transmit(&hi2c1, 0x00, &CommandBuf[4], I2CTxBufLen, 10) != HAL_OK)
				{
					printf("Err: I2C Send Command!\r\n");	
				}
			}
			
			if(HAL_UART_Transmit(&huart2, ReplyBuf, CommandBufLen, 10) != HAL_OK)
			{
				printf("Err: Command Reply!\r\n");	
			}
		}
		else
		{
			printf("Err: Command Head = 0x%2x 0x%2x 0x%2x 0x%2x\r\n", 
				CommandBuf[0], CommandBuf[1], CommandBuf[2], CommandBuf[3]);
		}
	}
	else
	{
		printf("Err: Command Length = %d\r\n", BufLen);
	}
}



/*************************************************
Function: USART2_RX_Proc
Description: USART2_RX_Proc
Input:
    buf, data buffer
    len, data len
Output: None
Return: None
Others: None
*************************************************/
void USART2_RX_Proc(uint8_t *buf, uint32_t len)
{
	UserCommand(buf, len);
}


