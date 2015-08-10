/*
 * WaterPump.cpp
 *
 *  Created on: 2015��8��6��
 *      Author: Chris
 */
#include <Arduino.h>
#include "WaterPump.h"

void WaterPump::init(const int ctrlPin)
{
	state = false;	// ��״̬��ˮ���Ƿ�����false��ʾû�й�����
	pin = ctrlPin;
	pinMode(ctrlPin, OUTPUT);
	digitalWrite(pin, HIGH);
}

void WaterPump::start(void)
{
	if (!state)
	{
		digitalWrite(pin, LOW);
		state = true;
	}
}

void WaterPump::stop(void)
{
	if (state)
	{
		digitalWrite(pin, HIGH);
		state = false;
	}
}
