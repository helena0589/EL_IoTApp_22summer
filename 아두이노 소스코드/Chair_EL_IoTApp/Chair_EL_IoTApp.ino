/* 
 * 2022년도 EL 하계 프로젝트
 * IoT App조
 * 주제 : 스마트 책상 & 의자
 * 조원 : 조희우, 김신영, 김준호, 김진희, 김혜현
 * 
 * 코드 : 의자 압력센서
 * 기능 : 의자의 압력치 측정
 * 코드 제작 : 김혜현
 * 사용 부품 : 
 *  - 보드 : Arduino R3
 *  - 압력센서 : FSR 406 Tabs [30-73258]
*/

#include <DS1302.h>
int FSR_Pin0 = A0; //analog pin 0 다리 왼쪽_엉덩이
int FSR_Pin1 = A1; //analog pin 1 다리 왼쪽_허벅지
int FSR_Pin2 = A2; //analog pin 2 다리 오른쪽_엉덩이
int FSR_Pin3 = A3; //analog pin 3 다리 오른쪽_허벅지

//시간
unsigned long timeVal;
unsigned int Read_Value = 0;
int sec;
int min;
int hour;
int second;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int FSRReading0 = analogRead(FSR_Pin0);
  int FSRReading1 = analogRead(FSR_Pin1);
  int FSRReading2 = analogRead(FSR_Pin2);
  int FSRReading3 = analogRead(FSR_Pin3);

  Serial.print("왼엉 : ");
  Serial.print(FSRReading0);
  Serial.print(" 왼허 : ");
  Serial.print(FSRReading1);
  Serial.print(" 오엉 : ");
  Serial.print(FSRReading2);
  Serial.print(" 오허 : ");
  Serial.println(FSRReading3);
  delay(500);
  if(FSRReading0 > 0 || FSRReading1 > 0 || FSRReading2 > 0 || FSRReading3 > 0){
    //4개의 센서 중 하나라도 압력이 가해졌을 경우 시간 측정
    if(millis()-timeVal>=1000){
      sec++; //1초증가
      timeVal = millis();
      if(sec >= 60){
        min++; //1분 증가
        sec = 0; //초 초기화
        if(min >= 60){
          hour++; //1시간 증가
          min = 0; //분 초기화
        }
      }
      Serial.print(hour);
      Serial.print(" 시 ");
      Serial.print(min);
      Serial.print(" 분 ");
      Serial.print(sec);
      Serial.println(" 초 ");
    }
  }
  else if(FSRReading0 <= 0 || FSRReading1 <= 0 || FSRReading2 <= 0 || FSRReading3 <= 0){
    if(hour > 0 || min > 0 || sec > 0){
      if(millis()-timeVal>=1000){
       second++; //1초증가
        timeVal = millis();
        if(second > 3){
         hour = 0;
         min = 0;
         sec = 0;
        }
        Serial.print(hour);
        Serial.print(" 시 ");
        Serial.print(min);
        Serial.print(" 분 ");
        Serial.print(sec);
        Serial.println(" 초 ");
      }      
    }
  }    
  delay(500);
}
