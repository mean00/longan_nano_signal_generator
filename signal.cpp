#include "lnArduino.h"
#include "signal.h"
#include "math.h"


#if 0
            enum SignalForm    
            {
                SignalSine=0,
                SignalSquare=1,
                SignalTriangle=2
            };
#endif
/**
 * 
 * @param pin
 */
SignalGenerator::SignalGenerator(lnPin pin,int dac)
{
    pinMode(pin,lnDAC_MODE);
    _dac=new lnDAC(dac);
}
/**
 * 
 */
SignalGenerator:: ~SignalGenerator()
{
    delete _dac;
    _dac=NULL;
}
/**
 * 
 */
void    SignalGenerator::stop()
{
    _dac->stopDmaMode();
}
/**
 * 
 * @param fq
 * @param form
 */
void    SignalGenerator::start(int fq, WaveForm form)
{
    _dac->startDmaMode(fq*100);
    int actualFq=_dac->getDmaFrequency();
    float pointFloat=(float)(actualFq+fq/2)/(float)fq;
    _nbPoints=floor(pointFloat+0.49);
    Logger("In Fq=%d outFq=%d, # points=%d pointsF=%f\n",fq,actualFq,_nbPoints,pointFloat);
    switch(form)
    {
        case SignalSine:    
        {            
            for(int i=0;i<_nbPoints;i++)
            {
                  float angle=2.*M_PI;
                  angle/=(float)_nbPoints;
                  angle*=(float)i;        
                  _waveForm[i]=2048.+2047.*sin(angle);
            }
        }
            break;
        case SignalSquare:
        {
            int left=_nbPoints/2;
            int right=_nbPoints-left;
            memset(_waveForm,0,left*2);
            memset(&_waveForm[left],0xff,right*2);
        }
        break;
          case SignalTriangle:
        {
            for(int i=0;i<_nbPoints;i++)
            {
                float f=(float)i/(float)_nbPoints;
                f*=4095;
                _waveForm[i]=(int)f;
            }
        }
        break;
        default:
            xAssert(0);
            break;
    }
    _dac->dmaWrite(_nbPoints,_waveForm,true);

}
/**
 * 
 * @return 
 */
int     SignalGenerator::getActualFrequency()
{
    return _dac->getDmaFrequency();
}
// EOF
