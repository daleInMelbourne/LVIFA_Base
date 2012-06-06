/*********************************************************************************
 **
 **  LVIFA_Firmware - Provides Functions For Interfacing With The Arduino Uno
 **
 **  Written By:    Sam Kristoff - National Instruments
 **  Written On:    November 2010
 **  Last Updated:  Dec 2011 - Kevin Fort - National Instruments
 **
 **  This File May Be Modified And Re-Distributed Freely. Original File Content
 **  Written By Sam Kristoff And Available At www.ni.com/arduino.
 **
 *********************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <Mcp23s17.h>

    /*********************************************************************************
    ** Start - Freestyle BON stuff
    *********************************************************************************/

#define MCP23S17_SLAVE_SELECT_PIN  10 //arduino   <->   SPI Slave Select
#define K1_ISENSE 4
#define K2_IREF 15
#define K3_HI_REF 14
#define ISENSE_PORT 3
#define picKit 1
#define emberISA 2


MCP23S17 MCP23S17_U2 = MCP23S17( MCP23S17_SLAVE_SELECT_PIN,0x7 );
MCP23S17 MCP23S17_U5 = MCP23S17( MCP23S17_SLAVE_SELECT_PIN,0x5 );

int getIsense(int ISink)
{

MCP23S17_U2.pinMode(OUTPUT);
MCP23S17_U5.pinMode(OUTPUT);

MCP23S17_U2.digitalWrite(K1_ISENSE, 0); // 0 = DUT direct to 3V6, 1 = DUT via Rsense to 3V6
MCP23S17_U2.digitalWrite(K2_IREF, 0); // 1 = Current sink, 0 = DUT
MCP23S17_U2.digitalWrite(K3_HI_REF, 0);

selectProgrammer(picKit,1);
//delay(1000);
//selectProgrammer(emberISA,1);

}      

void selectProgrammer(int selProgrammer, char onOff){
MCP23S17 MCP23S17_U2 = MCP23S17( MCP23S17_SLAVE_SELECT_PIN,0x7 );
MCP23S17 MCP23S17_U5 = MCP23S17( MCP23S17_SLAVE_SELECT_PIN,0x5 );

MCP23S17_U2.pinMode(OUTPUT);
MCP23S17_U5.pinMode(OUTPUT);  
MCP23S17_U5.port(0x0000);
MCP23S17_U2.port(0x0000);

if(selProgrammer == picKit){
  // picKit programmer connect
  if(onOff){
    MCP23S17_U2.port(0x0700); // picKit programmer connect
    setIsmRly(7, 8, 0); //picKit Vcc Connect
  }
  else{
    // ISA programmer connect
    MCP23S17_U2.port(0x0000); // ISA programmer connect
    setIsmRly(7, 0, 0); //ISA Vcc Connect
  }
}
else if(selProgrammer == emberISA){
  if(onOff){
    // ISA programmer connect
    MCP23S17_U5.port(0x7F00); // ISA programmer connect
    setIsmRly(7, 1, 0); //ISA Vcc Connect
  }
  else{
    // ISA programmer connect
    MCP23S17_U5.port(0x0000); // ISA programmer connect
    setIsmRly(7, 0, 0); //ISA Vcc Connect
  }
}


}

int setIsmRly(int rlyCard, int rlyPosition, int readWrite)
{
const int slaveSelectPin = rlyCard;
int rlyRetVal=0;
//if(rlyPosition > 0){
      digitalWrite(slaveSelectPin,LOW);
      SPI.transfer(0x46);//Send opcode 
      delay(50);
      SPI.transfer(rlyPosition);
      delay(50);
      SPI.transfer(readWrite);
//      rlyRetVal = SPI.transfer(0xFF);
      digitalWrite(slaveSelectPin,HIGH);
      return rlyRetVal;
//}
}

    /*********************************************************************************
    ** End - Freestyle BON stuff
    *********************************************************************************/

