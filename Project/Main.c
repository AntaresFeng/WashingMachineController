#include "Hardware_Profile.h"  //������"Ӳ�������ļ�" -Hardware specific definitions for Processor
#include "washingmachine.h"

void Osccon_Initial(void);  //�����ʼ������:������ؾ������
void Prot_Initial(void); //�˿ڳ�ʼ������
void Timer_Initial(void); //��ʱ����ʼ������



/****************************************************************************
*������-Function:	int main(void)
*����- Description:	������
*�������-Input:	None
*�������-output:	None
*ע������-Note��	
*****************************************************************************/
int main(void)
{  
	mClose_WDT;   // �رտ��Ź�	 
	mDisable_GIE; //�ر��ж�
	
	Prot_Initial(); // �˿ڳ�ʼ��
	//Timer_Initial(); 
	init_TM1638(); // TM1638��ʼ��
	Osccon_Initial(); //�����ʼ������:������ؾ������

	mEnable_GIE;  // �������ж�//good ���ڳ�ʼ�������

	Init_LCD_TFT_ILI9325(); //"����LCD_TFT_ILI9325" ��ʹ��: ���������Դ�ĳ�ʼ��

	LCD_TFT_Show_StandbyPage(); //��ʾ"����ҳ��"-"����LCD_TFT_ILI9325"
	while(P4IN & BIT2);	//wait until key is pressed
	LCD_TFT_Clear(WHITE);
	LCD_TFT_Show_QuestionPage(); //��ʾ"��Ŀҳ��"
	while(P4IN & BIT2);	//wait until key is pressed
	LCD_TFT_Clear(WHITE);

	runWashingMachine(); // ϴ�»�����������

//	while(1)
//	{
//
//	}
}


/****************************************************************************
*������-Function:	void Prot_Initial(void)
*����- Description:		�˿ڳ�ʼ������
*�������-Input:	None
*�������-output:	None
*ע������-Note��
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

// ��ʱ����ʼ��
void Timer_Initial(void) {
	TBCCR0 = 4545 - 1; //
	TBCTL = TBSSEL_2 + MC_1 + TBCLR; // SMCLK��upmode��clear TBR
	TBCCTL0 = CCIE; // TBCCR0 interrupt enabled
	
}

/****************************************************************************
*������-Function:	void Osccon_Initial(void)
*����- Description:		�����ʼ������:������ؾ������
*�������-Input:	None
*�������-output:	None
*ע������-Note��	
*****************************************************************************/
void Osccon_Initial(void)  //�����ʼ������:������ؾ������
{

	//IO��ʼ�����ڷ���"����"���õ�ǰ��
//P1DIR |= BIT0;		 // ACLK set out to pins
//P1SEL |= BIT0;	   		

//==care=����֮����һ��Ҫ�У���֪��Ϊʲô?//////////////////
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
//==care=��Ƶ��20M, ����һ��Ҫ��������,��Ȼ��Ӱ�쵽RTC//////
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
	
	//UCSCTL4 |= SELS_4;   //SMCLKѡ��DCOCLKDIV (��Ƶ���Ƶ��)
	
	UCSCTL4 |= SELA_0; // ACLK = LFTX1 (by default)=32.768KHz

}
