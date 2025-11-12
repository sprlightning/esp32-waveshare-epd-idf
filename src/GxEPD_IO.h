/*****************************************************************************
* | File      	:   GxEPD_IO.h
* | Author      :   MLX
* | Function    :   Define EPD-IO of GoodDisplay's ESP32-L
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2025-11-12
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef _GxEPD_IO_H_
#define _GxEPD_IO_H_

/**
 * IO备注 - 适用于GoodDisplay的ESP32-L
 * A0-IO36 // TP_INT
 * A3-IO39 // TP_RST
 * A6-IO34
 * A7-IO35
 * A4-IO32-T9
 * A5-IO33-T8
 * A18-IO25-DAC1
 * A19-IO26-DAC2
 * A17-IO27-T7-CS // EPD
 * A16-IO14-T6-D/C // EPD
 * A15-IO12-T5-RES // EPD
 * A14-IO13-T4-BUSY // EPD
 * A13-IO15-T3-SPI_CS1 // 外部字库GT30L32S4W
 * A12-IO2-T2-SPI_CS2 //外部Flash
 * A11-IO0-T1
 * A10-IO4-T0 // LED_GREEN
 * IO18-SPI_SCK // EPD
 * IO19-SPI_MISO // 未使用
 * IO21-SDA // TP
 * IO3-RXD
 * IO1-TXD
 * IO22-SCL // TP
 * IO23-SPI_MOSI // EPD
 * 
 */
/////////////////////define of Analog IO/////////////////////////
#ifndef A0 
#define A0 36
#endif

#ifndef A3
#define A3 39
#endif

#ifndef A6
#define A6 34
#endif

#ifndef A7 
#define A7 35
#endif

#ifndef A4
#define A4 32
#endif

#ifndef A5
#define A5 33
#endif

#ifndef A18
#define A18 25
#endif

#ifndef A19
#define A19 26
#endif

#ifndef A17
#define A17 27
#endif

#ifndef A16
#define A16 14
#endif

#ifndef A15
#define A15 12
#endif

#ifndef A14
#define A14 13
#endif

#ifndef A13
#define A13 15
#endif

#ifndef A12
#define A12 2
#endif

#ifndef A11
#define A11 0
#endif

#ifndef A10
#define A10 4
#endif

/////////////////////define of Digital IO/////////////////////////
#ifndef T9
#define T9 (A4)
#endif

#ifndef T8
#define T8 (A5)
#endif

#ifndef T7
#define T7 (A17)
#endif

#ifndef T6
#define T6 (A16)
#endif

#ifndef T5
#define T5 (A15)
#endif

#ifndef T4
#define T4 (A14)
#endif

#ifndef T3
#define T3 (A13)
#endif

#ifndef T2
#define T2 (A12)
#endif

#ifndef T1
#define T1 (A11)
#endif

#ifndef T0
#define T0 (A10)
#endif
/////////////////////end of IO define/////////////////////////

#endif // _GxEPD_IO_H_
/////////////////////end of file/////////////////////////
