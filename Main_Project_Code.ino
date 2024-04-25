#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLI9NslMKt"
#define BLYNK_DEVICE_NAME "pROJECT"
#define BLYNK_AUTH_TOKEN "Wz2JD3dvWsqCvi9fPCBg-GBkIpyUBDY2"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <MFRC522.h>
#include <Servo.h>

int buzzer = D2;
bool isBuzzerOn = true;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Souvik";
char pass[] = "aaaallll";

constexpr uint8_t RST_PIN = 16;
constexpr uint8_t SDA_PIN = 15;

byte readCard[4];
String tagID = "";

MFRC522 mfrc522(SDA_PIN, RST_PIN);

Servo myservo;

BLYNK_WRITE(V0)
{   
  int value = param.asInt();
  Serial.println(value);
  myservo.write(value);
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  myservo.attach(0);
  Blynk.begin(auth, ssid, pass);
}

void loop() 
{
  Blynk.run();
  delay(5000);
  myservo.write(0);
  
  while (getUID()) 
  {
    if (tagID == "C1F03D24" || tagID == "4151B126") 
    { 
      Serial.println("Authorized");
      noTone(buzzer);
      myservo.write(140);
      delay(4000); 
      myservo.write(0);
    }
    else
    {
      Serial.println("Unauthorized!");
      myservo.write(0);
      tone(buzzer, HIGH);
      delay(2500);
    } 
    delay(2000);
  }
}

boolean getUID() 
{
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  
  if (!mfrc522.PICC_ReadCardSerial()) {
    return false;
  }
  
  tagID = "";
  for (uint8_t i = 0; i < 4; i++) {
    tagID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tagID.toUpperCase();
  
  String content= "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
    if (i < mfrc522.uid.size - 1) {
      content += "-";
    }
  }
  content.toUpperCase();
  Serial.println("UID tag: " + content);
  
  mfrc522.PICC_HaltA();
  return true;
}
