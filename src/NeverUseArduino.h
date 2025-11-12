/*****************************************************************************
* | File      	:   NeverUseArduino.h
* | Author      :   MLX
* | Function    :   Config Arduino's Functions in ESP-IDF
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2025-11-13
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
#ifndef _NEVERUSEARDUINO_H_
#define _NEVERUSEARDUINO_H_

#define NO_ARDUINO (1) // 如果仅使用ESP-IDF，需打开此定义
#define USE_GxEPD (1)  // 如果使用GoodDisplay的ESP32-L，需打开此定义


#if defined(NO_ARDUINO)
#include "AudioTools.h" // 引入采用idf方法重构的Arduino基本函数
#include "driver/spi_master.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#else
#include "Arduino.h"
#include "Wire.h"
#endif

#if defined(USE_GxEPD)
#include "GxEPD_IO.h"
#endif

#endif
