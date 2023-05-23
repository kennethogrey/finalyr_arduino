#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <RTClib.h>
#include <Wire.h>

RTC_DS3231 rtc;
char t[32];

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial gpsSerial(RXPin, TXPin);

SoftwareSerial gsmSerial(5, 6);  // RX, TX

int buttonState;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 100;

void setup() {
  pinMode(2, INPUT);
  buttonState = digitalRead(2);
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
  gsmSerial.begin(9600);  // Setting the baud rate of GSM Module
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  //rtc.adjust(DateTime(2019, 1, 21, 5, 0, 0));
  attachInterrupt(0, buttonPressed, CHANGE);
}

void loop() {
  DateTime now = rtc.now();
  sprintf(t, "%02d:%02d %02d/%02d/%02d", now.hour(), now.minute(), now.day(), now.month(), now.year());
  
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
    if (gps.location.isUpdated()){
      Serial.print("Latitude= ");
      Serial.print(gps.location.lat(), 8);
      Serial.print(" Longitude= ");
      Serial.println(gps.location.lng(), 8);
    }
  }
  
  unsigned long currentMillis = millis();
  if (currentMillis - lastDebounceTime > debounceDelay) {
    lastDebounceTime = currentMillis;
    int newState = digitalRead(2);
    if(buttonState != newState){
      Serial.println("Button has been pressed");
      buttonState = newState;
      
      Serial.println("Attach to GPRS");
      gsmSerial.println("AT+CGATT=1\r");
      delay(2000);
      Serial.println("Setting the APN");
      gsmSerial.println("AT+CSTT=\"airtelgprs.com\"\r");
      delay(2000);
      Serial.println("Bringing up the wireless connection");
      gsmSerial.println("AT+CIICR\r");
      delay(8000);
      Serial.println("Getting local IP address");
      gsmSerial.println("AT+CPIN?");
      delay(5000);
      while (gsmSerial.available() > 0) {
        Serial.write(gsmSerial.read());
      }
      String lat = String(gps.location.lat(), 8);
      String lng = String(gps.location.lng(), 8);
      String time = String(t);
      //Serial.println("Sending data to website: " + lat " " + lng + " " + time);
      Serial.println("Sending data to website: " + lat + " " + lng + " " + time);
      //gsmSerial.println("AT+HTTPPARA=\"URL\",\"https://gpsbse23-10.000webhostapp.com/gpsdata.php?lat="+lat+"&lon="+lng+"&time="+time+"\r"");
      gsmSerial.println("AT+HTTPPARA=\"URL\",\"https://gpsbse23-10.000webhostapp.com/gpsdata.php?lat="+lat+"&lng="+lng+"&time="+time+"\"\r");
      delay(2000);
      gsmSerial.println("AT+HTTPACTION=0");
      delay(20000);
      while (gsmSerial.available() > 0) {
        Serial.write(gsmSerial.read());
      }
    }
  }
}

void buttonPressed(){
  // do nothing, the code is handled in the main loop
}
