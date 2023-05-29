#include <SoftwareSerial.h>
#include <RTClib.h>
#include <Wire.h>
#include <string.h>
#include <TinyGPS++.h>

// The TinyGPS++ object
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(4, 3);
RTC_DS3231 rtc;
char time[32];
SoftwareSerial gprsSerial(5, 6);
int buttonState = 0;

void setup() {
  pinMode(2, INPUT);
  attachInterrupt(0, buttonPressed, CHANGE);
  ss.begin(9600);
  Wire.begin();
  rtc.begin();
  if (rtc.lostPower()) {
    Serial.println("RTC is not running! Setting the time...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  gprsSerial.begin(9600);               // the GPRS baud rate
  Serial.begin(9600);    // the GPRS baud rate
  delay(1000);
}

void loop() {
  DateTime now = rtc.now();
  sprintf(time, "%02d:%02d:%02d %02d-%02d-%02d", now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());
  Serial.print(F("Date/Time: "));
  Serial.println(time);
  delay(1000);

  if (ss.available()) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {
      float longitude = gps.location.lng();
      float latitude = gps.location.lat();
      Serial.print("Latitude = ");
      Serial.print(latitude, 8);
      Serial.print(" Longitude = ");
      Serial.println(longitude, 8);
    }
  }

  if (gprsSerial.available()) {
    Serial.write(gprsSerial.read());
    executeCommands();
    sendData();
    closeConnection();
  }
}

void executeCommands() {
  sendCommand("AT",1000);
  sendCommand("AT+CPIN?",1000);
  sendCommand("AT+CREG?",1000);
  sendCommand("AT+CGATT?",1000);
  sendCommand("AT+CIPSHUT",1000);
  sendCommand("AT+CIPSTATUS",2000);
  sendCommand("AT+CIPMUX=0",2000);
  sendCommand("AT+CSTT=\"internet\"",1000);
  sendCommand("AT+CIICR",3000);
  sendCommand("AT+CIFSR",2000);
  sendCommand("AT+CIPSPRT=0",3000);
  sendCommand("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"",6000);
  sendCommand("AT+CIPSEND",4000);
}

void sendCommand(const char* command,int delayTime) {
  gprsSerial.println(command);
  delay(delayTime);
  ShowSerialData();
}

void sendData() {
  String str = "GET https://api.thingspeak.com/update?api_key=G4Y49K8IMI36URKX&field1=" +
               String(gps.location.lng()) + "&field2=" +
               String(gps.location.lat()) + "&field3=" +
               String(time) + "&field4=normal";
  Serial.println(str);
  gprsSerial.println(str);
  delay(4000);
  ShowSerialData();
   gprsSerial.println((char)26);
  delay(5000);
  gprsSerial.println();
  ShowSerialData();
}

void closeConnection() {
  gprsSerial.println("AT+CIPSHUT");
  delay(100);
  ShowSerialData();
}

void ShowSerialData() {
  while (gprsSerial.available() != 0)
    Serial.write(gprsSerial.read());
  delay(5000);
}

void buttonPressed() {
  buttonState = digitalRead(2);
  if (buttonState == 1) {
    delay(10);
    buttonState = digitalRead(2);
    if (buttonState == 0) {
      DateTime now = rtc.now();
      sprintf(time, "%02d:%02d:%02d %02d-%02d-%02d", now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());
      Serial.print(F("Date/Time: "));
      Serial.println(time);
      delay(1000);

      if (ss.available()) {
        gps.encode(ss.read());
        if (gps.location.isUpdated()) {
          float longitude = gps.location.lng();
          float latitude = gps.location.lat();
          Serial.print("Latitude = ");
          Serial.print(latitude, 8);
          Serial.print(" Longitude = ");
          Serial.println(longitude, 8);
        }
      }

      if (gprsSerial.available()) {
        Serial.write(gprsSerial.read());
        executeCommands();
        sendPanicData();
      }
    }
  } else {
    delay(100);
  }
}

void sendPanicData() {
  String str = "GET https://api.thingspeak.com/update?api_key=G4Y49K8IMI36URKX&field1=" +
                String(gps.location.lng()) + "&field2=" +
                String(gps.location.lat()) + "&field3=" +
                String(time) + "&field4=panic";
  Serial.println(str);
  gprsSerial.println(str);
  delay(4000);
  ShowSerialData();
  gprsSerial.println((char)26);
  delay(5000);
  gprsSerial.println();
  ShowSerialData();
}
