#include "lnArduino.h"
#include "lnSPI.h"
#include "signal.h"
#include "gd32ST7735.h"
#include "assets/fonts/FreeSans7pt7b.h"
#include "assets/fonts/FreeSans9pt7b.h"
#include "RotaryEncoder.h"

#include "assets.h"

#define splash_width 128
#define splash_height 96


// green = PA1, blue = PA2, RED PC13
#define LED     PA2
#define LEDRED  PC13

// Rotary Encoder
#define ROT_LEFT    PB6
#define ROT_RIGHT   PB7
#define ROT_PUSH    PB5


// MOSI is PA7
// CLK  is PA5
// MISO is PA6
// NSS  is PA4

#define PINDC PB0
#define PINCS PB2
#define PINRST PB1
lnRotary *re=NULL;

const unsigned char *GetIcon(SignalGenerator::SignalForm a);

/**
 * 
 */
void xstat(int v)
{
    uint32_t *s=(uint32_t *)0x40013008;
    Logger("STAT=%d:%x  \n",v,*s);
}
void setup()
{
    pinMode(LED,OUTPUT);
    digitalWrite(LED,HIGH);
}
/**
 */
void loop()
{
    Logger("Entering main app...\n");
    
    // make sure the NSS pin is high before initializing everything
    // else it will trigger CONF ERROR when intializing the screen
    pinMode(PA4,OUTPUT);
    digitalWrite(PA4,false);
    
    xstat(0);

    re=new lnRotary(ROT_PUSH,ROT_LEFT,ROT_RIGHT);
    re->start();
   
#if 1   
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
    
    xstat(10);
    gd32ST7735 *lcd=new gd32ST7735(160,80,spi,PINDC,PINCS);
    xstat(11);
    lcd->init();
    lcd->setRotation(2);
    
    lcd->fillScreen(0xff);
    xDelay(100);
    lcd->fillScreen(0);
        xstat(1);
    
    // init fonts
    lcd->setFontFamily(&FreeSans7pt7b,&FreeSans9pt7b,&FreeSans7pt7b);
    lcd->setFontSize(st7735::SmallFont);    
    
    lcd->fillScreen(0);
    lcd->setTextColor(0xFFFF,0);
    lcd->print(2,20,"Signal Generator");
    lcd->setFontSize(st7735::MediumFont);    
    xstat(2);
#endif    
#if 0
    while(1)
    {
        xDelay(10);
    }
#else
    Logger("Starting Signal Generator...\n");
    SignalGenerator *signal=new SignalGenerator(PA4,0);    
    signal->start(25000,SignalGenerator::SignalSine);
  
    Logger("waiting...\n");
    xstat(3);
    int count=0;
    lcd->drawRLEBitmap(sine_width,sine_height,2,2,0xffff,0,GetIcon(SignalGenerator::SignalSquare));
    lcd->print(64+4+2,20,"123.4kHz");
    while(1)
    {
       // lcd->print(2,20,"Signal Generator");
        int ev=re->waitForEvent();
        if(ev & lnRotary::SHORT_PRESS)
        {
            Logger("Short press\n");
        }
        if(ev & lnRotary::LONG_PRESS)
        {
            Logger("LONG press\n");
        }
        if(ev & lnRotary::ROTARY_CHANGE)
        {
            Logger("change press\n");
            count=count+re->getCount();
            Logger("New value=%d\n",count);
        }
        
    }
#endif    
}

