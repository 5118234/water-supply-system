/*
 * WaterPump.h
 *
 *  Created on: 2015��8��6��
 *      Author: Chris
 */

#ifndef WATERPUMP_WATERPUMP_H_
#define WATERPUMP_WATERPUMP_H_

class WaterPump
{
public:
	void init(const int ctrlPin);
	void start(void);
	void stop(void);
private:
	int pin;
	bool state;
};

#endif /* WATERPUMP_WATERPUMP_H_ */
