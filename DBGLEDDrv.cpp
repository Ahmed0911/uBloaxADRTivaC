/*
 * IODrv.cpp
 *
 *  Created on: 25. stu 2016.
 *      Author: ivan.murgas
 */

#include "DBGLedDrv.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#define ENABLEPORT 	SYSCTL_PERIPH_GPION
#define PORT 		GPIO_PORTN_BASE
#define PIN 		GPIO_PIN_1

void DBGLedDrv::Init()
{
	SysCtlPeripheralEnable(ENABLEPORT);
	GPIOPinTypeGPIOOutput(PORT, PIN);
	Set(false);
}

void DBGLedDrv::Set(bool set)
{
	if( set)
	{
		// set
		GPIOPinWrite(PORT, PIN, PIN );
	}
	else
	{
		// reset
		GPIOPinWrite(PORT, PIN, 0 );
	}
}
