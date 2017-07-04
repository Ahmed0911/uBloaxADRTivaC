/*
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "EtherDriver.h"
#include "PWMDrv.h"
#include "IODrv.h"
#include "DBGLEDDrv.h"
#include "CommandStructs.h"
#include "SerialDriver.h"
#include "SpeedParser.h"

uint32_t g_ui32SysClock;
EtherDriver etherDrv;
PWMDrv pwmDrv;
IODrv ioDrv;
DBGLedDrv dbgLedDrv;
SerialDriver serialU0;
SpeedParser parser;

#define SysTickFrequency 100
volatile bool SysTickIntHit = false;
int MainLoopCounter;
void SendPeriodicDataEth(void);

// debug
float DBGDutyCycle = 0;
int DBGEnable = 0;

// Buffers
#define COMMBUFFERSIZE 1024
BYTE CommBuffer[COMMBUFFERSIZE];

//LEDs
#define CLP_D1_PORT         GPIO_PORTN_BASE
#define CLP_D1_PIN          GPIO_PIN_1
#define CLP_D2_PORT         GPIO_PORTN_BASE
#define CLP_D2_PIN          GPIO_PIN_0
#define CLP_D3_PORT         GPIO_PORTF_BASE
#define CLP_D3_PIN          GPIO_PIN_4
#define CLP_D4_PORT         GPIO_PORTF_BASE
#define CLP_D4_PIN          GPIO_PIN_0

void main(void)
{
	// Ensure that ext. osc is used!
	SysCtlMOSCConfigSet(SYSCTL_MOSC_HIGHFREQ);

	// set clock
	g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

	// IO
	pwmDrv.Init();
	ioDrv.Init();
	dbgLedDrv.Init();

	// Ethernet Init
	etherDrv.Init();

	// Serial driver
	serialU0.Init(UART0_BASE, 115200);

	// Systick
	SysTickPeriodSet(g_ui32SysClock/SysTickFrequency);
	SysTickIntEnable();
	SysTickEnable();

	// Master INT Enable
	IntMasterEnable();

	while(1)
	{
		MainLoopCounter++;
		// process ethernet (RX)
		//etherDrv.Process(1000/SysTickFrequency); // 2.5ms tick

		// send periodic data
		//SendPeriodicDataEth();

		// Get data from serial port
        int rd = serialU0.Read(CommBuffer, COMMBUFFERSIZE); // read data from UART0
        if( parser.NewData(CommBuffer, rd) )
        {
            // got new data
            int frequency = parser.Speed;
            if( frequency < 50 ) frequency = 0; // speed too low
            if( frequency > 20000 ) frequency = 0; // speed too high
            pwmDrv.SetPWMFrequency(frequency);
        }

		// LED Output
		dbgLedDrv.Set(DBGEnable);

		// wait next
		while(!SysTickIntHit);
		SysTickIntHit = false;
	}
}

// Process commands received from Ethernet and HopeRF
void ProcessCommand(int cmd, unsigned char* data, int dataSize)
{
	switch( cmd )
	{
		case 0x20:
		{
			SCommSetPWM pwmCommand;
			if( dataSize == sizeof(pwmCommand))
			{
				memcpy(&pwmCommand, data, sizeof(pwmCommand));
				// execute command
				//pwmDrv.SetPWM(pwmCommand.PWMDutyCycle);
				//ioDrv.Set(pwmCommand.Enable);

				// dbg
				DBGDutyCycle = pwmCommand.PWMDutyCycle;
				DBGEnable = pwmCommand.Enable;
			}
			break;
		}

		default:
			break;
	}
}

void SendPeriodicDataEth(void)
{
	unsigned int data = 100;

	// send packet (type 0x20 - data)
	etherDrv.SendPacket(0x20, (char*)&data, sizeof(data));
}

///////////////
// INTERRUPTS
///////////////
extern "C" void UART0IntHandler(void)
{
    serialU0.IntHandler();
}
extern "C" void SysTickIntHandler(void)
{
	SysTickIntHit = true;
}
