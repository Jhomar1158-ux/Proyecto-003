#include <Arduino.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define VERT 14       //D5
#define HOR 12        //D6
#define FINGERL 13    //D7
#define FINGERR 15    //D8
#define INPUT_BOX 16  //D0

enum STATE{
  STATE_START = 0,
  STATE_VERT0,
  STATE_HOR0,
  STATE_FINGER0,
  STATE_HOR1,
  STATE_VERT1,
  STATE_FINGER1,
  STATE_STOP
};

Servo servoMotorVert;
Servo servoMotorHor;
Servo servoMotorFingerR;
Servo servoMotorFingerL;

unsigned long previousMillis = 0;
const long interval = 1000;
int ledState = LOW;

const char* ssid = "TP-Link_F365";
const char* password = "96290361";

//UDP
WiFiUDP UDPServer;
unsigned int UDPPort = 1102;

byte packetBuffer[8];

int reqState = STATE_STOP;
int myState = STATE_STOP;

int angle = 0;

const int FINGER_ANGLE_R = 10;
const int FINGER_ANGLE_L = 10;
const int HOR_ANGLE = 90;
const int VERT_ANGLE = 60;

void setup() { 
  servoMotorVert.attach(VERT, 500, 2600);
  servoMotorVert.write(0);

  servoMotorHor.attach(HOR, 500, 2600);
  servoMotorHor.write(HOR_ANGLE);

  servoMotorFingerR.attach(FINGERR, 500, 2600);
  servoMotorFingerR.write(0);

  servoMotorFingerL.attach(FINGERL, 500, 2600);
  servoMotorFingerL.write(FINGER_ANGLE_L);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(INPUT_BOX, INPUT);

  WiFi.begin(ssid, password);
  Serial.begin(9600);
  Serial.println("");

  // Espera de conexión
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  UDPServer.begin(UDPPort); 
}

void loop() { 
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW)
      ledState = HIGH;  // LED OFF
    else
      ledState = LOW;   // LED ON
    digitalWrite(LED_BUILTIN, ledState);
  }
  
  processPostData();
  actuatorControl();
  delay(10);
}

void actuatorControl(){
  if(myState==STATE_START){
    myState = STATE_VERT0;
  }
  else if(myState==STATE_VERT0){
    servoMotorVert.write(VERT_ANGLE); // 120
    delay(500);
    myState = STATE_HOR0;
  }
  else if(myState==STATE_HOR0){
    if(angle <= HOR_ANGLE){
      angle += 5;
      servoMotorHor.write(HOR_ANGLE-angle);
      delay(50);
    }else{
      angle = 0;
      myState = STATE_VERT1;
    }
  }
  else if(myState==STATE_VERT1){
    servoMotorVert.write(0);
    delay(500);
    myState = STATE_FINGER0;
  }
  else if(myState==STATE_FINGER0){
    if(digitalRead(INPUT_BOX)==LOW){
      servoMotorFingerR.write(FINGER_ANGLE_R);
      servoMotorFingerL.write(0); //Cierra FINGER
      delay(800);
      myState = STATE_HOR1;
    }
  }
  else if(myState==STATE_HOR1){
    if(angle <= HOR_ANGLE){
      angle += 5;
      servoMotorHor.write(angle);
      delay(50);
    }else{
      angle = 0;
      myState = STATE_FINGER1;
      delay(500);
    }
  }
  else if(myState==STATE_FINGER1){
    servoMotorFingerR.write(FINGER_ANGLE_R*2/3);
    servoMotorFingerL.write(FINGER_ANGLE_L/3);
    delay(70);
    servoMotorFingerR.write(FINGER_ANGLE_R/3);
    servoMotorFingerL.write(FINGER_ANGLE_L*2/3);
    delay(70);
    servoMotorFingerR.write(FINGER_ANGLE_R/4);
    servoMotorFingerL.write(FINGER_ANGLE_L*3/4);
    myState = STATE_STOP;
  }
  else if(myState==STATE_STOP){
    if(reqState==STATE_START){
      myState=STATE_START;
      reqState=STATE_STOP;
    }
    servoMotorVert.write(0);
    servoMotorHor.write(HOR_ANGLE);
    servoMotorFingerR.write(0);
    servoMotorFingerL.write(FINGER_ANGLE_L);
  }
  if(myState!=STATE_STOP){
    Serial.println(myState);
    Serial.println(digitalRead(INPUT_BOX));
  }
}

void processPostData(){
  int cb = UDPServer.parsePacket();
  if (cb) {
    for(int i=0; i < sizeof(packetBuffer); i++){
      packetBuffer[i] = 0;
    }
    UDPServer.read(packetBuffer, sizeof(packetBuffer));
    String data = String((const char*)packetBuffer);
    Serial.println(data);
    if(data.equals("START")){
      reqState = STATE_START;
    }else if(data.equals("STOP")){
      myState = STATE_STOP;
    }
  }
}
