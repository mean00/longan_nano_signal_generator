#Longan Nano Signal Generator

This is a simple signal generator for the longan nano board.
It is more a demo than anything else :

![screenshot](web/signalGenerator.jpg?raw=true "front")


##Features

* Square, sine and triangle waveform
* From ~ 10hz to 100 khZ

##Shopping list :
Longan nano board with LCD
Rotary encoder
That's it

##Connecting
The signal output it on __PA4__.
Rotary encode must be connected the following way :
* __PB5__: Button 
* __PB6__: Left
* __PB7__: Right

##Building
* Clone the project, including submodules
* Modify  platformConfig to point to your toolchain
* mkdir build && cd build && cmake .. && make
