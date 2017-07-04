/*
 * PWMDrv.h
 *
 *  Created on: 25. stu 2016.
 *      Author: ivan.murgas
 */

#ifndef PWMDRV_H_
#define PWMDRV_H_

class PWMDrv
{
public:
	void Init();
	void SetPWMFrequency(float frequency);

private:
	int m_Frequency;
};

#endif /* PWMDRV_H_ */
