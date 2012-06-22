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
#include "Adafruit_MCP23017.h"
    /*********************************************************************************
    ** Start - Freestyle BON stuff
    *********************************************************************************/

// Mux card GPIO Allocation
#define K1_ISENSE   4
#define K2_LOAD     15
#define K3_HI_REF   14
#define ISENSE_PORT 3

#define picKit 1
#define emberISA 2
// Relay state for power source
#define SOURCE_RSENSE  0
#define SOURCE_3V6REG  1
// Current Sink Device
#define SINK_IS_DUT  1
#define SINK_IS_REF  0
// Current Sink Reference select
#define SINK_HIGH 0
#define SINK_LOW  1

#define EN_REG 15
#define VREG3V6_OFF 0
#define VREG3V6_ON 1

Adafruit_MCP23017 MCP23S17_U2;
Adafruit_MCP23017 MCP23S17_U5;


void initTestBed(){
//delay(1000);
 // disPrg();
  pwrOff();
//  disableRegister();
  MCP23S17_U2.begin(2);
  MCP23S17_U5.begin(3);

  for(int i=0;i<16;i++){
    MCP23S17_U2.pinMode(i, OUTPUT);
    MCP23S17_U5.pinMode(i, OUTPUT);
  }
}

int readAN3(){
  MCP23S17_U2.digitalWrite(3, 0); // DisconnectConnect ISENSE to AN3
  return analogRead(3);
}

void refSenseOn(){
  MCP23S17_U5.digitalWrite(EN_REG, VREG3V6_ON); //
  MCP23S17_U2.digitalWrite(K1_ISENSE, SOURCE_RSENSE);
  MCP23S17_U2.digitalWrite(K2_LOAD, SINK_IS_REF); // iRefSink ON
  MCP23S17_U2.digitalWrite(K3_HI_REF, 1);
}
void dutSenseOn(){
  MCP23S17_U5.digitalWrite(EN_REG, VREG3V6_ON); //
  MCP23S17_U2.digitalWrite(K1_ISENSE, SOURCE_RSENSE);
  MCP23S17_U2.digitalWrite(K2_LOAD, SINK_IS_DUT); // DUT ON
}
void dutOn(){
  MCP23S17_U5.digitalWrite(EN_REG, VREG3V6_ON); //
  MCP23S17_U2.digitalWrite(K1_ISENSE, SOURCE_3V6REG);
  MCP23S17_U2.digitalWrite(K2_LOAD, SINK_IS_DUT); // DUT ON
}
void pwrOff(){
  MCP23S17_U5.digitalWrite(EN_REG, VREG3V6_OFF); //
  MCP23S17_U2.digitalWrite(K1_ISENSE, 0);
  MCP23S17_U2.digitalWrite(K2_LOAD, SINK_IS_REF);
}

void enPrg(){
  // Use direct power not through Isense Resistor
  MCP23S17_U5.digitalWrite(EN_REG, VREG3V6_ON); //
  MCP23S17_U2.digitalWrite(K1_ISENSE, SOURCE_3V6REG);//
  MCP23S17_U2.digitalWrite(K2_LOAD, SINK_IS_DUT); // DUT ON
  // Now connect PIC programming lines
  MCP23S17_U2.digitalWrite(8,HIGH); // ICD connect
  MCP23S17_U2.digitalWrite(9,HIGH); // ICD connect
  MCP23S17_U2.digitalWrite(10,HIGH); // ICD connect
  // And Ember programming lines
  MCP23S17_U5.digitalWrite(8,HIGH); // ICD connect
  MCP23S17_U5.digitalWrite(9,HIGH); // ICD connect
  MCP23S17_U5.digitalWrite(10,HIGH); // ICD connect
  MCP23S17_U5.digitalWrite(11,HIGH); // ICD connect
  MCP23S17_U5.digitalWrite(12,HIGH); // ICD connect
  MCP23S17_U5.digitalWrite(13,HIGH); // ICD connect
  MCP23S17_U5.digitalWrite(14,HIGH); // ICD connect
  setIsmRly(7, 0x0c, 0); //Enable power sense for PIC and EMBER
}

void disPrg(){
  // Use direct power not through Isense Resistor
  MCP23S17_U5.digitalWrite(EN_REG, VREG3V6_OFF); //
  MCP23S17_U2.digitalWrite(K1_ISENSE, 0);//
  // Now dconnect PIC programming lines
  MCP23S17_U2.digitalWrite(8,LOW); // ICD connect
  MCP23S17_U2.digitalWrite(9,LOW); // ICD connect
  MCP23S17_U2.digitalWrite(10,LOW); // ICD connect
  // And Ember programming lines
  MCP23S17_U5.digitalWrite(8,LOW); // ICD connect
  MCP23S17_U5.digitalWrite(9,LOW); // ICD connect
  MCP23S17_U5.digitalWrite(10,LOW); // ICD connect
  MCP23S17_U5.digitalWrite(11,LOW); // ICD connect
  MCP23S17_U5.digitalWrite(12,LOW); // ICD connect
  MCP23S17_U5.digitalWrite(13,LOW); // ICD connect
  MCP23S17_U5.digitalWrite(14,LOW); // ICD connect
  setIsmRly(7, 0, 0); //Enable power sense for PIC and EMBER
}

void enableRegister(){
  // Use direct power not through Isense Resistor
  MCP23S17_U5.digitalWrite(EN_REG, VREG3V6_ON); //
  MCP23S17_U2.digitalWrite(K1_ISENSE, SOURCE_3V6REG);//
  MCP23S17_U2.digitalWrite(K2_LOAD, SINK_IS_DUT); // DUT ON
  MCP23S17_U5.digitalWrite(0,HIGH); // Register connect
  MCP23S17_U5.digitalWrite(1,HIGH); // Register connect
  MCP23S17_U5.digitalWrite(2,HIGH); // Register connect
  MCP23S17_U5.digitalWrite(4,HIGH); // Register connect
  MCP23S17_U5.digitalWrite(5,HIGH); // Register connect
  MCP23S17_U5.digitalWrite(6,HIGH); // Register connect
  setIsmRly(7, 0x20, 0); //Register 3V6 Connect
}

void disableRegister(){
  MCP23S17_U5.digitalWrite(0,LOW); // Register connect
  MCP23S17_U5.digitalWrite(1,LOW); // Register connect
  MCP23S17_U5.digitalWrite(2,LOW); // Register connect
  MCP23S17_U5.digitalWrite(4,LOW); // Register connect
  MCP23S17_U5.digitalWrite(5,LOW); // Register connect
  MCP23S17_U5.digitalWrite(6,LOW); // Register connect
  setIsmRly(7, 0, 0); //Register 3V6 Connect
}
/*
int setIsmRly(int rlyCard, int rlyPosition, int readWrite)
*/
int setIsmRly(int rlyCard, int rlyPosition, int readWrite)
{
const int slaveSelectPin = rlyCard;
int rlyRetVal=0;
//if(rlyPosition > 0){
  pinMode(slaveSelectPin,OUTPUT);
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

