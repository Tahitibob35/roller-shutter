#include <Preferences.h>
#include "SomfyRTS.h"

Preferences prefs;

SomfyRTS::SomfyRTS(byte pinTx) {

  _pinTx = pinTx;
  _EEPROM_address = 0;
  _RTS_address = 0x121300;
  _actionCommand = STOP;
  _virtualRemoteNumber = 0;

  this->_initRadio();
}
    

void SomfyRTS::_initRadio() {
  
  pinMode(_pinTx, OUTPUT);
  
}


void SomfyRTS::_configRTS(unsigned int EEPROM_address, unsigned long RTS_address) {
  
  _EEPROM_address = EEPROM_address;
  //_RTS_address = RTS_address;
  
}


void SomfyRTS::_buildFrameSomfy() {
  
  unsigned int  Code;

  prefs.begin("SomfyRTS", false);

  char key[10];
  snprintf(key, 10, "%lu", _RTS_address);
  Code = prefs.getUInt(key);
  Serial.print("SomfyRTS::_buildFrameSomfy - Code read : ");
  Serial.println(Code);

  frame[0] = 0xA7; // Encryption key. Doesn't matter much
  frame[1] = _actionCommand << 4;  // Which button did  you press? The 4 LSB will be the checksum
  frame[2] = Code >> 8;    // Rolling code (big endian)
  frame[3] = Code;         // Rolling code
  frame[4] = _RTS_address + _virtualRemoteNumber >> 16; // Remote address
  frame[5] = _RTS_address + _virtualRemoteNumber >>  8; // Remote address
  frame[6] = _RTS_address + _virtualRemoteNumber;     // Remote address

  // Checksum calculation: a XOR of all the nibbles
  byte checksum = 0;
  for (byte i = 0; i < 7; i++) {
    checksum = checksum ^ frame[i] ^ (frame[i] >> 4);
  }
  checksum &= 0b1111; // We keep the last 4 bits only


  //Checksum integration
  frame[1] |= checksum; //  If a XOR of all the nibbles is equal to 0, the blinds will
  // consider the checksum ok.

  // Obfuscation: a XOR of all the bytes
  for (byte i = 1; i < 7; i++) {
    frame[i] ^= frame[i - 1];
  }

  Serial.print("SomfyRTS::_buildFrameSomfy - Code write : ");
  Serial.println(Code);
  prefs.putUInt(key, Code+1);
  
}


void SomfyRTS::_sendCommandSomfy(byte sync) {
  
  if (sync == 2) { // Only with the first frame.
    //Wake-up pulse & Silence
    digitalWrite(_pinTx, HIGH);
    delayMicroseconds(9415);
    digitalWrite(_pinTx, LOW);
    //delayMicroseconds(89565U);
    delay(89);
  }

  // Hardware sync: two sync for the first frame, seven for the following ones.
  for (int i = 0; i < sync; i++) {
    digitalWrite(_pinTx, HIGH);
    delayMicroseconds(4 * SYMBOL);
    digitalWrite(_pinTx, LOW);
    delayMicroseconds(4 * SYMBOL);
  }

  // Software sync
  digitalWrite(_pinTx, HIGH);
  delayMicroseconds(4550);
  digitalWrite(_pinTx, LOW);
  delayMicroseconds(SYMBOL);


  //Data: bits are sent one by one, starting with the MSB.
  for (byte i = 0; i < 56; i++) {
    if (((frame[i / 8] >> (7 - (i % 8))) & 1) == 1) {
      digitalWrite(_pinTx, LOW);
      delayMicroseconds(SYMBOL);
      digitalWrite(_pinTx, HIGH);
      delayMicroseconds(SYMBOL);
    }
    else {
      digitalWrite(_pinTx, HIGH);
      delayMicroseconds(SYMBOL);
      digitalWrite(_pinTx, LOW);
      delayMicroseconds(SYMBOL);
    }
  }

  digitalWrite(_pinTx, LOW);
  delayMicroseconds(30415); // Inter-frame silence
  
}


void SomfyRTS::_sendSomfy(unsigned char virtualRemoteNumber, unsigned char actionCommand) {
  
  _virtualRemoteNumber = virtualRemoteNumber;
  _actionCommand = actionCommand;

  this->_buildFrameSomfy();
  this->_sendCommandSomfy(2);
  for (int i = 0; i < 2; i++) {
    this->_sendCommandSomfy(7);
    
  }

}


void SomfyRTS::moveup( int remote ) {
  
  this->_sendSomfy(remote, UP);
  
}


void SomfyRTS::movedown( int remote ) {
  
  this->_sendSomfy(remote, DOWN);
  
}


void SomfyRTS::stop( int remote ) {
  
  this->_sendSomfy(remote, STOP);
  
}


void SomfyRTS::prog( int remote ) {
  
  this->_sendSomfy(remote, PROG);
  
}