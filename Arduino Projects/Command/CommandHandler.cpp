/*
 * CommandHandler.cpp
 *
 *  Created on: 2015��7��31��
 *      Author: Chris
 */
#include "CommandHandler.h"
#include "../Core/Core.h"
#include "assert.h"
#include "string.h"

void autoModeCommandHandler(const char* args)
{
	assert(args != NULL);
	// DBGLN("execute auto command.");
	switchToAutoMode();
}

void manualModeCommandHandler(const char* args)
{
	assert(args != NULL);
	// DBGLN("execute manual command.");
	switchToManualMode();
}

void supplyCommandHandler(const char* args)
{
	assert(args != NULL);
	// DBGLN("execute supply command.");
	supplyWater();
}

void stopCommandHandler(const char* args)
{
	assert(args != NULL);
	// DBGLN("execute stop command.");
	stopSupplyingWater();
}

void caliCommandHandler(const char* args)
{
	assert(args != NULL);
	// DBGLN("execute cali command.");
	// DBGLN(args);
	// �����õ���-��-��-ʱ-��-��
	calibrateTime(args);
}

void clockCommandHandler(const char* args)
{
	assert(args != NULL);
	// DBGLN("execute clock command.");
	// DBGLN(args);
	setClock(args);
}

