/*
 * Command.h
 *
 *  Created on: 2015��7��31��
 *      Author: Chris
 */

#ifndef COMMAND_H_
#define COMMAND_H_

//
// ��������
//
typedef enum
{
	NoErr,
	NoMatchingCmd,
}ErrorType;

//
// �������б�
//
class CmdLineTable
{
public:
	const char* cmd;	// ��������
	void (*handlerFunc)(const char* args);	// �����Ӧ��ִ�к���
};

extern void buildCmdLineTable(void);

extern void executeCmdLine(const char* cmdLine);

#endif /* COMMAND_H_ */
