/*
 * SpeedParser.h
 *
 *  Created on: 4. srp 2017.
 *      Author: ivan.murgas
 */

#ifndef SPEEDPARSER_H_
#define SPEEDPARSER_H_

typedef unsigned char BYTE;

class SpeedParser
{
public:
    SpeedParser();
    virtual ~SpeedParser();

private:
    enum ERXPhase { ERX_START, ERX_B0, ERX_B1, ERX_B2, ERX_B3 };
    ERXPhase RXPhase;

    BYTE Bytes[4];

public:
    bool NewData(BYTE* data, int len);

public:
    int ReceivedDataCount;
    int Speed;
};

#endif /* SPEEDPARSER_H_ */
