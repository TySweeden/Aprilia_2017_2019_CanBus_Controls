#include <SPI.h>
#include <mcp2515_can.h>
#define MAX_DATA_SIZE 8;
#define IS_EXTENSION_ID 0;

const int SPI_CS_PIN = 1; // NANO - 10 -- XIAO - 3? 1?
const int CAN_INT_PIN = 7;
const int SPI_CLOCK = 9600; // 9600 - NANO || 115200 - UNO

unsigned char data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned long can_id = 0x300;
byte ext = IS_EXTENSION_ID;
byte len = MAX_DATA_SIZE;

mcp2515_can CAN(SPI_CS_PIN); // Set CS pin

int i=0x00;
bool pressed=false;
bool messageSent=false;
const int buttonPin2 = 2;
const int buttonPin3 = 3;
const int buttonPin4 = 4;
const int buttonPin5 = 5;
const int buttonPin6 = 6; // SHIFT

void setup() {
  SERIAL_PORT_MONITOR.begin(9600); 

  // maybe the lib is not setting it
  //pinMode(SPI_CS_PIN, OUTPUT);
  //digitalWrite(SPI_CS_PIN, LOW);

  while(!Serial){};

  while (CAN_OK != CAN.begin(CAN_1000KBPS)) { // by default MCP_16MHz
    SERIAL_PORT_MONITOR.println("CAN init failed, retry...");
    delay(200);
  }

  SERIAL_PORT_MONITOR.println(F("CAN init success!"));
  SERIAL_PORT_MONITOR.println(SPI_CS_PIN);
  SERIAL_PORT_MONITOR.println(CAN_INT_PIN);

  //delay(100);

  //CAN BUS
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
}

void loop() {
  if(pressed) {
    //SERIAL_PORT_MONITOR.println((!messageSent && (data[3] != 0x00 || data[4] != 0x00)));
    //if(!messageSent) {
      //SERIAL_PORT_MONITOR.println(messageSent);
      CAN.sendMsgBuf(can_id, ext, 0, len, data);
      
      //messageSent = true;
    //}
  } ///else if(messageSent) {
    //messageSent = false;
  //}

  if(digitalRead(buttonPin6)) {//if (digitalRead(buttonPin2) && digitalRead(buttonPin6)) {//Down+Shift - shift requires two buttons to be pressed
    data[3]=0x20;
    //SERIAL_PORT_MONITOR.println("DOWN"); // MENU DOWN 
  } else if (digitalRead(buttonPin2)) {//Up
    data[3]=0x80;
    //SERIAL_PORT_MONITOR.println("UP");  // MENU UP
  } else if(digitalRead(buttonPin3)) { //if (digitalRead(buttonPin4) && digitalRead(buttonPin6)) {//Left+shift - shift requires two buttons to be pressed
    data[3]=0x08;
    //SERIAL_PORT_MONITOR.println("LEFT");  // MENU LEFT
  } else if (digitalRead(buttonPin5)) {//Right
    data[3]=0x02;
    //SERIAL_PORT_MONITOR.println("RIGHT"); // MENU RIGHT
  } else if (digitalRead(buttonPin4)) {//Click
    data[4]=0x80;
    //SERIAL_PORT_MONITOR.println("CLICK"); // CLICK THE MENU
  } else {
    if(data[3] != 0x00) {
      data[3]=0x00;
      pressed=false;
      //SERIAL_PORT_MONITOR.println("DEBUG1");
    } else if(data[4] != 0x00) {
      data[4]=0x00;
      pressed=false;
      //SERIAL_PORT_MONITOR.println("DEBUG2");
    }
  }

  //data[7] = i;

  //if (i==0x0F) {
  //  i=0x00; //reset
  //} else {
  //  i++;
  //}


  if(data[3] != 0x00 || data[4] != 0x00) {
    //SERIAL_PORT_MONITOR.println("SEND MSG");
    //CAN.sendMsgBuf(can_id, ext, 0, len, data);
    //CAN.MCP_CAN::sendMsgBuf(can_id, ext, len, data);
    pressed=true;
  }
}
