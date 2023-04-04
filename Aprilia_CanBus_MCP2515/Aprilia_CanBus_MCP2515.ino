#include <SPI.h>

#include <can-serial.h>
#include <mcp2515_can.h>
#include <mcp2515_can_dfs.h>
#include <mcp2518fd_can.h>
#include <mcp2518fd_can_dfs.h>
#include <mcp_can.h>
//#include <mcp2515.h>
//#include <mcp2515_can.h>

const int SPI_CS_PIN = 3; // NANO - 10 -- XIAO - 3
const int CAN_INT_PIN = 7;

const int SPI_CLOCK = 9600; // 9600 - NANO || 115200 - UNO

//MCP2515 mcp2515_can(SPI_CS_PIN, SPI_CLOCK, &SPI); // Set CS pin
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#define MAX_DATA_SIZE 8;
#define IS_EXTENSION_ID 0;

//struct can_frame canMsg;
unsigned char data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned long can_id = 0x300;
byte ext = IS_EXTENSION_ID;
byte len = MAX_DATA_SIZE;

int i=0x00;
int pressed=0;
const int buttonPin2 = 2;
const int buttonPin1 = 1;
const int buttonPin4 = 4;
const int buttonPin5 = 5;
const int buttonPin6 = 6; // SHIFT

void setup() {
  Serial.begin(9600); 
  SPI.begin();

  //CAN.reset();
  //CAN.setBitrate(CAN_1000KBPS);
  //CAN.setNormalMode();

  while(!Serial){};

  while (CAN_OK != CAN.begin(CAN_1000KBPS)) {             // init can bus : baudrate = 500k
    Serial.println("CAN init fail, retry...");
    delay(100);
  }

  Serial.println(F("CAN init ok!"));
  Serial.println(SPI_CS_PIN);
  Serial.println(CAN_INT_PIN);

  delay(100);

  //ATC Control
  /*pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);

  pinMode(A1, OUTPUT);
  digitalWrite(A1, LOW);

  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);

  pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);

  pinMode(A4, OUTPUT);
  digitalWrite(A4, LOW);*/
  

  //CAN BUS
  pinMode(buttonPin2, INPUT);
  digitalWrite(buttonPin2, LOW);

  pinMode(buttonPin1, INPUT);
  digitalWrite(buttonPin1, LOW);

  pinMode(buttonPin4, INPUT);
  digitalWrite(buttonPin4, LOW);

  pinMode(buttonPin5, INPUT);
  digitalWrite(buttonPin5, LOW);

  pinMode(buttonPin6, INPUT);
  digitalWrite(buttonPin6, LOW);

  /*canMsg.can_id  = 0x300;
  canMsg.can_dlc = 8;

  canMsg.data[0] = 0x00;
  canMsg.data[1] = 0x00;
  canMsg.data[2] = 0x00;
  canMsg.data[3] = 0x00;
  canMsg.data[4] = 0x00;
  canMsg.data[5] = 0x00;
  canMsg.data[6] = 0x00;
  canMsg.data[7] = 0x00;*/
}

void loop() {

  //Serial.println("SUCCESS");
  //delay(2000);

  if(digitalRead(buttonPin6)) {//if (digitalRead(buttonPin2) && digitalRead(buttonPin6)) {//Down+Shift - shift requires two buttons to be pressed
    //canMsg.data[3]=0x20;
    data[3]=0x20;
    Serial.println("DOWN"); // MENU DOWN 
  } else if (digitalRead(buttonPin2)) {//Up
    //canMsg.data[3]=0x80;  // DOES MENU UP
    data[3]=0x80;
    Serial.println("UP");  // MENU UP
  } else if(digitalRead(buttonPin1)) { //if (digitalRead(buttonPin4) && digitalRead(buttonPin6)) {//Left+shift - shift requires two buttons to be pressed
    //canMsg.data[3]=0x08;
    data[3]=0x08;
    Serial.println("LEFT");  // MENU LEFT
  } else if (digitalRead(buttonPin5)) {//Right
    //canMsg.data[3]=0x02;
    data[3]=0x02;
    Serial.println("RIGHT"); // MENU RIGHT
  } else if (digitalRead(buttonPin4)) {//Click
    //canMsg.data[4]=0x80;
    data[4]=0x80;
    Serial.println("CLICK"); // CLICK THE MENU
  } else {
    if(data[3] != 0x00) { //if(canMsg.data[3] != 0x00) {
      //canMsg.data[3]=0x00;
      data[3]=0x00;
      pressed=0;
      //Serial.println("DEBUG1");
    } else if(data[4] != 0x00) { // else if(canMsg.data[4] != 0x00) {
      //canMsg.data[4]=0x00;
      data[4]=0x00;
      pressed=0;
      //Serial.println("DEBUG2");
    }
  }

  //canMsg.data[7] = i; 
  data[7] = i;

  if (i==0x0F) {
    i=0x00; //reset
  } else {
    i++;
  }

  
  
  if( (data[3] != 0x00 || data[4] != 0x00) && pressed == 0) { //if( (canMsg.data[3] != 0x00 || canMsg.data[4] != 0x00) && pressed == 0) {
    pressed=1;
    Serial.println("SEND MSG");
    //CAN.sendMessage(&canMsg);
    CAN.MCP_CAN::sendMsgBuf(can_id, ext, len, data);
  }
/*
  //Serial.println(pressed);

  //ATC part
  if (digitalRead(A0)) {//ATC NO=A2, NC=A1 // pressed
    Serial.println("TC + CLICKED");
    digitalWrite(A2,LOW);
    digitalWrite(A1,HIGH);
  } else {
    //Serial.println("TC + RELEASED");
    digitalWrite(A1,LOW); // not pressed
    digitalWrite(A2,HIGH);
  }
  
  if (digitalRead(A3)) {//ATC NO=A5, NC=A4 // pressed
    Serial.println("TC - CLICKED");
    digitalWrite(A5,LOW);
    digitalWrite(A4,HIGH);
  } else {                      // not pressed
    //Serial.println("TC - RELEASED");
    digitalWrite(A4,LOW);
    digitalWrite(A5,HIGH);
  }

*/

  //END ATC Part
  delay(50);
}
