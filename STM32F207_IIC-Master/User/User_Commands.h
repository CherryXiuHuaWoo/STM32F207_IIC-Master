
#ifndef User_Commands_H
#define User_Commands_H

#include "stm32f2xx.h"

/***0x0n: System Command***/
#define SYSTEM_RESET            0x01
#define CONFIG_MODE             0x02
#define INTERFACE_MODE          0x03                                     
#define OUTPUT_FORMAT           0x06
#define MEASURE_PERIOD			0x07                                        
#define OUTPUT_PERIOD           0x09
#define UART_BOUNDRATE          0x08

/***0x1n:Algorthm Command***/
#define INTTIME                 0x11
#define INTTIME_MODE            0x14
#define KALMAN_FILTER           0x16
#define AMB_COMP_MODE           0x17
#define REF_COMP_MODE           0x18    
#define MAXRANGE_MODE           0x19
#define UNIT                    0x1a
#define TILIT_TOOLINT_DEFAULTS  0x1d

/***0x2n:Extend Algorthm Command***/
#define AMP_THRE_MIN            0x20
#define AMP_THRE_MAX            0x21
#define BLIND_THRE_MAX          0x22
#define INT3TO0_TRANSAMMPTHRES	0x23

/***0x3n:Compensation Command***/
#define OFFSET                  0x31
#define KALAM_SYSTEM_COV_INT    0x32
#define KALAM_SYSTEM_COV_NOR    0x33
#define KALAM_DISTLIMIT_IDX     0x34


/***0x4n:Trigger Command***/
#define SET_IIC_ADDR              	0x43    
#define USER_CONFIG_DEFAULTS        0xff    /*恢复用户一键配置指令*/

void UserCommand(uint8_t *CommandBuf, int BufLen);
void USART2_RX_Proc(uint8_t *buf, uint32_t len);

#endif

