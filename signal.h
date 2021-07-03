
#pragma once
#include "lnDAC.h"
/**
 */
class SignalGenerator
{
public:
            enum WaveForm    
            {
                SignalSine=0,
                SignalSquare=1,
                SignalTriangle=2
            };
public:
                    SignalGenerator(lnPin pin,int fac);
            virtual ~SignalGenerator();
            void    stop();
            void    start(int fq, WaveForm form);
            int     getActualFrequency();

protected:
            lnDAC       *_dac;
            uint16_t    _waveForm[256];
            int         _nbPoints;
    
};