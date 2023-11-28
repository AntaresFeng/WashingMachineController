#include "Hardware_Profile.h"  //������"Ӳ�������ļ�" -Hardware specific definitions for Processor
#include "washingmachine.h"
#include "tm1638.h"
#include "LCD_TFT_ILI9325.h"

WashProgramType proTypeSet; // ϴ�³���ѡ��
u8 proSeqIndex; //��ǰϴ�»������
u16 remainingTime; // ��ǰ����ʣ��ʱ��
u16 totalRemainingTime; // ��ʣ��ʱ��

u16 scheduledTimeList[] = { 30, 60, 90, 120, 180 }; // ԤԼʱ��ѡ���б�
char scheduledTimeIndex = (char)-1; // �б���� -1:��ԤԼ 0-4:��ԤԼ
u16 scheduledTime = 0; // ԤԼ��ʱ��

WaterLevel waterLevel = LOW; // ˮλ

WashProgramStruct WashingProgramSettings[7] = { 
	{
		STANDARD, //��׼ϴ
		{ FILLING, WASHING, RINSING, SPINNING, FILLING, RINSING, SPINNING, FINISHED},
		{ 5, 20, 20, 5, 5, 10, 10}
	},
	{
		QUICK, //��ϴ
		{ FILLING, WASHING, RINSING, SPINNING, FINISHED},
		{ 5, 15, 10, 5},
	},
	{
		SUPER, //����ϴ
		{ FILLING, WASHING, RINSING, SPINNING, FILLING, RINSING, SPINNING, FINISHED},
		{ 5, 30, 30, 5, 5, 20, 10},
	},
	{
		WOOL, //��ë
		{ FILLING, WASHING, RINSING, SPINNING, FILLING, RINSING, SPINNING, FINISHED},
		{ 5, 30, 30, 10, 5, 20, 15},
	},
	{
		SOAK, //����
		{ FILLING, WASHING, RINSING, SPINNING, FILLING, RINSING, SPINNING, FINISHED},
		{ 60, 20, 20, 5, 5, 10, 10},
	},
	{
		ONLYSPIN, //��ˮ
		{ SPINNING, FINISHED},
		{ 15},
	}
};

// ����ת�ַ�������
void u16ToString(u16 num, char* str) {
	u8 i = 3;
	str[3] = '\0';

	//������ֲ��Ϸ�������"999"
	if (num > 999) {
		for (i = 0; i < 3; i++)
			str[i] = '9';
		return;
	}

	// �����ֵ����λ��ʼ��ȡ���洢���ַ�����
	do {
		i--;
		str[i] = '0' + (num % 10); // ������ת��Ϊ�ַ����洢���ַ�����
		num /= 10;
	} while (num != 0 && i > 0);

	// ������ֲ��� MAX_DIGITS λ������ǰ���ո�
	while (i > 0) {
		i--;
		str[i] = ' ';
	}
}

// ������ͺ���
u16 sumArray(u16* arr, u16 size) {
	u16 sum = 0;
	u16 i;
	for (i = 0; i < size; ++i) {
		sum += arr[i];
	}
	return sum;
}

// LCD��ʼ��ʾ��������ʾ�˵���һ��������
void initDisplay(void) {
	u8 i;
	char stateStr[][5] = { "����", "��ͣ", "ֹͣ", "ԤԼ", "����", "ˮλ" };
	char numStr[2] = "0";
	LCD_TFT_Clear(GBLUE);
	LCD_TFT_FillRectangle(0, 0, 39, 319, BLUE);
	LCD_TFT_ShowChString(7, 88, "ϴ�»�������", WHITE, BLUE);
	LCD_TFT_FillRectangle(38, 0, 39, 319, BLACK);
	LCD_TFT_ShowChString(48, 0, "����˵����", BLACK, GBLUE);
	for (i = 1; i <= 3; i++) { // "����", "��ͣ", "ֹͣ"
		numStr[0] = i + '0';
		LCD_TFT_ShowString((i + 1) * 40 + 12, 0, numStr, FONT1608, BLACK, GBLUE);
		LCD_TFT_ShowChString((i + 1) * 40 + 8, 8, stateStr[i - 1], BLACK, GBLUE);
	} // �� 56
	for (i = 4; i <= 6; i++) { // "ԤԼ", "����", "ˮλ"
		numStr[0] = i + '0';
		LCD_TFT_ShowString((i - 2) * 40 + 12, 58, numStr, FONT1608, BLACK, GBLUE);
		LCD_TFT_ShowChString((i - 2) * 40 + 8, 66, stateStr[i - 1], BLACK, GBLUE);
	} // �� 114

	LCD_TFT_FillRectangle(40, 120, 239, 121, BLACK);

	LCD_TFT_ShowChString( 48, 128, "ϴ�³���", BLACK, GBLUE);
	LCD_TFT_ShowChString( 88, 128, "��ǰ״̬��", BLACK, GBLUE);
	LCD_TFT_ShowChString(128, 128, "ʣ��ʱ�䣺", BLACK, GBLUE);
	LCD_TFT_ShowChString(168, 128, "ԤԼʱ�䣺", BLACK, GBLUE);
	LCD_TFT_ShowChString(208, 128, "��ˮλ����", BLACK, GBLUE);

	LCD_TFT_ShowChString(128, 260, "����", BLACK, GBLUE);
	LCD_TFT_ShowChString(168, 260, "����", BLACK, GBLUE);
}

char proTypeStr[][7] = { "��׼ϴ", "��ϴ��", "����ϴ", "��ë��", "���ݡ�", "��ˮ��", "�Զ���" };
char stateStr[][5] = { "�ر�", "��ͣ", "��ˮ", "ϴ��", "Ưϴ", "��ˮ", "���", "ԤԼ" };
char waterLevelStr[][3] = { "��", "��", "��" };

// LCD��ʾ������������ʾ��ǰ״̬��ʣ��ʱ��
void updateDisplay(WashingMachineState state, u16 remainingTime) {
	// ��ʾ��ǰ״̬��ʣ��ʱ��
	// ���� (40,114,239,319)

	char timeStr[4] = "999";
	LCD_TFT_ShowChString(48, 236, proTypeStr[proTypeSet], BLACK, GBLUE); // ϴ�³���ѡ��
	LCD_TFT_ShowChString(88, 236, stateStr[state], BLACK, GBLUE); // ��ǰ״̬
	u16ToString(remainingTime, timeStr); // ת�����ַ���
	LCD_TFT_ShowString(132, 236, timeStr, FONT1608, BLACK, GBLUE); // ʣ��ʱ��
	if (state == SCHEDULED) { // ԤԼ״̬��ʾʣ��ʱ��
		u16ToString(scheduledTime, timeStr);
		LCD_TFT_ShowString(172, 236, timeStr, FONT1608, BLACK, GBLUE);
	}
	else { // ��ԤԼ״̬��ʾ"---"
		LCD_TFT_ShowString(172, 236, "---", FONT1608, BLACK, GBLUE);
	}
	LCD_TFT_ShowChString(208, 236, waterLevelStr[waterLevel], BLACK, GBLUE); // ˮλ
}

// ��ʼϴ�³���
void washStart(WashProgramStruct* proStructPointer) {
	totalRemainingTime = sumArray(proStructPointer->proTime,
		MAX_SEQUENCE_LENGTH); // ��ʣ��ʱ��
	proSeqIndex = 0; // ϴ���������0
	remainingTime = proStructPointer->proTime[proSeqIndex];
}

void setWaterLevel(void) {
	waterLevel = (waterLevel == HIGH ? LOW : waterLevel + 1); // ���и�ѭ��
}

void alarm(void) {
	u8 i;
	BUZZER_PORT |= BUZZER_BIT;
	// ��Ļ��ʾ
	for (i = 0; i < 3; i++) {
		LCD_TFT_ShowChString(88, 236, "���", BLACK, GBLUE);
		_delay_ms(100);
		LCD_TFT_ShowChString(88, 236, "���", GBLUE, BLACK);
		_delay_ms(100);
	}
	// ������
	for (i = 0; i < 4; i++) {
		BUZZER_PORT ^= BUZZER_BIT;
		_delay_ms(1000);
	}
	BUZZER_PORT &= ~BUZZER_BIT;
}

// ��ȡ��������
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
	WashingMachineState nextStatus; // ��һ��״̬
	WashProgramStruct* proStructPointer = &(WashingProgramSettings[proTypeSet]); // ָ��ǰϴ������
	Button but = readButtonInput(); // ��ȡ����
	switch (but) {
	case NONE:
	case PAUSE:
	case STOP: // ��Ч����
		nextStatus = IDLE;
		break;
	case START: // ��ʼϴ��
		washStart(proStructPointer);
		nextStatus = proStructPointer->proSeq[proSeqIndex];
		break;
	case TIMER: // �趨ԤԼʱ��
		scheduledTimeIndex = 0;
		scheduledTime = scheduledTimeList[scheduledTimeIndex];
		nextStatus = SCHEDULED;
		break;
	case SETPRO: // �趨ϴ�³���
		proTypeSet = proTypeSet == ONLYSPIN ? STANDARD : proTypeSet + 1;
		nextStatus = IDLE;
		break;
	case SETWATER: // �趨ˮλ
		setWaterLevel();
		nextStatus = IDLE;
		break;
	default:
		break;
	}
	return nextStatus;
}

WashingMachineState doPAUSED(WashingMachineState lastState) {
	WashingMachineState nextStatus; // ��һ��״̬
	Button but = readButtonInput(); // ��ȡ����
	switch (but) {
	case NONE:
	case PAUSE:
	case TIMER:
	case SETPRO:
	case SETWATER: // ��Ч����
		nextStatus = PAUSED;
		break;
	case START: // ������ͣǰ��״̬
		nextStatus = lastState;
		break;
	case STOP: // ֹͣ
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
	WashingMachineState nextStatus; // ��һ��״̬
	WashProgramStruct* proStructPointer = WashingProgramSettings + proTypeSet; // ָ��ǰϴ������
	Button but = readButtonInput(); // ��ȡ����
	switch (but) {
	case NONE:
	case START:
	case TIMER:
	case SETPRO:
	case SETWATER: // ��Ч����
		totalRemainingTime--;
		remainingTime--; // ʱ��ݼ�
		if (remainingTime == 0) { // ��ǰ���̽���
			proSeqIndex++; // �л���һ������
			remainingTime = proStructPointer->proTime[proSeqIndex];
		}
		nextStatus = proStructPointer->proSeq[proSeqIndex]; // �л���һ������
		break;
	case PAUSE: // ��ͣ
		nextStatus = PAUSED; // ������ͣ״̬
		break;
	case STOP: // ֹͣ
		totalRemainingTime = 0;
		remainingTime = 0;
		nextStatus = IDLE; // ���ؿ���״̬
		break;
	default:
		break;
	}
	return nextStatus;
}

WashingMachineState doFINISHED(void) {
	alarm(); // ������ʾ
	return IDLE;
}

WashingMachineState doSCHEDULED(void) {
	WashingMachineState nextStatus; // ��һ��״̬
	WashProgramStruct* proStructPointer = &(WashingProgramSettings[proTypeSet]); // ָ��ǰϴ������
	Button but = readButtonInput(); // ��ȡ����
	switch (but) {
	case NONE:
	case PAUSE: // ��Ч����
		if (scheduledTime == 0) { // ԤԼʱ�䵽
			washStart(proStructPointer); // ����ϴ������
			nextStatus = proStructPointer->proSeq[proSeqIndex];
		}
		else {
			scheduledTime--; // ʱ��ݼ�
			nextStatus = SCHEDULED; // ��Ȼ��ԤԼ״̬
		}
		break;
	case START:
		washStart(proStructPointer); // ֱ�ӽ���ϴ��
		nextStatus = proStructPointer->proSeq[proSeqIndex];
		break;
	case STOP: // ֹͣ
		nextStatus = IDLE; // ȡ��ԤԼ
		break;
	case TIMER:
		if (scheduledTimeIndex == 4) {  // ����ԤԼʱ�����ֵ��ȡ��ԤԼ
			nextStatus = IDLE;
		}
		else { // ��һ��ԤԼʱ��
			scheduledTimeIndex++;
			scheduledTime = scheduledTimeList[scheduledTimeIndex];
			nextStatus = SCHEDULED;
		}
		break;
	case SETPRO: // ѡ��ϴ�³���
		proTypeSet = proTypeSet == ONLYSPIN ? STANDARD : proTypeSet + 1;
		nextStatus = SCHEDULED;
		break;
	case SETWATER: // ѡ��ˮλ
		setWaterLevel();
		nextStatus = SCHEDULED;
		break;
	default:
		break;
	}
	return nextStatus;
}

// ִ��ϴ�»�״̬�ĸ��ºͿ���
void runWashingMachine(void) {
	WashingMachineState tempState; // ״̬���ڲ���ʱ״̬
	WashingMachineState currentState = IDLE; // ��ǰ״̬
	WashingMachineState lastState = IDLE; // ��һ��״̬

	proTypeSet = STANDARD;

	initDisplay();
	while (1) {
		switch (currentState) { // ���ݵ�ǰ״̬ѡ���Ӧ�ĺ���
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

		//����״̬
		if (currentState != tempState) {
			lastState = currentState;
			currentState = tempState;
		}

		// ����LCD��ʾ
		updateDisplay(currentState, totalRemainingTime);

		//P4OUT ^= BIT5;
		//while(P4IN & BIT2);
		_delay_ms(10);
		// ����ʱ
	}
}
