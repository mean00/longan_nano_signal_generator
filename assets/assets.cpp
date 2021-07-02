#include "lnArduino.h"
#include "sine_compressed.h"
#include "square_compressed.h"
#include "triangle_compressed.h"
#include "signal.h"
/**
 * 
 * @param a
 * @return 
 */
const unsigned char *GetIcon(SignalGenerator::SignalForm a)
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
}
// EOF
