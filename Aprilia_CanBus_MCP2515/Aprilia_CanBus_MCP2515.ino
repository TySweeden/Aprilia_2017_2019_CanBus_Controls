#include <SPI.h>
#include <mcp2515.h>
#include <mcp2515_can.h>

const int SPI_CS_PIN = 1;
const int CAN_INT_PIN = 7;
const int SPI_CLOCK = 9600; // 9600 - NANO || 115200 - UNO

MCP2515 mcp2515_can(SPI_CS_PIN); // Set CS pin
#define MAX_DATA_SIZE 8

struct can_frame canMsg;

int i=0x00;
int pressed=0;
const int buttonPin2 = 2;
const int buttonPin3 = 3;
const int buttonPin4 = 4;
const int buttonPin5 = 5;
const int buttonPin6 = 6;

void setup() {
  Serial.begin(9600); 
  SPI.begin();

  // mcp2515 header
  mcp2515_can.reset();
  mcp2515_can.setBitrate(CAN_1000KBPS);
  mcp2515_can.setNormalMode();

  /*
  // mcp2515_can header
  while (CAN_OK != mcp2515_can.begin(CAN_1000KBPS)) {
    SERIAL_PORT_MONITOR.println("CAN init failed, retry...");
    delay(100);
  }
  */

  Serial.println(F("CAN init success!"));
  Serial.println(SPI_CS_PIN);
  Serial.println(CAN_INT_PIN);

  delay(100);

  //SWITCH BUTTONS - CAN BUS
  pinMode(buttonPin2, INPUT);
  digitalWrite(buttonPin2, LOW);

  pinMode(buttonPin3, INPUT);
  digitalWrite(buttonPin3, LOW);

  pinMode(buttonPin4, INPUT);
  digitalWrite(buttonPin4, LOW);

  pinMode(buttonPin5, INPUT);
  digitalWrite(buttonPin5, LOW);

  pinMode(buttonPin6, INPUT);
  digitalWrite(buttonPin6, LOW);

  // CAN MESSAGE DATA STRUCT
  canMsg.can_id  = 0x300;
  canMsg.can_dlc = MAX_DATA_SIZE;

  canMsg.data[0] = 0x00;
  canMsg.data[1] = 0x00;
  canMsg.data[2] = 0x00;
  canMsg.data[3] = 0x00;
  canMsg.data[4] = 0x00;
  canMsg.data[5] = 0x00;
  canMsg.data[6] = 0x00;
  canMsg.data[7] = 0x00;
}

void loop() {

  if(digitalRead(buttonPin6)) {//if (digitalRead(buttonPin2) && digitalRead(buttonPin6)) {//Down+Shift - shift requires two buttons to be pressed
    canMsg.data[3]=0x20;
    Serial.println("DOWN"); // MENU DOWN 
  } else if (digitalRead(buttonPin2)) {//Up
    canMsg.data[3]=0x80;  // DOES MENU UP
    Serial.println("UP");  // MENU UP
  } else if(digitalRead(buttonPin3)) { //if (digitalRead(buttonPin4) && digitalRead(buttonPin6)) {//Left+shift - shift requires two buttons to be pressed
    canMsg.data[3]=0x08;
    Serial.println("LEFT");  // MENU LEFT
  } else if (digitalRead(buttonPin5)) {//Right
    canMsg.data[3]=0x02;
    Serial.println("RIGHT"); // MENU RIGHT
  } else if (digitalRead(buttonPin4)) {//Click
    canMsg.data[4]=0x80;
    Serial.println("CLICK"); // CLICK THE MENU
  } else {
    if(canMsg.data[3] != 0x00) {
      canMsg.data[3]=0x00;
      pressed=0;
      //Serial.println("DEBUG1");
    } else if(canMsg.data[4] != 0x00) {
      canMsg.data[4]=0x00;
      pressed=0;
      //Serial.println("DEBUG2");
    }
  }

  canMsg.data[7] = i; 

  if (i==0x0F) {
    i=0x00; //reset
  } else {
    i++;
  }

  if( (canMsg.data[3] != 0x00 || canMsg.data[4] != 0x00) && pressed == 0) {
    pressed=1;
    Serial.println("SEND MSG");
    mcp2515_can.sendMessage(&canMsg);
  }

  delay(50);
}
