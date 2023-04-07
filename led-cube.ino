
// Author: Ben Hilton
// Date: May 2021

// scripts for LED cube

#include <SPI.h>

// initialize global array for cube values (5x5x5)
bool a[5][5][5] = {0}; // [x][y][z] where z is vertical, [0] is base and [4] is top layer
bool b[25] = {0};
bool c[25] = {0};
int width = 5;
int serialPin = 11; // serial pin out (MOSI pin)
int clockPin = 13; // clockPin (SCK pin)
int latchPin = 5; // latch pin
int column25 = 2;
int levelZeroPin = 6;
int levelOnePin = 7;
int levelTwoPin = 8;
int levelThreePin = 9;
int levelFourPin = 10;

byte counter = 0;

void setup () {
  // set output pins as outputs:
  pinMode(serialPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(column25, OUTPUT);
  pinMode(levelZeroPin, OUTPUT);
  pinMode(levelOnePin, OUTPUT);
  pinMode(levelTwoPin, OUTPUT);
  pinMode(levelThreePin, OUTPUT);
  pinMode(levelFourPin, OUTPUT);
  SPI.begin(); // begin SPI communication
  Serial.begin(9600);
}


void layer2oneD(int layerNum) {
  for (int i = 0; i < 5; i++){
    for (int j = 0; j < 5; j++){
      b[width * i + j] = a[i][j][layerNum];
    }
  }
  // hey, we both wish I didn't have to do this:
  c[0] = b[14];
  c[1] = b[12];
  c[2] = b[10];
  c[3] = b[8];
  c[4] = b[6];
  c[5] = b[4];
  c[6] = b[2];
  c[7] = b[0];
  c[8] = b[16];
  c[9] = b[1];
  c[10] = b[3];
  c[11] = b[5];
  c[12] = b[7];
  c[13] = b[9];
  c[14] = b[11];
  c[15] = b[13];
  c[16] = b[18];
  c[17] = b[17];
  c[18] = b[19];
  c[19] = b[21];
  c[20] = b[23];
  c[21] = b[15];
  c[22] = b[20];
  c[23] = b[22];
  c[24] = b[24];
}

void zero_3d_array() {
  for (int i = 0; i < 5; i++){
    for (int j = 0; j < 5; j++){
      for (int k = 0; k < 5; k++){
        a[i][j][k] = 0;
      }
    }
  }
}

void array_3d_plane() {
  for (int i = 0; i < 5; i++){
    for (int j = 0; j < 5; j++){
      for (int k = 0; k < 5; k++){
        if (i == j){
          a[i][j][k] = 1;
        }
        else{
          a[i][j][k] = 1;
        }
      }
    }
  }
}

void send_layer(int layerNum) { 
  // convert layer to three bytes
  layer2oneD(layerNum); // converts current 3d array to 1d array
  byte byteOne = 0;
  byte byteTwo = 0;
  byte byteThree = 0;
  for (int i = 0; i < 8; i++){
    if(c[i]){
      byteOne |= (1 << i);
    }
  }  
  for (int i = 8; i < 16; i++){
    if(c[i]){
      byteTwo |= (1 << (i-8));
    }
  }  
  for (int i = 16; i < 24; i++){
    if(c[i]){
      byteThree |= (1 << (i-16));
    }
  }

// TEST CODE -------------------
  //Serial.println("The three Bytes");
  //Serial.println(byteOne);
  //Serial.println(byteTwo);
  //Serial.println(byteThree);
  digitalWrite(latchPin, LOW);
  SPI.transfer(byteOne);
  SPI.transfer(byteTwo);
  SPI.transfer(byteThree);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
  digitalWrite(column25, c[24]);
}


void send_cube(int frameDelay){
  int myDelay = 3; //delay in milliseconds between layers
  int iterations = frameDelay / (myDelay * 5);
  for (int i = 0; i < iterations; i++){
    digitalWrite(levelZeroPin, HIGH);
    send_layer(0);
    delay(myDelay);
    digitalWrite(levelZeroPin, LOW);
    digitalWrite(levelOnePin, HIGH);
    send_layer(1);
    delay(myDelay);
    digitalWrite(levelOnePin, LOW);
    digitalWrite(levelTwoPin, HIGH);
    send_layer(2);
    delay(myDelay);
    digitalWrite(levelTwoPin, LOW);
    digitalWrite(levelThreePin, HIGH);
    send_layer(3);
    delay(myDelay);
    digitalWrite(levelThreePin, LOW);
    digitalWrite(levelFourPin, HIGH);
    send_layer(4);
    delay(myDelay);
    digitalWrite(levelFourPin, LOW);
  }
}

void bounce(){
  for (int f = 0; f < 5; f++){
    for (int i = 0; i < 5; i++){
      for (int j = 0; j < 5; j++){
        for (int k = 0; k < 5; k++){
          if (i == f){
            a[i][j][k] = 1;
          }
          else{
            a[i][j][k] = 0;
          }
        }
      }
    }
    send_cube(80);
  }
  for (int f = 3; f > 0; f--){
    for (int i = 0; i < 5; i++){
      for (int j = 0; j < 5; j++){
        for (int k = 0; k < 5; k++){
          if (i == f){
            a[i][j][k] = 1;
          }
          else{
            a[i][j][k] = 0;
          }
        }
      }
    }
    send_cube(80);
  } 
}

void bounce_sideways(){
  for (int f = 0; f < 5; f++){
    for (int i = 0; i < 5; i++){
      for (int j = 0; j < 5; j++){
        for (int k = 0; k < 5; k++){
          if (j == f){
            a[i][j][k] = 1;
          }
          else{
            a[i][j][k] = 0;
          }
        }
      }
    }
    send_cube(80);
  }
  for (int f = 3; f > 0; f--){
    for (int i = 0; i < 5; i++){
      for (int j = 0; j < 5; j++){
        for (int k = 0; k < 5; k++){
          if (j == f){
            a[i][j][k] = 1;
          }
          else{
            a[i][j][k] = 0;
          }
        }
      }
    }
    send_cube(80);
  } 
}

void loop(){
  for(int i = 0; i < 1; i++){
    bounce();
  }
  for(int i = 0; i < 1; i++){
    bounce_sideways();
  }
  
}
