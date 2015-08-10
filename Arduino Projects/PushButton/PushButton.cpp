/*
 * PushButton.cpp
 *
 *  Created on: 2015��8��9��
 *      Author: Chris
 */
#include "PushButton.h"
#include <Arduino.h>
#include "../Board/Board.h"

#define PUSH_BUTTON_MAX_COUNT	3

//
// ��������ʱ������ָ��
//
static handlerFunc pushButtonPressedEventHandler = NULL;
static handlerFunc pushButtonReleasedEventHandler = NULL;
static KeyCodeTable keyCodeTable[PUSH_BUTTON_MAX_COUNT + 1] =
{
		{ SWITCH_MODE_PUSH_BUTTON_PIN, 0 },
		{ SUPPLY_WATER_PUSH_BUTTON_PIN, 1 },
		{ STOP_SUPPLYING_PUSH_BUTTON_PIN, 2 },
		{ -1, -1 }	// End of the keyCodeTable
};

//
// �����¼�
//
static void raisePushButtonPressedEvent(int keyCode);
static void raisePushButtonReleasedEvent(int keyCode);

//
// �ܽų�ʼ��
// ʹ���ڲ���������
//
void pushButtonInitAll(void)
{
	for (int i = 0; i < PUSH_BUTTON_MAX_COUNT + 1; i++)
	{
		if (keyCodeTable[i].pushButtonPin != -1)
		{
			pinMode(keyCodeTable[i].pushButtonPin, INPUT_PULLUP);
		}
	}
}

//
// �������
// ���ڶ�ʱѭ���У����鶨ʱ����Ϊ10ms����
//
int pushButtonDetect(void)
{
	// ����ÿ�����ֻ����Ӧһ���������µ��¼�
	// ��ĳ���������º���Ҫ�ȴ�10ms��������
	// ���ȷ�������Ƿ�����ǰ�����
	// һ�������İ��������ǣ� ����--����--̧��
	static bool isPressed = false;
	static int pushButtonPin = -1;
	static int keyCode = -1;

	if (pushButtonPin == -1)
	{
		for (int i = 0; i < PUSH_BUTTON_MAX_COUNT; i++)
		{
			if (keyCodeTable[i].keyCode != -1)
			{
				if (digitalRead(keyCodeTable[i].pushButtonPin) == 0)
				{
					pushButtonPin = keyCodeTable[i].pushButtonPin;
					keyCode = keyCodeTable[i].keyCode;
					return -1;
				}
			}
		}
		keyCode = -1;
		pushButtonPin = -1;
	}
	else
	{
		// 15ms��ʱ�����ʱ�����������
		// �ٴμ�ⱻ��¼�ļ��Ƿ���Ȼ���ڵ͵�ƽ״̬������ǣ���ȷ���ǰ�����
		// ��ʱ����Ӧ����
		if (!isPressed)
		{
			if (digitalRead(pushButtonPin) == 0)
			{
				isPressed = true;
				raisePushButtonPressedEvent(keyCode);
				return keyCode;
			}
		}
		else
		{
			if (digitalRead(pushButtonPin) == 1)
			{
				isPressed = false;
				raisePushButtonReleasedEvent(keyCode);
				pushButtonPin = -1;
				keyCode = -1;
			}
		}
	}

	return -1;
}

void registerPushButtonPressedEventHandler(handlerFunc handler)
{
	if (handler != NULL)
	{
		pushButtonPressedEventHandler = handler;
	}
}

void raisePushButtonPressedEvent(int keyCode)
{
	if (pushButtonPressedEventHandler != NULL)
	{
		pushButtonPressedEventHandler(keyCode);
	}
}

void registerPushButtonReleasedEventHandler(handlerFunc handler)
{
	if (handler != NULL)
	{
		pushButtonReleasedEventHandler = handler;
	}
}

void raisePushButtonReleasedEvent(int keyCode)
{
	if (pushButtonReleasedEventHandler != NULL)
	{
		pushButtonReleasedEventHandler(keyCode);
	}
}
