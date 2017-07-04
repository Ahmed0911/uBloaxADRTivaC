/*
 * IODrv.cpp
 *
 *  Created on: 25. stu 2016.
 *      Author: ivan.murgas
 */

#include "IODrv.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#define ENABLEPORT 	SYSCTL_PERIPH_GPIOL
#define PORT 		GPIO_PORTL_BASE
#define PIN 		GPIO_PIN_0

void IODrv::Init()
{
	SysCtlPeripheralEnable(ENABLEPORT);
	GPIOPinTypeGPIOOutput(PORT, PIN);
	Set(false);
}

void IODrv::Set(bool set)
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
