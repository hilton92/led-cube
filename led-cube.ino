
// Author: Ben Hilton
// Date: May 2021

// scripts for LED cube

#include <SPI.h>


int serialPin = 11; // serial pin out (MOSI pin)
int clockPin = 13; // clockPin (SCK pin)
int latchPin = 6; // Latch pin
byte counter = 0;

void setup () {
  // set output pins as outputs:
  pinMode(serialPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  SPI.begin();
  Serial.begin(9600);
  Serial.print("setup complete");
  //sendFrame(0xC3); // 11011011
}


/*int sendFrame(byte muhByte) { 
  digitalWrite(latchPin, LOW);
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(muhByte);
  SPI.endTransaction();
  digitalWrite(latchPin, HIGH);
  delay(5);
  digitalWrite(latchPin, LOW);
  Serial.print("function complete");
}*/

void loop(){
  digitalWrite(latchPin, LOW);
  SPI.transfer(counter);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
  counter = counter + 1;
  delay(500);
  
  /*sendFrame(0xDB); // 11011011
  delay(1000);
  sendFrame(0xC3); // 11000011
  delay(1000);
  sendFrame(0x81); // 10000001
  delay(1000);
  Serial.print("loop complete");*/
}
