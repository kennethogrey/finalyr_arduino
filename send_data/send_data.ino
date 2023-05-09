#include <SoftwareSerial.h>
SoftwareSerial gsm(5,6)// RX, TX
void setup() {
  gsm.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}
