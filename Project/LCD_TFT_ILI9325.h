
#ifndef __LCD_TFT_ILI9325_H   //"彩屏LCD_TFT_ILI9325" -驱动程序-头文件(外部资源) 
#define __LCD_TFT_ILI9325_H 

#include "stdint.h" //定义了一些整数类型(标准C库的头文件)
	//  例:uint8_t; uint16_t 都在"stdint.h"头文件定义
	// 按照posix标准，一般整型对应的*_t类型


//#define uchar unsigned char
//#define uint unsigned int


typedef uint32_t  u32;	 //unsigned long
typedef uint16_t  u16;  //unsigned int
typedef uint8_t   u8;   //unsigned char 
//typedef signed char int8_t;
//typedef unsigned char uint8_t;



//画笔颜色
#define BLACK	     0x0000  // 黑色    0,   0,   0
#define WHITE	     0xFFFF  // 白色    255, 255, 255  

#define RED  	     0xF800  // 红色    255,   0,   0    
#define BRED         0XF81F
#define BRRED 	     0XFC07  //棕红色
#define GRED 	     0XFFE0
#define MAROON       0x7800  //深红色  127,   0,   0      
#define MAGENTA      0xF81F  //品红    255,   0, 255      

#define BROWN 	     0XBC40  //棕色


#define GREEN        0x07E0  //绿色    0, 255,   0     
//#define LGREEN       0x07FC  //浅绿色 
#define LGREEN       0x07F8  //浅绿色 



#define OLIVE        0x7BE0  //橄榄绿  127, 127,   0      
#define DGREEN       0x03E0  //深绿色  0,  127,  0
#define DCYAN        0x03EF  //深青色  0,  127, 127       


#define BLUE 	     0x001F  //蓝色    0,   0, 255     
#define GBLUE	     0X07FF  //浅蓝色//显示字符串时，采用这种颜色作为背景色
#define LBLUE	     0X07FF  //浅蓝色
#define NAVY         0x000F  //深蓝色  0,   0, 127
#define CYAN         0x07FF  //青色    0, 255, 255               


#define YELLOW       0xFFE0  //黄色    255, 255, 0       


#define GRAY0        0xEF7D  //灰色0 3165 00110 001011 00101
#define GRAY1        0x8410  //灰色1      00000 000000 00000
#define GRAY2        0x4208  //灰色2  1111111111011111
#define GRAY  		 0X8430  //灰色

#define DGRAY        0x7BEF  //深灰色  127, 127, 127      

#define PURPLE       0x780F  //紫色    127,   0, 127     



//GUI颜色
#define DARKBLUE     0X01CF	 //深蓝色
#define LIGHTBLUE    0X7D7C  //浅蓝色
#define GRAYBLUE     0X5458  //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN   0X841F  //浅绿色
//#define LIGHTGRAY    0XEF5B  //浅灰色(PANNEL)
#define LGRAY 	     0XC618  //浅灰色 //灰白色  192, 192, 192     
#define LGRAYBLUE    0XA651  //浅灰蓝色
#define LBBLUE       0X2B12  //浅棕蓝色



////////////////////////////////////////////////////////////////////////////
//--**配置显示模式:横向或纵向**--------------------------------//
//--care-- 下面2个宏定义，只能"1"个有效
#define USE_HORIZONTAL_LCD_TFT   //使用的"水平(横向)显示模式"
//#define USE_VERTICAL_LCD_TFT     //使用的"垂直(纵向)显示模式"


////////////////////////////////////////////////////////////////////////////
//--**配置字体大小**------------------------------------------------//
#define FONT1206   (1) //字体12*6
#define FONT1608   (2) //字体16*8




extern u16 POINT_COLOR;  //配置"画笔颜色": 默认为"黑色"
extern u16 BACK_COLOR;   //配置"背景颜色": 默认为"白色"




////////////////////////////////////////////////////////////////////////////
//==**"此模块专用"函数声明**Exported Module-specific funcitions**======//
////////////////////////////////////////////////////////////////////////////

void ILI9325_WriteByte(u8 data);  //SPI向ILI9325 控制器写一个字节数据
void ILI9325_WriteRegIndex(u16 index); //向ILI9325，写"控制命令寄存器地址"
void ILI9325_WriteData(u16 data);  //向ILI9325，写"16 位数据"


void LCD_TFT_WriteReg(u8 index, u16 data);  //配置ILI9325寄存器的值(寄存器址+寄存器值)
void LCD_TFT_WriteRAM_CMD(void); //向ILI9325写"GRAM命令地址"
void LCD_TFT_WriteRAM(u16 RGB_data);  //向ILI9325写"GRAM数据"



void LCD_TFT_SetWindow(u16 xSta, u16 ySta, u16 xEnd, u16 yEnd);  //设置显示窗口的区域大小(起点和终点)-"彩屏LCD_TFT_ILI9325"  
void LCD_TFT_SetCursor(u16 x, u16 y);  //设置GRAM的光标位置-"彩屏LCD_TFT_ILI9325"  



extern void Init_LCD_TFT_ILI9325(void); //"彩屏LCD_TFT_ILI9325" 初使化: 所有相关资源的初始化
extern void Init_GPIO_LCD_TFT_ILI9325(void);  //初始化: "彩屏LCD_TFT_ILI9325"  IO端口
extern void Reset_LCD_TFT_ILI9325(void); //复位"彩屏LCD_TFT_ILI9325"模块
extern void Init_Register_LCD_TFT_ILI9325(void);  //初始化: "彩屏LCD_TFT_ILI9325"  控制芯片的寄存器



extern void LCD_TFT_Clear(u16 color);   //全屏清屏-"彩屏LCD_TFT_ILI9325"  

extern void LCD_TFT_DrawPoint(u16 x, u16 y, u16 color); //画点( 指定坐标,颜色)-"彩屏LCD_TFT_ILI9325"  

extern void LCD_TFT_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);  //画线-"彩屏LCD_TFT_ILI9325"  

extern void LCD_TFT_DrawRectangle(u16 xSta, u16 ySta, u16 xEnd, u16 yEnd,u16 color);  //画矩形框-"彩屏LCD_TFT_ILI9325"  

extern void LCD_TFT_DrawCircle(u16 x0,u16 y0,u8 r,u16 color);  //画圆-"彩屏LCD_TFT_ILI9325"  


extern void LCD_TFT_FillRectangle(u16 xSta,u16 ySta,u16 xEnd,u16 yEnd,u16 color);  //填充一块LCD矩形区域(指定大小、颜色)-"彩屏LCD_TFT_ILI9325" 


	//显示一个字符( 指定坐标,字体大小,颜色)-"彩屏LCD_TFT_ILI9325"  
extern void LCD_TFT_ShowChar(u16 x,u16 y,u8 AscNum,u8 Font,u16 pointColor,u16 backColor); 
	//显示字符串( 指定坐标,字体大小,颜色)-"彩屏LCD_TFT_ILI9325"  
extern void LCD_TFT_ShowString(u16 line,u16 column,char *ArrayPoint,u8 Font,u16 pointColor,u16 backColor);

	//显示1个24*24的汉字-"彩屏LCD_TFT_ILI9325"  
extern void LCD_TFT_Show_ChFont2424(u16 x,u16 y,u8 index,u16 pointColor,u16 backColor); 

extern void LCD_TFT_Show_StandbyPage(void); //显示"待机页面"-"彩屏LCD_TFT_ILI9325"
extern void LCD_TFT_Show_QuestionPage(void);//显示题目界面
extern void LCD_TFT_ShowChString(u16 line, u16 column, char *ArrayPoint, u16 pointColor,u16 backColor); //显示字符串( 指定坐标,字体大小,颜色)-"彩屏LCD_TFT_ILI9325"
extern u16 findChineseIndex(const char* chineseChar); //在字库中查找指定汉字的索引位置


#endif     

