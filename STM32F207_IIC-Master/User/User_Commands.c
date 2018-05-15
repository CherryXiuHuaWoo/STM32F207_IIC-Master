
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
#define CommanfRx_Fail	0xf0

#define TrtgCommandHead0	0x88
#define TrigCommandHead1	0x77
#define TrigCommandHead2	0x66
#define I2CTrigLen			0x03

#define I2CTrigBufLen		0x7

uint8_t g_SlaveAddr = 0;
extern uint8_t g_usart2_rx_buf[USART_BUF_SIZE];    /*usart2_rx_buf*/

/*
 * Function: Command  Management
 */
void UserCommand(uint8_t *CommandBuf, int BufLen)
{
	
	int i;
	uint8_t ReplyBuf[CommandBufLen] = {CommandBufHead0, CommandBufHead1, CommandBufHead2, CommandBufHead3};
	uint8_t I2CTrigBuf[7];

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
					ReplyBuf[3] = CommandRx_OK;
					break;
					
				case CONFIG_MODE:
					ReplyBuf[3] = CommandRx_OK;
					break;
				
				case INTERFACE_MODE:
					ReplyBuf[3] = (CommandBuf[2] < 3) ? CommandRx_OK : CommanfRx_Fail;					
					break;
				
				case OUTPUT_FORMAT: 
					ReplyBuf[3] = (CommandBuf[2] < 5) ? CommandRx_OK : CommanfRx_Fail;
					break;
				
				case MEASURE_PERIOD: 
					ReplyBuf[3] = CommandRx_OK;					
					break;		
				
				case OUTPUT_PERIOD:
					ReplyBuf[3] = CommandRx_OK;
					break;
				
				case UART_BOUNDRATE:
					ReplyBuf[3] = (CommandBuf[2] < 0x13) ? CommandRx_OK : CommanfRx_Fail;
					break;

				/***0x1n:Algorthm Command***/
				case INTTIME:
					ReplyBuf[3] = ((CommandBuf[2] == 0) ||
								   (CommandBuf[2] == 3) ||
								   (CommandBuf[2] == 7)) ? CommandRx_OK : CommanfRx_Fail;		
					break;
				
				case INTTIME_MODE:
					ReplyBuf[3] = (CommandBuf[2] < 0x02) ? CommandRx_OK : CommanfRx_Fail;
					break;
				
				case KALMAN_FILTER:
					ReplyBuf[3] = (CommandBuf[2] < 0x02) ? CommandRx_OK : CommanfRx_Fail;
					break;
				
				case AMB_COMP_MODE:
					ReplyBuf[3] = (CommandBuf[2] < 0x02) ? CommandRx_OK : CommanfRx_Fail;
					break;
				
				case REF_COMP_MODE:
					ReplyBuf[3] = (CommandBuf[2] < 0x02) ? CommandRx_OK : CommanfRx_Fail;
					break;
				
				case MAXRANGE_MODE:
					ReplyBuf[3] = (CommandBuf[2] < 0x02) ? CommandRx_OK : CommanfRx_Fail;
					break;
				
				case UNIT:
					ReplyBuf[3] = (CommandBuf[2] < 0x02) ? CommandRx_OK : CommanfRx_Fail;
					break;
				
				case TILIT_TOOLINT_DEFAULTS:
					ReplyBuf[3] = (CommandBuf[2] < 0x1) ? CommandRx_OK : CommanfRx_Fail;
					break;

				/***0x2n:Extend Algorthm Command***/
				case AMP_THRE_MIN:
					ReplyBuf[3] = CommandRx_OK;
					break;
				
				case AMP_THRE_MAX:
					ReplyBuf[3] = CommandRx_OK;
					break;
				
				case BLIND_THRE_MAX:
					ReplyBuf[3] = CommandRx_OK;
					break;
				
				case INT3TO0_TRANSAMMPTHRES:
					ReplyBuf[3] = (CommandBuf[2] < 0x1) ? CommandRx_OK : CommanfRx_Fail;
					break;

				/***0x3n:Compensation Command***/
				case OFFSET:
					ReplyBuf[3] = CommandRx_OK;
					break;
				
				case KALAM_SYSTEM_COV_INT:
					ReplyBuf[3] = CommandRx_OK;
					break;
				
				case KALAM_SYSTEM_COV_NOR:
					ReplyBuf[3] = CommandRx_OK;
					break;
				
				case KALAM_DISTLIMIT_IDX:
					ReplyBuf[3] = CommandRx_OK;
					break;

				/***0x4n:Trigger Command***/
				case SET_IIC_ADDR:  
					ReplyBuf[3] = (CommandBuf[2] < 0x1) ? CommandRx_OK : CommanfRx_Fail;
					g_SlaveAddr = CommandBuf[4];
					break;

				default:
					ReplyBuf[3] = 0xff;
					break;
			}
			
			if(ReplyBuf[3] == CommandRx_OK)
			{
				if(HAL_I2C_Master_Transmit(&hi2c1, g_SlaveAddr, &CommandBuf[4], I2CTxBufLen, 10) != HAL_OK)
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
	else if(BufLen == I2CTrigLen)
	{
		if((CommandBuf[0] == TrtgCommandHead0) && 
		   (CommandBuf[1] == TrigCommandHead1) &&
		   (CommandBuf[2] == TrigCommandHead2))
		{
			if(HAL_I2C_Master_Receive(&hi2c1, g_SlaveAddr, I2CTrigBuf, I2CTrigBufLen,10) == HAL_OK)
			{
				printf("cordist=%5d, amp=%5d, inttime=%5d\r\n", 
						(I2CTrigBuf[0] | (I2CTrigBuf[1] << 8)),
						(I2CTrigBuf[2] | (I2CTrigBuf[3] << 8)),
						I2CTrigBuf[4]);
			}
			else
			{
				printf("Err: I2C Trigger\r\n");
			}
		}
		else
		{
			printf("Err: I2C Trigger Command\r\n");
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


