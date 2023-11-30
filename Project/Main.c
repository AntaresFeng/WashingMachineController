#include "Hardware_Profile.h"  //处理器"硬件配置文件" -Hardware specific definitions for Processor
#include "washingmachine.h"

void Osccon_Initial(void);  //晶振初始化函数:配置相关晶振参数
void Prot_Initial(void); //端口初始化函数
void Timer_Initial(void); //定时器初始化函数



/****************************************************************************
*函数名-Function:	int main(void)
*描述- Description:	主函数
*输入参数-Input:	None
*输出参数-output:	None
*注意事项-Note：	
*****************************************************************************/
int main(void)
{  
	mClose_WDT;   // 关闭看门狗	 
	mDisable_GIE; //关闭中断
	
	Prot_Initial(); // 端口初始化
	//Timer_Initial(); 
	init_TM1638(); // TM1638初始化
	Osccon_Initial(); //晶振初始化函数:定义相关晶振参数

	mEnable_GIE;  // 开启总中断//good 放在初始化最后面

	Init_LCD_TFT_ILI9325(); //"彩屏LCD_TFT_ILI9325" 初使化: 所有相关资源的初始化

	LCD_TFT_Show_StandbyPage(); //显示"待机页面"-"彩屏LCD_TFT_ILI9325"
	while(P4IN & BIT2);	//wait until key is pressed
	LCD_TFT_Clear(WHITE);
	LCD_TFT_Show_QuestionPage(); //显示"题目页面"
	while(P4IN & BIT2);	//wait until key is pressed
	LCD_TFT_Clear(WHITE);

	runWashingMachine(); // 洗衣机控制主程序

//	while(1)
//	{
//
//	}
}


/****************************************************************************
*函数名-Function:	void Prot_Initial(void)
*描述- Description:		端口初始化函数
*输入参数-Input:	None
*输出参数-output:	None
*注意事项-Note：
*****************************************************************************/
void Prot_Initial(void)
{
	P4DIR &= ~(BIT2); //setting IO for input
	P4DIR|= BIT4+BIT5+BIT6+BUZZER_BIT; // P4.4,P4.5,P4.6 set as output
	P4OUT &= ~(BIT4+BIT5+BIT6+BUZZER_BIT);

	P3DIR |= BIT5;
	P3DIR |= BIT4 + BIT2;

	P1DIR |= BIT0; // ACLK set out to pins
	P1SEL |= BIT0;

//	P3DIR |= BIT4; // SMCLK set out to pins
//	P3SEL |= BIT4;

}

// 定时器初始化
void Timer_Initial(void) {
	TBCCR0 = 4545 - 1; //
	TBCTL = TBSSEL_2 + MC_1 + TBCLR; // SMCLK，upmode，clear TBR
	TBCCTL0 = CCIE; // TBCCR0 interrupt enabled
	
}

/****************************************************************************
*函数名-Function:	void Osccon_Initial(void)
*描述- Description:		晶振初始化函数:配置相关晶振参数
*输入参数-Input:	None
*输出参数-output:	None
*注意事项-Note：	
*****************************************************************************/
void Osccon_Initial(void)  //晶振初始化函数:配置相关晶振参数
{

	//IO初始化，在放在"晶振"配置的前面
//P1DIR |= BIT0;		 // ACLK set out to pins
//P1SEL |= BIT0;	   		

//==care=下面之两句一定要有，不知道为什么?//////////////////
//P3DIR |= BIT4;							  // SMCLK set out to pins
//P3SEL |= BIT4;							  

	
/************************************/

	SetVCore(PMMCOREV_3);			 // Set Vcore to accomodate for max. allowed system speed

	UCSCTL3 |= SELREF_2;					  // Set DCO FLL reference = REFO

	__bis_SR_register(SCG0);				  // Disable the FLL control loop
	UCSCTL0 = 0x0000;						  // Set lowest possible DCOx, MODx
	UCSCTL1 = DCORSEL_5;					  // Select DCO range 24MHz operation
	UCSCTL2 = FLLD_1 + 374; 				  // Set DCO Multiplier for 12MHz
									  // (N + 1) * FLLRef = Fdco
									  // (374 + 1) * 32768 = 12MHz
									  // Set FLL Div = fDCOCLK/2
	__bic_SR_register(SCG0);				  // Enable the FLL control loop
	

/******************
//==care=倍频到20M, 程序一定要放在这里,不然会影响到RTC//////
	 SetVCore(PMMCOREV_3);			 // Set Vcore to accomodate for max. allowed system speed
	 UCSCTL3 |= SELREF_2;				 // Set DCO FLL reference = REFO
	// UCSCTL4 |= SELA_2;				 // Set ACLK = REFO
	 Init_FLL_Settle(20000, 630);	// MCLK=DCO = 20MHz // Set system clock to max (20MHz)
*******/


									 
////////////////////////////////////////////////////////////////////////////								 
	 while(BAKCTL & LOCKIO) 				   // Unlock XT1 pins for operation
		BAKCTL &= ~(LOCKIO);   
	 

	 P7SEL |= BIT2+BIT3;					   // Port select XT2

	 UCSCTL6 &= ~XT2OFF;					   // Enable XT2 
//	 UCSCTL3 |= SELREF_2;					   // Set DCO FLL reference = REFO
											   // Since LFXT1 is not used,
											   // sourcing FLL with LFXT1 can cause
											   // XT1OFFG flag to set
	//  UCSCTL4 |= SELA_2; 					   // ACLK=REFO=32.768KHz,SMCLK=DCO,MCLK=DCO


	 UCSCTL6 &= ~(XT1OFF);					   // XT1 On
	 UCSCTL6 |= XCAP_3; 					   // Internal load cap 


	// Loop until XT1,XT2 & DCO stabilizes - in this case loop until XT2 settles
	do
	{
	  UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
											  // Clear XT2,XT1,DCO fault flags
	  SFRIFG1 &= ~OFIFG;					  // Clear fault flags
	}while (SFRIFG1&OFIFG); 				  // Test oscillator fault flag

	UCSCTL6 &= ~XT2DRIVE0;					  // Decrease XT2 Drive according to  expected frequency
							
	//UCSCTL4 |= SELS_5 + SELM_5; 			  // SMCLK=MCLK=XT2
	UCSCTL4 |= SELS_5;	   // SMCLK=XT2 = 4MHz
	
	//UCSCTL4 |= SELS_4;   //SMCLK选择DCOCLKDIV (倍频后的频率)
	
	UCSCTL4 |= SELA_0; // ACLK = LFTX1 (by default)=32.768KHz

}
