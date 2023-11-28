
#ifndef __Hardware_Profile_H
#define __Hardware_Profile_H


#include "msp430f6638.h"   // "处理器"头文件: 内部寄存相关定义等(处理器内部资源) 
#include "../F5xx_F6xx_Core_Lib/HAL_PMM.h"  //F5xx_F6xx_Core_Lib//系统电源管理模块-头文件
#include "../F5xx_F6xx_Core_Lib/HAL_UCS.h"  //F5xx_F6xx_Core_Lib//系统时钟配置-头文件


#include "SPI.h"     //"SPI 同步串行外设接口" -驱动程序 -头文件(处理器内部资源)

#include "LCD_TFT_ILI9325.h"  // "彩屏LCD_TFT_ILI9325" -驱动程序(外部资源) -头文件
#include "tm1638.h"



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




////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//==**单片机内部定义**================================//

#define mClose_WDT      (WDTCTL = WDTPW + WDTHOLD)  // 关闭看门狗
#define mClr_WDTCNT     (WDTCTL = WDTPW + WDTCNTCL) // 清看门狗(对于MSP430单片机)
#define	mNop	        (__no_operation())    //空操作指令(用于"短延时"或"调试")

//#define	mEnable_GIE     (__bis_SR_register(GIE))  // 开启总中断
#define	mEnable_GIE     (_EINT())  // 开启总中断GIE
#define	mDisable_GIE    (_DINT())  // 关闭总中断GIE

#define	mReset_Soft    (PMMCTL0 |= PMMSWPOR) //"软件复位" (MSP430单片机)



#define	Fosc_System	    (12)		//系统运行频率 unit:MHz (倍频后，即最终的)
////////////////////////////////////////////////////////////////////////////
//==** 系统频率MCLK =12M  (DCO  FLL倍频到12MHz)   **==============//
	//ms级的死等延时，调用WaitingDelayMs(n); //死等待延时n个ms(包含清看门狗操作)
	//us级的死等延时，调用mDelay_Cycles(n);	//死等待延时n个时钟周期
#define mDelay_Cycles(n) (__delay_cycles(n)) // 死等待延时"n"个时钟周期.
	//例: __delay_cycles(12000000); //(1/12M)*12000000=1s

#define M_Delay_1us   (12)   //死等延时"1us" //系统频率为12 M时
#define M_Delay_2us   (24)   //死等延时"2us" //系统频率为12 M时
#define M_Delay_5us   (60)  //死等延时"5us" //系统频率为12 M时

#define M_Delay_10us  (120)      //死等延时"10us"//系统频率为12 M时
#define M_Delay_100us (1200)     //死等延时"100us"//系统频率为12 M时
#define M_Delay_1ms   (12000)    //死等延时"1ms"//系统频率为12 M时(示波器实测1.02ms)
#define M_Delay_1s    (12000000) //死等延时"1s"//系统频率为12 M时(示波器实测1.02s)


#define CPU_F ((double)12000000)
#define _delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define _delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))



////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//==**内部"资源IO口使用宏定义**=======================//

	////////////////////////////////////////////////////////////////////////////
	//==**SPI_B *IO口使用宏定义**=============================//

//--**P8.4 引脚: SPI_B_CLK (P8.4/UCB1CLK/UCA1STE/S11)** -------------------//
#define SPI_B_CLK_SEL  (P8SEL)
#define SPI_B_CLK_DIR  (P8DIR)  
#define SPI_B_CLK_PORT (P8OUT)

#define SPI_B_CLK_IO   (BIT4)	

	//配置引脚为"外围模块功能"引脚
#define mSelect_SPI_B_CLK_FunctionPin  (SPI_B_CLK_SEL |= SPI_B_CLK_IO) 

	//配置"SPI_B_CLK" 引脚为"输出"
#define mSetOut_SPI_B_CLK (SPI_B_CLK_DIR |= SPI_B_CLK_IO)  

	//配置引脚为"输出",并输出"高电平"
#define mHigh_SPI_B_CLK  {SPI_B_CLK_DIR |= SPI_B_CLK_IO; SPI_B_CLK_PORT |= SPI_B_CLK_IO;}      //引脚输出"高电平(1)"
	//配置引脚为"输出",并输出"低电平"
#define mLow_SPI_B_CLK   {SPI_B_CLK_DIR |= SPI_B_CLK_IO; SPI_B_CLK_PORT &= ~SPI_B_CLK_IO;}    //引脚输出"低电平(0)"




//--**P8.5 引脚: SPI_B_MOSI (P8.5/UCB1SIMO/UCB1SDA/S10)** ----------------//
#define SPI_B_MOSI_SEL  (P8SEL)
#define SPI_B_MOSI_DIR  (P8DIR)  
#define SPI_B_MOSI_IO   (BIT5)

	//配置引脚为"外围模块功能"引脚
#define mSelect_SPI_B_MOSI_FunctionPin  (SPI_B_MOSI_SEL |= SPI_B_MOSI_IO) 

	 //配置"SPI_B_MOSI" 引脚为"输出"
#define mSetOut_SPI_B_MOSI (SPI_B_MOSI_DIR |= SPI_B_MOSI_IO)  


//--**P8.6引脚: SPI_B_MISO (P8.6/UCB1SOMI/UCB1SCL/S9)** ----------------//
#define SPI_B_MISO_SEL  (P8SEL)
#define SPI_B_MISO_DIR  (P8DIR)  
#define SPI_B_MISO_IO   (BIT6)

	//配置引脚为"外围模块功能"引脚
#define mSelect_SPI_B_MISO_FunctionPin  (SPI_B_MISO_SEL |= SPI_B_MISO_IO) 

	 //配置"SPI_B_MISO" 引脚为"输入"
#define mSetIn_SPI_B_MISO (SPI_B_MISO_DIR &= (~SPI_B_MISO_IO))  



	////////////////////////////////////////////////////////////////////////////
	//==**多路复用器TS3A5017 *IO口使用宏定义**================//

//--**P1.7 引脚: 连接到多路复用器:引脚IN1** -------------------//
#define TS3A5017_B_IN1_DIR  (P1DIR)  
#define TS3A5017_B_IN1_PORT (P1OUT)
#define TS3A5017_B_IN1_IO   (BIT7)

#define mSetOut_TS3A5017_B_IN1 (TS3A5017_B_IN1_DIR |= TS3A5017_B_IN1_IO)   //设置IN1引脚都为"输出"
#define mHigh_TS3A5017_B_IN1   (TS3A5017_B_IN1_PORT |= TS3A5017_B_IN1_IO)    //引脚输出"高电平(1)"
#define mLow_TS3A5017_B_IN1    (TS3A5017_B_IN1_PORT &= (~TS3A5017_B_IN1_IO)) //引脚输出"低电平(0)"

#define mConfig_TS3A5017_B_IN1(IN1) {mSetOut_TS3A5017_B_IN1; IN1 ? mHigh_TS3A5017_B_IN1 : mLow_TS3A5017_B_IN1;} 

//--**P1.6 引脚: 连接到多路复用器:引脚IN2** -------------------//
#define TS3A5017_B_IN2_DIR  (P1DIR)  
#define TS3A5017_B_IN2_PORT (P1OUT)
#define TS3A5017_B_IN2_IO   (BIT6)	

#define mSetOut_TS3A5017_B_IN2 (TS3A5017_B_IN2_DIR |= TS3A5017_B_IN2_IO)   //设置IN2引脚都为"输出"
#define mHigh_TS3A5017_B_IN2   (TS3A5017_B_IN2_PORT |= TS3A5017_B_IN2_IO)    //引脚输出"高电平(1)"
#define mLow_TS3A5017_B_IN2    (TS3A5017_B_IN2_PORT &= (~TS3A5017_B_IN2_IO)) //引脚输出"低电平(0)"

#define mConfig_TS3A5017_B_IN2(IN2) {mSetOut_TS3A5017_B_IN2; IN2 ? mHigh_TS3A5017_B_IN2 : mLow_TS3A5017_B_IN2;} 



	////////////////////////////////////////////////////////////////////////////
	//==**"LCD_Segment 段式LCD" 驱动器*IO口使用宏定义**========//

//--**P8.0 引脚:  LCD_Segment背光(1有效)** ---------------------------//
#define LCD_SEG_BackLight_DIR  (P8DIR)  
#define LCD_SEG_BackLight_PORT (P8OUT)
#define LCD_SEG_BackLight_IO   (BIT0)

	//配置引脚为"输出",并"打开"LCD_Segment 背光
#define mOpen_LCD_SEG_BackLight {LCD_SEG_BackLight_DIR |= LCD_SEG_BackLight_IO; LCD_SEG_BackLight_PORT |= LCD_SEG_BackLight_IO;}   
	//配置引脚为"输出",并"关闭"LCD_Segment 背光
#define mClose_LCD_SEG_BackLight {LCD_SEG_BackLight_DIR |= LCD_SEG_BackLight_IO; LCD_SEG_BackLight_PORT &= (~LCD_SEG_BackLight_IO);} 


//--**P5.5, P5.4, P5.3引脚:  用作LCD_B驱动引脚** ----------------------//
#define LCD_SEG_COM_SEL  (P5SEL)

#define LCD_SEG_COM_IO   (BIT3 + BIT4 + BIT5)

	//配置引脚为"外围模块功能"引脚
#define mSelect_LCD_SEG_FunctionPin (LCD_SEG_COM_SEL |= LCD_SEG_COM_IO) //P5.5, P5.4, P5.3 用作LCD_B驱动引脚(配置引脚为"外围模块功能"引脚 )

#define mSelect_LCD_SEG_IoPin (LCD_SEG_COM_SEL &= ~LCD_SEG_COM_IO) //配置引脚为"通用数字IO"引脚




	////////////////////////////////////////////////////////////////////////////
	//==**LCD_TFT 液晶显示器*IO口使用宏定义**===============//

//--**P5.2 引脚: 控制LCD_TFT背光** ----------------------------------//
	//根据引脚输出的占空比，调试背光亮度
#define LCD_TFT_BackLight_DIR  (P5DIR)  
#define LCD_TFT_BackLight_PORT (P5OUT)
#define LCD_TFT_BackLight_IO   (BIT2)

	//引脚: 输出高电平(占空比为100%)(此时背光最亮)
	//配置引脚为"输出",并"打开"LCD_TFT背光
#define mOpen_LCD_TFT_BackLight {LCD_TFT_BackLight_DIR |= LCD_TFT_BackLight_IO; LCD_TFT_BackLight_PORT |= LCD_TFT_BackLight_IO;}   

	//引脚: 输出低电平(占空比为  0%)(此时背光关闭)
	//配置引脚为"输出",并"关闭"LCD_TFT背光
#define mClose_LCD_TFT_BackLight {LCD_TFT_BackLight_DIR |= LCD_TFT_BackLight_IO; LCD_TFT_BackLight_PORT &= (~LCD_TFT_BackLight_IO);} 


//--**P2.6 引脚: LCD_TFT使能引脚CS ("低"有效)** -------------------//
#define LCD_TFT_CS_DIR  (P2DIR)  
#define LCD_TFT_CS_PORT (P2OUT)
#define LCD_TFT_CS_IO   (BIT6)

	//配置引脚都为"输出"
#define mSetOut_LCD_TFT_CS (LCD_TFT_CS_DIR |= LCD_TFT_CS_IO) 
	//输出"高"电平
#define mHigh_LCD_TFT_CS (LCD_TFT_CS_PORT |= LCD_TFT_CS_IO) 
	//输出"低"电平
#define mLow_LCD_TFT_CS  (LCD_TFT_CS_PORT &= ~LCD_TFT_CS_IO)
		//配置引脚为"输出",并"使能"LCD_TFT 模块(低电平有效)
#define mOpen_LCD_TFT_CS  {LCD_TFT_CS_DIR |= LCD_TFT_CS_IO; LCD_TFT_CS_PORT &= (~LCD_TFT_CS_IO);} 
		//配置引脚为"输出",并"禁用"LCD_TFT 模块
#define mClose_LCD_TFT_CS {LCD_TFT_CS_DIR |= LCD_TFT_CS_IO; LCD_TFT_CS_PORT |= LCD_TFT_CS_IO;} 
			
	//配置引脚为"输出" ,并"触发"CS引脚信号(产生下降沿)
#define mTrigger_LCD_TFT_CS {LCD_TFT_CS_DIR |= LCD_TFT_CS_IO; LCD_TFT_CS_PORT |= LCD_TFT_CS_IO; LCD_TFT_CS_PORT &= ~LCD_TFT_CS_IO;} 


//--**P2.7 引脚: 控制LCD_TFT复位** ----------------------------------//
#define LCD_TFT_Reset_DIR  (P2DIR)  
#define LCD_TFT_Reset_PORT (P2OUT)
#define LCD_TFT_Reset_IO   (BIT7)

	//配置引脚都为"输出"
#define mSetOut_LCD_TFT_Reset (LCD_TFT_Reset_DIR |= LCD_TFT_Reset_IO) 
	//输出"低"电平"==使能"复位功能(复位"低"电平有效)
#define mOpen_LCD_TFT_Reset  (LCD_TFT_Reset_PORT &= ~LCD_TFT_Reset_IO)  
	//输出"高"电平=="关闭"复位功能
#define mClose_LCD_TFT_Reset (LCD_TFT_Reset_PORT |= LCD_TFT_Reset_IO)


//--**P8.4 引脚: LCD_TFT 的时钟引脚CLK** -------------------//
#define LCD_TFT_CLK_SEL  (P8SEL)
#define LCD_TFT_CLK_DIR  (P8DIR)  
#define LCD_TFT_CLK_PORT (P8OUT)
#define LCD_TFT_CLK_IO   (BIT4)

	//配置引脚为通用数字"IO"引脚
#define mSelect_LCD_TFT_CLK_IoPin  (LCD_TFT_CLK_SEL &= ~LCD_TFT_CLK_IO) 
	//配置引脚都为"输出"
#define mSetOut_LCD_TFT_CLK (LCD_TFT_CLK_DIR |= LCD_TFT_CLK_IO) 
	//输出"高"电平
#define mHigh_LCD_TFT_CLK (LCD_TFT_CLK_PORT |= LCD_TFT_CLK_IO)   
	//输出"低"电平	
#define mLow_LCD_TFT_CLK  (LCD_TFT_CLK_PORT &= ~LCD_TFT_CLK_IO)   


//--**P8.5 引脚: LCD_TFT 的数据引脚SDA(MOSI)** -------------------//
#define LCD_TFT_SDA_SEL  (P8SEL)
#define LCD_TFT_SDA_DIR  (P8DIR)  
#define LCD_TFT_SDA_PORT (P8OUT)
#define LCD_TFT_SDA_IO   (BIT5)

	//配置引脚为通用数字"IO"引脚
#define mSelect_LCD_TFT_SDA_IoPin  (LCD_TFT_SDA_SEL &= ~LCD_TFT_SDA_IO) 
	//配置引脚都为"输出"
#define mSetOut_LCD_TFT_SDA (LCD_TFT_SDA_DIR |= LCD_TFT_SDA_IO) 
	//输出"高"电平
#define mHigh_LCD_TFT_SDA (LCD_TFT_SDA_PORT |= LCD_TFT_SDA_IO)
	//输出"低"电平
#define mLow_LCD_TFT_SDA  (LCD_TFT_SDA_PORT &= ~LCD_TFT_SDA_IO) 


//--**P8.6 引脚: LCD_TFT 的命令寄存器、数据寄存器选择引脚RS(A0)** -------------//
//0:  选择命令寄存器 //1: 选择数据寄存器
#define LCD_TFT_RS_SEL  (P8SEL)
#define LCD_TFT_RS_DIR  (P8DIR)  
#define LCD_TFT_RS_PORT (P8OUT)
#define LCD_TFT_RS_IO   (BIT6)

	//配置引脚为通用数字"IO"引脚
#define mSelect_LCD_TFT_RS_IoPin  (LCD_TFT_RS_SEL &= ~LCD_TFT_RS_IO) 
	//配置引脚都为"输出"
#define mSetOut_LCD_TFT_RS (LCD_TFT_RS_DIR |= LCD_TFT_RS_IO) 
	//输出"高"电平 //1: 选择数据寄存器
#define mHigh_LCD_TFT_RS (LCD_TFT_RS_PORT |= LCD_TFT_RS_IO) 
	//输出"低"电平 //0:  选择命令寄存器
#define mLow_LCD_TFT_RS  (LCD_TFT_RS_PORT &= ~LCD_TFT_RS_IO) 


	////////////////////////////////////////////////////////////////////////////
	//==**CC1101 模块*IO口使用宏定义**=========================//
//--**PJ.1 引脚: CC1101 模块的使能引脚CS** ----------------------//
#define CC1101_CS_DIR  (PJDIR)  
#define CC1101_CS_PORT (PJOUT)
#define CC1101_CS_IO   (BIT1)
	//配置引脚为"输出",并"使能"CC1101 模块(低电平有效)
#define mOpen_CC1101_CS  {CC1101_CS_DIR |= CC1101_CS_IO; CC1101_CS_PORT &= (~CC1101_CS_IO);} 

	//配置引脚为"输出",并"禁用"CC1101 模块
#define mClose_CC1101_CS {CC1101_CS_DIR |= CC1101_CS_IO; CC1101_CS_PORT |= CC1101_CS_IO;} 


	////////////////////////////////////////////////////////////////////////////
	//==**SD卡模块*IO口使用宏定义**=========================//
//--**PJ.0 引脚: SD卡模块的使能引脚CS** ----------------------//
#define SDcard_CS_DIR  (PJDIR)  
#define SDcard_CS_PORT (PJOUT)
#define SDcard_CS_IO   (BIT0)
	//配置引脚为"输出",并"使能"SD卡模块(低电平有效)
#define mOpen_SDcard_CS  {SDcard_CS_DIR |= SDcard_CS_IO; SDcard_CS_PORT &= (~SDcard_CS_IO);} 

	//配置引脚为"输出",并"禁用"SD卡 模块
#define mClose_SDcard_CS {SDcard_CS_DIR |= SDcard_CS_IO; SDcard_CS_PORT |= SDcard_CS_IO;} 
		

	////////////////////////////////////////////////////////////////////////////
	//==**SPI 各个模块，使能控制**=========================//
	//SPI用于驱动LCD_TFT时，要禁用其他用到SPI的模块
#define mSPI_Select_LCD_TFT {mOpen_LCD_TFT_CS; mClose_CC1101_CS; mClose_SDcard_CS;}              	
	//SPI用于驱动CC1101时，要禁用其他用到SPI的模块
#define mSPI_Select_CC1101  {mOpen_CC1101_CS; mClose_LCD_TFT_CS; mClose_SDcard_CS;}    
	//SPI用于驱动SD卡时，要禁用其他用到SPI的模块
#define mSPI_Select_SDcard  {mOpen_SDcard_CS; mClose_LCD_TFT_CS; mClose_CC1101_CS;}    

////////////////////////////////////////////////////////////////////////////
//==**蜂鸣器端口**=========================//
#define BUZZER_BIT BIT1
#define BUZZER_PORT P4OUT




////////////////////////////////////////////////////////////////////////////
//==**"外部" API 函数声明**Exported  API funcitions**===============//
////////////////////////////////////////////////////////////////////////////




#endif  /* __Hardware_Profile_H */






