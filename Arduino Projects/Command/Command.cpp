/*
 * Command.cpp
 *
 *  Created on: 2015��7��31��
 *      Author: Chris
 */
#include "Command.h"
#include "CommandHandler.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"

//
// ������������
//
static const int MAX_CMDLINE_COUNT = 6;

//
// ����������󳤶�
//
static const int MAX_CMD_LEN = 12;

//
// ������󳤶�
//
static const int MAX_ARG_LEN = 24;

//
// �����б�ʵ��
//
static CmdLineTable cmdLineTable[MAX_CMDLINE_COUNT];

static ErrorType queryCmd(const char* cmd, int* matchingIndex);
static void parseCmdLine(const char* cmdLine, char* cmd, char* args);

//
// ���������б�
//
void buildCmdLineTable(void)
{
	// �Զ�ģʽ
	cmdLineTable[0].cmd = "auto";
	cmdLineTable[0].handlerFunc = autoModeCommandHandler;

	// �ֶ�ģʽ
	cmdLineTable[1].cmd = "manual";
	cmdLineTable[1].handlerFunc = manualModeCommandHandler;

	// ��ˮ
	cmdLineTable[2].cmd = "supply";
	cmdLineTable[2].handlerFunc = supplyCommandHandler;

	// ֹͣ��ˮ
	cmdLineTable[3].cmd = "stop";
	cmdLineTable[3].handlerFunc = stopCommandHandler;

	// У׼ʱ��
	cmdLineTable[4].cmd = "cali";
	cmdLineTable[4].handlerFunc = caliCommandHandler;

	// ���ö�ʱ
	cmdLineTable[5].cmd = "clock";
	cmdLineTable[5].handlerFunc = clockCommandHandler;
}

//
// ִ��������
//
void executeCmdLine(const char* cmdLine)
{
	assert(cmdLine != NULL);

	char cmd[MAX_CMD_LEN];
	char args[MAX_ARG_LEN];

	parseCmdLine(cmdLine, cmd, args);

	int index = -1;
	ErrorType err = queryCmd(cmd, &index);
	switch(err)
	{
	case NoErr:
		cmdLineTable[index].handlerFunc(args);
		break;
	case NoMatchingCmd:
		break;
	}
	return;
}

//
// ��ѯ���������Ƿ��������б��С�������ڣ�����ƥ�����������ֵ
//
static ErrorType queryCmd(const char* cmd, int* matchingIndex)
{
	assert(cmd != NULL && matchingIndex != NULL);

	for (int i = 0; i < MAX_CMDLINE_COUNT; i++)
	{
		if (strcmp(cmdLineTable[i].cmd, cmd) == 0)
		{
			*matchingIndex = i;
			return NoErr;
		}
	}
	*matchingIndex = -1;
	return NoMatchingCmd;
}

//
// ���������У����������ƺͲ����ַ����ָ��
//
static void parseCmdLine(const char* cmdLine, char* cmd, char* args)
{
	assert(cmdLine != NULL && cmd != NULL && args != NULL);

	//
	// ��ȡ�İ취�����Ȳ�����û�пո���ڣ�������ڿո����ʾ����в���������û�в�����
	// û�пո�ʱ��������ֱ�Ӹ�cmd���ɡ��пո�ʱ����ȡ��һ���ո��λ�ã��ѵ�һ���ո�֮ǰ��
	// �ַ�������cmd��������args����ʵ�ַ��롣
	//
	char* firstSpacePos = strchr(cmdLine, ' ');
	const char* cursor = cmdLine;

	if (firstSpacePos != NULL)
	{
		while (*cursor != '\0')
		{
			if (cursor < firstSpacePos)
			{
				*cmd++ = *cursor++;
			}
			else if (cursor == firstSpacePos)
			{
				*cmd = '\0';
				cursor++;
			}
			else
			{
				*args++ = *cursor++;
			}
		}
		*args = '\0';
	}
	else
	{
		strcpy(cmd, cmdLine);
	}
}

