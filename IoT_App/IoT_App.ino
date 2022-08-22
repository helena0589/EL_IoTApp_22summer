#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27,16,2); //2004 -> 1602

#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define mosfet 2
#define btn_on 3
#define btn_off 4
#define btn 7

int bluetoothTx = 8;
int bluetoothRx = 9;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

int humi_max = 75;
int humi_min = 50;

int led = 6;

unsigned long t = 0;
int a;
int b;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //아두이노의 결과를 PC에 출력하겠다!
  bluetooth.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(mosfet,OUTPUT); //모스펫핀을 출력으로 설정!
  pinMode(btn_on,INPUT); //아두이노에 내장된 저항을 이용해서 버튼을 활용하겠다!
  pinMode(btn_off,INPUT); //아두이노에 내장된 저항을 이용해서 버튼을 활용하겠다!
  pinMode(btn, INPUT);
  pinMode(led, OUTPUT);
  int a=0;
  int b=0;
}

void loop() {
 //블루투스 값 입력
 String inputString="";
 
 if(bluetooth.available()){
  inputString=bluetooth.readStringUntil('/n');
 }

 //가습기 자동 수동
 if(inputString=="auto_on"){
  a+=1;
 }
 else if(inputString=="auto_off"){
  a+=1;
 }
 
 //가습기 모듈 on/off
 if (inputString=="h_on"){
  b+=1;
 }
 else if(inputString=="h_off"){
  b+=1;
 }
  

  
  int avrPPM = averageAnalogRead(A0); 
  int co2ppm = map(avrPPM, 0, 1023, 400, 5000);
  Serial.println(co2ppm);
  delay(300);
  

  if(co2ppm > 2000){
    digitalWrite(led,HIGH);
  }
  else{
    digitalWrite(led,LOW);
  }

  if(digitalRead(btn) == HIGH){
    a+=1;
    Serial.println("모드 변경됨.");
  }

  if(a%2==0){
    Serial.println("수동모드");
    
    if(digitalRead(btn_on) == HIGH){
      //버튼이 눌려진것!
        b+=1;
     }
     if(digitalRead(btn_off) == HIGH){
        b+=1;
     }
    if(b%2==0){
      digitalWrite(mosfet,HIGH);
      Serial.println("가습기 켬");
    }
    else{
      digitalWrite(mosfet,LOW);
      Serial.println("가습기 끔");
     }        
   }
  

  
  else{
    Serial.println("자동모드");
    if(millis() - t > 1000){
       t = millis();
      //1초에 한번 온도와 습도를 측정하세요!
      float h = dht.readHumidity();
      float t = dht.readTemperature();

      Serial.print(h);
      Serial.print("\t");
      Serial.println(t);

      lcd.setCursor(0,0); //첫번째줄~
      lcd.print("HUMI=");
      lcd.print(h);
      lcd.print("%  ");
      lcd.setCursor(0,1); //두번째줄~
      lcd.print("TEMP=");
      lcd.print(t);
      lcd.print("'C  ");
   
      if(h > humi_max){
      Serial.println("습도가 충분해서 자동으로 OFF합니다");
      digitalWrite(mosfet,LOW);
         }
      else if(h <= humi_min){
      Serial.println("습도가 낮아서 자동으로 ON합니다");
      digitalWrite(mosfet,HIGH);
          }
      else{
      Serial.println("온습도 안정");
      digitalWrite(mosfet, LOW);//이 부분은 사용자가 수정해서 사용하게 할까..
     }
    }
  }

  delay(1000);
  
}


int averageAnalogRead(int pinToRead)
{
  byte count = 10;
  float value = 0;

  for( int x = 0;  x < count ; x++){
    value += analogRead(pinToRead);
    delay(20);
  }
  
  value = value/count;
  return(value);
}
