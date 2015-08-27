/*
 * CommandHandler.h
 *
 *  Created on: 2015��7��31��
 *      Author: Chris
 */

#ifndef COMMANDHANDLER_H_
#define COMMANDHANDLER_H_

extern void autoModeCommandHandler(const char* args);
extern void manualModeCommandHandler(const char* args);
extern void supplyCommandHandler(const char* args);
extern void stopCommandHandler(const char* args);
extern void caliCommandHandler(const char* args);
extern void clockCommandHandler(const char* args);

#endif /* COMMANDHANDLER_H_ */
