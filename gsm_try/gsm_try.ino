#include <SoftwareSerial.h>
SoftwareSerial gprsSerial(5, 6);

void setup()
{
  gprsSerial.begin(9600);
  Serial.begin(9600);

  Serial.println("Config SIM800L...");
  delay(2000);
  Serial.println("Done!...");

  // attach or detach from GPRS service
  gprsSerial.println("AT+CGATT?");
  delay(100);
  toSerial();

  // bearer settings
  gprsSerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(2000);
  toSerial();

  // bearer settings
  gprsSerial.println("AT+SAPBR=3,1,\"APN\",\"internet\"");
  delay(2000);
  toSerial();

  // bearer settings
  gprsSerial.println("AT+SAPBR=1,1");
  delay(2000);
  toSerial();
}

void loop()
{
  // initialize http service
  gprsSerial.println("AT+HTTPINIT");
  delay(2000);
  toSerial();

  // Set HTTP param value
  String domain = "https://gpsbse23-10.000webhostapp.com";
  String path = "/gpsdata.php";
  String lat = "111";
  String lng = "222";
  String time = "333";

  String url = domain + path + "?lat=" + lat + "&lng=" + lng + "&time=" + time;

  gprsSerial.print("AT+HTTPPARA=\"URL\",\"");
  gprsSerial.println(url);
  gprsSerial.println("\"");
  delay(2000);
  toSerial();

  // set http action type 0 = GET, 1 = POST, 2 = HEAD
  gprsSerial.println("AT+HTTPACTION=0");
  delay(10000);
  toSerial();

  // read server response
  gprsSerial.println("AT+HTTPREAD");
  delay(1000);
  toSerial();

  gprsSerial.println("");
  gprsSerial.println("AT+HTTPTERM");
  toSerial();
  delay(300);

  gprsSerial.println("");
  delay(10000);
}

void toSerial()
{
  while (gprsSerial.available() != 0)
  {
    Serial.write(gprsSerial.read());
  }
}
