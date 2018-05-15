
# STM32F207_IIC-Master
Use STM32F207ZGT6U as IIC Master。

## Overview
---
**Hardware**： NUCLEO-F207ZG (STM32F207ZGT6U)

**Input Frequency**： 8 MHz

**Rescures:** http://www.st.com/zh/evaluation-tools/nucleo-f207zg.html

## Log
---
### 2018/5/11:
- USART2(PD5/PD6) Printf done.
- Find the Intput Frequnency Value is 8 Mhz. But I don't know why?

### 2018/5/15:
- Add I2C interface.
- Add UART Command Proc and I2C Command Proc.

## DOCS
---
### Interface
UART:
- USART2
- Boundrate: 921600
- Tx Pin: PD5
- Rx Pin: PD6

I2C:
- I2C1
- Clock Speed: 40KHz

### Command 

#### Trigger Command：0x88 0x77 0x66
- Trigger Mini in I2C Mode
- Receive the dist message from Mini, and printf the message into UART
- If this Command receive error, UART printf "Err: I2C Trigger Command\r\n"
- If Trigger fail, UART printf "Err: I2C Trigger\r\n"
- If Trigger succeed, URT will printf the dist message "cordist=%5d, amp=%5d, inttime=%5d\r\n"

#### Mini UART Command and I2C Command
This program code work in NUCLEO can receive Mini UART Command by UART, and send to Mini working in I2C Mode by I2C Interface.  

NOTE:
- Mini UART Command's length is 8 bytes.
- Mini I2C Command's length is 4 bytes.
- All the I2C Command is the last 4 bytes in each UART Command.
- If Set the I2C Slave Address by UART, and the next time Trigger will this I2C Slave Address. After power off, the I2C Slave Address will reset as 0x00. User should set the I2C Slave Address in each power on.

For example:

UART Command: 0x42 0x57 0x02 0x00 0x00 0x00 0x01 0x06
Transform into I2C is: 0x00 0x00 0x01 0x06

