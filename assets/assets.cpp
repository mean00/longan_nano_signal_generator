#include "lnArduino.h"
#include "pics/generated/sine_compressed.h"
#include "pics/generated/square_compressed.h"
#include "pics/generated/triangle_compressed.h"
#include "signal.h"
/**
 * 
 * @param a
 * @return 
 */
const unsigned char *GetIcon(SignalGenerator::WaveForm a)
{
    switch(a)
    {
        case SignalGenerator::SignalSine : return sine;break;
        case SignalGenerator::SignalTriangle: return triangle;break;
        case SignalGenerator::SignalSquare: return square;break;
        default:
            break;
    }
    xAssert(0);
    return NULL;
}
// EOF
