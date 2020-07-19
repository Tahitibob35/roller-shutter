#ifndef SOMFYRTS_H
#define SOMFYRTS_H
#include <Arduino.h>


#define SYMBOL 640
#define UP   0x2
#define STOP 0x1
#define DOWN 0x4
#define PROG 0x8


class SomfyRTS {
  public:
    SomfyRTS(byte pinTx);
    void moveup( int remote );
    void movedown( int remote );
    void stop( int remote );
    void prog( int remote );

  private:
    void _initRadio();
    void _sendSomfy(unsigned char virtualRemoteNumber, unsigned char actionCommand);
    void _configRTS(unsigned int EEPROM_address, unsigned long RTS_address);
    void _sendCommandSomfy(byte sync);
    void _buildFrameSomfy();
    

  protected:


    byte _pinTx;
    unsigned int _EEPROM_address;
    unsigned long _RTS_address;
    unsigned char _actionCommand;
    unsigned char _virtualRemoteNumber;
    byte frame[7]; // frame for Somfy protocol

};

#endif //SOMFYRTS_H