
#include <TFT.h>  // Arduino LCD library
#include <SPI.h>
#include <Stepper.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

const int stepsPerRevolution = 64;
//int switchPin = A0;
boolean buttonSelect = false;
boolean buttonUp = false;
boolean buttonDown = false;
boolean buttonStart = false;
int picNum = 1;
int del = 250;
boolean field = false;
unsigned long lastTime;
int motorPin1 = 4;
int motorPin2 = 5;
int motorPin3 = 6;
int motorPin4 = 7;
int relayPin = 3;


TFT TFTscreen = TFT(cs, dc, rst);
Stepper myStepper(stepsPerRevolution, motorPin1, motorPin2, motorPin3, motorPin4);

char picNumPrintout[6];
char delPrintout[6];

void setup() {
  
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();
  myStepper.setSpeed(300);
  Serial.begin(9600);
  

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);

  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);
  // set the font size
  TFTscreen.setTextSize(2);
  // write the text to the top left corner of the screen
  TFTscreen.text("PICS  DELAY :\n ", 0, 0);
  // ste the font size very large for the loop
  TFTscreen.setTextSize(3);

   String(picNum).toCharArray(picNumPrintout, 6);
       TFTscreen.stroke(255,255,255);
       TFTscreen.text(picNumPrintout, 2, 50);
   String(float(del)/1000).toCharArray(delPrintout, 6);
       TFTscreen.stroke(255,255,255);
       TFTscreen.text(delPrintout, 75, 50);
       
}


void loop() {

 
    // 608   479   396   200
  //690 / 526 / 431 / 362
  int buttonVal0 = digitalRead(A0);
  int buttonVal1 = digitalRead(A1);
  int buttonVal2 = digitalRead(A2);
  int buttonVal3 = digitalRead(A3);
  

  if (buttonVal1==LOW && (buttonSelect==false)){
   delay(5);
    buttonVal1 = digitalRead(A1);
    if(buttonVal1==LOW){
      buttonSelect = true;
    field =!field;
    }
  }
   if (buttonVal1 == HIGH && buttonSelect == true){
    delay(5);
    buttonVal1 = digitalRead(A1);
    if(buttonVal1 == HIGH){
   buttonSelect = false;
    }
  }
  
  if (buttonVal0==LOW && ((buttonUp==false)||(lastTime+350)<millis())){
    delay(5);
    buttonVal0 = digitalRead(A0);
    if(buttonUp==false)lastTime = millis();
    if(buttonVal0==LOW){
    buttonUp = true;
    if(field==false && (picNum<3000)) {
       
       TFTscreen.stroke(0,0,0);
       TFTscreen.text(picNumPrintout, 2, 50);
       picNum+=1;
       String(picNum).toCharArray(picNumPrintout, 6);
       TFTscreen.stroke(255,255,255);
       TFTscreen.text(picNumPrintout, 2, 50);
    }
    if(field==true && (del<20000)){
       TFTscreen.stroke(0,0,0);
       TFTscreen.text(delPrintout, 75, 50);
       del+=250;
      String(float(del)/1000).toCharArray(delPrintout, 6);
       TFTscreen.stroke(255,255,255);
       TFTscreen.text(delPrintout, 75, 50);
     }
    }
    
  }
  
  if (buttonVal0 == HIGH && buttonUp == true){
    delay(5);
    buttonVal0 = digitalRead(A0);
    if(buttonVal0 == HIGH){
    buttonUp = false;
    }
  }
  
  if(buttonVal2 == LOW && ((buttonDown==false)||(lastTime+350)<millis())){
    delay(5);
    buttonVal2 = digitalRead(A2);
    if(buttonDown==false)lastTime = millis();
    if(buttonVal2 == LOW){
    buttonDown = true;
    if(field==false && (1<picNum)) {
       TFTscreen.stroke(0,0,0);
       TFTscreen.text(picNumPrintout, 2, 50);
       picNum-=1;
       String(picNum).toCharArray(picNumPrintout, 6);
       TFTscreen.stroke(255,255,255);
       TFTscreen.text(picNumPrintout, 2, 50);
    }
    if(field==true && (250<del)) {
      TFTscreen.stroke(0,0,0);
       TFTscreen.text(delPrintout, 75, 50);
       del-=250;
       String(float(del)/1000).toCharArray(delPrintout, 6);
       TFTscreen.stroke(255,255,255);
       TFTscreen.text(delPrintout, 75, 50);
      }
    }
  }
  if (buttonVal2 == HIGH && buttonDown == true){
    delay(5);
    buttonVal2 = digitalRead(A2);
    if(buttonVal2 == HIGH){
    buttonDown = false;
    }
  }
  if(buttonVal3 == LOW && (buttonStart==false)){
    delay(5);
    buttonVal3 = digitalRead(A3);
    if(buttonVal3 == LOW){
    buttonStart = true;
    move(picNum , del);
    }
  }
  if (buttonVal3 == HIGH && buttonStart == true){
    delay(5);
    buttonVal3 = digitalRead(A3);
    if(buttonVal3==HIGH){
    buttonStart = false;
    }
  }
}

void move(int count , int dwell){

  int steps = 0;
  float tempSteps = 4096/(float)count;
  float reminderSteps = 0;
  float reminder = tempSteps - (int)tempSteps;
  Serial.println((String)reminder);
  
  for (int i=0;i<count;i++){
    
    reminderSteps = reminderSteps + reminder;
    Serial.println((String)reminderSteps);
    steps = (int)tempSteps + (int)reminderSteps;
    if(1.0<reminderSteps) reminderSteps = reminderSteps - (int)reminderSteps;
    
    delay(200);
    digitalWrite(relayPin, HIGH);
    delay(200);
    digitalWrite(relayPin, LOW);
    delay(dwell);
    myStepper.step(steps);
    int buttonVal3 = digitalRead(A3);
    if(buttonVal3 == LOW)break;
    }
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, LOW);
}

