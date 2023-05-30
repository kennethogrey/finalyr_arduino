#include <SoftwareSerial.h>

SoftwareSerial gprsSerial(5, 6);  // RX, TX pins

String ispName;  // Variable to store ISP name
String accountBalance; // Variable for account balance

void setup() {
  Serial.begin(9600);
  gprsSerial.begin(9600);
  delay(1000);
}

void loop() {
  // Read the response from the SIM800L module
  if (gprsSerial.available()) {
    // Send AT command to retrieve the ISP name
    gprsSerial.println("AT+COPS?");
    delay(1000);
    String response = gprsSerial.readString();
    // Check if the response contains the ISP name
    if (response.indexOf("+COPS:") != -1) {
      int startIndex = response.indexOf("\"") + 1;
      int endIndex = response.indexOf("\"", startIndex);
      // Extract the ISP name from the response
      ispName = response.substring(startIndex, endIndex);
      
      if (ispName == "AIRTEL-UGANDA") {
        // Send USSD code to check account balance for AIRTEL-UGANDA
        gprsSerial.println("AT+CUSD=1,\"*175*4#\",15");
      } else {
        // Send USSD code to check account balance for other ISPs
        gprsSerial.println("AT+CUSD=1,\"*131#\",15");
      }
      delay(1000);
      response = gprsSerial.readString();
      if (response.indexOf(F("+CUSD:")) != -1) {
        // Extract the account balance from the response
        int startIndex = response.indexOf(F("\"")) + 1;
        int endIndex = response.indexOf(F("\""), startIndex);
        accountBalance = response.substring(startIndex, endIndex);
        // Account balance is stored in the accountBalance variable
        Serial.print("Account Balance: ");
        Serial.println(accountBalance);
      }
    }
  }
}