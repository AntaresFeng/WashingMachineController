
#ifndef __Hardware_Profile_H
#define __Hardware_Profile_H


#include "msp430f6638.h"   // "������"ͷ�ļ�: �ڲ��Ĵ���ض����(�������ڲ���Դ) 
#include "../F5xx_F6xx_Core_Lib/HAL_PMM.h"  //F5xx_F6xx_Core_Lib//ϵͳ��Դ����ģ��-ͷ�ļ�
#include "../F5xx_F6xx_Core_Lib/HAL_UCS.h"  //F5xx_F6xx_Core_Lib//ϵͳʱ������-ͷ�ļ�


#include "SPI.h"     //"SPI ͬ����������ӿ�" -�������� -ͷ�ļ�(�������ڲ���Դ)

#include "LCD_TFT_ILI9325.h"  // "����LCD_TFT_ILI9325" -��������(�ⲿ��Դ) -ͷ�ļ�
#include "tm1638.h"



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




////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//==**��Ƭ���ڲ�����**================================//

#define mClose_WDT      (WDTCTL = WDTPW + WDTHOLD)  // �رտ��Ź�
#define mClr_WDTCNT     (WDTCTL = WDTPW + WDTCNTCL) // �忴�Ź�(����MSP430��Ƭ��)
#define	mNop	        (__no_operation())    //�ղ���ָ��(����"����ʱ"��"����")

//#define	mEnable_GIE     (__bis_SR_register(GIE))  // �������ж�
#define	mEnable_GIE     (_EINT())  // �������ж�GIE
#define	mDisable_GIE    (_DINT())  // �ر����ж�GIE

#define	mReset_Soft    (PMMCTL0 |= PMMSWPOR) //"�����λ" (MSP430��Ƭ��)



#define	Fosc_System	    (12)		//ϵͳ����Ƶ�� unit:MHz (��Ƶ�󣬼����յ�)
////////////////////////////////////////////////////////////////////////////
//==** ϵͳƵ��MCLK =12M  (DCO  FLL��Ƶ��12MHz)   **==============//
	//ms����������ʱ������WaitingDelayMs(n); //���ȴ���ʱn��ms(�����忴�Ź�����)
	//us����������ʱ������mDelay_Cycles(n);	//���ȴ���ʱn��ʱ������
#define mDelay_Cycles(n) (__delay_cycles(n)) // ���ȴ���ʱ"n"��ʱ������.
	//��: __delay_cycles(12000000); //(1/12M)*12000000=1s

#define M_Delay_1us   (12)   //������ʱ"1us" //ϵͳƵ��Ϊ12 Mʱ
#define M_Delay_2us   (24)   //������ʱ"2us" //ϵͳƵ��Ϊ12 Mʱ
#define M_Delay_5us   (60)  //������ʱ"5us" //ϵͳƵ��Ϊ12 Mʱ

#define M_Delay_10us  (120)      //������ʱ"10us"//ϵͳƵ��Ϊ12 Mʱ
#define M_Delay_100us (1200)     //������ʱ"100us"//ϵͳƵ��Ϊ12 Mʱ
#define M_Delay_1ms   (12000)    //������ʱ"1ms"//ϵͳƵ��Ϊ12 Mʱ(ʾ����ʵ��1.02ms)
#define M_Delay_1s    (12000000) //������ʱ"1s"//ϵͳƵ��Ϊ12 Mʱ(ʾ����ʵ��1.02s)


#define CPU_F ((double)12000000)
#define _delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define _delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))



////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//==**�ڲ�"��ԴIO��ʹ�ú궨��**=======================//

	////////////////////////////////////////////////////////////////////////////
	//==**SPI_B *IO��ʹ�ú궨��**=============================//

//--**P8.4 ����: SPI_B_CLK (P8.4/UCB1CLK/UCA1STE/S11)** -------------------//
#define SPI_B_CLK_SEL  (P8SEL)
#define SPI_B_CLK_DIR  (P8DIR)  
#define SPI_B_CLK_PORT (P8OUT)

#define SPI_B_CLK_IO   (BIT4)	

	//��������Ϊ"��Χģ�鹦��"����
#define mSelect_SPI_B_CLK_FunctionPin  (SPI_B_CLK_SEL |= SPI_B_CLK_IO) 

	//����"SPI_B_CLK" ����Ϊ"���"
#define mSetOut_SPI_B_CLK (SPI_B_CLK_DIR |= SPI_B_CLK_IO)  

	//��������Ϊ"���",�����"�ߵ�ƽ"
#define mHigh_SPI_B_CLK  {SPI_B_CLK_DIR |= SPI_B_CLK_IO; SPI_B_CLK_PORT |= SPI_B_CLK_IO;}      //�������"�ߵ�ƽ(1)"
	//��������Ϊ"���",�����"�͵�ƽ"
#define mLow_SPI_B_CLK   {SPI_B_CLK_DIR |= SPI_B_CLK_IO; SPI_B_CLK_PORT &= ~SPI_B_CLK_IO;}    //�������"�͵�ƽ(0)"




//--**P8.5 ����: SPI_B_MOSI (P8.5/UCB1SIMO/UCB1SDA/S10)** ----------------//
#define SPI_B_MOSI_SEL  (P8SEL)
#define SPI_B_MOSI_DIR  (P8DIR)  
#define SPI_B_MOSI_IO   (BIT5)

	//��������Ϊ"��Χģ�鹦��"����
#define mSelect_SPI_B_MOSI_FunctionPin  (SPI_B_MOSI_SEL |= SPI_B_MOSI_IO) 

	 //����"SPI_B_MOSI" ����Ϊ"���"
#define mSetOut_SPI_B_MOSI (SPI_B_MOSI_DIR |= SPI_B_MOSI_IO)  


//--**P8.6����: SPI_B_MISO (P8.6/UCB1SOMI/UCB1SCL/S9)** ----------------//
#define SPI_B_MISO_SEL  (P8SEL)
#define SPI_B_MISO_DIR  (P8DIR)  
#define SPI_B_MISO_IO   (BIT6)

	//��������Ϊ"��Χģ�鹦��"����
#define mSelect_SPI_B_MISO_FunctionPin  (SPI_B_MISO_SEL |= SPI_B_MISO_IO) 

	 //����"SPI_B_MISO" ����Ϊ"����"
#define mSetIn_SPI_B_MISO (SPI_B_MISO_DIR &= (~SPI_B_MISO_IO))  



	////////////////////////////////////////////////////////////////////////////
	//==**��·������TS3A5017 *IO��ʹ�ú궨��**================//

//--**P1.7 ����: ���ӵ���·������:����IN1** -------------------//
#define TS3A5017_B_IN1_DIR  (P1DIR)  
#define TS3A5017_B_IN1_PORT (P1OUT)
#define TS3A5017_B_IN1_IO   (BIT7)

#define mSetOut_TS3A5017_B_IN1 (TS3A5017_B_IN1_DIR |= TS3A5017_B_IN1_IO)   //����IN1���Ŷ�Ϊ"���"
#define mHigh_TS3A5017_B_IN1   (TS3A5017_B_IN1_PORT |= TS3A5017_B_IN1_IO)    //�������"�ߵ�ƽ(1)"
#define mLow_TS3A5017_B_IN1    (TS3A5017_B_IN1_PORT &= (~TS3A5017_B_IN1_IO)) //�������"�͵�ƽ(0)"

#define mConfig_TS3A5017_B_IN1(IN1) {mSetOut_TS3A5017_B_IN1; IN1 ? mHigh_TS3A5017_B_IN1 : mLow_TS3A5017_B_IN1;} 

//--**P1.6 ����: ���ӵ���·������:����IN2** -------------------//
#define TS3A5017_B_IN2_DIR  (P1DIR)  
#define TS3A5017_B_IN2_PORT (P1OUT)
#define TS3A5017_B_IN2_IO   (BIT6)	

#define mSetOut_TS3A5017_B_IN2 (TS3A5017_B_IN2_DIR |= TS3A5017_B_IN2_IO)   //����IN2���Ŷ�Ϊ"���"
#define mHigh_TS3A5017_B_IN2   (TS3A5017_B_IN2_PORT |= TS3A5017_B_IN2_IO)    //�������"�ߵ�ƽ(1)"
#define mLow_TS3A5017_B_IN2    (TS3A5017_B_IN2_PORT &= (~TS3A5017_B_IN2_IO)) //�������"�͵�ƽ(0)"

#define mConfig_TS3A5017_B_IN2(IN2) {mSetOut_TS3A5017_B_IN2; IN2 ? mHigh_TS3A5017_B_IN2 : mLow_TS3A5017_B_IN2;} 



	////////////////////////////////////////////////////////////////////////////
	//==**"LCD_Segment ��ʽLCD" ������*IO��ʹ�ú궨��**========//

//--**P8.0 ����:  LCD_Segment����(1��Ч)** ---------------------------//
#define LCD_SEG_BackLight_DIR  (P8DIR)  
#define LCD_SEG_BackLight_PORT (P8OUT)
#define LCD_SEG_BackLight_IO   (BIT0)

	//��������Ϊ"���",��"��"LCD_Segment ����
#define mOpen_LCD_SEG_BackLight {LCD_SEG_BackLight_DIR |= LCD_SEG_BackLight_IO; LCD_SEG_BackLight_PORT |= LCD_SEG_BackLight_IO;}   
	//��������Ϊ"���",��"�ر�"LCD_Segment ����
#define mClose_LCD_SEG_BackLight {LCD_SEG_BackLight_DIR |= LCD_SEG_BackLight_IO; LCD_SEG_BackLight_PORT &= (~LCD_SEG_BackLight_IO);} 


//--**P5.5, P5.4, P5.3����:  ����LCD_B��������** ----------------------//
#define LCD_SEG_COM_SEL  (P5SEL)

#define LCD_SEG_COM_IO   (BIT3 + BIT4 + BIT5)

	//��������Ϊ"��Χģ�鹦��"����
#define mSelect_LCD_SEG_FunctionPin (LCD_SEG_COM_SEL |= LCD_SEG_COM_IO) //P5.5, P5.4, P5.3 ����LCD_B��������(��������Ϊ"��Χģ�鹦��"���� )

#define mSelect_LCD_SEG_IoPin (LCD_SEG_COM_SEL &= ~LCD_SEG_COM_IO) //��������Ϊ"ͨ������IO"����




	////////////////////////////////////////////////////////////////////////////
	//==**LCD_TFT Һ����ʾ��*IO��ʹ�ú궨��**===============//

//--**P5.2 ����: ����LCD_TFT����** ----------------------------------//
	//�������������ռ�ձȣ����Ա�������
#define LCD_TFT_BackLight_DIR  (P5DIR)  
#define LCD_TFT_BackLight_PORT (P5OUT)
#define LCD_TFT_BackLight_IO   (BIT2)

	//����: ����ߵ�ƽ(ռ�ձ�Ϊ100%)(��ʱ��������)
	//��������Ϊ"���",��"��"LCD_TFT����
#define mOpen_LCD_TFT_BackLight {LCD_TFT_BackLight_DIR |= LCD_TFT_BackLight_IO; LCD_TFT_BackLight_PORT |= LCD_TFT_BackLight_IO;}   

	//����: ����͵�ƽ(ռ�ձ�Ϊ  0%)(��ʱ����ر�)
	//��������Ϊ"���",��"�ر�"LCD_TFT����
#define mClose_LCD_TFT_BackLight {LCD_TFT_BackLight_DIR |= LCD_TFT_BackLight_IO; LCD_TFT_BackLight_PORT &= (~LCD_TFT_BackLight_IO);} 


//--**P2.6 ����: LCD_TFTʹ������CS ("��"��Ч)** -------------------//
#define LCD_TFT_CS_DIR  (P2DIR)  
#define LCD_TFT_CS_PORT (P2OUT)
#define LCD_TFT_CS_IO   (BIT6)

	//�������Ŷ�Ϊ"���"
#define mSetOut_LCD_TFT_CS (LCD_TFT_CS_DIR |= LCD_TFT_CS_IO) 
	//���"��"��ƽ
#define mHigh_LCD_TFT_CS (LCD_TFT_CS_PORT |= LCD_TFT_CS_IO) 
	//���"��"��ƽ
#define mLow_LCD_TFT_CS  (LCD_TFT_CS_PORT &= ~LCD_TFT_CS_IO)
		//��������Ϊ"���",��"ʹ��"LCD_TFT ģ��(�͵�ƽ��Ч)
#define mOpen_LCD_TFT_CS  {LCD_TFT_CS_DIR |= LCD_TFT_CS_IO; LCD_TFT_CS_PORT &= (~LCD_TFT_CS_IO);} 
		//��������Ϊ"���",��"����"LCD_TFT ģ��
#define mClose_LCD_TFT_CS {LCD_TFT_CS_DIR |= LCD_TFT_CS_IO; LCD_TFT_CS_PORT |= LCD_TFT_CS_IO;} 
			
	//��������Ϊ"���" ,��"����"CS�����ź�(�����½���)
#define mTrigger_LCD_TFT_CS {LCD_TFT_CS_DIR |= LCD_TFT_CS_IO; LCD_TFT_CS_PORT |= LCD_TFT_CS_IO; LCD_TFT_CS_PORT &= ~LCD_TFT_CS_IO;} 


//--**P2.7 ����: ����LCD_TFT��λ** ----------------------------------//
#define LCD_TFT_Reset_DIR  (P2DIR)  
#define LCD_TFT_Reset_PORT (P2OUT)
#define LCD_TFT_Reset_IO   (BIT7)

	//�������Ŷ�Ϊ"���"
#define mSetOut_LCD_TFT_Reset (LCD_TFT_Reset_DIR |= LCD_TFT_Reset_IO) 
	//���"��"��ƽ"==ʹ��"��λ����(��λ"��"��ƽ��Ч)
#define mOpen_LCD_TFT_Reset  (LCD_TFT_Reset_PORT &= ~LCD_TFT_Reset_IO)  
	//���"��"��ƽ=="�ر�"��λ����
#define mClose_LCD_TFT_Reset (LCD_TFT_Reset_PORT |= LCD_TFT_Reset_IO)


//--**P8.4 ����: LCD_TFT ��ʱ������CLK** -------------------//
#define LCD_TFT_CLK_SEL  (P8SEL)
#define LCD_TFT_CLK_DIR  (P8DIR)  
#define LCD_TFT_CLK_PORT (P8OUT)
#define LCD_TFT_CLK_IO   (BIT4)

	//��������Ϊͨ������"IO"����
#define mSelect_LCD_TFT_CLK_IoPin  (LCD_TFT_CLK_SEL &= ~LCD_TFT_CLK_IO) 
	//�������Ŷ�Ϊ"���"
#define mSetOut_LCD_TFT_CLK (LCD_TFT_CLK_DIR |= LCD_TFT_CLK_IO) 
	//���"��"��ƽ
#define mHigh_LCD_TFT_CLK (LCD_TFT_CLK_PORT |= LCD_TFT_CLK_IO)   
	//���"��"��ƽ	
#define mLow_LCD_TFT_CLK  (LCD_TFT_CLK_PORT &= ~LCD_TFT_CLK_IO)   


//--**P8.5 ����: LCD_TFT ����������SDA(MOSI)** -------------------//
#define LCD_TFT_SDA_SEL  (P8SEL)
#define LCD_TFT_SDA_DIR  (P8DIR)  
#define LCD_TFT_SDA_PORT (P8OUT)
#define LCD_TFT_SDA_IO   (BIT5)

	//��������Ϊͨ������"IO"����
#define mSelect_LCD_TFT_SDA_IoPin  (LCD_TFT_SDA_SEL &= ~LCD_TFT_SDA_IO) 
	//�������Ŷ�Ϊ"���"
#define mSetOut_LCD_TFT_SDA (LCD_TFT_SDA_DIR |= LCD_TFT_SDA_IO) 
	//���"��"��ƽ
#define mHigh_LCD_TFT_SDA (LCD_TFT_SDA_PORT |= LCD_TFT_SDA_IO)
	//���"��"��ƽ
#define mLow_LCD_TFT_SDA  (LCD_TFT_SDA_PORT &= ~LCD_TFT_SDA_IO) 


//--**P8.6 ����: LCD_TFT ������Ĵ��������ݼĴ���ѡ������RS(A0)** -------------//
//0:  ѡ������Ĵ��� //1: ѡ�����ݼĴ���
#define LCD_TFT_RS_SEL  (P8SEL)
#define LCD_TFT_RS_DIR  (P8DIR)  
#define LCD_TFT_RS_PORT (P8OUT)
#define LCD_TFT_RS_IO   (BIT6)

	//��������Ϊͨ������"IO"����
#define mSelect_LCD_TFT_RS_IoPin  (LCD_TFT_RS_SEL &= ~LCD_TFT_RS_IO) 
	//�������Ŷ�Ϊ"���"
#define mSetOut_LCD_TFT_RS (LCD_TFT_RS_DIR |= LCD_TFT_RS_IO) 
	//���"��"��ƽ //1: ѡ�����ݼĴ���
#define mHigh_LCD_TFT_RS (LCD_TFT_RS_PORT |= LCD_TFT_RS_IO) 
	//���"��"��ƽ //0:  ѡ������Ĵ���
#define mLow_LCD_TFT_RS  (LCD_TFT_RS_PORT &= ~LCD_TFT_RS_IO) 


	////////////////////////////////////////////////////////////////////////////
	//==**CC1101 ģ��*IO��ʹ�ú궨��**=========================//
//--**PJ.1 ����: CC1101 ģ���ʹ������CS** ----------------------//
#define CC1101_CS_DIR  (PJDIR)  
#define CC1101_CS_PORT (PJOUT)
#define CC1101_CS_IO   (BIT1)
	//��������Ϊ"���",��"ʹ��"CC1101 ģ��(�͵�ƽ��Ч)
#define mOpen_CC1101_CS  {CC1101_CS_DIR |= CC1101_CS_IO; CC1101_CS_PORT &= (~CC1101_CS_IO);} 

	//��������Ϊ"���",��"����"CC1101 ģ��
#define mClose_CC1101_CS {CC1101_CS_DIR |= CC1101_CS_IO; CC1101_CS_PORT |= CC1101_CS_IO;} 


	////////////////////////////////////////////////////////////////////////////
	//==**SD��ģ��*IO��ʹ�ú궨��**=========================//
//--**PJ.0 ����: SD��ģ���ʹ������CS** ----------------------//
#define SDcard_CS_DIR  (PJDIR)  
#define SDcard_CS_PORT (PJOUT)
#define SDcard_CS_IO   (BIT0)
	//��������Ϊ"���",��"ʹ��"SD��ģ��(�͵�ƽ��Ч)
#define mOpen_SDcard_CS  {SDcard_CS_DIR |= SDcard_CS_IO; SDcard_CS_PORT &= (~SDcard_CS_IO);} 

	//��������Ϊ"���",��"����"SD�� ģ��
#define mClose_SDcard_CS {SDcard_CS_DIR |= SDcard_CS_IO; SDcard_CS_PORT |= SDcard_CS_IO;} 
		

	////////////////////////////////////////////////////////////////////////////
	//==**SPI ����ģ�飬ʹ�ܿ���**=========================//
	//SPI��������LCD_TFTʱ��Ҫ���������õ�SPI��ģ��
#define mSPI_Select_LCD_TFT {mOpen_LCD_TFT_CS; mClose_CC1101_CS; mClose_SDcard_CS;}              	
	//SPI��������CC1101ʱ��Ҫ���������õ�SPI��ģ��
#define mSPI_Select_CC1101  {mOpen_CC1101_CS; mClose_LCD_TFT_CS; mClose_SDcard_CS;}    
	//SPI��������SD��ʱ��Ҫ���������õ�SPI��ģ��
#define mSPI_Select_SDcard  {mOpen_SDcard_CS; mClose_LCD_TFT_CS; mClose_CC1101_CS;}    

////////////////////////////////////////////////////////////////////////////
//==**�������˿�**=========================//
#define BUZZER_BIT BIT1
#define BUZZER_PORT P4OUT




////////////////////////////////////////////////////////////////////////////
//==**"�ⲿ" API ��������**Exported  API funcitions**===============//
////////////////////////////////////////////////////////////////////////////




#endif  /* __Hardware_Profile_H */






