/*
 * Core.cpp
 *
 *  Created on: 2015��8��6��
 *      Author: Chris
 */
#include "Core.h"
#include "../Debug/Debug.h"
#include "../Board/Board.h"
#include "../StatusLED/StatusLED.h"
#include "../Command/Command.h"
#include "../PushButton/PushButton.h"
#include "../RTClib/RTClib.h"
#include "../WaterPump/WaterPump.h"
#include "../SolenoidValve/SolenoidValve.h"
#include <Wire.h>
#include <string.h>

////////////////////////////////////////////////////////////
// RTCʱ��ģ��
////////////////////////////////////////////////////////////
RTC_DS1307 rtc;

////////////////////////////////////////////////////////////
// ��ʱ�Զ���ˮʱ�䣬�����ʼ��Ϊ2015��8��9��6ʱ30��0��
////////////////////////////////////////////////////////////
DateTime clock = DateTime(2015, 1, 1, 6, 30, 0);

////////////////////////////////////////////////////////////
// ״ָ̬ʾ��
////////////////////////////////////////////////////////////
// �Զ�ģʽ״̬LED
static StatusLED autoModeStatusLED(AUTO_MODE_STATUS_LED_PIN);

// �ֶ�ģʽ״̬LED
static StatusLED manualModeStatusLED(MANUAL_MODE_STATUS_LED_PIN);

// ��ˮ״̬LED
static StatusLED waterSupplyStatusLED(WATER_SUPPLY_STATUS_LED_PIN);

/////////////////////////////////////////////////////////////
// ˮ�ü���ŷ�
/////////////////////////////////////////////////////////////
WaterPump waterPump;
SolenoidValve solenoidValve;

/////////////////////////////////////////////////////////////
// ����ģʽ
/////////////////////////////////////////////////////////////
static OperatingMode operatingMode = Auto;

////////////////////////////////////////////////////////////
// ����Ƿ��ڹ�ˮ״̬
// true -- ���ڹ�ˮ
// false -- û�й�ˮ
////////////////////////////////////////////////////////////
static bool isSupplyingWaterFlag = false;

void initCore(void)
{
	initRtc();
	manualModeStatusLED.init();
	autoModeStatusLED.init();
	waterSupplyStatusLED.init();
	pushButtonInitAll();
	registerPushButtonReleasedEventHandler(pushButtonReleased);

	if (operatingMode == Auto)
	{
		autoModeStatusLED.on();
	}
	else if (operatingMode == Manual)
	{
		manualModeStatusLED.on();
	}

	waterPump.init(WATER_PUMP_CTRL_PIN);
	solenoidValve.init(SOLENOID_VALVE_CTRL_PIN);

	buildCmdLineTable();
}

void initRtc(void)
{
	Wire.begin();
	rtc.begin();
}

void commandLineProcess(const char* commandLine)
{
	executeCmdLine(commandLine);
}

void setClock(const char* time)
{
	DBG("call setClock(): ");
	if (strlen(time) == 5)
	{
		// ʱ
		int hour = (time[0] - '0') * 10 + (time[1] - '0');
		// ��
		int minute = (time[3] - '0') * 10 + (time[4] - '0');

		// ��������
		clock = DateTime(2015, 1, 1, hour, minute, 0);

		DBG(clock.hour());
		DBG(":");
		DBGLN(clock.minute());
	}
}

int toNum(int ch)
{
	return ch - '0';
}

void calibrateTime(const char* longTime)
{
	DBGLN("call calibrateTime().");
	if (strlen(longTime) == 19)
	{
		// ��
		int year = toNum(longTime[0]) * 1000 + toNum(longTime[1]) * 100
				+ toNum(longTime[2]) * 10 + toNum(longTime[3]) * 1;
		// ��
		int mon = toNum(longTime[5]) * 10 + toNum(longTime[6]) * 1;
		// ��
		int date = toNum(longTime[8]) * 10 + toNum(longTime[9]) * 1;
		// ʱ
		int hour = toNum(longTime[11]) * 10 + toNum(longTime[12]) * 1;
		// ��
		int min = toNum(longTime[14]) * 10 + toNum(longTime[15]) * 1;
		// ��
		int sec = toNum(longTime[17]) * 10 + toNum(longTime[18]) * 1;

		// ���õõ��µ�ʱ��
		DateTime newTime = DateTime(year, mon, date, hour, min, sec);
		rtc.adjust(newTime);
	}
}

bool isTimeUp(void)
{
	// ��ȡ��ǰʱ��
	// ֻ�Ƚ�ʱ�ͷּ���
	DateTime now = rtc.now();

	DBG(now.hour());
	DBG(":");
	DBG(now.minute());
	DBG(":");
	DBGLN(now.second());

	if (now.second() == clock.second() && now.minute() == clock.minute()
			&& now.hour() == clock.hour())
	{
		return true;
	}
	return false;
}

void supplyWater(void)
{
	DBGLN("call supplyWater().");
	if (isSupplyingWaterFlag)
	{
		return;
	}
	waterPump.start();
	solenoidValve.start();
	waterSupplyStatusLED.on();
	isSupplyingWaterFlag = true;
}

bool isWaterFull(void)
{
	return false;
}

void stopSupplyingWater(void)
{
	DBGLN("call stopSupplyingWater().");
	if (isSupplyingWaterFlag == false)
	{
		return;
	}
	waterPump.stop();
	solenoidValve.stop();
	waterSupplyStatusLED.off();
	isSupplyingWaterFlag = false;
}

void switchToManualMode(void)
{
	DBGLN("call switchToManualMode().");
	operatingMode = Manual;
	manualModeStatusLED.on();
	autoModeStatusLED.off();
}

void switchToAutoMode(void)
{
	DBGLN("call switchAutoMode().");
	operatingMode = Auto;
	autoModeStatusLED.on();
	manualModeStatusLED.off();
}

void waterSupplyStatusLEDToggle(void)
{
	waterSupplyStatusLED.toggle();
}

OperatingMode getOperatingMode(void)
{
	return operatingMode;
}

bool isSupplyingWater(void)
{
	return isSupplyingWaterFlag;
}

void pushButtonReleased(int keyCode)
{
	switch (keyCode)
	{
	case 0:		// ģʽ�л�
		DBGLN("Key 0");
		if (operatingMode == Manual)
		{
			switchToAutoMode();
		}
		else
		{
			switchToManualMode();
		}
		break;
	case 1:		// ��ˮ
		DBGLN("Key 1");
		if (operatingMode == Manual)
		{
			supplyWater();
		}
		break;
	case 2:		// ֹͣ��ˮ
		DBGLN("Key 2");
		stopSupplyingWater();
		break;
	default:
		break;
	}
}
