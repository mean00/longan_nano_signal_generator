#pragma once
#include "signal.h"
class SignalSettings
{
public:
    int                        frequency;
    SignalGenerator::WaveForm  waveForm;
};

extern SignalSettings signalSettings;
