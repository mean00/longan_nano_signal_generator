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
DigitAction scaleDigit(6);


const unsigned char *GetIcon(SignalGenerator::WaveForm a);


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

#define DIGITWIDTH 16    
#define DIGITSPACE (DIGITWIDTH+2)
#define DIGITHEIGHT 4
#define DIGITDOT    10

void drawSel(int widget, int color)
{
        
        switch(widget)
        {
            case 0: // Wave
                lcd->square(color,0,0,56,56);
                break;
            case 1: // HI
                lcd->square(color,57,32,DIGITWIDTH,DIGITHEIGHT);
                break;
            case 2: // LOW
               lcd->square(color,57+DIGITDOT+DIGITSPACE,32,DIGITWIDTH,DIGITHEIGHT);
                break;
            case 3: // SCALE
                lcd->square(color,57+DIGITDOT+2*DIGITSPACE,32,DIGITWIDTH,DIGITHEIGHT);
                break;
        }
}

void redraw()
{
    lcd->fillScreen(0);
    
    int sel=top.getSelection();
    if(sel==-1) // top widget
    {
        int widget=top.getValue();
        drawSel(widget,0x1f<<11);
    }else
    {
          drawSel(sel,0x1f<<6);
    }
        
    
    lcd->drawRLEBitmap(sine_width,sine_height,2,2,0xffff,0,GetIcon(signalSettings.waveForm));
    lcd->setFontSize(st7735::BigFont);    
    int fq=hiDigit.getValue()*10+loDigit.getValue();
    int r=scaleDigit.getValue();
    while(r--)
    {
        fq=fq*10;
    }
    fq/=10;
    if(fq<1) fq=1;
    if(fq>100000) fq=100000;
    signalSettings.frequency=fq;
    char str[5]="0000";
    str[0]='0'+hiDigit.getValue();
    str[1]='.';
    str[2]='0'+loDigit.getValue();
    str[3]='0'+scaleDigit.getValue();
    lcd->print(48+4+2,28,str);
    updateSignal();
    
    int actualFq=signal->getActualFrequency();
    if(actualFq)
    {
        const char *ext="";
        if(actualFq>10*000)
        {
            actualFq/=1000;
            ext="k";
        }else
        {
            
        }
        char str[20];
        sprintf(str,"%d%s",actualFq,ext);
        lcd->setFontSize(st7735::SmallFont); 
        lcd->print(48+4+2,56,str);
    }
}
/**
 * 
 */
void loop()
{
    bool pressed=false;
    int count=0;
    int ev=re->waitForEvent(5000);
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
    if(!ev)
    {
        redraw();
        return;
    }

    while(count)
    {
        Action *currentWidget=top.getCurrent();
        if(count>0)
        {
            currentWidget->turnRight();
            count--;
        }else
        {
            currentWidget->turnLeft();
            count++;            
        }
        redraw();
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
/**
 * 
 */
void setup()
{
    Logger("Entering main app...\n");
    
    pinMode(LED,OUTPUT);
    digitalWrite(LED,HIGH);
    
    
    
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
    redraw();
}
// EOF
