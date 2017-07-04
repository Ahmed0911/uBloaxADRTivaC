/*
 * PWMDrv.cpp
 *
 *  Created on: 25. stu 2016.
 *      Author: ivan.murgas
 */

#include "PWMDrv.h"
#include "PWMDrv.h"
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"

extern uint32_t g_ui32SysClock;

void PWMDrv::Init()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0); // PWM Module 0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	PWMClockSet(PWM0_BASE, PWM_SYSCLK_DIV_64); // TM4C129

	// configure PWM pins
	GPIOPinConfigure(GPIO_PF1_M0PWM1);
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);

	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DBG_RUN); // M0PWM0, M0PWM1

	// set default frequency to 100Hz
	m_Frequency = 100;
	unsigned long ulPeriod = (g_ui32SysClock / 64) /  m_Frequency;
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ulPeriod);

	// set output to zero
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, ulPeriod/2);

	// kill PWM output
	PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, false);

	// enable PWM module
	PWMGenEnable(PWM0_BASE, PWM_GEN_0);

}

void PWMDrv::SetPWMFrequency(float frequency)
{
    if( frequency > 0 )
    {
        // set PWM timer period
        unsigned long ulPeriod = (g_ui32SysClock / 64) / frequency;
        PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ulPeriod);
        m_Frequency = frequency;

        // set period to 50%
        PWMPulseWidthSet( PWM0_BASE, PWM_OUT_1, ulPeriod/2);

        // enable PWM output to PINs
        PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);

    }
    else
    {
        // kill PWM
        PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, false);
    }
}
