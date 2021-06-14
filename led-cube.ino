
// Author: Ben Hilton
// Date: May 2021

// scripts for LED cube

#include <SPI.h>

// initialize global array for cube values (5x5x5)
bool a[5][5][5] = {0}; // [x][y][z] where z is vertical, [0] is base and [4] is top layer
bool b[25] = {0};
int width = 5;
int serialPin = 11; // serial pin out (MOSI pin)
int clockPin = 13; // clockPin (SCK pin)
int latchPin = 6; // latch pin
byte counter = 0;

void setup () {
  // set output pins as outputs:
  pinMode(serialPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  
  SPI.begin(); // begin SPI communication
  Serial.begin(9600);
}


void layer2oneD(int layerNum) {
  for (int i = 0; i < 5; i++){
    for (int j = 0; j < 5; j++){
      b[width * i + j] = a[i][j][layerNum];
    }
  }
}

void zero3dArray() {
  for (int i = 0; i < 5; i++){
    for (int j = 0; j < 5; j++){
      for (int k = 0; k < 5; k++){
        a[i][j][k] = 0;
      }
    }
  }
}

void array3dPlane() {
  for (int i = 0; i < 5; i++){
    for (int j = 0; j < 5; j++){
      for (int k = 0; k < 5; k++){
        if (i == j){
          a[i][j][k] = 1;
        }
        else{
          a[i][j][k] = 0;
        }
      }
    }
  }
}

void sendLayer(int layerNum) { 
  // convert layer to three bytes
  layer2oneD(layerNum); // converts current 3d array to 1d array
  byte byteOne = 0;
  byte byteTwo = 0;
  byte byteThree = 0;
  for (int i = 0; i < 8; i++){
    if(b[i]){
      byteOne |= (1 << i);
    }
  }  
  for (int i = 8; i < 16; i++){
    if(b[i]){
      byteTwo |= (1 << (i-8));
    }
  }  
  for (int i = 16; i < 24; i++){
    if(b[i]){
      byteThree |= (1 << (i-16));
    }
  }

// TEST CODE -------------------
  Serial.println("The three Bytes");
  Serial.println(byteOne);
  Serial.println(byteTwo);
  Serial.println(byteThree);
  
  digitalWrite(latchPin, LOW);
  SPI.transfer(byteOne);
  SPI.transfer(byteTwo);
  SPI.transfer(byteThree);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
}



void loop(){
  array3dPlane();
  sendLayer(0);
  delay(20000);
}
