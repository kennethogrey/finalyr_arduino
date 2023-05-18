#include <SoftwareSerial.h>

SoftwareSerial sim800l(5, 6); // RX, TX pins

void setup() {
  Serial.begin(9600);
  sim800l.begin(9600);

  delay(1000);

  // Configure SIM800L as a HTTP client
  sim800l.println("AT+HTTPINIT");
  delay(1000);
  sim800l.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);
}

void loop() {
  // Your data to send
  String dataToSend = "Hello, World!";

  // Encode the data for URL transmission
  String urlEncodedData = urlEncode(dataToSend);

  // Set the target URL
  String url = "https://gpsbse23-10.000webhostapp.com/gpsdata?lat=";

  // Prepare the POST request
  String postRequest = "AT+HTTPPARA=\"URL\",\"" + url + "\"";
  sim800l.println(postRequest);
  delay(1000);

  // Set the POST data
  String postData = "AT+HTTPDATA=" + String(urlEncodedData.length()) + ",10000";
  sim800l.println(postData);
  delay(1000);
  sim800l.println(urlEncodedData);
  delay(1000);

  // Send the POST request
  sim800l.println("AT+HTTPACTION=1");
  delay(5000);

  // Read the response
  sim800l.println("AT+HTTPREAD");
  delay(1000);
  while (sim800l.available()) {
    char c = sim800l.read();
    Serial.print(c);
  }
  Serial.println();

  // End HTTP session
  sim800l.println("AT+HTTPTERM");
  delay(1000);

  delay(5000); // Wait before sending the next request
}

String urlEncode(String str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (uint16_t i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
    yield();
  }
  return encodedString;
}
