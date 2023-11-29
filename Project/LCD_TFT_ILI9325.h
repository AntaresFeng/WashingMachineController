
#ifndef __LCD_TFT_ILI9325_H   //"����LCD_TFT_ILI9325" -��������-ͷ�ļ�(�ⲿ��Դ) 
#define __LCD_TFT_ILI9325_H 

#include "stdint.h" //������һЩ��������(��׼C���ͷ�ļ�)
	//  ��:uint8_t; uint16_t ����"stdint.h"ͷ�ļ�����
	// ����posix��׼��һ�����Ͷ�Ӧ��*_t����


//#define uchar unsigned char
//#define uint unsigned int


typedef uint32_t  u32;	 //unsigned long
typedef uint16_t  u16;  //unsigned int
typedef uint8_t   u8;   //unsigned char 
//typedef signed char int8_t;
//typedef unsigned char uint8_t;



//������ɫ
#define BLACK	     0x0000  // ��ɫ    0,   0,   0
#define WHITE	     0xFFFF  // ��ɫ    255, 255, 255  

#define RED  	     0xF800  // ��ɫ    255,   0,   0    
#define BRED         0XF81F
#define BRRED 	     0XFC07  //�غ�ɫ
#define GRED 	     0XFFE0
#define MAROON       0x7800  //���ɫ  127,   0,   0      
#define MAGENTA      0xF81F  //Ʒ��    255,   0, 255      

#define BROWN 	     0XBC40  //��ɫ


#define GREEN        0x07E0  //��ɫ    0, 255,   0     
//#define LGREEN       0x07FC  //ǳ��ɫ 
#define LGREEN       0x07F8  //ǳ��ɫ 



#define OLIVE        0x7BE0  //�����  127, 127,   0      
#define DGREEN       0x03E0  //����ɫ  0,  127,  0
#define DCYAN        0x03EF  //����ɫ  0,  127, 127       


#define BLUE 	     0x001F  //��ɫ    0,   0, 255     
#define GBLUE	     0X07FF  //ǳ��ɫ//��ʾ�ַ���ʱ������������ɫ��Ϊ����ɫ
#define LBLUE	     0X07FF  //ǳ��ɫ
#define NAVY         0x000F  //����ɫ  0,   0, 127
#define CYAN         0x07FF  //��ɫ    0, 255, 255               


#define YELLOW       0xFFE0  //��ɫ    255, 255, 0       


#define GRAY0        0xEF7D  //��ɫ0 3165 00110 001011 00101
#define GRAY1        0x8410  //��ɫ1      00000 000000 00000
#define GRAY2        0x4208  //��ɫ2  1111111111011111
#define GRAY  		 0X8430  //��ɫ

#define DGRAY        0x7BEF  //���ɫ  127, 127, 127      

#define PURPLE       0x780F  //��ɫ    127,   0, 127     



//GUI��ɫ
#define DARKBLUE     0X01CF	 //����ɫ
#define LIGHTBLUE    0X7D7C  //ǳ��ɫ
#define GRAYBLUE     0X5458  //����ɫ
//������ɫΪPANEL����ɫ

#define LIGHTGREEN   0X841F  //ǳ��ɫ
//#define LIGHTGRAY    0XEF5B  //ǳ��ɫ(PANNEL)
#define LGRAY 	     0XC618  //ǳ��ɫ //�Ұ�ɫ  192, 192, 192     
#define LGRAYBLUE    0XA651  //ǳ����ɫ
#define LBBLUE       0X2B12  //ǳ����ɫ



////////////////////////////////////////////////////////////////////////////
//--**������ʾģʽ:���������**--------------------------------//
//--care-- ����2���궨�壬ֻ��"1"����Ч
#define USE_HORIZONTAL_LCD_TFT   //ʹ�õ�"ˮƽ(����)��ʾģʽ"
//#define USE_VERTICAL_LCD_TFT     //ʹ�õ�"��ֱ(����)��ʾģʽ"


////////////////////////////////////////////////////////////////////////////
//--**���������С**------------------------------------------------//
#define FONT1206   (1) //����12*6
#define FONT1608   (2) //����16*8




extern u16 POINT_COLOR;  //����"������ɫ": Ĭ��Ϊ"��ɫ"
extern u16 BACK_COLOR;   //����"������ɫ": Ĭ��Ϊ"��ɫ"




////////////////////////////////////////////////////////////////////////////
//==**"��ģ��ר��"��������**Exported Module-specific funcitions**======//
////////////////////////////////////////////////////////////////////////////

void ILI9325_WriteByte(u8 data);  //SPI��ILI9325 ������дһ���ֽ�����
void ILI9325_WriteRegIndex(u16 index); //��ILI9325��д"��������Ĵ�����ַ"
void ILI9325_WriteData(u16 data);  //��ILI9325��д"16 λ����"


void LCD_TFT_WriteReg(u8 index, u16 data);  //����ILI9325�Ĵ�����ֵ(�Ĵ���ַ+�Ĵ���ֵ)
void LCD_TFT_WriteRAM_CMD(void); //��ILI9325д"GRAM�����ַ"
void LCD_TFT_WriteRAM(u16 RGB_data);  //��ILI9325д"GRAM����"



void LCD_TFT_SetWindow(u16 xSta, u16 ySta, u16 xEnd, u16 yEnd);  //������ʾ���ڵ������С(�����յ�)-"����LCD_TFT_ILI9325"  
void LCD_TFT_SetCursor(u16 x, u16 y);  //����GRAM�Ĺ��λ��-"����LCD_TFT_ILI9325"  



extern void Init_LCD_TFT_ILI9325(void); //"����LCD_TFT_ILI9325" ��ʹ��: ���������Դ�ĳ�ʼ��
extern void Init_GPIO_LCD_TFT_ILI9325(void);  //��ʼ��: "����LCD_TFT_ILI9325"  IO�˿�
extern void Reset_LCD_TFT_ILI9325(void); //��λ"����LCD_TFT_ILI9325"ģ��
extern void Init_Register_LCD_TFT_ILI9325(void);  //��ʼ��: "����LCD_TFT_ILI9325"  ����оƬ�ļĴ���



extern void LCD_TFT_Clear(u16 color);   //ȫ������-"����LCD_TFT_ILI9325"  

extern void LCD_TFT_DrawPoint(u16 x, u16 y, u16 color); //����( ָ������,��ɫ)-"����LCD_TFT_ILI9325"  

extern void LCD_TFT_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);  //����-"����LCD_TFT_ILI9325"  

extern void LCD_TFT_DrawRectangle(u16 xSta, u16 ySta, u16 xEnd, u16 yEnd,u16 color);  //�����ο�-"����LCD_TFT_ILI9325"  

extern void LCD_TFT_DrawCircle(u16 x0,u16 y0,u8 r,u16 color);  //��Բ-"����LCD_TFT_ILI9325"  


extern void LCD_TFT_FillRectangle(u16 xSta,u16 ySta,u16 xEnd,u16 yEnd,u16 color);  //���һ��LCD��������(ָ����С����ɫ)-"����LCD_TFT_ILI9325" 


	//��ʾһ���ַ�( ָ������,�����С,��ɫ)-"����LCD_TFT_ILI9325"  
extern void LCD_TFT_ShowChar(u16 x,u16 y,u8 AscNum,u8 Font,u16 pointColor,u16 backColor); 
	//��ʾ�ַ���( ָ������,�����С,��ɫ)-"����LCD_TFT_ILI9325"  
extern void LCD_TFT_ShowString(u16 line,u16 column,char *ArrayPoint,u8 Font,u16 pointColor,u16 backColor);

	//��ʾ1��24*24�ĺ���-"����LCD_TFT_ILI9325"  
extern void LCD_TFT_Show_ChFont2424(u16 x,u16 y,u8 index,u16 pointColor,u16 backColor); 

extern void LCD_TFT_Show_StandbyPage(void); //��ʾ"����ҳ��"-"����LCD_TFT_ILI9325"
extern void LCD_TFT_Show_QuestionPage(void);//��ʾ��Ŀ����
extern void LCD_TFT_ShowChString(u16 line, u16 column, char *ArrayPoint, u16 pointColor,u16 backColor); //��ʾ�ַ���( ָ������,�����С,��ɫ)-"����LCD_TFT_ILI9325"
extern u16 findChineseIndex(const char* chineseChar); //���ֿ��в���ָ�����ֵ�����λ��


#endif     

