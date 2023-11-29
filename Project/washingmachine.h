#ifndef __WASHINGMACHINE_H
#define __WASHINGMACHINE_H

#include <stdint.h>
#define MAX_SEQUENCE_LENGTH 10
typedef uint32_t u32; //unsigned long
typedef uint16_t u16; //unsigned int
typedef uint8_t u8; //unsigned char

// ����ϴ�»���״̬
typedef enum WashingMachineState {
	IDLE, // ����
	PAUSED, // ��ͣ
	FILLING, // ��ˮ
	WASHING, // ϴ��
	RINSING, // Ưϴ
	SPINNING, // ��ˮ
	FINISHED, // ���
	SCHEDULED // ԤԼ
} WashingMachineState;

// ��������
typedef enum Button {
	NONE = 0, // û�м�������
	START = 1, // ��ʼ��
	PAUSE = 2, // ��ͣ��
	STOP = 3, // ֹͣ��
	TIMER = 4, // ԤԼ��
	SETPRO = 5, // ����ѡ���
	SETWATER = 6 // ˮλѡ���
} Button;

//  ���岻ͬ���͵�ϴ�³���
typedef enum WashProgramType {
	STANDARD, // ��׼ϴ
	QUICK, // ��ϴ
	SUPER, // ����ϴ
	WOOL, // ��ë
	SOAK, // ����
	ONLYSPIN // ��ˮ
	//CUSTOMPRO, // �û��Զ������
} WashProgramType;

// ���岻ͬˮλ
typedef enum WaterLevel {
	LOW,
	MIDDLE,
	HIGH
}WaterLevel;

// ����ϴ�³���ṹ�壬�����������͡�ϴ���������С�����ʱ��
typedef struct WashProgramStruct {
	WashProgramType pro; //ϴ�³���
	WashingMachineState proSeq[MAX_SEQUENCE_LENGTH]; //��������
	u16 proTime[MAX_SEQUENCE_LENGTH]; //����ʱ��
} WashProgramStruct;

void u16ToString(u16 num, char *str);// ����ת�ַ�������
u16 sumArray(u16* arr, u16 size);// ������ͺ���
void initDisplay(void);// LCD��ʼ��ʾ��������ʾ�˵���һ��������
void updateDisplay(WashingMachineState state, u16 remainingTime);// LCD��ʾ������������ʾ��ǰ״̬��ʣ��ʱ��
void washStart(WashProgramStruct* proStructPointer);// ��ʼϴ�³���
void alarm(void);// ������ʾ
Button readButtonInput(void);// ��ȡ�������룬�����Ϸ���
WashingMachineState doIDLE(void);
WashingMachineState doPAUSED(WashingMachineState lastState);
WashingMachineState doWORKING(void);
WashingMachineState doFINISHED(void);
void runWashingMachine(void); // ��ѭ������

#endif
