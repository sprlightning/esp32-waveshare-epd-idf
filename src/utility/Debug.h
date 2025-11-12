/*****************************************************************************
* | File      	:	Debug.h
* | Author      :   Waveshare team; MLX
* | Function    :	debug with printf
* | Info        :
*   Image scanning
*      Please use progressive scanning to generate images or fonts
*----------------
* |	This version:   V1.0
* | Date        :   2025-11-13
* | Info        :   Basic version
*
******************************************************************************/
#ifndef __DEBUG_H
#define __DEBUG_H

// #include <Wire.h>
#include "NeverUseArduino.h"

#define USE_DEBUG 1
#if USE_DEBUG
	#define Debug(__info) Serial.print(__info)
#else
	#define Debug(__info)  
#endif

#endif

