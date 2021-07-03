#include "lnArduino.h"
#include "lnSPI.h"
#include "signal.h"
#include "gd32ST7735.h"
#include "assets/fonts/FreeSans7pt7b.h"
#include "assets/fonts/FreeSans9pt7b.h"
#include "assets/fonts/FreeSans12pt7b.h"
#include "assets/fonts/FreeSansBold18pt7b.h"
#include "RotaryEncoder.h"

#include "assets.h"
#include "signalSettings.h"
#include "action.h"


#include "pinout.h"

SignalSettings signalSettings;
lnRotary *re=NULL;
SignalGenerator *signal=NULL;
gd32ST7735 *lcd=NULL;
TopAction   top;
WaveAction waveForm(3,&(signalSettings.waveForm));
DigitAction hiDigit(10);
DigitAction loDigit(10);
DigitAction scaleDigit(4);


const unsigned char *GetIcon(SignalGenerator::WaveForm a);
/**
 * 
 */
void setup()
{
    pinMode(LED,OUTPUT);
    digitalWrite(LED,HIGH);
}
/**
 * 
 */
void updateSignal()
{
    signal->stop();
    signal->start(signalSettings.frequency,signalSettings.waveForm);
}



/**
 * 
 */
static const char *unit[3]={"Hz","kHz","MHz"};
/**
 * 
 */
void redraw()
{
    lcd->drawRLEBitmap(sine_width,sine_height,2,2,0xffff,0,GetIcon(SignalGenerator::SignalSquare));
    lcd->setFontSize(st7735::BigFont);    
    lcd->print(48+4+2,24,"987");
}
/**
 * 
 */
void runLoop()
{
    bool pressed=true;
    int count=0;
    int ev=re->waitForEvent();
    if(ev & lnRotary::SHORT_PRESS)
    {
        pressed=true;
    }
    if(ev & lnRotary::LONG_PRESS)
    {
        pressed=true;
    }
    if(ev & lnRotary::ROTARY_CHANGE)
    {
        count=re->getCount();
    }


    while(count)
    {
        Action *currentWidget=top.getCurrent();
        if(count>0)
        {
            currentWidget->turnLeft();
            count--;
        }else
        {
            currentWidget->turnRight();
            count++;            
        }
    }

    if(pressed)
    {
        Action *currentWidget=top.getCurrent();
        currentWidget->shortPress();            
    }
    redraw();
}
/**
 */
void loop()
{
    Logger("Entering main app...\n");
    
    signalSettings.frequency=10*1000;
    signalSettings.waveForm=SignalGenerator::SignalSine;
    
    // make sure the NSS pin is high before initializing everything
    // else it will trigger CONF ERROR when intializing the screen
    pinMode(PA4,OUTPUT);
    digitalWrite(PA4,false);
    

    re=new lnRotary(ROT_PUSH,ROT_LEFT,ROT_RIGHT);
    re->start();
   

    Logger("Starting LCD...\n");
    hwlnSPIClass *spi=new hwlnSPIClass(0,-1);
    spi->begin();
    lnSPISettings transaction(30*1000*1000, SPI_MSBFIRST, SPI_MODE0,-1);
    spi->beginTransaction(transaction);
    // Reset LCD
    pinMode(PINRST,OUTPUT);
    digitalWrite(PINRST,HIGH);
    xDelay(50);
    digitalWrite(PINRST,LOW);
    xDelay(50);
    digitalWrite(PINRST,HIGH);
    
    lcd=new gd32ST7735(160,80,spi,PINDC,PINCS);
    lcd->init();
    lcd->setRotation(2);
    
    lcd->fillScreen(0xff);
    // init fonts
    lcd->setFontFamily(&FreeSans7pt7b,&FreeSans9pt7b,&FreeSansBold18pt7b);
    lcd->setFontSize(st7735::SmallFont);    
    
    lcd->fillScreen(0);
    lcd->setTextColor(0xFFFF,0);

    Logger("Starting Signal Generator...\n");
    signal=new SignalGenerator(PA4,0);    
    signal->start(signalSettings.frequency,signalSettings.waveForm);
    Logger("waiting...\n");
    int count=0;
    lcd->drawRLEBitmap(sine_width,sine_height,2,2,0xffff,0,GetIcon(SignalGenerator::SignalSquare));
    lcd->setFontSize(st7735::BigFont);    
    lcd->print(48+4+2,24,"987");
    
    
    top.addAction(&waveForm);
    top.addAction(&hiDigit);
    top.addAction(&loDigit);
    top.addAction(&scaleDigit); 
    hiDigit.setValue(1);
    loDigit.setValue(0);
    scaleDigit.setValue(3); // 1khz default value

    while(1)
    {
        runLoop();
    }
    
}

