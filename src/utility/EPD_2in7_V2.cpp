/*****************************************************************************
* | File      	:   EPD_2in7_V2.c
* | Author      :   Waveshare team
* | Function    :   2.7inch V2 e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-08-18
* | Info        :
* -----------------------------------------------------------------------------
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
#include "EPD_2in7_V2.h"
#include "Debug.h"

UBYTE LUT_DATA_4Gray[159] =
{
0x40,	0x48,	0x80,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x8,	0x48,	0x10,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x2,	0x48,	0x4,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x20,	0x48,	0x1,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0xA,	0x19,	0x0,	0x3,	0x8,	0x0,	0x0,					
0x14,	0x1,	0x0,	0x14,	0x1,	0x0,	0x3,					
0xA,	0x3,	0x0,	0x8,	0x19,	0x0,	0x0,					
0x1,	0x0,	0x0,	0x0,	0x0,	0x0,	0x1,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x22,	0x22,	0x22,	0x22,	0x22,	0x22,	0x0,	0x0,	0x0,			
0x22,	0x17,	0x41,	0x0,	0x32,	0x1C
};

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN7_V2_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(20);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(2);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(20);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_2IN7_V2_SendCommand(UBYTE Reg)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_2IN7_V2_SendData(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
static void EPD_2IN7_V2_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    do {
        if(DEV_Digital_Read(EPD_BUSY_PIN) == 0)
			break;
    } while(1);
    DEV_Delay_ms(20);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Setting the display window
parameter:
	Xstart : X-axis starting position
	Ystart : Y-axis starting position
	Xend : End position of X-axis
	Yend : End position of Y-axis
note: 这是原始坐标系，x为垂直方向，y为水平方向
******************************************************************************/
static void EPD_2IN7_V2_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    // 设置RAM X地址范围（垂直方向）
    EPD_2IN7_V2_SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    EPD_2IN7_V2_SendData((Xstart>>3) & 0xFF);
    EPD_2IN7_V2_SendData((Xend>>3) & 0xFF);
	
    // 设置RAM Y地址范围（水平方向）
    EPD_2IN7_V2_SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    EPD_2IN7_V2_SendData(Ystart & 0xFF);
    EPD_2IN7_V2_SendData((Ystart >> 8) & 0xFF);
    EPD_2IN7_V2_SendData(Yend & 0xFF);
    EPD_2IN7_V2_SendData((Yend >> 8) & 0xFF);
}

/******************************************************************************
function :	Set Cursor
parameter:
	Xstart : X-axis starting position
	Ystart : Y-axis starting position
******************************************************************************/
static void EPD_2IN7_V2_SetCursor(UWORD Xstart, UWORD Ystart)
{
    // 设置RAM X地址计数器
    EPD_2IN7_V2_SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    EPD_2IN7_V2_SendData(Xstart & 0xFF);

    // 设置RAM Y地址计数器
    EPD_2IN7_V2_SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    EPD_2IN7_V2_SendData(Ystart & 0xFF);
    EPD_2IN7_V2_SendData((Ystart >> 8) & 0xFF);
}

/******************************************************************************
function :	Turn on display
parameter:
******************************************************************************/
static void EPD_2IN7_V2_TurnOnDisplay(void)
{
	EPD_2IN7_V2_SendCommand(0x22); //Display Update Control
    EPD_2IN7_V2_SendData(0xF7);
	EPD_2IN7_V2_SendCommand(0x20); //Activate Display Update Sequence
    EPD_2IN7_V2_ReadBusy();
}

static void EPD_2IN7_V2_TurnOnDisplay_Fast(void)
{
	EPD_2IN7_V2_SendCommand(0x22); //Display Update Control
    EPD_2IN7_V2_SendData(0xC7);
	EPD_2IN7_V2_SendCommand(0x20); //Activate Display Update Sequence
    EPD_2IN7_V2_ReadBusy();
}

static void EPD_2IN7_V2_TurnOnDisplay_Partial(void)
{
    EPD_2IN7_V2_SendCommand(0x22);
    EPD_2IN7_V2_SendData(0xFF);
	EPD_2IN7_V2_SendCommand(0x20);
    EPD_2IN7_V2_ReadBusy();
}

static void EPD_2IN7_V2_TurnOnDisplay_4GRAY(void)
{
    EPD_2IN7_V2_SendCommand(0x22);
    EPD_2IN7_V2_SendData(0xC7);
	EPD_2IN7_V2_SendCommand(0x20);
    EPD_2IN7_V2_ReadBusy();
}
/******************************************************************************
function :	set the look-up tables
parameter:
******************************************************************************/
static void EPD_2IN7_V2_Lut(void)
{
    unsigned int count;
    EPD_2IN7_V2_SendCommand(0x32); //vcom
    for(count = 0; count < 153; count++) {
        EPD_2IN7_V2_SendData(LUT_DATA_4Gray[count]);
    }
}


/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN7_V2_Init(void)
{
    EPD_2IN7_V2_Reset();
	EPD_2IN7_V2_ReadBusy();
	
	EPD_2IN7_V2_SendCommand(0x12); //SWRESET
	EPD_2IN7_V2_ReadBusy();

    EPD_2IN7_V2_SendCommand(0x45); //set Ram-Y address start/end position          
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x07); //0x0107-->(263+1)=264
	EPD_2IN7_V2_SendData(0x01);

    EPD_2IN7_V2_SendCommand(0x4F);   // set RAM y address count to 0;    
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x00);

    EPD_2IN7_V2_SendCommand(0x11);   // data entry mode
	EPD_2IN7_V2_SendData(0x03);

}
void EPD_2IN7_V2_Init_Fast(void)
{
    EPD_2IN7_V2_Reset();
    EPD_2IN7_V2_ReadBusy();

    EPD_2IN7_V2_SendCommand(0x12); //SWRESET
    EPD_2IN7_V2_ReadBusy();

    EPD_2IN7_V2_SendCommand(0x18); //Read built-in temperature sensor
    EPD_2IN7_V2_SendData(0x80);	

    EPD_2IN7_V2_SendCommand(0x22); // Load temperature value
    EPD_2IN7_V2_SendData(0xB1);		
    EPD_2IN7_V2_SendCommand(0x20);	
    EPD_2IN7_V2_ReadBusy();

    EPD_2IN7_V2_SendCommand(0x1A); // Write to temperature register
    EPD_2IN7_V2_SendData(0x64);		
    EPD_2IN7_V2_SendData(0x00);	

    EPD_2IN7_V2_SendCommand(0x45); //set Ram-Y address start/end position          
	EPD_2IN7_V2_SendData(0x00);   
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x07); //0x0107-->(263+1)=264
	EPD_2IN7_V2_SendData(0x01);

    EPD_2IN7_V2_SendCommand(0x4F);   // set RAM y address count to 0;    
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x00);

    EPD_2IN7_V2_SendCommand(0x11);   // data entry mode      
	EPD_2IN7_V2_SendData(0x03);
                
    EPD_2IN7_V2_SendCommand(0x22); // Load temperature value
    EPD_2IN7_V2_SendData(0x91);		
    EPD_2IN7_V2_SendCommand(0x20);	
    EPD_2IN7_V2_ReadBusy(); 
}
void EPD_2IN7_V2_Init_4GRAY(void)
{
	EPD_2IN7_V2_Reset();

	EPD_2IN7_V2_ReadBusy();
	EPD_2IN7_V2_SendCommand(0x12); // soft reset
	EPD_2IN7_V2_ReadBusy();

	EPD_2IN7_V2_SendCommand(0x74); //set analog block control       
	EPD_2IN7_V2_SendData(0x54);
	EPD_2IN7_V2_SendCommand(0x7E); //set digital block control          
	EPD_2IN7_V2_SendData(0x3B);

	EPD_2IN7_V2_SendCommand(0x01); //Driver output control      
	EPD_2IN7_V2_SendData(0x07);
	EPD_2IN7_V2_SendData(0x01);
	EPD_2IN7_V2_SendData(0x00);

	EPD_2IN7_V2_SendCommand(0x11); //data entry mode       
	EPD_2IN7_V2_SendData(0x03);

	EPD_2IN7_V2_SendCommand(0x44); //set Ram-X address start/end position   
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x15);    //0x15-->(21+1)*8=176

	EPD_2IN7_V2_SendCommand(0x45); //set Ram-Y address start/end position          
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x07);//0x0107-->(263+1)=264
	EPD_2IN7_V2_SendData(0x01);


	EPD_2IN7_V2_SendCommand(0x3C); //BorderWavefrom
	EPD_2IN7_V2_SendData(0x00);	


	EPD_2IN7_V2_SendCommand(0x2C);     //VCOM Voltage
	EPD_2IN7_V2_SendData(LUT_DATA_4Gray[158]);    //0x1C


	EPD_2IN7_V2_SendCommand(0x3F); //EOPQ    
	EPD_2IN7_V2_SendData(LUT_DATA_4Gray[153]);
	
	EPD_2IN7_V2_SendCommand(0x03); //VGH      
	EPD_2IN7_V2_SendData(LUT_DATA_4Gray[154]);

	EPD_2IN7_V2_SendCommand(0x04); //      
	EPD_2IN7_V2_SendData(LUT_DATA_4Gray[155]); //VSH1   
	EPD_2IN7_V2_SendData(LUT_DATA_4Gray[156]); //VSH2   
	EPD_2IN7_V2_SendData(LUT_DATA_4Gray[157]); //VSL   
   
	EPD_2IN7_V2_Lut(); //LUT

	
	EPD_2IN7_V2_SendCommand(0x4E);   // set RAM x address count to 0;
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendCommand(0x4F);   // set RAM y address count to 0X199;    
	EPD_2IN7_V2_SendData(0x00);
	EPD_2IN7_V2_SendData(0x00);
    EPD_2IN7_V2_ReadBusy();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN7_V2_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1);
    Height = EPD_2IN7_V2_HEIGHT;

    EPD_2IN7_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7_V2_SendData(0XFF);
        }
    }

	EPD_2IN7_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
	Image : Image data
******************************************************************************/
void EPD_2IN7_V2_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1);
    Height = EPD_2IN7_V2_HEIGHT;
	
    EPD_2IN7_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7_V2_SendData(Image[i + j * Width]);
        }
    }
	
	EPD_2IN7_V2_TurnOnDisplay();
}

void EPD_2IN7_V2_Display_Fast(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1);
    Height = EPD_2IN7_V2_HEIGHT;
	
    EPD_2IN7_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7_V2_SendData(Image[i + j * Width]);
        }
    }
	EPD_2IN7_V2_TurnOnDisplay_Fast();
}

/******************************************************************************
function :	Refresh a base image
parameter:
	Image : Image data	
note      : 全屏刷新，较慢，刷新时屏幕会黑白屏闪烁
******************************************************************************/
void EPD_2IN7_V2_Display_Base(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1);
    Height = EPD_2IN7_V2_HEIGHT;

	EPD_2IN7_V2_SendCommand(0x24);   //Write Black and White image to RAM
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7_V2_SendData(Image[i + j * Width]);
        }
    }
	EPD_2IN7_V2_SendCommand(0x26);   //Write Black and White image to RAM
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7_V2_SendData(Image[i + j * Width]);
        }
    }
	EPD_2IN7_V2_TurnOnDisplay();	
}

/******************************************************************************
function :	Refresh the background color
parameter:
	color : Color to refresh the background
note      : If you didn't use the EPD_2IN7_V2_Display_Base() function to refresh the image before,
            use the EPD_2IN7_V2_Display_Base_color() function to refresh the background color,
            otherwise the background color will be garbled.
******************************************************************************/
void EPD_2IN7_V2_Display_Base_color(UBYTE color)
{
    UWORD Width, Height;
    Width = (EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1);
    Height = EPD_2IN7_V2_HEIGHT;

	EPD_2IN7_V2_SendCommand(0x24);   //Write Black and White image to RAM
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7_V2_SendData(color);
        }
    }
	EPD_2IN7_V2_SendCommand(0x26);   //Write Black and White image to RAM
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN7_V2_SendData(color);
        }
    }
	// EPD_2IN7_V2_TurnOnDisplay();	
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and partial refresh
parameter:
	Image : Image data
    Xstart : Start X coordinate
    Ystart : Start Y coordinate
    Xend   : End X coordinate
    Yend   : End Y coordinate
note      : 依据输入的范围局部刷新，速度快，刷新时不闪烁
******************************************************************************/
void EPD_2IN7_V2_Display_Partial(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    // 坐标转换（希望传入的是原点在左上角，x水平，y垂直），下面的原始坐标是原点右上角，y水平，x垂直
    UWORD tempXstart = Xstart, tempXend = Xend;
    Xstart = Ystart; // 新的Ystart对应原坐标的Xstart
    Xend = Yend;     // 新的Yend对应原坐标的Xend
    Ystart = EPD_2IN7_V2_HEIGHT - tempXend; // 新Xend转化为原坐标的Ystart（水平反转）
    Yend = EPD_2IN7_V2_HEIGHT - tempXstart; // 新Xstart转化为原坐标的Yend（水平反转）

    // X参数（垂直方向）处理，需为8的倍数，转换为字节数
    if((Xstart % 8 + Xend % 8 == 8 && Xstart % 8 > Xend % 8) || Xstart % 8 + Xend % 8 == 0 || (Xend - Xstart)%8 == 0)
    {
        Xstart = Xstart / 8 ;
        Xend = Xend / 8;
    }
    else
    {
        Xstart = Xstart / 8 ;
        Xend = Xend % 8 == 0 ? Xend / 8 : Xend / 8 + 1;
    }
    

    UWORD i, Width;
    // 计算区域尺寸和数据长度
	Width = Xend -  Xstart;
	UWORD IMAGE_COUNTER = Width * (Yend-Ystart); // 确保Y方向长度为正

    // 调整结束地址（调整为闭区间）
	Xend -= 1;
	Yend -= 1;	

    //Reset
    EPD_2IN7_V2_Reset();

    // 设置边框波形
    EPD_2IN7_V2_SendCommand(0x3C); //BorderWavefrom
    EPD_2IN7_V2_SendData(0x80);	
    
    // 设置RAM X地址范围（垂直方向）
    EPD_2IN7_V2_SendCommand(0x44);       // set RAM x address start/end, in page 35
    EPD_2IN7_V2_SendData(Xstart & 0xff);    // RAM x address start at 00h;
    EPD_2IN7_V2_SendData(Xend & 0xff);    // RAM x address end at 0fh(15+1)*8->128 

    // 设置RAM Y地址范围（水平方向）
    EPD_2IN7_V2_SendCommand(0x45);       // set RAM y address start/end, in page 35
    EPD_2IN7_V2_SendData(Ystart & 0xff);    // RAM y address start at 0127h;
    EPD_2IN7_V2_SendData((Ystart>>8) & 0x01);    // RAM y address start at 0127h;
    EPD_2IN7_V2_SendData(Yend & 0xff);    // RAM y address end at 00h;
    EPD_2IN7_V2_SendData((Yend>>8) & 0x01); 

    // 设置RAM X地址计数器
    EPD_2IN7_V2_SendCommand(0x4E);   // set RAM x address count to 0;
    EPD_2IN7_V2_SendData(Xstart & 0xff); 

    // 设置RAM Y地址计数器
    EPD_2IN7_V2_SendCommand(0x4F);   // set RAM y address count to 0X127;    
    EPD_2IN7_V2_SendData(Ystart & 0xff);
    EPD_2IN7_V2_SendData((Ystart>>8) & 0x01);

    // 发送图像数据
    EPD_2IN7_V2_SendCommand(0x24);   //Write Black and White image to RAM
    for (i = 0; i < IMAGE_COUNTER; i++) {
	    EPD_2IN7_V2_SendData(Image[i]);
	}

    // 执行局部刷新
	EPD_2IN7_V2_TurnOnDisplay_Partial();
}

/******************************************************************************
function :	Full screen partial update display
parameter:
	Image : Image data
note      : 采用局部刷新方法的全屏刷新，速度快，刷新时不闪烁
******************************************************************************/
void EPD_2IN7_V2_Display_Partial_All(UBYTE *Image)
{
	UWORD Width, Height;
    Width = (EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1);
    Height = EPD_2IN7_V2_HEIGHT;
	
	//Reset
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(1);
    DEV_Digital_Write(EPD_RST_PIN, 1);

	EPD_2IN7_V2_SendCommand(0x3C); //BorderWavefrom
	EPD_2IN7_V2_SendData(0x80);	

    // // 控制高度（水平方向）（非必须）
	// EPD_2IN7_V2_SendCommand(0x01); //Driver output control      
    // EPD_2IN7_V2_SendData((EPD_2IN7_V2_HEIGHT-1) & 0xff);
    // EPD_2IN7_V2_SendData(((EPD_2IN7_V2_HEIGHT-1) >> 8) & 0x01);
	// EPD_2IN7_V2_SendData(0x00);
	
    // // 配置数据输入模式（非必须）
	// EPD_2IN7_V2_SendCommand(0x11); //data entry mode       
	// EPD_2IN7_V2_SendData(0x03); // waveshare（正常）
    // // EPD_2IN7_V2_SendData(0x02); // GoodDisplay（花屏）

    // // 设置显示窗口和初始值位置（使用全屏作为显示窗口）（非必须）
	// EPD_2IN7_V2_SetWindows(0, 0, EPD_2IN7_V2_WIDTH - 1, EPD_2IN7_V2_HEIGHT - 1);
	// EPD_2IN7_V2_SetCursor(0, 0);

	EPD_2IN7_V2_SendCommand(0x24);   //Write Black and White image to RAM
    for (UWORD i = 0; i < Width * Height; i++) {
	    EPD_2IN7_V2_SendData(Image[i]);
	}

	EPD_2IN7_V2_TurnOnDisplay_Partial();
}

/******************************************************************************
function :	Show image in 4 gray Levels
parameter:
	Image : Image data
note      : 4级灰度显示
******************************************************************************/
void EPD_2IN7_V2_4GrayDisplay(UBYTE *Image)
{
    UDOUBLE i,j,k;
    UBYTE temp1,temp2,temp3;

    // old  data
    EPD_2IN7_V2_SendCommand(0x24);
    for(i=0; i<5808; i++) {             //5808*4  46464
        temp3=0;
        for(j=0; j<2; j++) {
            temp1 = Image[i*2+j];
            for(k=0; k<2; k++) {
                temp2 = temp1&0xC0;
                if(temp2 == 0xC0)
                    temp3 |= 0x00;
                else if(temp2 == 0x00)
                    temp3 |= 0x01; 
                else if(temp2 == 0x80)
                    temp3 |= 0x01; 
                else //0x40
                    temp3 |= 0x00; 
                temp3 <<= 1;

                temp1 <<= 2;
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0) 
                    temp3 |= 0x00;
                else if(temp2 == 0x00) 
                    temp3 |= 0x01;
                else if(temp2 == 0x80)
                    temp3 |= 0x01; 
                else    //0x40
                    temp3 |= 0x00;	
                if(j!=1 || k!=1)
                    temp3 <<= 1;

                temp1 <<= 2;
            }

        }
        EPD_2IN7_V2_SendData(temp3);
        // printf("%x",temp3);
    }

    EPD_2IN7_V2_SendCommand(0x26);   //write RAM for black(0)/white (1)
    for(i=0; i<5808; i++) {             //5808*4  46464
        temp3=0;
        for(j=0; j<2; j++) {
            temp1 = Image[i*2+j];
            for(k=0; k<2; k++) {
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0)
                    temp3 |= 0x00;//white
                else if(temp2 == 0x00)
                    temp3 |= 0x01;  //black
                else if(temp2 == 0x80)
                    temp3 |= 0x00;  //gray1
                else //0x40
                    temp3 |= 0x01; //gray2
                temp3 <<= 1;

                temp1 <<= 2;
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0)  //white
                    temp3 |= 0x00;
                else if(temp2 == 0x00) //black
                    temp3 |= 0x01;
                else if(temp2 == 0x80)
                    temp3 |= 0x00; //gray1
                else    //0x40
                    temp3 |= 0x01;	//gray2
                if(j!=1 || k!=1)
                    temp3 <<= 1;

                temp1 <<= 2;
            }
        }
        EPD_2IN7_V2_SendData(temp3);
        // printf("%x",temp3);
    }

    EPD_2IN7_V2_TurnOnDisplay_4GRAY();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN7_V2_Sleep(void)
{
    EPD_2IN7_V2_SendCommand(0X10); //enter deep sleep
    EPD_2IN7_V2_SendData(0x01);
    // DEV_Delay_ms(100); // not necessary
}

/******************************************************************************
function :	Awake EPD from sleep mode
parameter:
info     :  This is an example of awake EPD, copy it to your main.cpp as a function
******************************************************************************/
// void EPD_2IN7_V2_Awake(void) {
//     Debug("Awake EPD...\r\n");
    
//     // reset EPD
//     DEV_Digital_Write(EPD_RST_PIN, 0);
//     DEV_Delay_ms(10);
//     DEV_Digital_Write(EPD_RST_PIN, 1);
//     DEV_Delay_ms(10);
    
//     // reinitialize EPD
//     EPD_2IN7_V2_Init();
    
//     // reload image cache(need to define the image cache in your main.cpp as global variable)
//     if (BlackImage == NULL) {
//         if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) 
//         {
//             Debug("Failed to apply for black memory...\r\n");
//             while (1);
//         }
//     }
//     Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 270, WHITE);
//     Paint_SelectImage(BlackImage);
// }
