#include <RTClib.h>
#include <Wire.h>

RTC_DS3231 rtc;
char t[32];
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  //rtc.adjust(DateTime(2019, 1, 21, 5, 0, 0));
}
void loop()
{
  DateTime now = rtc.now();
  sprintf(t, "%02d:%02d %02d/%02d/%02d", now.hour(), now.minute(), now.day(), now.month(), now.year());  
  Serial.print(F("Date/Time: "));
  Serial.println(t);
  delay(1000);
}

/*

The above code sets up an Arduino sketch to read the current date and time from an RTC DS3231 module and print it to the serial monitor. 
Here's a brief explanation of the different parts of the code:

  - #include <RTClib.h> and #include <Wire.h>: These are the libraries used to interface with the RTC module and I2C communication.

  - RTC_DS3231 rtc;: This line creates an instance of the RTC_DS3231 class.

  char t[32];: This line declares a character array of size 32 to hold the formatted date and time string.

  - Serial.begin(9600);: This line initializes the serial communication with a baud rate of 9600.

  - Wire.begin();: This line initializes the I2C communication.

  - rtc.begin();: This line initializes the RTC module.

  - rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));: This line sets the RTC module to the current date and time of the computer at the time of uploading the sketch. 
  
    Alternatively, you could manually set the date and time using rtc.adjust(DateTime(year, month, day, hour, minute, second));

  - DateTime now = rtc.now();: This line reads the current date and time from the RTC module and assigns it to the now variable of type DateTime.

  - sprintf(t, "%02d:%02d %02d/%02d/%02d", now.hour(), now.minute(), now.day(), now.month(), now.year());: This line formats the date and time values into a string using the sprintf() function and stores it in the t variable.

  - Serial.print(F("Date/Time: "));: This line prints the label "Date/Time: " to the serial monitor.

  - Serial.println(t);: This line prints the formatted date and time string to the serial monitor and adds a new line.

  - delay(1000);: This line introduces a delay of 1 second between each reading of the date and time.

Overall, this code demonstrates how to interface with an RTC module to read the current date and time and format it into a string for display on the serial monitor.
*/