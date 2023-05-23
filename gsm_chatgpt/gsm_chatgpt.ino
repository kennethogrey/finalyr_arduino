#include <SoftwareSerial.h>

// Set up the software serial connection
SoftwareSerial sim800(5, 6); // RX, TX

void setup() {
  // Start the hardware serial connection
  Serial.begin(9600);
  
  // Start the software serial connection
  sim800.begin(9600);
  
  // Wait for the SIM800 module to initialize
  delay(2000);
  
  // Check if the module is responding
  sendCommand("AT");
  
  // Check if SIM is registered on the network
  sendCommand("AT+CREG?");
  
  // Connect to the GPRS network
  sendCommand("AT+CGATT=1");
  
  // Close any previous TCP connections
  sendCommand("AT+CIPSHUT");
  
  // Set the single IP connection
  sendCommand("AT+CIPMUX=0");
  
  // Set up the APN for TCP connection
  sendCommand("AT+CSTT=\"airtelgprs.com\",\"\",\"\"");
  
  // Bring up the wireless connection
  sendCommand("AT+CIICR");
  
  // Get the local IP address of the modem
  sendCommand("AT+CIFSR");
  
  // Establish TCP/IP connection with the server
  sendCommand("AT+CIPSTART=\"TCP\",\"gpsbse23-10.000webhostapp.com\",\"80\"");
}

void loop() {
  // Send data to the server
  sendDataToServer("lat&lng=lng&time=time");
  delay(5000);
}

void sendCommand(String command) {
  sim800.println(command);
  delay(1000);
  
  while (sim800.available()) {
    Serial.write(sim800.read());
  }
  
  Serial.println();
}

void sendDataToServer(String data) {
  // Start data transmission
  sendCommand("AT+CIPSEND=50");
  
  // Construct the URL with data
  String url = "GET gpsbse23-10.000webhostapp.com/gpsdata.php?lat=" + data + " HTTP/1.1\r\n";
  url += "Host: gpsbse23-10.000webhostapp.com\r\n";
  url += "Connection: close\r\n\r\n";
  
  // Send the URL
  sim800.println(url);
  sim800.write(0x1A); // CTRL+Z to terminate the data
  
  delay(1000);
  
  while (sim800.available()) {
    Serial.write(sim800.read());
  }
  
  Serial.println();
}
