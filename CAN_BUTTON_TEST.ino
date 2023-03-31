#include <SPI.h>
#include <mcp2515.h>
#include <mcp2515_can.h>

const int SPI_CS_PIN = 10;
const int CAN_INT_PIN = 2;

MCP2515 mcp2515_can(SPI_CS_PIN); // Set CS pin
#define MAX_DATA_SIZE 8


struct can_frame canMsg;

int i=0x00;
int pressed=0;
const int buttonPin3 = 3;
const int buttonPin4 = 4;
const int buttonPin5 = 5;
const int buttonPin6 = 6;
const int buttonPin7 = 7; // SHIFT
const int D9 = 9;

void setup() {
  SERIAL_PORT_MONITOR.begin(9600); // 9600 - NANO || 115200 - UNO

  mcp2515_can.reset();
  mcp2515_can.setBitrate(CAN_1000KBPS);
  mcp2515_can.setNormalMode();

    SERIAL_PORT_MONITOR.println(F("CAN init ok!"));
    Serial.println(SPI_CS_PIN);
    Serial.println(CAN_INT_PIN);

    

  //ATC Control
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);

  pinMode(A1, OUTPUT);
  digitalWrite(A1, LOW);

  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);

  pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);

  pinMode(A4, OUTPUT);
  digitalWrite(A4, LOW);

  // test pin
  pinMode(D9, OUTPUT);
  digitalWrite(D9, HIGH);

  //CAN BUS
  pinMode(buttonPin3, INPUT);
  digitalWrite(buttonPin3, LOW);

  pinMode(buttonPin4, INPUT);
  digitalWrite(buttonPin4, LOW);

  pinMode(buttonPin5, INPUT);
  digitalWrite(buttonPin5, LOW);

  pinMode(buttonPin6, INPUT);
  digitalWrite(buttonPin6, LOW);

  pinMode(buttonPin7, INPUT);
  digitalWrite(buttonPin7, LOW);

  canMsg.can_id  = 0x300;
  canMsg.can_dlc = 8;

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



  if(digitalRead(buttonPin7)) {//if (digitalRead(buttonPin3) && digitalRead(buttonPin7)) {//Down+Shift - shift requires two buttons to be pressed
    canMsg.data[3]=0x20;
    SERIAL_PORT_MONITOR.println("DOWN"); // MENU DOWN 
  } else if (digitalRead(buttonPin3)) {//Up
    canMsg.data[3]=0x80;  // DOES MENU UP
    SERIAL_PORT_MONITOR.println("UP");  // MENU UP
  } else if(digitalRead(buttonPin6)) { //if (digitalRead(buttonPin4) && digitalRead(buttonPin7)) {//Left+shift - shift requires two buttons to be pressed
    canMsg.data[3]=0x08;
    SERIAL_PORT_MONITOR.println("LEFT");  // MENU LEFT
  } else if (digitalRead(buttonPin4)) {//Right
    canMsg.data[3]=0x02;
    SERIAL_PORT_MONITOR.println("RIGHT"); // MENU RIGHT
  } else if (digitalRead(buttonPin5)) {//Click
    canMsg.data[4]=0x80;
    SERIAL_PORT_MONITOR.println("CLICK"); // CLICK THE MENU
  } else {
    if(canMsg.data[3] != 0x00) {
      canMsg.data[3]=0x00;
      pressed=0;
      //SERIAL_PORT_MONITOR.println("DEBUG1");
    } else if(canMsg.data[4] != 0x00) {
      canMsg.data[4]=0x00;
      pressed=0;
      //SERIAL_PORT_MONITOR.println("DEBUG2");
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

    SERIAL_PORT_MONITOR.println("SEND MSG");
    mcp2515_can.sendMessage(&canMsg);
  }

  //SERIAL_PORT_MONITOR.println(pressed);

  //ATC part
  if (digitalRead(A0)) {//ATC NO=A2, NC=A1 // pressed
    SERIAL_PORT_MONITOR.println("TC + CLICKED");
    digitalWrite(A2,LOW);
    digitalWrite(A1,HIGH);
  } else {
    //SERIAL_PORT_MONITOR.println("TC + RELEASED");
    digitalWrite(A1,LOW); // not pressed
    digitalWrite(A2,HIGH);
  }
  
  if (digitalRead(A3)) {//ATC NO=A5, NC=A4 // pressed
    SERIAL_PORT_MONITOR.println("TC - CLICKED");
    digitalWrite(A5,LOW);
    digitalWrite(A4,HIGH);
  } else {                      // not pressed
    //SERIAL_PORT_MONITOR.println("TC - RELEASED");
    digitalWrite(A4,LOW);
    digitalWrite(A5,HIGH);
  }



  //END ATC Part
  //delay(200);
}
