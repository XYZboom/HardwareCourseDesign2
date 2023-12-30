//
// Created by XYZboom on 2023/9/19.
//

#ifndef DESIGN2_PROJECT_CONSTANTS_H
#define DESIGN2_PROJECT_CONSTANTS_H

//<editor-fold desc="MIO">
#define __MIO_PIN(N) (* (volatile unsigned long *) (0xF800700 + (N << 2)))

#define MIO_PIN_0 __MIO_PIN(0)
#define MIO_PIN_1 __MIO_PIN(1)
#define MIO_PIN_2 __MIO_PIN(2)
#define MIO_PIN_3 __MIO_PIN(3)
#define MIO_PIN_4 __MIO_PIN(4)
#define MIO_PIN_5 __MIO_PIN(5)
#define MIO_PIN_6 __MIO_PIN(6)
#define MIO_PIN_7 __MIO_PIN(7)
#define MIO_PIN_8 __MIO_PIN(8)
#define MIO_PIN_9 __MIO_PIN(9)
#define MIO_PIN_10 __MIO_PIN(10)
#define MIO_PIN_11 __MIO_PIN(11)
#define MIO_PIN_12 __MIO_PIN(12)
#define MIO_PIN_13 __MIO_PIN(13)
#define MIO_PIN_14 __MIO_PIN(14)
#define MIO_PIN_15 __MIO_PIN(15)
#define MIO_PIN_16 __MIO_PIN(16)
#define MIO_PIN_17 __MIO_PIN(17)
#define MIO_PIN_18 __MIO_PIN(18)
#define MIO_PIN_19 __MIO_PIN(19)
#define MIO_PIN_20 __MIO_PIN(20)
#define MIO_PIN_21 __MIO_PIN(21)
#define MIO_PIN_22 __MIO_PIN(22)
#define MIO_PIN_23 __MIO_PIN(23)
#define MIO_PIN_24 __MIO_PIN(24)
#define MIO_PIN_25 __MIO_PIN(25)
#define MIO_PIN_26 __MIO_PIN(26)
#define MIO_PIN_27 __MIO_PIN(27)
#define MIO_PIN_28 __MIO_PIN(28)
#define MIO_PIN_29 __MIO_PIN(29)
#define MIO_PIN_30 __MIO_PIN(30)
#define MIO_PIN_31 __MIO_PIN(31)
#define MIO_PIN_32 __MIO_PIN(32)
#define MIO_PIN_33 __MIO_PIN(33)
#define MIO_PIN_34 __MIO_PIN(34)
#define MIO_PIN_35 __MIO_PIN(35)
#define MIO_PIN_36 __MIO_PIN(36)
#define MIO_PIN_37 __MIO_PIN(37)
#define MIO_PIN_38 __MIO_PIN(38)
#define MIO_PIN_39 __MIO_PIN(39)
#define MIO_PIN_40 __MIO_PIN(40)
#define MIO_PIN_41 __MIO_PIN(41)
#define MIO_PIN_42 __MIO_PIN(42)
#define MIO_PIN_43 __MIO_PIN(43)
#define MIO_PIN_44 __MIO_PIN(44)
#define MIO_PIN_45 __MIO_PIN(45)
#define MIO_PIN_46 __MIO_PIN(46)
#define MIO_PIN_47 __MIO_PIN(47)
#define MIO_PIN_48 __MIO_PIN(48)
#define MIO_PIN_49 __MIO_PIN(49)
#define MIO_PIN_50 __MIO_PIN(50)
#define MIO_PIN_51 __MIO_PIN(51)
#define MIO_PIN_52 __MIO_PIN(52)
#define MIO_PIN_53 __MIO_PIN(53)
//</editor-fold>
//<editor-fold desc="UART">
#define UART_CLK_CTRL                   (* (volatile unsigned long *) 0xF8000154)
#define UART_CONTROL_REG0               (* (volatile unsigned long *) 0xE0001000)
#define UART_MODE_REG0                  (* (volatile unsigned long *) 0xE0001004)
#define UART_BAUD_RATE_GEN_REG0         (* (volatile unsigned long *) 0xE0001018)
#define UART_BAUD_RATE_DIVIDER_REG0     (* (volatile unsigned long *) 0xE0001034)
#define UART_Tx_Rx_FIFO0                (* (volatile unsigned long *) 0xE0001030)
#define UART_CHANNEL_STS_REG0           (* (volatile unsigned long *) 0xE000102C)
//</editor-fold>

// 前导码
#define MODBUS_PREAMBLE ((unsigned char) 0x23)
#ifdef SIMULATION
#include <windows.h>
#define SIMULATION_COM ("COM2")
#define DEBUG_COM_BUFFER_SIZE (16)
#define DEBUG_COM_BAUD_RATE (CBR_115200)
extern HANDLE COM_HANDLE;
#endif

#endif //DESIGN2_PROJECT_CONSTANTS_H
