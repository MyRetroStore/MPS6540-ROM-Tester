/*
  MPS6540 ROM Tester
  myretrostore.co.uk
  22 Sept 2025
  
  Tested using Arduino v1.8.19

  Set READONLY flag in config.h to only read contents of ROM and output to serial port at 115200

  Requires DIO2 library.
*/

#include "config.h"
#include "MD5.h"
#include "crc.h"
#include "DIO2.h"

const  uint8_t DATABUS[] = {36, 32, 41, 34, 33, A7, 60, 59 };
const  uint8_t ROM6540_ADDRBUS[] = {25, 24, 23, 22, 54, 55, 42, 40, 31, 56, 58 };
#define ROM6540_CS1 57
#define ROM6540_CS2 37
#define ROM6540_CS3 26
#define ROM6540_CS4 27
#define ROM6540_CS5 28
#define ROM6540_CLK 39

#define LED_Pass 44
#define LED_Fail 43

uint16_t BIN_SIZE = 2048;

uint16_t ROM_counter = 0;
uint16_t counter;
boolean hashmatch = false;
size_t index = 0;

char ROM[4100] = {};  // Buffer to hold 4k of ROM
int ledState = LOW;     // the current state of LED

//=======================================================================================
// Initialize pins
//=======================================================================================
void Initialize() {

  pinMode2(ROM6540_CS1, OUTPUT);
  digitalWrite2(ROM6540_CS1, HIGH); //Active high

  pinMode2(ROM6540_CS2, OUTPUT);
  digitalWrite2(ROM6540_CS2, HIGH); //Active high

  pinMode2(ROM6540_CS3, OUTPUT);
  digitalWrite2(ROM6540_CS3, HIGH); //Active low

  pinMode2(ROM6540_CS4, OUTPUT);
  digitalWrite2(ROM6540_CS4, LOW); //Active low

  pinMode2(ROM6540_CS5, OUTPUT);
  digitalWrite2(ROM6540_CS5, LOW); //Active low


  pinMode2(ROM6540_CLK, OUTPUT);
  digitalWrite2(ROM6540_CLK, HIGH); //Active low

  for (int i = 0; i < sizeof(ROM6540_ADDRBUS); i++) pinMode2(ROM6540_ADDRBUS[i], OUTPUT);

}

//=======================================================================================
// Pass / Fail LEDs and HALT
//=======================================================================================
void Display_LEDs(boolean error) {

  if (error == true) {  // Fail
    digitalWrite2(LED_Pass, LOW);
    while (1) {
      digitalWrite2(LED_Fail, LOW);
      delay(500);
      digitalWrite2(LED_Fail, HIGH);
      delay(500);
    }

  } else {  // Pass
    digitalWrite2(LED_Pass, HIGH);
    digitalWrite2(LED_Fail, LOW);
    while (1);
  }
}

//=======================================================================================
// Setup
//=======================================================================================
void setup() {
  Serial.begin(115200);
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB port only

  Serial.println("\n======================");
  Serial.println("  MPS6540 ROM Tester");
  Serial.println("  myretrostore.co.uk");
  Serial.print("     "); Serial.println(VERSION);
  Serial.println("======================");

  pinMode2(LED_Fail, OUTPUT);
  pinMode2(LED_Pass, OUTPUT);

  digitalWrite2(LED_Fail, LOW);
  digitalWrite2(LED_Pass, LOW);

}

//=======================================================================================
// Set Address
//=======================================================================================
void setAddress(uint16_t addr) {
  for (uint8_t bitno = 0 ; bitno < sizeof(ROM6540_ADDRBUS) ; bitno++) {
    if (bitRead(addr, bitno) == 1) {
      digitalWrite2(ROM6540_ADDRBUS[bitno], HIGH);
    } else {
      digitalWrite2(ROM6540_ADDRBUS[bitno], LOW);
    }
  }
  return;
}

//=======================================================================================
// Read Address
//=======================================================================================
byte readAddress(int addr) {
  digitalWrite2(ROM6540_CS3, HIGH);
  //CS3_HIGH;
  byte data = 0;
  setAddress(addr);
  digitalWrite2(ROM6540_CS3,  LOW);
  delayMicroseconds(1);
  for (int bitno = 0; bitno < 8; bitno++) {
    if (digitalRead2(DATABUS[bitno]) == HIGH) {
      bitSet(data, bitno);
    }
  }
  digitalWrite2(ROM6540_CS3, LOW);
  //CS3_LOW;
  return data;
}

//=======================================================================================
// Main Loop
//=======================================================================================

void loop() {

  //Calculate size of md5 arrays
  // const size_t md5Cnt_8192 = sizeof md5_8192 / sizeof * md5_8192;
  const size_t md5Cnt_4096 = sizeof md5_4096 / sizeof * md5_4096;
  uint8_t ROM_Byte;
  char str[40];

  Initialize();

  if (ReadOnly == false) {
    Serial.println("\nReading EPROM...");
  } else {
    Serial.println("Dumping EPROM (Not comparing)\n");
  }

  //Read entire rom and calculate md5
  for (int i = 0; i < sizeof(DATABUS); i++) pinMode2(DATABUS[i], INPUT);

  for (uint16_t address = 0; address < BIN_SIZE; address++)
  {

    delayMicroseconds(1);
    ROM_Byte = readAddress(address);

    ROM[ROM_counter] = ROM_Byte;
    ROM_counter++;

    unsigned char* hash;
    char *md5str;

    if ( address == 2047)  {
      ROM_counter = 0;
      hash = MD5::make_hash(ROM, 2048);
      md5str = MD5::make_digest(hash, 16);

      for (index; index < md5Cnt_4096; index++) {
        if (strcmp_P(md5str, md5_4096[index].hash1) == 0) {
          Serial.println("\nFound a valid MD5 hash");
          Serial.print("ROM is "); Serial.print((__FlashStringHelper*) md5_4096[index].fileName);
          Display_LEDs(false);
        }
      }

      if ((ReadOnly == false) and (hashmatch == false)) { // Failed to match first hash.
        Serial.println("\nUnable to find valid MD5 match for ROM");
        Serial.print("\nMD5 read from ROM is "); Serial.println(md5str);
        Serial.println("\nCheck that the ROM type is included in crc.h otherwise the ROM is corrupt");
        Serial.println("Try the ReadOnly switch in the config.h and examine the dump");
        Display_LEDs(true);
      }
    }

    if (address % 128 == 0) //Visual progress feedback. Toggle LEDs
    {
      digitalWrite2(LED_Pass, ledState);
      digitalWrite2(LED_Fail, !ledState);
      ledState = !ledState;
    }

    if (ReadOnly == true) // If ReadOnly flag set we are only going to dump ROM to screen.
    {
      if (address % 16 == 0) // Setup formatting
      {
        sprintf(str, "\r\n%04X:", address);
        Serial.print(str);
      }
      sprintf(str, " %02X", ROM_Byte);
      Serial.print(str);
    }
    delayMicroseconds(1);
  }

  if (ReadOnly == true) {
    Serial.println("\n\nEPROM Dump Complete");
  }

  Display_LEDs(true);
}
