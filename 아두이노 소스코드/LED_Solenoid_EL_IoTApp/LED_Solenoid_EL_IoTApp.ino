/* 
 * 2022년도 EL 하계 프로젝트
 * IoT App조
 * 주제 : 스마트 책상 & 의자
 * 조원 : 조희우, 김신영, 김준호, 김진희, 김혜현
 * 
 * 코드 제작 : 조희우
 * 
 * 기능 :
 *  - LED 원격 제어
 *  - 솔레노이드 원격 제어 / 비밀번호 인증 기능
 *  - 솔레노이드 RFID 제어
 *  - 키 인증 성공시 솔레노이드 잠금해제, 초록색 LED 점등
 *  - 키 인증 실패시 부저울림, 붉은색 LED 점등
 *  
 * 사용 부품 : 
 *  - 보드 : Arduino R3
 *  - LED : 13mm 표준형 12V 방수 LED바
 *  - 솔레노이드 : ROB-15324
 *  - RFID : RC522 리더기 모듈
 *  - LED : red, green
 *  - 수동 부저
 *  - 릴레이모듈
*/

#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5 //define green LED pin
#define LED_R 4 //define red LED
#define SOLENOID 3 //SOLENOID pin
#define BUZZER 2 //buzzer pin
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

#define LED_BAR 6
String UID = "43 82 A6 09";
String inputString;
String PW = "1234";
String KEY;
int stringLen;

#define TX 7
#define RX 8
SoftwareSerial bluetooth(TX, RX);

void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  bluetooth.begin(9600);// Initiate Blutooth
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(SOLENOID, OUTPUT);
  pinMode(LED_BAR, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  digitalWrite(SOLENOID, LOW);
  digitalWrite(LED_BAR, LOW);
  
  Serial.println("Put your card to the reader...");
  Serial.println();
}
void loop()
{
  // 블루투스 값 입력
  inputString="";
  KEY="";
  if(bluetooth.available()){
    inputString=bluetooth.readStringUntil('\n');
    stringLen=inputString.length();
  }

  // 솔레노이드 블루투스
  if (inputString.endsWith("#")){
    KEY=inputString.substring(stringLen-5,stringLen-1);
    if(KEY==PW){
      Lock_off();
    }
    else{
      Lock_error();
    }
  }
  

  // 블루투스 스위치 on/off
  if(inputString=="on"){
    digitalWrite(LED_BAR, HIGH);
  }
  else if(inputString=="off"){
    digitalWrite(LED_BAR, LOW);
  }


  // 카드 인식
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  
  // UID 인식
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  // UID 출력
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();


  // UID 대조
  if (content.substring(1) == UID)
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    Lock_off();
  }
 
 else   {
    Serial.println(" Access denied");
    Lock_error();
  }  

}

void Lock_error(){
  digitalWrite(LED_R, HIGH);
  tone(BUZZER, 300);
  delay(DENIED_DELAY);
  digitalWrite(LED_R, LOW);
  noTone(BUZZER);
}
void Lock_off(){
  digitalWrite(SOLENOID, HIGH);
  digitalWrite(LED_G, HIGH);
  delay(ACCESS_DELAY);
  digitalWrite(SOLENOID, LOW);
  digitalWrite(LED_G, LOW);
}
