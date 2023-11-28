#include "Hardware_Profile.h"  //处理器"硬件配置文件" -Hardware specific definitions for Processor
#include "washingmachine.h"
#include "tm1638.h"
#include "LCD_TFT_ILI9325.h"

WashProgramType proTypeSet; // 洗衣程序选择
u8 proSeqIndex; //当前洗衣环节序号
u16 remainingTime; // 当前环节剩余时间
u16 totalRemainingTime; // 总剩余时间

u16 scheduledTimeList[] = { 30, 60, 90, 120, 180 }; // 预约时间选择列表
char scheduledTimeIndex = (char)-1; // 列表序号 -1:无预约 0-4:有预约
u16 scheduledTime = 0; // 预约的时间

WaterLevel waterLevel = LOW; // 水位

WashProgramStruct WashingProgramSettings[7] = { 
	{
		STANDARD, //标准洗
		{ FILLING, WASHING, RINSING, SPINNING, FILLING, RINSING, SPINNING, FINISHED},
		{ 5, 20, 20, 5, 5, 10, 10}
	},
	{
		QUICK, //速洗
		{ FILLING, WASHING, RINSING, SPINNING, FINISHED},
		{ 5, 15, 10, 5},
	},
	{
		SUPER, //超级洗
		{ FILLING, WASHING, RINSING, SPINNING, FILLING, RINSING, SPINNING, FINISHED},
		{ 5, 30, 30, 5, 5, 20, 10},
	},
	{
		WOOL, //羊毛
		{ FILLING, WASHING, RINSING, SPINNING, FILLING, RINSING, SPINNING, FINISHED},
		{ 5, 30, 30, 10, 5, 20, 15},
	},
	{
		SOAK, //浸泡
		{ FILLING, WASHING, RINSING, SPINNING, FILLING, RINSING, SPINNING, FINISHED},
		{ 60, 20, 20, 5, 5, 10, 10},
	},
	{
		ONLYSPIN, //脱水
		{ SPINNING, FINISHED},
		{ 15},
	}
};

// 数字转字符串函数
void u16ToString(u16 num, char* str) {
	u8 i = 3;
	str[3] = '\0';

	//如果数字不合法，返回"999"
	if (num > 999) {
		for (i = 0; i < 3; i++)
			str[i] = '9';
		return;
	}

	// 从数字的最低位开始提取并存储到字符串中
	do {
		i--;
		str[i] = '0' + (num % 10); // 将数字转换为字符并存储在字符串中
		num /= 10;
	} while (num != 0 && i > 0);

	// 如果数字不足 MAX_DIGITS 位数，向前补空格
	while (i > 0) {
		i--;
		str[i] = ' ';
	}
}

// 数组求和函数
u16 sumArray(u16* arr, u16 size) {
	u16 sum = 0;
	u16 i;
	for (i = 0; i < size; ++i) {
		sum += arr[i];
	}
	return sum;
}

// LCD初始显示函数，显示菜单等一次性内容
void initDisplay(void) {
	u8 i;
	char stateStr[][5] = { "启动", "暂停", "停止", "预约", "程序", "水位" };
	char numStr[2] = "0";
	LCD_TFT_Clear(GBLUE);
	LCD_TFT_FillRectangle(0, 0, 39, 319, BLUE);
	LCD_TFT_ShowChString(7, 88, "洗衣机控制器", WHITE, BLUE);
	LCD_TFT_FillRectangle(38, 0, 39, 319, BLACK);
	LCD_TFT_ShowChString(48, 0, "按键说明：", BLACK, GBLUE);
	for (i = 1; i <= 3; i++) { // "启动", "暂停", "停止"
		numStr[0] = i + '0';
		LCD_TFT_ShowString((i + 1) * 40 + 12, 0, numStr, FONT1608, BLACK, GBLUE);
		LCD_TFT_ShowChString((i + 1) * 40 + 8, 8, stateStr[i - 1], BLACK, GBLUE);
	} // 列 56
	for (i = 4; i <= 6; i++) { // "预约", "程序", "水位"
		numStr[0] = i + '0';
		LCD_TFT_ShowString((i - 2) * 40 + 12, 58, numStr, FONT1608, BLACK, GBLUE);
		LCD_TFT_ShowChString((i - 2) * 40 + 8, 66, stateStr[i - 1], BLACK, GBLUE);
	} // 列 114

	LCD_TFT_FillRectangle(40, 120, 239, 121, BLACK);

	LCD_TFT_ShowChString( 48, 128, "洗衣程序：", BLACK, GBLUE);
	LCD_TFT_ShowChString( 88, 128, "当前状态：", BLACK, GBLUE);
	LCD_TFT_ShowChString(128, 128, "剩余时间：", BLACK, GBLUE);
	LCD_TFT_ShowChString(168, 128, "预约时间：", BLACK, GBLUE);
	LCD_TFT_ShowChString(208, 128, "　水位　：", BLACK, GBLUE);

	LCD_TFT_ShowChString(128, 260, "分钟", BLACK, GBLUE);
	LCD_TFT_ShowChString(168, 260, "分钟", BLACK, GBLUE);
}

char proTypeStr[][7] = { "标准洗", "速洗　", "超级洗", "羊毛　", "浸泡　", "脱水　", "自定义" };
char stateStr[][5] = { "关闭", "暂停", "进水", "洗涤", "漂洗", "脱水", "完成", "预约" };
char waterLevelStr[][3] = { "低", "中", "高" };

// LCD显示函数，用于显示当前状态和剩余时间
void updateDisplay(WashingMachineState state, u16 remainingTime) {
	// 显示当前状态和剩余时间
	// 区域 (40,114,239,319)

	char timeStr[4] = "999";
	LCD_TFT_ShowChString(48, 236, proTypeStr[proTypeSet], BLACK, GBLUE); // 洗衣程序选择
	LCD_TFT_ShowChString(88, 236, stateStr[state], BLACK, GBLUE); // 当前状态
	u16ToString(remainingTime, timeStr); // 转换成字符串
	LCD_TFT_ShowString(132, 236, timeStr, FONT1608, BLACK, GBLUE); // 剩余时间
	if (state == SCHEDULED) { // 预约状态显示剩余时间
		u16ToString(scheduledTime, timeStr);
		LCD_TFT_ShowString(172, 236, timeStr, FONT1608, BLACK, GBLUE);
	}
	else { // 非预约状态显示"---"
		LCD_TFT_ShowString(172, 236, "---", FONT1608, BLACK, GBLUE);
	}
	LCD_TFT_ShowChString(208, 236, waterLevelStr[waterLevel], BLACK, GBLUE); // 水位
}

// 开始洗衣程序
void washStart(WashProgramStruct* proStructPointer) {
	totalRemainingTime = sumArray(proStructPointer->proTime,
		MAX_SEQUENCE_LENGTH); // 总剩余时间
	proSeqIndex = 0; // 洗衣流程序号0
	remainingTime = proStructPointer->proTime[proSeqIndex];
}

void setWaterLevel(void) {
	waterLevel = (waterLevel == HIGH ? LOW : waterLevel + 1); // 低中高循环
}

void alarm(void) {
	u8 i;
	BUZZER_PORT |= BUZZER_BIT;
	// 屏幕显示
	for (i = 0; i < 3; i++) {
		LCD_TFT_ShowChString(88, 236, "完成", BLACK, GBLUE);
		_delay_ms(100);
		LCD_TFT_ShowChString(88, 236, "完成", GBLUE, BLACK);
		_delay_ms(100);
	}
	// 蜂鸣器
	for (i = 0; i < 4; i++) {
		BUZZER_PORT ^= BUZZER_BIT;
		_delay_ms(1000);
	}
	BUZZER_PORT &= ~BUZZER_BIT;
}

// 获取按键输入
Button readButtonInput(void) {
	u8 i = Read_key();
	if (i > 0 && i < 7) {
		//while (Read_key() == i);
		return i;
	}
	else {
		return NONE;
	}
}

WashingMachineState doIDLE(void) {
	WashingMachineState nextStatus; // 下一个状态
	WashProgramStruct* proStructPointer = &(WashingProgramSettings[proTypeSet]); // 指向当前洗衣流程
	Button but = readButtonInput(); // 获取按键
	switch (but) {
	case NONE:
	case PAUSE:
	case STOP: // 无效按键
		nextStatus = IDLE;
		break;
	case START: // 开始洗衣
		washStart(proStructPointer);
		nextStatus = proStructPointer->proSeq[proSeqIndex];
		break;
	case TIMER: // 设定预约时间
		scheduledTimeIndex = 0;
		scheduledTime = scheduledTimeList[scheduledTimeIndex];
		nextStatus = SCHEDULED;
		break;
	case SETPRO: // 设定洗衣程序
		proTypeSet = proTypeSet == ONLYSPIN ? STANDARD : proTypeSet + 1;
		nextStatus = IDLE;
		break;
	case SETWATER: // 设定水位
		setWaterLevel();
		nextStatus = IDLE;
		break;
	default:
		break;
	}
	return nextStatus;
}

WashingMachineState doPAUSED(WashingMachineState lastState) {
	WashingMachineState nextStatus; // 下一个状态
	Button but = readButtonInput(); // 获取按键
	switch (but) {
	case NONE:
	case PAUSE:
	case TIMER:
	case SETPRO:
	case SETWATER: // 无效按键
		nextStatus = PAUSED;
		break;
	case START: // 返回暂停前的状态
		nextStatus = lastState;
		break;
	case STOP: // 停止
		totalRemainingTime = 0;
		remainingTime = 0;
		nextStatus = IDLE;
		break;
	default:
		break;
	}
	return nextStatus;
}

WashingMachineState doWORKING(void) {
	WashingMachineState nextStatus; // 下一个状态
	WashProgramStruct* proStructPointer = WashingProgramSettings + proTypeSet; // 指向当前洗衣流程
	Button but = readButtonInput(); // 获取按键
	switch (but) {
	case NONE:
	case START:
	case TIMER:
	case SETPRO:
	case SETWATER: // 无效按键
		totalRemainingTime--;
		remainingTime--; // 时间递减
		if (remainingTime == 0) { // 当前流程结束
			proSeqIndex++; // 切换下一个流程
			remainingTime = proStructPointer->proTime[proSeqIndex];
		}
		nextStatus = proStructPointer->proSeq[proSeqIndex]; // 切换下一个流程
		break;
	case PAUSE: // 暂停
		nextStatus = PAUSED; // 进入暂停状态
		break;
	case STOP: // 停止
		totalRemainingTime = 0;
		remainingTime = 0;
		nextStatus = IDLE; // 返回空闲状态
		break;
	default:
		break;
	}
	return nextStatus;
}

WashingMachineState doFINISHED(void) {
	alarm(); // 报警提示
	return IDLE;
}

WashingMachineState doSCHEDULED(void) {
	WashingMachineState nextStatus; // 下一个状态
	WashProgramStruct* proStructPointer = &(WashingProgramSettings[proTypeSet]); // 指向当前洗衣流程
	Button but = readButtonInput(); // 获取按键
	switch (but) {
	case NONE:
	case PAUSE: // 无效按键
		if (scheduledTime == 0) { // 预约时间到
			washStart(proStructPointer); // 进入洗衣流程
			nextStatus = proStructPointer->proSeq[proSeqIndex];
		}
		else {
			scheduledTime--; // 时间递减
			nextStatus = SCHEDULED; // 依然是预约状态
		}
		break;
	case START:
		washStart(proStructPointer); // 直接进入洗衣
		nextStatus = proStructPointer->proSeq[proSeqIndex];
		break;
	case STOP: // 停止
		nextStatus = IDLE; // 取消预约
		break;
	case TIMER:
		if (scheduledTimeIndex == 4) {  // 到达预约时间最大值，取消预约
			nextStatus = IDLE;
		}
		else { // 下一个预约时间
			scheduledTimeIndex++;
			scheduledTime = scheduledTimeList[scheduledTimeIndex];
			nextStatus = SCHEDULED;
		}
		break;
	case SETPRO: // 选择洗衣程序
		proTypeSet = proTypeSet == ONLYSPIN ? STANDARD : proTypeSet + 1;
		nextStatus = SCHEDULED;
		break;
	case SETWATER: // 选择水位
		setWaterLevel();
		nextStatus = SCHEDULED;
		break;
	default:
		break;
	}
	return nextStatus;
}

// 执行洗衣机状态的更新和控制
void runWashingMachine(void) {
	WashingMachineState tempState; // 状态机内部临时状态
	WashingMachineState currentState = IDLE; // 当前状态
	WashingMachineState lastState = IDLE; // 上一个状态

	proTypeSet = STANDARD;

	initDisplay();
	while (1) {
		switch (currentState) { // 根据当前状态选择对应的函数
		case IDLE:
			tempState = doIDLE();
			break;
		case PAUSED:
			tempState = doPAUSED(lastState);
			break;
		case FILLING:
		case WASHING:
		case RINSING:
		case SPINNING:
			tempState = doWORKING();
			break;
		case FINISHED:
			tempState = doFINISHED();
			break;
		case SCHEDULED:
			tempState = doSCHEDULED();
			break;
		default:
			break;
		}

		//更新状态
		if (currentState != tempState) {
			lastState = currentState;
			currentState = tempState;
		}

		// 更新LCD显示
		updateDisplay(currentState, totalRemainingTime);

		//P4OUT ^= BIT5;
		//while(P4IN & BIT2);
		_delay_ms(10);
		// 软延时
	}
}
