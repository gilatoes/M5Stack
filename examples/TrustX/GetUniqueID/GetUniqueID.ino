/**
 * MIT License
 *
 * Copyright (c) 2019 Infineon Technologies AG
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE
 *
 * Demonstrates use of the
 * Infineon Technologies AG OPTIGA™ Trust X Arduino library
 */

#include "OPTIGATrustX.h"
#include <Arduino.h>
#include "debug.h"
#include <M5Stack.h>

uint16_t UID_LENGTH=27;

uint8_t system_start =0;
void setup()
{
  /*
   * Initialise a serial port for debug output
   */
  Serial.begin(115200, SERIAL_8N1);
  delay(100);

 /*
   * Initialise an OPTIGA™ Trust X Board
   */
  if(reset()==0){
    system_start=1;
  }else{
    system_start=0;
  }
}


uint8_t reset()
{
  uint32_t ret = 0;
  Serial.println("Initialize Trust X...");
  ret = trustX.begin();
  if (ret) {
    Serial.print("Failed");
    return -1;
  }
  
  Serial.print("Initializing setting: ");
  ret = trustX.setCurrentLimit(6);
  if (ret) {
    Serial.println("Failed");
    return -1;
  }
  Serial.println("Ok");
  return 0;
}


void loop()
{
  uint32_t ret = 0;
  uint8_t  UID[UID_LENGTH];
  char tmp[100];

  // Initialize the M5Stack object
  M5.begin();
  
  if(system_start)
  {
    /*
     * Getting co-processor Unique ID of 27 bytes
     */
    Serial.println("\r\nGetting co-processor Unique ID:");
    ret = trustX.getUniqueID(UID, UID_LENGTH);

    if (ret) {
      Serial.println("Failed");
      Serial.println(ret,HEX);
      //close the connection
      trustX.end();
    }else{
    HEXDUMP(UID, UID_LENGTH);

    M5.Lcd.setTextSize(2);
    M5.Lcd.print("Trust X UID:\r\n");

    sprintf(tmp, "CIM Identifier:\t\t%2X\r\n", UID[0]);
    M5.Lcd.print(tmp); 
    sprintf(tmp, "Platform Identifier:\t%2X\r\n", UID[1]);
    M5.Lcd.print(tmp); 
    sprintf(tmp, "Model Identifier:\t%.2X\r\n", UID[2]);
    M5.Lcd.print(tmp); 
    sprintf(tmp, "ID of ROM mask:\t\t%.2X %.2X\r\n",UID[3], UID[4]);
    M5.Lcd.print(tmp); 
    sprintf(tmp, "Chip type:\t\t%.2X %.2X %.2X %.2X %.2X %.2X\r\n", UID[5], UID[6],UID[7], UID[8],UID[9], UID[10]);
    M5.Lcd.print(tmp); 
    sprintf(tmp, "Batch Number:\t\t%.2X %.2X %.2X %.2X %.2X %.2X\r\n", UID[11], UID[12],UID[13], UID[14],UID[15], UID[16]);
    M5.Lcd.print(tmp); 
    sprintf(tmp, "Position X:\t\t%.2X %.2X\r\n", UID[17], UID[18]);
    M5.Lcd.print(tmp); 
    sprintf(tmp, "Position Y:\t\t%.2X %.2X\r\n", UID[19], UID[20]); 
    M5.Lcd.print(tmp); 
    sprintf(tmp, "Firmware Identifier:\t%.2X %.2X %.2X %.2X\r\n", UID[21], UID[22], UID[23], UID[24]);
    M5.Lcd.print(tmp); 
    sprintf(tmp, "ESW Build number:\t%.2X %.2X\r\n", UID[25], UID[26]);
    M5.Lcd.print(tmp); 
#if 0    
    

#endif    
    }

	//close the connection
    trustX.end();

  }
  Serial.println("\r\nPress i for an iteration...");
  while (Serial.available()==0){} //Wait for user input
  String input = Serial.readString();  //Reading the Input string from Serial port.
  input.trim();
  if(input=="i"){
    if(reset()!=0){
      //Do not execute
      system_start=0;
      //close the connection
      trustX.end();
    }else{
      system_start=1;
      }
  }

}