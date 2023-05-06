#include <SoftwareSerial.h>
SoftwareSerial gsmSerial(5, 6);  // RX, TX
void setup() {
  gsmSerial.begin(9600);  // Setting the baud rate of GSM Module
  Serial.begin(9600);     // Setting the baud rate of Serial Monitor (Arduino)
  delay(1000);
  Serial.println("Preparing to ping");
  Ping();
}
void loop() {
}
void Ping() {
  Serial.println("Attach to GPRS");
  gsmSerial.println("AT+CGATT=1\r");
  delay(2000);
  Serial.println("Setting the APN");
  gsmSerial.println("AT+CSTT=\"internet\"\r");
  delay(2000);
  Serial.println("Bringing up the wireless connection");
  gsmSerial.println("AT+CIICR\r");
  delay(8000);
  Serial.println("Getting local IP address");
  gsmSerial.println("AT+CPIN?"); 
  delay(5000);
  //Print the response on the Serial Monitor
  while (gsmSerial.available() > 0) {
    Serial.write(gsmSerial.read());
  }
  Serial.println("Ping to Google");
  gsmSerial.println("AT+CIPPING=\"www.google.com\"\r");
  delay(5000);
  //Print the response on the Serial Monitor
  while (gsmSerial.available() > 0) {
    Serial.write(gsmSerial.read());
  }
}