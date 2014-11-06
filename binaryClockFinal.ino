
#include <Wire.h>
#include "RTClib.h"

 RTC_DS1307 RTC;
 int datapin = 2;
 int clockpin = 3;
 int latchpin = 4;
 int datapin2 = 8;
 int clockpin2 = 9;
 int latchpin2= 10;
 byte data1 = 0;
 byte data2 = 0;
 const int  buttonPin = 11;    // the pin that the pushbutton is attached to
 int buttonPushCounter = 1;   // counter for the number of button presses
 int buttonState = 0;     // current state of the button
 int buttonState1 = 0;
 int lastButtonState = 0;
 int bandera = 0; // previous state of the button
 const int buttonPin2 = 7;
 int minus;
 int secus;
 int hor;
 
void setup(){
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();        
  if (! RTC.isrunning()) {
  Serial.println("RTC is NOT running!");}
  pinMode(datapin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  pinMode(latchpin, OUTPUT);
  pinMode(datapin2, OUTPUT);
  pinMode(clockpin2, OUTPUT);
  pinMode(latchpin2, OUTPUT); 
  pinMode(buttonPin2, INPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  buttonState1 = digitalRead(buttonPin2);
  Serial.print("Bandera:");
  Serial.print(bandera);
  if(lastButtonState != buttonState){
    if ((buttonState == HIGH) ||(buttonState1 == HIGH)) {
    Serial.print('Cambio');  
    bandera++;
    if(bandera>2){
      bandera=0;
    }
    Serial.println(bandera);
    Serial.println(buttonState1);
    delay(300);
    //lastButtonState = buttonState;
  }
  }
  if(bandera == 0){
    reloj();
    Serial.println("Hola");
  }
  if(bandera == 1){
    temperatura();
    Serial.println("Bien");
  }
  if(bandera == 2){
     hor=0;
     minus=0;
     secus=0;
     while (bandera == 2){
     cronometro();
     }
  }
  
} 
     
void writeByte(byte data, byte set)
{
  int d,c,l;
  if (set==1)
  {
    d = 2;
    c = 3;
    l = 4;
  }
  else if (set==2)
  {
    d = 8;
    c = 9;
    l = 10;
  }
  shiftOut(d, c, MSBFIRST, data);
  // toggle the latch pin so that the data appears as an output
  digitalWrite(l, HIGH);
  digitalWrite(l, LOW);
}

void reloj(){
  Serial.begin(57600);
  DateTime now = RTC.now();
  // All used for checking the time of the clock
  // This section can be removed when everything is working
  int mins = now.minute();
  int secs = now.second();
  int hr = now.hour();
  // convert to 12 hour time
  if (hr>12)
  {
    hr = hr-12;
  }

  // variables to describe pattern of on lights
  byte data1 = 0;
  byte data2 = 0;

  // encode the time
  // hr = 1st four bits controlled by the first shift register
  for (int i =0;i<4;i++)
  {
    if (bitRead(hr,i)==1)
    {
      bitWrite(data1,3-i,1);
      Serial.print(hr);
    }
  }
  // mins on the first shift register (last 4 leds)
  for (int i =2;i<6;i++)
  {
    if (bitRead(mins,i)==1)
    {
      bitWrite(data1,9-i,1);
    }
  }
  // mins on the second shift register (first 2 leds)
  for (int i =0;i<2;i++)
  {
    if (bitRead(mins,i)==1)
    {
      bitWrite(data2,1-i,1);
    }
  }
  // seconds, controlled by the second shift register (all 6 leds)
  for (int i =2;i<8;i++)
  {
    if (bitRead(secs,i-2)==1)
    {
      bitWrite(data2,9-i,1);
    }
  }
  // output the information
  Serial.println(mins);
  Serial.println(secs);
  writeByte(data1,1);
  writeByte(data2,2);
  // a pause every one second for the serial monitor output
  delay(1000);

}


void cronometro(){
 
  // All used for checking the time of the clock
  // This section can be removed when everything is working

  byte data1 = 0;
  byte data2 = 0;
  // encode the time
  // hr = 1st four bits controlled by the first shift register
  for (int i =0;i<4;i++)
  {
    if (bitRead(hor,i)==1)
    {
      bitWrite(data1,3-i,1);
      Serial.print(hor);
    }
  }
  // mins on the first shift register (last 4 leds)
  for (int i =2;i<6;i++)
  {
    if (bitRead(minus,i)==1)
    {
      bitWrite(data1,9-i,1);
    }
  }
  // mins on the second shift register (first 2 leds)
  for (int i =0;i<2;i++)
  {
    if (bitRead(minus,i)==1)
    {
      bitWrite(data2,1-i,1);
    }
  }
  // seconds, controlled by the second shift register (all 6 leds)
  for (int i =2;i<8;i++)
  {
    if (bitRead(secus,i-2)==1)
    {
      bitWrite(data2,9-i,1);
    }
  }
  Serial.println(minus);
  Serial.println(secus);
  writeByte(data1,1);
  writeByte(data2,2);
  // a pause every one second for the serial monitor output
  delay(1000);
  if (secus>58){
    minus= minus+1;
    secus=0;
  }
  if (minus > 58){
    hor=hor+1;
    minus=0;
  }
  if (hor>12){
    hor=0;
  }   
  secus= secus+1;
}

void temperatura(){
  delay(1000);
  const int temperaturePin = 0;
  Serial.begin(9600);
  float voltage;
  int degreesC;// degreesF;
  int dato1 = 0;
  int dato2 = 0;
  voltage = getVoltage(temperaturePin);
  degreesC = (voltage - 0.5) * 100.0;
  //degreesF = degreesC * (9.0/5.0) + 32.0;
 
  // mins on the first shift register (last 4 leds)
  for (int i =2;i<6;i++)
  {
    if (bitRead(degreesC,i)==1)
    {
      bitWrite(dato1,9-i,1);
    }
  }
  // mins on the second shift register (first 2 leds)
  for (int i =0;i<2;i++)
  {
    if (bitRead(degreesC,i)==1)
   
      bitWrite(dato2,1-i,1);
    }
  writeByte(dato1,1);
  writeByte(dato2,2);
  Serial.print("voltage: ");
  Serial.print(voltage);
  Serial.print("  deg C: ");
  Serial.print(degreesC);
 // Serial.print("  deg F: ");
 // Serial.println(degreesF);
  delay(1000);
}

float getVoltage(int pin)
{
  return (analogRead(pin) * 0.004882814); 
}




