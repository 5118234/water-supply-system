/*
 * SolenoidValve.cpp
 *
 *  Created on: 2015��8��6��
 *      Author: Chris
 */
#include <Arduino.h>
#include "SolenoidValve.h"

void SolenoidValve::init(const int ctrlPin)
{
	state = false;	// ��״̬��¼��ŷ��Ƿ�����false��ʾû�й�����
	pin = ctrlPin;
	pinMode(ctrlPin, OUTPUT);
	digitalWrite(pin, HIGH);
}

void SolenoidValve::start(void)
{
	if (!state)
	{
		digitalWrite(pin, LOW);
		state = true;
	}
}

void SolenoidValve::stop(void)
{
	if (state)
	{
		digitalWrite(pin, HIGH);
		state = false;
	}
}



