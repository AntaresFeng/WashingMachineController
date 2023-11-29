#ifndef __WASHINGMACHINE_H
#define __WASHINGMACHINE_H

#include <stdint.h>
#define MAX_SEQUENCE_LENGTH 10
typedef uint32_t u32; //unsigned long
typedef uint16_t u16; //unsigned int
typedef uint8_t u8; //unsigned char

// 定义洗衣机的状态
typedef enum WashingMachineState {
	IDLE, // 空闲
	PAUSED, // 暂停
	FILLING, // 进水
	WASHING, // 洗涤
	RINSING, // 漂洗
	SPINNING, // 脱水
	FINISHED, // 完成
	SCHEDULED // 预约
} WashingMachineState;

// 按键定义
typedef enum Button {
	NONE = 0, // 没有键被按下
	START = 1, // 开始键
	PAUSE = 2, // 暂停键
	STOP = 3, // 停止键
	TIMER = 4, // 预约键
	SETPRO = 5, // 程序选择键
	SETWATER = 6 // 水位选择键
} Button;

//  定义不同类型的洗衣程序
typedef enum WashProgramType {
	STANDARD, // 标准洗
	QUICK, // 速洗
	SUPER, // 超级洗
	WOOL, // 羊毛
	SOAK, // 浸泡
	ONLYSPIN // 脱水
	//CUSTOMPRO, // 用户自定义程序
} WashProgramType;

// 定义不同水位
typedef enum WaterLevel {
	LOW,
	MIDDLE,
	HIGH
}WaterLevel;

// 定义洗衣程序结构体，包括程序类型、洗衣流程序列、流程时间
typedef struct WashProgramStruct {
	WashProgramType pro; //洗衣程序
	WashingMachineState proSeq[MAX_SEQUENCE_LENGTH]; //流程序列
	u16 proTime[MAX_SEQUENCE_LENGTH]; //流程时间
} WashProgramStruct;

void u16ToString(u16 num, char *str);// 数字转字符串函数
u16 sumArray(u16* arr, u16 size);// 数组求和函数
void initDisplay(void);// LCD初始显示函数，显示菜单等一次性内容
void updateDisplay(WashingMachineState state, u16 remainingTime);// LCD显示函数，用于显示当前状态和剩余时间
void washStart(WashProgramStruct* proStructPointer);// 开始洗衣程序
void alarm(void);// 报警提示
Button readButtonInput(void);// 获取按键输入，并检查合法性
WashingMachineState doIDLE(void);
WashingMachineState doPAUSED(WashingMachineState lastState);
WashingMachineState doWORKING(void);
WashingMachineState doFINISHED(void);
void runWashingMachine(void); // 主循环函数

#endif
