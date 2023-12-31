#include "Hardware_Profile.h"  //处理器"硬件配置文件" -Hardware specific definitions for Processor


#include "LCD_TFT_FONT.h"  // 液晶点阵字库(中文、英文)-头文件(外部资源) 

#include "LCD_TFT_ILI9325.h"  // "彩屏LCD_TFT_ILI9325" -驱动程序(外部资源) -头文件




u16 POINT_COLOR = BLACK;  //配置"画笔颜色": 默认为"黑色"
u16 BACK_COLOR = WHITE;   //配置"背景颜色": 默认为"白色"





/****************************************************************************
*函数名-Function:	void ILI9325_WriteRegIndex(u16 index)
*描述- Description:		向ILI9325，写"控制命令寄存器地址"
*输入参数-Input:	index: 16位的控制命令寄存器地址
*输出参数-output:	None
*注意事项-Note：	
*****************************************************************************/
void ILI9325_WriteRegIndex(u16 index) //向ILI9325，写"控制命令寄存器地址"
{
	mLow_LCD_TFT_RS;  //配置引脚为"输出",并"输出低电平" //0:  选择命令寄存器

	Send_OneByte_SPI_B(index >> 8);  //高字节
	Send_OneByte_SPI_B(index);  	 //低字节
}


/****************************************************************************
*函数名-Function:	void ILI9325_WriteData(u16 data)
*描述- Description:		向ILI9325，写"16 位数据"
*输入参数-Input:	data: 16位的数据
*输出参数-output:	None
*注意事项-Note：
*****************************************************************************/
void ILI9325_WriteData(u16 data)  //向ILI9325，写"16 位数据"
{
	mHigh_LCD_TFT_RS; //配置引脚为"输出",并"输出高电平"  //1: 选择数据寄存器

	Send_OneByte_SPI_B(data >> 8);	//高字节	//	start byte RS=1,RW=0----Write a GRAM data
	Send_OneByte_SPI_B(data);        //低字节
}


/****************************************************************************
*函数名-Function:	void LCD_TFT_WriteReg(u8 index, u16 data)
*描述- Description:		配置ILI9325寄存器的值(寄存器址+寄存器值)
*输入参数-Input:	index: 16位寄存器址 ;  data: 16位寄存器值
*输出参数-output:	None
*注意事项-Note：	
*****************************************************************************/
void LCD_TFT_WriteReg(u8 index, u16 data)  //配置ILI9325寄存器的值(寄存器址+寄存器值)
{
	ILI9325_WriteRegIndex(index);
	ILI9325_WriteData(data);
}


/****************************************************************************
*函数名-Function:	void LCD_TFT_WriteRAM_CMD(void)
*描述- Description:		向ILI9325写"GRAM命令地址"
*输入参数-Input:	None
*输出参数-output:	None
*注意事项-Note：
*****************************************************************************/
void LCD_TFT_WriteRAM_CMD(void) //向ILI9325写"GRAM命令地址"
{
	ILI9325_WriteRegIndex(0x22);   //写RAM
}


/****************************************************************************
*函数名-Function:	void LCD_TFT_WriteRAM_CMD(void)
*描述- Description:		向ILI9325写"GRAM数据"
*输入参数-Input:	None
*输出参数-output:	None
*注意事项-Note：
*****************************************************************************/
void LCD_TFT_WriteRAM(u16 RGB_data)  //向ILI9325写"GRAM数据"
{
    ILI9325_WriteData(RGB_data); //写GRAM数据
}


/****************************************************************************
*函数名-Function:	void Init_LCD_TFT_ILI9325(void)
*描述- Description:		"彩屏LCD_TFT_ILI9325" 初使化: 所有相关资源的初始化
*输入参数-Input:	None
*输出参数-output:	None
*注意事项-Note：	
*****************************************************************************/
void Init_LCD_TFT_ILI9325(void) //"彩屏LCD_TFT_ILI9325" 初使化: 所有相关资源的初始化
{
////////////////////////////////////////////////////////////////////////////
//==**背光控制**===========================================//
		//引脚: 输出高电平(占空比为100%)(此时背光最亮)
	mOpen_LCD_TFT_BackLight; //设置引脚为"输出",并"打开""彩屏LCD_TFT"背光

				
////////////////////////////////////////////////////////////////////////////
//==**初始化: "彩屏LCD_TFT_ILI9325"  IO端口**====================//
	Init_GPIO_LCD_TFT_ILI9325();  //初始化: "彩屏LCD_TFT_ILI9325"  IO端口

	
////////////////////////////////////////////////////////////////////////////
//==**复位"彩屏LCD_TFT_ILI9325"模块**==========================//
	Reset_LCD_TFT_ILI9325(); //复位"彩屏LCD_TFT_ILI9325"模块


////////////////////////////////////////////////////////////////////////////
//==**初始化: "彩屏LCD_TFT_ILI9325"  控制芯片的寄存器**=======//
	Init_Register_LCD_TFT_ILI9325();  //初始化: "彩屏LCD_TFT_ILI9325"  控制芯片的寄存器

	
}

   
/****************************************************************************
*函数名-Function:	void Init_GPIO_LCD_TFT_ILI9325(void)
*描述- Description:		初始化: "彩屏LCD_TFT_ILI9325"  IO端口
*输入参数-Input:	None
*输出参数-output:	None
*注意事项-Note：	
*****************************************************************************/
void Init_GPIO_LCD_TFT_ILI9325(void)  //初始化: "彩屏LCD_TFT_ILI9325"  IO端口
{
	mSPI_Select_LCD_TFT; //SPI用于驱动LCD_TFT时，要禁用其他用到SPI的模块  

////////////////////////////////////////////////////////////////////////////
//==**SPI 端口初始化**===================================//
	Initial_SPI_B();  //SPI_B 模块初始化:端口初始化、配置SPI功能
	
////////////////////////////////////////////////////////////////////////////
//==**配置"B模块"多路复用器TS3A5017: 选择IN2 、IN1**=========//
		// IN2=0; IN1=1; 对应SPI_B 引脚(CLK_B; SPI_MOSI_B; SPI_MISO_B)
	mConfig_TS3A5017_B_IN2(0);  //IN2 = 0
	mConfig_TS3A5017_B_IN1(1);  //IN1 = 1


////////////////////////////////////////////////////////////////////////////
//==**IO口的相关配置初始化**==============================//

//--**P2.6 引脚: LCD_TFT使能引脚CS ("低"有效)** -------------------//
	mSetOut_LCD_TFT_CS; //配置引脚为"输出"
	mLow_LCD_TFT_CS; //"输出低电平"
		

//--**P2.7 引脚: 控制LCD_TFT复位** ----------------------------------//
	mSetOut_LCD_TFT_Reset; //配置引脚为"输出"
	mClose_LCD_TFT_Reset;  //输出"高"电平=="关闭"复位功能


//--**P8.6 引脚: LCD_TFT 的命令寄存器、数据寄存器选择引脚RS(A0)** -------------//
	//0:  选择命令寄存器 //1: 选择数据寄存器
	mSelect_LCD_TFT_RS_IoPin;  //配置引脚为通用数字"IO"引脚
	mSetOut_LCD_TFT_RS;        //配置引脚为"输出"
	mLow_LCD_TFT_RS;           //配置引脚为"输出",并"输出低电平" //0:	选择命令寄存器
	
	
}


/****************************************************************************
*函数名-Function:	void Reset_LCD_TFT_ILI9325(void)
*描述- Description:		复位"彩屏LCD_TFT_ILI9325"模块
*输入参数-Input:	None
*输出参数-output:	None
*注意事项-Note：	
*****************************************************************************/
void Reset_LCD_TFT_ILI9325(void) //复位"彩屏LCD_TFT_ILI9325"模块
{
	mClose_LCD_TFT_Reset; //设置引脚为"输出",并"关闭"复位功能
	_delay_ms(1);
	
	mOpen_LCD_TFT_Reset; //设置引脚为"输出",并"使能"复位功能(复位"低"电平有效)
	_delay_ms(10);
	
	mClose_LCD_TFT_Reset; //设置引脚为"输出",并"关闭"复位功能
	_delay_ms(50);
}



/****************************************************************************
*函数名-Function:	void Init_Register_LCD_TFT_ILI9325(void)
*描述- Description:		初始化: "彩屏LCD_TFT_ILI9325"  控制芯片的寄存器
*输入参数-Input:	None
*输出参数-output:	None
*注意事项-Note：	
*****************************************************************************/
void Init_Register_LCD_TFT_ILI9325(void)  //初始化: "彩屏LCD_TFT_ILI9325"  控制芯片的寄存器
{

//////////////////////////////////////////////////////////////
//==Start Initial Sequence===============================//
//--Set internal timing----------------------------------//
	LCD_TFT_WriteReg(0x00E3, 0x3008); // Set internal timing
	LCD_TFT_WriteReg(0x00E7, 0x0012); // Set internal timing
	LCD_TFT_WriteReg(0x00EF, 0x1231); // Set internal timing

//--变址寄存器（IR）:指定寄存器的地址R00h ~ RFFh或者要访问的RAM的地址。----------------------------------//
//--状态读取(RS)反映了ILI9325内部的状态。//L[7:0] 指示目前正在驱动TFT面板的驱动线位置----------------------------------//
//--起振 （R00h）//当读该寄存器时，器件号码”9325”H被读出----------------------------------//

//--输出驱动控制(R01h)--SM=0--SS=1 -------------------------------//
	//SM=0，逐行扫描//SM=1，隔行扫描--隔行时，可放大显示图像，但像素降半
	//SS:从源驱动器选择输出的转变方向。--选择从上向下显示，或从下向上显示
		//当SS=0 输出转变方向是从S1到S720  //当SS=1 输出转变方向是从S720 到S1
#if defined USE_HORIZONTAL_LCD_TFT    //使用的"水平(横向)显示模式"
	LCD_TFT_WriteReg(0x0001, 0x0100); // 令SS=1//还要改"R01h"	、"R03h"	、"R60h"	
		
#elif defined USE_VERTICAL_LCD_TFT       //使用的"垂直(纵向)显示模式"
	LCD_TFT_WriteReg(0x0001, 0x0100); // 令SS=0//还要改"R01h"	、"R03h"	、"R60h"	
#endif

//--LCD驱动波形控制(R02h)--B/C= 1--EOR=1------------------------------//
	//B/C= 0：帧/场倒位// B/C= 1：行反转
	//EOR: EOR=1和B/C=1 设置行反转
	LCD_TFT_WriteReg(0x0002, 0x0700); // set 1 line inversion

//--进入模式(R03h)--TRI = 1--DFI= 0--BGR = 0--ORG = 0--I/D[1:0]=11--AM = 0--------//
	//TRI: 当TRI = 1的时候，在8位数据模式下是以 8bit * 3传输的，也就是传输三个字节到内部的RAM，
	    //同样也支持16位数据的模式，和使用SPI模式显示26万色，也就是说当TRI = 1 的时候，
		//传输的字节数基本上都是三个。这一位在显示26万色的时候有用的，或者使用8位数据接口的时候，
		//这个要看具体的应用来设置，但是注意如果不需要的时候，要设置为0.
	//DFI : 设置像内部RAM传输数据的的模式。这一位是要和TRI联合起来使用的。
	//BGR 交换写数据中红和蓝
		//BGR = 0 : 根据RGB顺序写像素点的数据。//BGR = 1:  交换RGB数据为BGR,写入GRAM
	//ORG : 当一个窗口的地址区域确定以后，根据上面I/D的设置，来移动原始地址。当高速写窗口地址域时，这个功能将被使能。
	//注意：1、当ORG =1 的时候，设置R20H,R21H,的原始地址的时候，只能设置0x0000  // 2、在RAM读操作时，要保证ORG = 0;
		//ORG = 0: 原始地址是不移动的。这种情况下，是通过指定地址来启动写操作的，这个地址是根据窗口显示区域的GRAM的地址表。
		//ORG = 1:原始地址是更加I/D的设置相应的移动的。
	//当一个窗口区域是由寄存器R50h ~R53h 来设置时，GRAM 区域地址更新仅仅根据I/D[1:0]和AM 位的设置来确定。
	//I/D[1:0] 的正确设置才能正确的显示图片，比如有时候发现显示出来的图片和输入img2lcd的图片方向是左右方向是反的，
		//或者上下 或者都是反的，那就是需要修改这个的地方了，可以根据上面的方向来选择合适的I/D.
	//I/D[1:0]=11=当更新一个像素数据时，I/D[1:0]位控制地址计数器（AC）自动增加或者减少1
	//这个地方对AM的选择将直接影响img2lcd软件的扫描方式控制项，这一位就是控制扫描方式的。
	//AM: 控制GRAM的更新方向
		//AM = 0: 在水平方向更新地址//AM = 1: 在垂直方向更新地址
			//@水平方向:递增； 垂直方向:递增
#if defined USE_HORIZONTAL_LCD_TFT    //使用的"水平(横向)显示模式"
	LCD_TFT_WriteReg(0x0003, 0x1038); // 令AM=1//还要改"R01h"	、"R03h"	、"R60h"	
		
#elif defined USE_VERTICAL_LCD_TFT       //使用的"垂直(纵向)显示模式"
	LCD_TFT_WriteReg(0x0003, 0x1030); // 令AM=0//还要改"R01h"	、"R03h"	、"R60h"	
#endif

  
//--调整控制寄存器(R04h)--RCV[1:0]=00--RCH[1:0]=00--RSZ[1:0]=00---------//
//比例缩放设置：不缩放//以调整为1/2为例，要注意一下，调整图像是9325自己调整的，但是你仍然需要把原始图像的数据全部写入到9325，只不过是9325不一定全部把数据都写入内部的GRAM,而是根据调整因子有选择的把原始图像的数据写入GRAM。
	//RCV[1:0]:同上面的RCH原理是一样的，这个是来保证垂直方向上减去几个像素点正好能被RSZ除尽。
		//00=0像素（1像素=1RGB） //01=1像素  //10=2像素  //11=3像素
	//RCH[1:0]: 当调整图像大小的时候设置水平余下的像素点的个数。实际上就是拿当前的图像的水平像素个数和缩小后水平像素个数取模，
		//原因是由于你的图像不可能正好能被缩小1/2，或者1/4，比如你的图像水平像素点是15个，如果需要缩小为1/2，
		//但是15除以2是有余数的，余数为1，RCH[1:0]这个时候就设置为1，实际上就是保证你的原始图像水平减去几个像素点正好能被RSZ除尽。
		//00=0像素（1像素=1RGB） //01=1像素  //10=2像素  //11=3像素
	//当设置了RSZ后，ILI9325将会根据RSZ设置的参数来调整图片的大小，这个时候水平和垂直方向的区域都会改变。
	//RSZ[1:0] : 设置调整参数（RSZ的意思就是resizeing）
		//00=不调整(X1)   //01=X1 /2   //10=设定禁止   //11=X1  /4
	LCD_TFT_WriteReg(0x0004, 0x0000); // Resize register


//////////////////////////////////////////////////////////////
//==显示控制===================================//

//--显示控制2(R08h)--FP[3:0]=010--BP[3:0]=010--设置后边沿和前沿------------//
	//FP[3:0]和 BP[3:0] 位 分别指定前后边沿周期的行号码 （porch 边沿，入口 ，periods 周期）
	//当设定FP[3:0]和 BP[3:0] 值时要保证以下情况：
		//BP + FP ≤ 16 lines       FP ≥ 2 lines       BP ≥ 2 lines
		//FP[3:0]=010== 2 Lines
		//BP[3:0]=010== 2 Lines
	LCD_TFT_WriteReg(0x0008, 0x0202); // set the back porch and front porch

//--显示控制3 (R09h)--PTS[2:0]=000--PTG[1:0]=00--ISC[3:0]=0000--设置非显示区时间间隔------//
	//PTS[2:0]设置不显示区域驱动周期的源输出级别
	//注意：1.在不显示驱动周期降低升压时钟频率和终止灰度运放可以提高电源效率
	          //2.在不亮显示区域的输出门级别的驱动周期由PTG[1:0]位来决定
		//当PTS[2]=1 产生灰度的运放（不是V0和V63）的操作被中止，不显示 驱动的升压时钟频率将会变成原来时钟的一半，以减少功率消耗。
		//PTS[2:0]=000=Positive Polarity=V63, Negative Polarity=V0
	//PTG[1:0] 设置 不显示区域的扫描模式。
		//00=Normal scan;    01=Setting Prohibited   10=Interval scan;  11=Setting Prohibited
	//ISC[3:0]: 当设定PTG[1:0]=10 选择扫描间隔时，指定门驱动在非显示区域的扫描间隔周期，扫描周期设定为在0~29帧周期中的奇数值，每个扫描周期极性都会反转。
		//ISC[3:0]=0000==扫描间隔周期为0帧
	LCD_TFT_WriteReg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]

//--显示控制4(R0Ah)--FMARKOE=0--FMI[2:0]=000--帧标记功能-----//
	//当FMARKOE=1，在FMI[2:0]输出间隔设置位设置下ILI9325开始产生FMARK信号
	//FMI[2:0] 根据显示数据重写周期和数据传输速率，设置FMARK信号的输出间隔
		//FMI[2:0]=000=输出间隔为1帧 //001=输出间隔为2帧
	    //011=输出间隔为4帧//101=输出间隔为5帧//其他=Setting Disabled
	LCD_TFT_WriteReg(0x000A, 0x0000); // FMARK function

//--RGB显示接口控制1(R0Ch)--ENC[2:0] =000--RM=0--DM[1:0] =00--RIM[1:0] =00------//
	//ENC[2:0] 通过RGB接口设置GRAM 的写周期
		//000=1帧  001=2帧  010=3帧  011=4帧
		//100=5帧  101=6帧  110=7帧  111=8帧
	//RM 选择访问GRAM 的接口当通过RGB接口写显示数据的时候设置RM=1
	//注意1：寄存器只能通过系统接口或者是SPI接口来设定
		   //2：模式转换参照RGN输入接口流程图部分
		//0=System interface/VSYNC interface    1=RGB interface
	//DM[1:0] 选择显示操作模式
	//DM[1:0] 的设置允许在内部时钟操作模式和外部显示接口操作模式之间切换。
	//禁止在RGB接口操作模式和VSYNC接口操作模式之间切换。
		//DM[1:0] =00=Internal system clock   01=RGB interface  10=VSYNC interface  11=Setting disabled
	//RIM[1:0] 选择RGB数据接口宽度
	//注意1：寄存器只能由系统接口设置
	       //2：当转换接口的时候保证一个像素数据（ 3 个点）传输完毕。
		//00=18-bit RGB interface(1 tansfer/pixel)   01=16-bit RGB interface(1 tansfer/pixel)
		//10= 6-bit RGB interface(3 tansfer/pixel)    11=Setting Disabled
	LCD_TFT_WriteReg(0x000C, 0x0000); // RGB interface setting

//--帧标记位(R0Dh)--EMP[8:0]=00000000--帧标记位置--------------------------//
	//EMP[8:0] 设置帧循环输出的位置（帧产生）
	//当FMP[8:0]=0 ，一个高主动脉冲 FMARK 在第一显示周期的 起始 后边沿周期 输出
    //保证：9’h000 ≦ FMP ≦ BP+NL+FP
	LCD_TFT_WriteReg(0x000D, 0x0000); // Frame marker Position

//--RGB显示接口控制2 (R0Fh)--VSPL = 0--HSPL = 0--EPL=0--DPL=0-------------//
	//VSPL: 设置VSYNC脚的信号极性
		//VSPL = 0 低电平有效    //VSPL = 1 高电平有效
	//HSPL:设置HSYNC脚的信号极性
		//HSPL = 0 低电平有效    //HSPL = 1 高电平有效
	//EPL: 设置使能脚的信号极性
		//EPL=0 当ENBLE=0 数据DB17-0 被写入 ，当ENBLE=1 不能进行写数据操作
		//EPL=1 当ENBLE=1 数据DB17-0 被写入 ，当ENBLE=0 不能进行写数据操作
	//DPL: 设置DOTCLK脚的信号输出极性
		//DPL=0 数据在 DOTCLK 的上升沿输出  DPL=1 数据在 DOTCLK 的下降沿输出
	LCD_TFT_WriteReg(0x000F, 0x0000); // RGB interface polarity


//////////////////////////////////////////////////////////////
//==电源控制Power On sequence =======================//

//--电源控制1(R10h)--SAP=0--BT[3:0] =000--APE=0--AP[2:0]=000--DSTB=0--SLP=0--STB=0--------------//
	//SAP：源驱动输出控制
		//SAP=0，源驱动禁止   //SAP=1，源驱动使能
	//BT[3:0] 设置使用升压电路的参数//为工作电压选择最佳升压因数，为了减少功率消耗设置最小的电压因数
	//设置APE=1 根据电源供给顺序开始供电
	//AP[2:0]: 在LCD电源供给电路调整操作运放电路的稳定电流。最大稳定电流可以
	//提高LCD 的驱动能力，但也增加了电流消耗。在不显示周期，设置AP[2:0]=000
	//停止运行运放电路，升高电路电压减少电流消耗。
		//AP[2:0]=000=Gamma Driver amplifiers为Halt; Source Driver amplifiers 为Halt；//001=Gamma Driver amplifiers为1; Source Driver amplifiers 为1；
			    //010=Gamma Driver amplifiers为1; Source Driver amplifiers 为0.75；   //011=Gamma Driver amplifiers为1; Source Driver amplifiers 为0.5；
			    //100=Gamma Driver amplifiers为0.75; Source Driver amplifiers 为1；   //101=Gamma Driver amplifiers为0.75; Source Driver amplifiers 为0.75；
			    //110=Gamma Driver amplifiers为0.75; Source Driver amplifiers 为0.5；//111=Gamma Driver amplifiers为0.5; Source Driver amplifiers 为0.5；
	//DSTB:当DSTB=1 ,ILI9325进入深度待机模式， 在深度待机模式，内部逻辑电源供给关闭以减少电源消耗，
		//GRAM和指令数据不会被保存在深度待机模式下，退出深度待机模式后，他们需要被复位。
	//SLP: 当SLP=1，ILI9325 进入睡眠模式，除了RC振荡器外显示操作都停止，以减少电源消耗。 
		//在睡眠模式，GRAM数据和指令不会被更新，除非通过下列两条指令：a，退出睡眠模式（SLP=0） b，开始震荡
	//STB: 当STB=1 ,ILI9325进入待机模式，除了GRAM 电源供给外，显示操作停止以减少电源消耗。
		//在待机模式，GRAM数据和指令不会被更新，除非通过下列两条指令：a，退出待机模式（STB） b，开始震荡
	LCD_TFT_WriteReg(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
	
//--电源控制2 (R11h)--DC1[2:0] =000--DC0[2:0] =000--VC[2:0]=111-------------//	
	//注意：保证 fDCDC1≥fDCDC2 当设定 DC0[2:0] 和DC1[2:0] 的时候
	//DC1[2:0] 选择升压电路2的工作频率。更高的升压电路工作频率可以提高升压电路的驱动能力和显示质量，但是会增加电流消耗，调整工作频率需要综合考虑显示质量和电路消耗。
		//000=Step-up频率为 Fosc/8    //010=Step-up频率为 Fosc/16    //011=Step-up频率为 Fosc/32
		//100=Step-up频率为 Fosc/64  //101=Step-up频率为 Fosc/128  //110=Step-up频率为 Fosc/256
		//111=Halt Step-up Circuit2
	//DC0[2:0] 选择升压电路1的工作频率。更高的升压电路工作频率可以提高升压电路的驱动能力和显示质量，但是会增加电流消耗，调整工作频率需要综合考虑显示质量和电路消耗。
		//000=Step-up频率为 Fosc/2     //010=Step-up频率为 Fosc/4    //011=Step-up频率为 Fosc/8
		//100=Step-up频率为 Fosc/16  //101=Step-up频率为 Fosc/32   //110=Step-up频率为 Fosc/64
		//111=Halt Step-up Circuit1
	//VC[2:0] :设置产生参考电压Vci1 的比例因数
		//000=0.95*Vci   001=0.90*Vci    010=0.85*Vci    011=0.80*Vci  
		//100=0.75*Vci   101=0.70*Vci    110=Disabled     111=1.0 *Vci
	LCD_TFT_WriteReg(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
	
//--电源控制3 (R12h)--VCIRE=0--PON=0--VHR[3:0] =0000--------------//	
	//VCIRE: 选择外部参考电压Vci 或者内部参考电压VCIR
		//VCIRE=0 外部参考电压Vci (默认)  //VCIRE =1  内部参考电压 2.5V
	//PON :控制电路3（VGL）输出的开和关
		//PON=0 VGL output is disabled     //PON=1 VGL output is enabled
	//VHR[3:0] 设置应用于VREG1OUT级的输出的放大速率（1.6~1.9），VREG1OUT级是VCOM级和灰度电压产生级的参考级
	//当Vci<2.5V，内部参考电压将会与Vci相同. 确保 VC 和VRH的设定限制在VREG1OUT ≦ (DDVDH - 0.2)V.
		//当VCIRE=0时，0000=VREG1OUT=Halt； 0001=VREG1OUT=Vci*2.00;    0010=VREG1OUT=Vci*2.05; ...
		//当VCIRE=1时，0000=VREG1OUT=Halt； 0001=VREG1OUT=2.5V*2.00;  0010=VREG1OUT=2.5V*2.05; ...
	LCD_TFT_WriteReg(0x0012, 0x0000); // VREG1OUT voltage
	
//--电源控制4(R13h)--VDV[4:0]=00000--------------------------------//
	//VDV[4:0] 选择VREG1OUT因数 来设置Vcom的交互电压振幅 范围为0.70 ~1.24  x VREG1OUT
		//设置VDV[4:0]让Vcom幅度小于6V
		//00000= Vcom amplitude= VREG1OUT *0.70;  00001= Vcom amplitude= VREG1OUT *0.72; ...
	LCD_TFT_WriteReg(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
	_delay_ms(200); // Dis-charge capacitor power voltage
	
//--电源控制1(R10h)----------------------------------//
	//SAP=1，源驱动使能
	//BT[3:0]=6=设置DDVDH=Vci1*2;  VCL=-Vci1; VGH=Vci1*4; VGL=-Vci1*3
	//APE=1 根据电源供给顺序开始供电
	//AP[2:0]001=Gamma Driver amplifiers为1; Source Driver amplifiers 为1；
	//DSTB=0,ILI9325 没有进入深度待机模式
	//SLP =0,ILI9325 没有进入睡眠模式
	//STB =0,ILI9325 没有进入待机模式
	LCD_TFT_WriteReg(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB

//--电源控制2 (R11h)----------------------------------//
	//DC1[2:0] =010==选择升压电路2的工作频率=Step-up频率为 Fosc/16  
	//DC0[2:0] =010==选择升压电路1的工作频率=Step-up频率为 Fosc/4  
	//VC[2:0] =111=设置产生参考电压Vci1 的比例因数=1.0 *Vci
	LCD_TFT_WriteReg(0x0011, 0x0227); // R11h=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]
	_delay_ms(50); // Delay 50ms
	
//--电源控制3 (R12h)----------------------------------//	
	//VCIRE=0 外部参考电压Vci (默认)  
	//PON=1 VGL output is enabled  //PON :控制电路3（VGL）输出的开和关
	//VHR[3:0] 设置应用于VREG1OUT级的输出的放大速率（1.6~1.9），VREG1OUT级是VCOM级和灰度电压产生级的参考级
	//VCIRE=0，故当VHR[3:0]=1100=设置VREG1OUT=Vci*1.80; 
	LCD_TFT_WriteReg(0x0012, 0x001C); // External reference voltage= Vci;
	_delay_ms(50); // Delay 50ms
	
//--电源控制4(R13h)------------------------------------//
	//VDV[4:0] 选择VREG1OUT因数 来设置Vcom的交互电压振幅 范围为0.70 ~1.24  x VREG1OUT
	//VDV[4:0] =11000= 设置Vcom amplitude= VREG1OUT *1.10;    //care//设置VDV[4:0]让Vcom幅度小于6V
	LCD_TFT_WriteReg(0x0013, 0x1800); // R13=1200 when R12=009D;VDV[4:0] for VCOM amplitude
	
//--电源控制 7(R29h)----------------------------------//
	//VCM[5:0] 设置内部 VcomH 电压=VREG1OUT * 0.825
	LCD_TFT_WriteReg(0x0029, 0x001C); // R29=000C when R12=009D;VCM[5:0] for VCOMH

//--帧速率和颜色控制(R2Bh)----------------------------------//
	//FRS[3:0]=1101=当内部电阻应用于震荡电路的时候，设置帧速率=128
	LCD_TFT_WriteReg(0x002B, 0x000D); // Frame Rate = 91Hz
	_delay_ms(50); // Delay 50ms

//--GRAM 水平位置寄存器变址 0x20----------------------------------//
	//AD[16:0] 设置变址计数器（AC）的初始化数值。
	//变址计数器（AC）会根据AM 和I/D位的设置来自动的更新其数值，当数据写入内部的GARM 中时。
	//当从内部GRAM中读数据时变址计数器不会自动更新其数值。
		//AD[16:0]=00000000=选择GRAM Data Map=第1行 GRAM Data  
	LCD_TFT_WriteReg(0x0020, 0x0000); // GRAM horizontal Address

//--GRAM 垂直位置寄存器变址 0x21----------------------------------//
	//AD[16:0] 设置变址计数器（AC）的初始化数值。
	//变址计数器（AC）会根据AM 和I/D位的设置来自动的更新其数值，当数据写入内部的GARM 中时。
	//当从内部GRAM中读数据时变址计数器不会自动更新其数值。
		//AD[16:0]=00000000=选择GRAM Data Map=第1行 GRAM Data  
	LCD_TFT_WriteReg(0x0021, 0x0000); // GRAM Vertical Address

//--写数据到GRAM(R22h)------------------------------------------------------//
//--从GRAM 中读数据（Read Data from GRAM (R22h)）--------------------------//


////////////////////////////////////////////////////////////////
//==伽马校正(R30h～R3Dh)==Adjust the Gamma Curve ========//
	LCD_TFT_WriteReg(0x0030, 0x0007);
	LCD_TFT_WriteReg(0x0031, 0x0302);
	LCD_TFT_WriteReg(0x0032, 0x0105);
	LCD_TFT_WriteReg(0x0035, 0x0206);
	LCD_TFT_WriteReg(0x0036, 0x0808);
	LCD_TFT_WriteReg(0x0037, 0x0206);
	LCD_TFT_WriteReg(0x0038, 0x0504);
	LCD_TFT_WriteReg(0x0039, 0x0007);
	LCD_TFT_WriteReg(0x003C, 0x0105);
	LCD_TFT_WriteReg(0x003D, 0x0808);

//////////////////////////////////////////////////////////////////////////////////////////
//==窗口设置=Set GRAM area  =窗口变址必须在 GRAM 变址空间之内=========//

//--水平方向窗口-------------------------------//
	//在设定这些位时必须保证 ："00h" ≤ HSA[7:0]< HEA[7:0] ≤"EFh"=239. 和"04h"≦ HEA-HSA.
	//通过设置HSA和HEA位能够限定写数据时候的GRAM的水平区域。
	//HSA和HEA位必须在开始写RAM操作之前就设定好。
	LCD_TFT_WriteReg(0x0050, 0x0000); //水平方向窗口变址的起始位置 Horizontal GRAM Start Address
	LCD_TFT_WriteReg(0x0051, 0x00EF); //水平方向窗口变址的终止位置 Horizontal GRAM End Address

//--垂直方向窗口-------------------------------//
	//在设定这些位时必须保证 ："000h"≤VSA[8:0]<VEA[8:0]≤"13Fh"=319
	//VSA 和 VEA 位能够限定写数据时候的 GRAM 的垂直区域。
	//VSA 和 VEA 位必须在开始写 RAM 操作之前就设定好。
	LCD_TFT_WriteReg(0x0052, 0x0000); //垂直方向窗口变址的起始位置 Vertical GRAM Start Address
	LCD_TFT_WriteReg(0x0053, 0x013F); //垂直方向窗口变址的终止位置 Vertical GRAM End Address

///////////////////////////////////////////////////////////////
//==门扫描设置=Gate Scan Control (R60h, R61h, R6Ah)========//

//--R60h--GS = 1--NL[5:0]=27H--SCN[5:0] =000000---------------//
	//GS: Sets the direction of scan by the gate driver in the range determined by SCN[4:0] and NL[4:0].//
		//GS = 0, the scan direction is from G1 to G320.  //GS = 1, the scan direction is from G320 to G1
	//NL[5:0]: Sets the number of lines to drive the LCD at an interval of 8 lines.
		//The GRAM address mapping is not affected by the number of lines set by NL[5:0]. 
		//The number of lines must be the same or more than the number of lines necessary for the size of the liquid crystal panel.
		//NL[5:0]=27H=LCD Drive Line=320 line
	//SCN[5:0] The ILI9325C allows to specify the gate line from which the gate driver starts to scan by setting the SCN[5:0] bits.
		//SCN[5:0] =000000 =Scanning Start Position=G320(当GS=1)  //与SM位和GS位有关。

#if defined USE_HORIZONTAL_LCD_TFT    //使用的"水平(横向)显示模式"
	LCD_TFT_WriteReg(0x0060, 0x2700); //令GS=0//还要改"R01h"	、"R03h"	、"R60h"	

#elif defined USE_VERTICAL_LCD_TFT       //使用的"垂直(纵向)显示模式"
	LCD_TFT_WriteReg(0x0060, 0xA700); //令GS=1//还要改"R01h"	、"R03h"	、"R60h"	
#endif


//--R61h--NDL=0--VLE=0--REV=1-------------------------------//
	//NDL: Sets the source driver output level in the non-display area.
		//NDL=0=Positive Polarity为V63；Negative Polarity为V0；
		//NDL=1=Positive Polarity为V0； Negative Polarity为V63；
	//VLE: Vertical scroll display enable bit. When VLE = 1, the ILI9325C starts displaying the base image from the line 
		//(of the physical display) determined by VL[8:0] bits. VL[8:0] sets the amount of scrolling, which is the number of lines 
		//to shift the start line of the display from the first line of the physical display. Note that the partial image display position is not affected by the base image scrolling.
		//The vertical scrolling is not available in external display interface operation. In this case, make sure to set VLE = "0".
	//REV: Enables the grayscale inversion of the image by setting REV=1.
	LCD_TFT_WriteReg(0x0061, 0x0001); // NDL,VLE, REV

//--R6Ah--VL[8:0]=00000000-------------------------------//
	//VL[8:0]: Sets the scrolling amount of base image. The base image is scrolled in vertical direction and displayed from 
		//the line determined by VL[8:0]. Make sure that VL[8:0] <= 320
	LCD_TFT_WriteReg(0x006A, 0x0000); // set scrolling line


//--SPI Read/Write Control (R66h, Write Only)-------------------------------//


////////////////////////////////////////////////////////////////
//==局部显示控制=Partial Display Control ================//

//--Partial Image 1 Display Position (R80h)------------------------------//
	//PTDP0[8:0]: Sets the display start position of partial image 1. The display areas of the partial images 1 and 2 must not overlap each another.
	LCD_TFT_WriteReg(0x0080, 0x0000);
	
//--Partial Image 1 RAM Start/End Address (R81h, R82h)-------------------//
	//PTSA0[8:0] PTEA0[8:0]: Sets the start line address and the end line address of the RAM area 
	//storing the data of partial image 1. Make sure PTSA0[8:0]  PTEA0[8:0].
	LCD_TFT_WriteReg(0x0081, 0x0000);
	LCD_TFT_WriteReg(0x0082, 0x0000);
	
//--Partial Image 2 Display Position (R83h)------------------------------//
	//PTDP1[8:0]: Sets the display start position of partial image 2 The display areas of the partial images 1 and 2 must not overlap each another.
	LCD_TFT_WriteReg(0x0083, 0x0000);

//--Partial Image 2 RAM Start/End Address (R84h, R85h)--------------------//
	//PTSA1[8:0] PTEA1[8:0]: Sets the start line address and the end line address of the RAM area 
	//storing the data of partial image 2 Make sure PTSA1[8:0]  PTEA1[8:0].
	LCD_TFT_WriteReg(0x0084, 0x0000);
	LCD_TFT_WriteReg(0x0085, 0x0000);
	
////////////////////////////////////////////////////////////////
//==Panel Control ====================================//

//--Panel Interface Control 1 (R90h)----------------------------//
	//RTNI[4:0]: Sets 1H (line) clock number of internal clock operating mode. 
	//In this mode, ILI9325C display operation is synchronized with internal clock signal.
		//RTNI[4:0]=10000=Clocks/Line为16 clocks
	//DIVI[1:0]: Sets the division ratio of internal clock frequency.
		//DIVI[1:0]=00=Division Ratio为1；Internal Operation Clock Frequency=fosc / 1；
	LCD_TFT_WriteReg(0x0090, 0x0010);

//--Panel Interface Control 2 (R92h)---------------------------//
	//Note: The gate output non-overlap period is defined by the number of frequency-divided internal clocks, 
		//the frequency of which is determined by instruction (DIVI), from the reference point.
	//NOWI[2:0]: Sets the gate output non-overlap period when ILI9325C display operation is synchronized with internal clock signal.
		//NOWI[2:0]=000=Gate Non-overlap Period=Setting inhibited
	LCD_TFT_WriteReg(0x0092, 0x0000);

//--Panel Interface Control 3 (R93h)-----------------------------------//	
	LCD_TFT_WriteReg(0x0093, 0x0003);

//--Panel Interface Control 4 (R95h)-----------------------------------//
	//DIVE[1:0]: Sets the division ratio of DOTCLK when ILI9325C display operation is synchronized with RGB interface signals.
	LCD_TFT_WriteReg(0x0095, 0x0110);

//--Panel Interface Control 5 (R97h)-----------------------------------//	
	//NOWE[3:0]: Sets the gate output non-overlap period when the ILI9325C display operation is synchronized with RGB interface signals.
		//NOWE[3:0]=0000=Gate Non-overlap Period=Setting inhibited
	LCD_TFT_WriteReg(0x0097, 0x0000);
	LCD_TFT_WriteReg(0x0098, 0x0000);

////////////////////////////////////////////////////////////////
//==其他寄存器==================================//
//--OTP VCM Programming Control (RA1h)  //--OTP VCM Status and Enable (RA2h)   //--OTP Programming ID Key (RA5h)
//--Write Display Brightness Value (RB1h)  //--Read Display Brightness Value (RB2h) 
//--Write CTRL Display Value (RB3h)         //--Read CTRL Display Value (RB4h)   
//--Write Content Adaptive Brightness Control Value (RB5h)    //--Read Content Adaptive Brightness Control Value (RB6h) 
//--Write CABC Minimum Brightness (RBEh)   //--Read CABC Minimum Brightness (RBFh)   
//--CABC Control 1 (RC7h)  //--CABC Control 1 (RC8h)   //--CABC Control 2 (RC9h)   //--CABC Control 3 (RCAh)  
//--CABC Control 4 (RCBh)   //--CABC Control 5 (RCCh)  //--CABC Control 6 (RCDh)   //--CABC Control 7 (RCEh) 


////////////////////////////////////////////////////////////////
//==显示使能=====================================//	
//--显示控制1 (R07h)--PTDE1/0 = 0--BASEE=1--GON=1; DTE=1--CL=0--D[1:0] = 11-----------------//
	//PTDE[1:0] 部分图像2 和部分图像1 使能位
		//PTDE1/0 = 0: 关闭部分图像，仅显示基本图像。
		//PTDE1/0 = 1: 开部分图像，设置基本图像显示使能位为0（BASEE=0）
	//BASEE 基本图像显示位。(D [1:0]设置比BASEE设置有更高的优先权。)
		//当BASEE=0  没有基本图像显示。ILI9325驱动液晶在不亮显示级别或者仅仅显示部分图像。
		//当BASEE=1  显示基本图像。
	//GON 和 DTE 是按照如下的 来设定G1～G320 的门驱动输出级别的.
		//GON=0; DTE=0; ==G1～G320输出VGH    //GON=0; DTE=1; ==G1～G320输出VGH
		//GON=1; DTE=0; ==G1～G320输出VGL    //GON=1; DTE=1; ==G1～G320正常显示输出
	//CL=0=262144色显示模式被选择。//CL=1=8色显示模式被选择。
	//D[1:0] 设定D[1:0]=11 来打开显示屏，设定D[1:0]=00 来关显示屏
	//通过和GON, DTE 位联合来设定，D[1:0] 来设定显示控制的 开或关。
	//当D1=0 屏上没有显示，所有的源输出呈低电平，以减少LCD用交流（AC）电压驱动液晶时产生的充电/放电电流。
		//D[1:0] = “01”, 来关闭显示，ILI9325仍然进行内部的显示操作。
		//D[1:0] = “00”, 来关闭显示时 ILI9325 的内部显示操作完全停止。
		//D[1:0] = “11”, 打开显示
	LCD_TFT_WriteReg(0x0007, 0x0133); // 262K color and display ON
	
}


/****************************************************************************
*函数名-Function:	void LCD_TFT_SetWindow(u16 xSta, u16 ySta, u16 xEnd, u16 yEnd)
*描述- Description:		设置显示窗口的区域大小(起点和终点)-"彩屏LCD_TFT_ILI9325"  
*输入参数-Input:	(xSta,ySta):窗口起点坐标； (yStat,yEnd):窗口终点坐标
*输出参数-output:	None
*注意事项-Note：	
*****************************************************************************/
void LCD_TFT_SetWindow(u16 xSta, u16 ySta, u16 xEnd, u16 yEnd)  //设置显示窗口的区域大小(起点和终点)-"彩屏LCD_TFT_ILI9325"  
{
	ILI9325_WriteRegIndex(0x50); //水平开始坐标
	ILI9325_WriteData(xSta);

	ILI9325_WriteRegIndex(0x52); //垂直开始坐标
	ILI9325_WriteData(ySta);


	ILI9325_WriteRegIndex(0x51);  //水平结束坐标
	ILI9325_WriteData(xEnd);

	ILI9325_WriteRegIndex(0x53);  //垂直结束坐标
	ILI9325_WriteData(yEnd);

	//LCD_WriteRegIndex(0x22);
}


/****************************************************************************
*函数名-Function:	void LCD_TFT_SetCursor(u16 x, u16 y)
*描述- Description:		设置GRAM的光标位置-"彩屏LCD_TFT_ILI9325"  
*输入参数-Input:	x: x轴 坐标;   y:y轴 坐标
*输出参数-output:	None
*注意事项-Note：	
*****************************************************************************/
void LCD_TFT_SetCursor(u16 x, u16 y)  //设置GRAM的光标位置-"彩屏LCD_TFT_ILI9325"  
{
	LCD_TFT_WriteReg(0x20, x);  // x轴 坐标
	LCD_TFT_WriteReg(0x21, y);  // y轴 坐标
}



/****************************************************************************
*函数名-Function:	void LCD_TFT_Clear(u16 color)
*描述- Description:		全屏清屏-"彩屏LCD_TFT_ILI9325"  
*输入参数-Input:	color: 清屏填充的颜色
*输出参数-output:	None
*注意事项-Note：	
*****************************************************************************/
void LCD_TFT_Clear(u16 color)   //全屏清屏-"彩屏LCD_TFT_ILI9325"  
{
	u32 index = 0;

	LCD_TFT_SetCursor(0x0000, 0x0000);  //设置GRAM的光标位置(0,0)
	
	LCD_TFT_WriteRAM_CMD(); //向ILI9325写"GRAM命令地址"
	
	for(index = 0; index < 76800; index++)
	{
		LCD_TFT_WriteRAM(color);   //向每个GRAM写入数据
	}
}





/****************************************************************************
*函数名-Function:	void LCD_TFT_DrawPoint(u16 x, u16 y, u16 color)
*描述- Description:		画点( 指定坐标,颜色)-"彩屏LCD_TFT_ILI9325"  
*输入参数-Input:	x,y: 起点坐标;   color : 颜色 
*输出参数-output:	None
*注意事项-Note：
*****************************************************************************/
void LCD_TFT_DrawPoint(u16 x, u16 y, u16 color) //画点( 指定坐标,颜色)-"彩屏LCD_TFT_ILI9325"  
{
	LCD_TFT_SetCursor(x, y);//设置GRAM的光标位置
	
	LCD_TFT_WriteRAM_CMD(); //向ILI9325写"GRAM命令地址"

	LCD_TFT_WriteRAM(color);//向GRAM写入数据
}



/****************************************************************************
*函数名-Function:	void LCD_TFT_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
*描述- Description:		画线-"彩屏LCD_TFT_ILI9325"  
*输入参数-Input:	x1,y1: 起点坐标;  x2,y2: 终点坐标;  color:线的颜色  
*输出参数-output:	None
*注意事项-Note：	
*****************************************************************************/
void LCD_TFT_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)  //画线-"彩屏LCD_TFT_ILI9325"  
{
	u16 t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;

	delta_x=x2-x1; //计算坐标增量
	delta_y=y2-y1;
	
	uRow=x1;
	uCol=y1;
	
	if(delta_x>0)incx=1; //设置单步方向
	else if(delta_x==0)incx=0;//垂直线
	else {incx=-1;delta_x=-delta_x;}
	
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;//水平线
	else{incy=-1;delta_y=-delta_y;}
	
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
	else distance=delta_y;
	
	for(t=0;t<=distance+1;t++ )//画线输出
	{
		LCD_TFT_DrawPoint(uRow,uCol,color);//画点
		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}

}



/****************************************************************************
*函数名-Function:	void LCD_TFT_DrawRectangle(u16 xSta, u16 ySta, u16 xEnd, u16 yEnd,u16 color)
*描述- Description:		画矩形框-"彩屏LCD_TFT_ILI9325"  
*输入参数-Input:	x1,y1: 起点坐标;  x2,y2: 终点坐标;  color:线的颜色  
*输出参数-output:	None
*注意事项-Note：	
*****************************************************************************/
void LCD_TFT_DrawRectangle(u16 xSta, u16 ySta, u16 xEnd, u16 yEnd,u16 color)  //画矩形框-"彩屏LCD_TFT_ILI9325"  
{
	LCD_TFT_DrawLine(xSta,ySta,xSta,yEnd,color); 
	LCD_TFT_DrawLine(xSta,yEnd,xEnd,yEnd,color); 
	LCD_TFT_DrawLine(xEnd,yEnd,xEnd,ySta,color); 
	LCD_TFT_DrawLine(xEnd,ySta,xSta,ySta,color); 
}



/****************************************************************************
*函数名-Function:	void LCD_TFT_DrawCircle(u16 x0,u16 y0,u8 r,u16 color)
*描述- Description:		画圆-"彩屏LCD_TFT_ILI9325"  
*输入参数-Input:	(x,y):中心点;  r    :半径; color:线的颜色 
*输出参数-output:	None
*注意事项-Note：	
*****************************************************************************/
void LCD_TFT_DrawCircle(u16 x0,u16 y0,u8 r,u16 color)  //画圆-"彩屏LCD_TFT_ILI9325"  
{
	int a,b;
	int di;
	a=0;b=r;
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_TFT_DrawPoint(x0-b,y0-a,color);             //3
		LCD_TFT_DrawPoint(x0+b,y0-a,color);             //0
		LCD_TFT_DrawPoint(x0-a,y0+b,color);          //   //1
		LCD_TFT_DrawPoint(x0-b,y0-a,color);         //    //  7
		LCD_TFT_DrawPoint(x0-a,y0-b,color);          //   //2
		LCD_TFT_DrawPoint(x0+b,y0+a,color);          //   //4
		LCD_TFT_DrawPoint(x0+a,y0-b,color);             //5
		LCD_TFT_DrawPoint(x0+a,y0+b,color);             //6
		LCD_TFT_DrawPoint(x0-b,y0+a,color);
		a++;
		//使用Bresenham算法画圆
		if(di<0)di +=4*a+6;
		else
		{
			di+=10+4*(a-b);
			b--;
		}
		LCD_TFT_DrawPoint(x0+a,y0+b,color);
	}

}


/****************************************************************************
*函数名-Function:	void LCD_TFT_FillRectangle(u16 xSta,u16 ySta,u16 xEnd,u16 yEnd,u16 color) 
*描述- Description:		填充一块LCD矩形区域(指定大小、颜色)-"彩屏LCD_TFT_ILI9325" 
*输入参数-Input:	(xSta,ySta): 起点坐标;  (xEnd,yEnd):终点坐标; color: 填充颜色
*输出参数-output:	None
*注意事项-Note：	
*****************************************************************************/
void LCD_TFT_FillRectangle(u16 xSta,u16 ySta,u16 xEnd,u16 yEnd,u16 color)  //填充一块LCD矩形区域(指定大小、颜色)-"彩屏LCD_TFT_ILI9325" 
{
	u16 i = 0;
	u16 j = 0;
	u16 Length = 0;

#if defined USE_HORIZONTAL_LCD_TFT    //使用的"水平(横向)显示模式"
	Length = yEnd - ySta + 1;

	for(i = xSta; i <= xEnd; i++)
	{
		LCD_TFT_SetCursor(i,ySta);	 //设置光标位置
		LCD_TFT_WriteRAM_CMD(); 	 //向ILI9325写"GRAM命令地址"
		
		for(j = 0; j < Length; j++) LCD_TFT_WriteRAM(color);//向GRAM写入数据
	}

#elif defined USE_VERTICAL_LCD_TFT       //使用的"垂直(纵向)显示模式"
	Length = xEnd - xSta + 1;

	for(i = ySta; i <= yEnd; i++)
	{
		LCD_TFT_SetCursor(xSta,i);	 //设置光标位置
		LCD_TFT_WriteRAM_CMD(); 	 //向ILI9325写"GRAM命令地址"
		
		for(j = 0; j < Length; j++) LCD_TFT_WriteRAM(color);//向GRAM写入数据
	}

#endif
}


/****************************************************************************
*函数名-Function:	void LCD_TFT_ShowChar(u16 x,u16 y,u8 AscNum,u8 Font,u16 pointColor,u16 backColor)
*描述- Description:		显示一个字符( 指定坐标,字体大小,颜色)-"彩屏LCD_TFT_ILI9325"  
*输入参数-Input:	x:  x轴 坐标;                                       y:  y轴 坐标;
						AscNum: 显示的字符(ASCII码);     Font:字体大小 12/16;
						pointColor: 画笔颜色;                        backColor: 背景颜色;
*输出参数-output:	None
*注意事项-Note：	
****************************************************************************/
void LCD_TFT_ShowChar(u16 x,u16 y,u8 AscNum,u8 Font,u16 pointColor,u16 backColor) //显示一个字符( 指定坐标,字体大小,颜色)-"彩屏LCD_TFT_ILI9325"  
{
    u8  AscChar = 0;
    u8  i = 0;
	u8  j = 0;
	
	u8  VerticalNum = 0;
	u8  horizontalNum = 0;

	AscNum = AscNum - 0x20;//得到偏移后的值//0x20为空格值

///////////////////////////////////////////////////////////////////////
//==使用的"水平(横向)显示模式"
///////////////////////////////////////////////////////////////////////
#if defined USE_HORIZONTAL_LCD_TFT 	//使用的"水平(横向)显示模式"
	u16 yBuf = 0;
	yBuf = y;  //备份y 坐标值

	switch(Font) 
	{
		case FONT1206:	//字体12*6
			VerticalNum = 12;
			horizontalNum = 6;
			break;	
			
		case FONT1608:  //字体16*8
			VerticalNum = 16;
			horizontalNum = 8;
			break;	
		default:   break;
	}

	/////////////////////////////////////////////////////////////////////////////
	for(i = 0; i < VerticalNum; i++)
	{
		switch(Font) 
		{
			case FONT1206:	//字体12*6
				AscChar = asc2_1206[AscNum][i];//调用1206字体
				break;	
				
			case FONT1608:	//字体16*8
				AscChar = asc2_1608[AscNum][i];//调用1608字体
				break;	
			default:   break;
		}
		
		/////////////////////////////////////////////////////////////////////////////
		for(j = 0; j < horizontalNum; j++)
		{
			if(AscChar & 0x01) LCD_TFT_DrawPoint(x, y, pointColor);  // 画字符处的点
			else  LCD_TFT_DrawPoint(x, y, backColor);  //画空白处的点

			AscChar >>= 1;
			y ++;
		}
		
		y = yBuf;
		x ++;
	}


///////////////////////////////////////////////////////////////////////
//==使用的"垂直(纵向)显示模式"
///////////////////////////////////////////////////////////////////////
#elif defined USE_VERTICAL_LCD_TFT  //使用的"垂直(纵向)显示模式"
	u16 xBuf = 0;
	xBuf = x;  //备份x 坐标值

	switch(Font) 
	{
		case FONT1206:	//字体12*6
			VerticalNum = 12;
			horizontalNum = 6;
			break;	
			
		case FONT1608:  //字体16*8
			VerticalNum = 16;
			horizontalNum = 8;
			break;	
		default:   break;
	}

	/////////////////////////////////////////////////////////////////////////////
	for(i = 0; i < VerticalNum; i++)
	{
		switch(Font) 
		{
			case FONT1206:	//字体12*6
				AscChar = asc2_1206[AscNum][i];//调用1206字体
				break;	
				
			case FONT1608:	//字体16*8
				AscChar = asc2_1608[AscNum][i];//调用1608字体
				break;	
			default:   break;
		}

		/////////////////////////////////////////////////////////////////////////////
		for(j = 0; j < horizontalNum; j++)
		{
			if(AscChar & 0x01) LCD_TFT_DrawPoint(x, y, pointColor);  // 画字符处的点
			else  LCD_TFT_DrawPoint(x, y, backColor);  //画空白处的点

			AscChar >>= 1;
			x ++;
		}
		
		x = xBuf;
		y ++;
	}

#endif
}



/****************************************************************************
*函数名-Function:	void LCD_TFT_ShowString(u16 line,u16 column,u8 *ArrayPoint,u8 Font,u16 pointColor,u16 backColor)
*描述- Description:		显示字符串( 指定坐标,字体大小,颜色)-"彩屏LCD_TFT_ILI9325"  
*输入参数-Input:	line:    第Line行;             column:   第column例;
						*ArrayPoint:字符串数组起始地址;     
						Font:字体大小 12/16;    pointColor: 画笔颜色;     backColor: 背景颜色;
*输出参数-output:	None
*注意事项-Note：	
****************************************************************************/
void LCD_TFT_ShowString(u16 line,u16 column,char *ArrayPoint,u8 Font,u16 pointColor,u16 backColor) //显示字符串( 指定坐标,字体大小,颜色)-"彩屏LCD_TFT_ILI9325"
{
	u16 x = 0;
	u16 y = 0;
	
	u8  VerticalNum = 0;
	u8  horizontalNum = 0;

	u16 max_x = 0;
	u16 max_y = 0;

#if defined USE_HORIZONTAL_LCD_TFT    //使用的"水平(横向)显示模式"
	x = line;
	y = column;

	switch(Font) 
	{
		case FONT1206:	//字体12*6
			VerticalNum = 12;
			horizontalNum = 6;
			
			max_x = 234;
			max_y = 314;
			break;	

		case FONT1608:  //字体16*8
			VerticalNum = 16;
			horizontalNum = 8;
			
			max_x = 232;
			max_y = 312;
			break;	
		default:   break;
	}

	//////////////////////////////////////////////////////////////////////
    while(*ArrayPoint != '\0')
	{
        if(x > max_x) {x = y = 0; LCD_TFT_Clear(backColor);}
        if(y > max_y) {y = 0; x += VerticalNum;}
		
		LCD_TFT_ShowChar(x,y,*ArrayPoint,Font,pointColor,backColor);
		
		y += horizontalNum;    //坐标地址累加，指向下一个字符的显示位置
        ArrayPoint ++;  //数组指针累加，指向下一个字符
    }
	
#elif defined USE_VERTICAL_LCD_TFT       //使用的"垂直(纵向)显示模式"
	x = column;
	y = line;
	switch(Font) 
	{
		case FONT1206:	//字体12*6
			VerticalNum = 12;
			horizontalNum = 6;
			break;	
			
		case FONT1608:  //字体16*8
			VerticalNum = 16;
			horizontalNum = 8;
			break;	
		default:   break;
	}

	//////////////////////////////////////////////////////////////////////
	while(*ArrayPoint != '\0')
	{		
        if(x > max_x) {x = 0; y += VerticalNum;}
        if(y > max_y) {x = y = 0; LCD_TFT_Clear(backColor);}
		
		LCD_TFT_ShowChar(x,y,*ArrayPoint,Font,pointColor,backColor);
		
		x += horizontalNum;    //坐标地址累加，指向下一个字符的显示位置
        ArrayPoint ++;  //数组指针累加，指向下一个字符
    }
#endif
}


/****************************************************************************
*函数名-Function:	void LCD_TFT_Show_ChFont2424(void)
*描述- Description:		显示1个24*24的汉字-"彩屏LCD_TFT_ILI9325"  
*输入参数-Input:	(x,y):汉字显示的位置    index:tfont24数组里面的第几个汉字
						pointColor: 汉字画笔颜色;     backColor: 背景颜色;
*输出参数-output:	None
*注意事项-Note：	
****************************************************************************/
void LCD_TFT_Show_ChFont2424(u16 x,u16 y,u8 index,u16 pointColor,u16 backColor) //显示1个24*24的汉字-"彩屏LCD_TFT_ILI9325"  
{
	u8 byteNum = 0;
	u8 i = 0;
	u8 temp;
	
	u16 x0=x;
	
	for(byteNum = 0; byteNum < 72; byteNum++)//每个24*24的汉字点阵 有72个字节
	{
		//二维数组每一维最大24字节
		if(byteNum < 24) temp = tfont24[index*3][byteNum];             //前24个字节
		else if(byteNum < 48) temp=tfont24[index*3 + 1][byteNum - 24]; //中24个字节
		else temp = tfont24[index*3 + 2][byteNum - 48];                //后24个字节
	
		for(i = 0; i < 8; i++)	 //每个字节8位画点
		{
			if(temp & 0x80) LCD_TFT_DrawPoint(x,y,pointColor); //画汉字的点
			else LCD_TFT_DrawPoint(x,y,backColor);   //画空白点（使用背景色）
			temp <<= 1;
			x ++;
			if((x-x0) == 24)	  //达到点阵的最大值则开始新的一行
			{
				x=x0;
				y++;
				break;
			}
		}
	}
}









/****************************************************************************
*函数名-Function:	void LCD_TFT_Show_StandbyPage(void)
*描述- Description:		显示"待机页面"-"彩屏LCD_TFT_ILI9325"  
*输入参数-Input:	None
*输出参数-output:	None
*注意事项-Note：
****************************************************************************/
void LCD_TFT_Show_StandbyPage(void) //显示"待机页面"-"彩屏LCD_TFT_ILI9325"  
{	
	u16 yStart = 0;
	
	LCD_TFT_Clear(WHITE); //全屏清屏-"彩屏LCD_TFT_ILI9325"
//	LCD_TFT_Clear(GREEN); //全屏清屏-"彩屏LCD_TFT_ILI9325"
//	LCD_TFT_Clear(BLUE); //全屏清屏-"彩屏LCD_TFT_ILI9325"

	LCD_TFT_FillRectangle(0, 0, 39, 319, RED);
//	LCD_TFT_FillRectangle(40, 0, 79, 319, BLACK);
//	LCD_TFT_FillRectangle(80, 0, 119, 319, BLUE);
//	LCD_TFT_FillRectangle(120, 0, 159, 319, GREEN);
//	LCD_TFT_FillRectangle(160, 0, 199, 319, GBLUE);
//	LCD_TFT_FillRectangle(200, 0, 239, 319, YELLOW);
	
	yStart = 24;
	LCD_TFT_ShowString(12, yStart, "2023", FONT1608, WHITE, RED);
	yStart += 8 * 4;
	LCD_TFT_ShowChString(8, yStart, "年", WHITE, RED);
	yStart += 24; // 年
	LCD_TFT_ShowString(12, yStart, "MSP430", FONT1608, WHITE, RED);
	yStart += 8 * 6;
	LCD_TFT_ShowChString(8, yStart, "单片机课程设计", WHITE, RED);
	LCD_TFT_ShowChString(48, 88, "电子信息工程",  BLACK, WHITE);
	LCD_TFT_ShowChString(88, 88, "冯子希",  BLACK, WHITE);
	LCD_TFT_ShowString(92, 160, "B20012614", FONT1608, BLACK, WHITE);
	LCD_TFT_ShowChString(128, 100, "邹雨",  BLACK, WHITE);
	LCD_TFT_ShowString(132, 160, "B20012617", FONT1608, BLACK, WHITE);
	LCD_TFT_ShowString(212, 120, "2023.10.15", FONT1608, BLACK, WHITE);
}


/****************************************************************************
*函数名-Function:	void LCD_TFT_Show_QuestionPage(void)
*描述- Description:		显示题目界面
*输入参数-Input:	None
*输出参数-output:	None
*注意事项-Note：
****************************************************************************/
void LCD_TFT_Show_QuestionPage(void)//显示题目界面
{
	LCD_TFT_ShowChString( 8, 0, "洗衣机控制器设计",  BLACK, WHITE);
	LCD_TFT_ShowChString(48, 0, "基本要求：\n自动洗衣流程，洗涤→漂洗→脱水→漂洗→脱水→关机。\n用ＬＣＤ显示当前状态，状态剩余洗涤时间。\n用键盘设计启动、暂停、停止键，在洗衣过程中键都有效。\n设计一个预约键，用于定时启动洗衣机。\n洗涤完成后用声音提示。\n发挥部分：\n不同模式下的洗涤过程\n设计一个水位键，用于指定不同水位。",BLACK, WHITE);
}

/****************************************************************************
*函数名-Function:	void LCD_TFT_ShowChString(u16 line,u16 column,u8 *ArrayPoint,u16 pointColor,u16 backColor)
*描述- Description:		显示字符串( 指定坐标,字体大小,颜色)-"彩屏LCD_TFT_ILI9325"
*输入参数-Input:	line:    第Line行;             column:   第column例;
						*ArrayPoint:字符串数组起始地址;
						pointColor: 画笔颜色;     backColor: 背景颜色;
*输出参数-output:	None
*注意事项-Note：
****************************************************************************/
void LCD_TFT_ShowChString(u16 line, u16 column, char *ArrayPoint, u16 pointColor,u16 backColor) //显示字符串( 指定坐标,字体大小,颜色)-"彩屏LCD_TFT_ILI9325"
{
	u16 x = 0;
	u16 y = 0;

	u8 VerticalNum = 24;
	u8 horizontalNum = 24;

	u16 max_x = 0;
	u16 max_y = 0;

	u16 ChNum;

	x = line;
	y = column;

	max_x = 216;
	max_y = 296;

	while (*ArrayPoint != '\0') {
		if (*ArrayPoint == '\n') {
			y = 0;
			x += VerticalNum;
			ArrayPoint++;
		} else if (y > max_y) {
			y = 0;
			x += VerticalNum;
		}
		if (x > max_x) {
			x = y = 0;
			LCD_TFT_Clear(backColor);
		}

		ChNum = findChineseIndex(ArrayPoint);
		LCD_TFT_Show_ChFont2424(x, y, ChNum, pointColor, backColor);

		y += horizontalNum; //坐标地址累加，指向下一个汉字的显示位置
		ArrayPoint += 2; //数组指针累加，指向下一个汉字
	}
}



/****************************************************************************
* 函数名 Function:   unsigned char findChineseIndex(const char* chineseChar)
* 描述 Description:  在字库中查找指定汉字的索引位置
* 输入参数 Input:     chineseChar - 要查找的汉字字符串，以两个字节表示一个汉字
* 输出参数 Output:    返回指定汉字在字库中的索引位置（以每个汉字为单位的索引）
* 注意事项 Note:
****************************************************************************/
u16 findChineseIndex(const char* chineseChar)
{
    u16 index = 0xffff; // 默认为0xffff，表示未找到
    u8 i;

    // 遍历字库
    for (i = 0; tfontIndex[i] != '\0' && tfontIndex[i + 1] != '\0'; i += 2) {
        // 检查当前位置和下一位置是否与目标汉字相等
        if (tfontIndex[i] == chineseChar[0] && tfontIndex[i + 1] == chineseChar[1]) {
            index = i >> 1; // 计算索引
            break; // 找到后结束循环
        }
    }

    return index;
}



