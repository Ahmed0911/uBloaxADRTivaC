/*
 * SpeedParser.cpp
 *
 *  Created on: 4. srp 2017.
 *      Author: ivan.murgas
 */

#include "SpeedParser.h"
#include <string.h>

SpeedParser::SpeedParser()
{
    ReceivedDataCount = 0;
    Speed = 0;
}

SpeedParser::~SpeedParser()
{

}

bool SpeedParser::NewData(BYTE* data, int len)
{
    bool hasNewData = false;

    // RX Parser
    for (int i = 0; i != len; i++)
    {
        BYTE b = data[i];
        switch (RXPhase)
        {
            // HEADER
            case SpeedParser::ERX_START:
                if (b == '$')
                {
                    RXPhase = SpeedParser::ERX_B0; // wait for start
                }
                break;

            case SpeedParser::ERX_B0:
                Bytes[0] = b;
                RXPhase = SpeedParser::ERX_B1; // next
                break;

            case SpeedParser::ERX_B1:
                Bytes[1] = b;
                RXPhase = SpeedParser::ERX_B2; // next
                break;

            case SpeedParser::ERX_B2:
                Bytes[2] = b;
                RXPhase = SpeedParser::ERX_B3; // next
                break;

            case SpeedParser::ERX_B3:
                Bytes[3] = b;
                RXPhase = SpeedParser::ERX_START; // restart

                // Compute Data
                int speed;
                memcpy(&speed, Bytes, 4);
                Speed = speed;
                hasNewData = true;
                break;
        }
    }

    return hasNewData;
}
