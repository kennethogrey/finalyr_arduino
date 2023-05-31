#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <AltSoftSerial.h>
#include <RTClib.h>
#include <Wire.h>
#include <string.h>

// Global variables
int buttonState = 0;                 // Variable to store the button state
int alertStatus = 0;                 // Variable to store the alert status
unsigned long lastDebounceTime = 0;  // Variable to store the last debounce time
unsigned long debounceDelay = 100;   // Delay time for debounce
RTC_DS3231 rtc;
int device_id = 1;
SoftwareSerial gprsSerial(5, 6);
//GPS Module RX pin to Arduino 9
//GPS Module TX pin to Arduino 8
AltSoftSerial neogps;
// The TinyGPS++ object
TinyGPSPlus gps;
// Global variables to store latitude and longitude
String latitude, longitude, time, date;

void setup()
{
  Serial.begin(9600);
  neogps.begin(9600);
  gprsSerial.begin(9600); // The GPRS baud rate
  pinMode(2, INPUT);
  attachInterrupt(0, buttonPressed, CHANGE);
  Wire.begin();
  rtc.begin();
  if (rtc.lostPower())
  {
    Serial.println("RTC is not running! Setting the time...");

    // Set the RTC to the date and time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // Alternatively, you can set a specific date and time manually
  }
}

void loop()
{
  //Can take up to 60 seconds
    boolean newData = false;
    for (unsigned long start = millis(); millis() - start < 2000;){
      while (neogps.available()){
        if (gps.encode(neogps.read())){
          newData = true;
          break;
        }
      }
    }
  
    //If newData is true
    if(true){
      newData = false;
  
      latitude = String(gps.location.lat(), 8); // Latitude in degrees (double)
      longitude = String(gps.location.lng(), 8); // Longitude in degrees (double)
    }

    Serial.print("Latitude= "); 
      Serial.print(latitude);
      Serial.print(" Longitude= "); 
      Serial.println(longitude);

  // Add a delay to control the repetition rate
  delay(1000);
  // Adjust the delay time as needed
  DateTime now = rtc.now();
  time = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  date = String(now.day()) + "-" + String(now.month()) + "-" + String(now.year());
  Serial.print(F("Time: "));
  Serial.println(time);
  Serial.print(F("Date: "));
  Serial.println(date);
  Serial.println(alertStatus);
  delay(1000);

  if (gprsSerial.available())
  {
    Serial.write(gprsSerial.read());
  }

  // GPRS commands
  gprsSerial.println("AT");
  delay(1000);

  gprsSerial.println("AT+CPIN?");
  delay(1000);

  gprsSerial.println("AT+CREG?");
  delay(1000);

  gprsSerial.println("AT+CGATT?");
  delay(1000);

  gprsSerial.println("AT+CIPSHUT");
  delay(1000);

  gprsSerial.println("AT+CIPSTATUS");
  delay(2000);

  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);

  ShowSerialData();

  gprsSerial.println("AT+CSTT=\"internet\""); // Start task and setting the APN
  delay(1000);

  ShowSerialData();

  gprsSerial.println("AT+CIICR"); // Bring up wireless connection
  delay(3000);

  ShowSerialData();

  gprsSerial.println("AT+CIFSR"); // Get local IP address
  delay(2000);

  ShowSerialData();

  gprsSerial.println("AT+CIPSPRT=0");
  delay(3000);

  ShowSerialData();

  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\""); // Start up the connection
  delay(6000);

  ShowSerialData();

  gprsSerial.println("AT+CIPSEND"); // Begin send data to remote server
  delay(4000);
  ShowSerialData();

  // Construct the request string
  String str = "GET https://api.thingspeak.com/update?api_key=G4Y49K8IMI36URKX&field1="+
                String(device_id)+"&field2="+latitude+"&field3="+longitude+"&field4="+
                time+"&field5="+date+"&field6="+String(alertStatus);
  //String str = "GET https://api.thingspeak.com/update?api_key=G4Y49K8IMI36URKX&field1="+String(device_id)+"&field2="+latitude+"&field3="+ longitude+"&field4="+String(t)+"&field5="+String(alertStatus);
  Serial.println(str);
  gprsSerial.println(str); // Begin send data to remote server

  delay(4000);
  ShowSerialData();

  gprsSerial.println((char)26); // Sending
  delay(5000);                  // Waiting for reply, important! The time is based on the condition of the internet
  gprsSerial.println();

  ShowSerialData();

  gprsSerial.println("AT+CIPSHUT"); // Close the connection
  delay(100);
  ShowSerialData();
  alertStatus = 0;
}

void buttonPressed()
{
  // Check if enough time has passed since the last button press
  if ((millis() - lastDebounceTime) < debounceDelay)
  {
    return;
  }

  // Read the button state
  buttonState = digitalRead(2);

  // Check if the button is pressed
  if (buttonState == HIGH)
  {
    alertStatus = 1;
    Serial.println(alertStatus);
  }

  // Update the debounce time
  lastDebounceTime = millis();
}

void ShowSerialData()
{
  while (gprsSerial.available() != 0)
    Serial.write(gprsSerial.read());
  delay(5000);
}