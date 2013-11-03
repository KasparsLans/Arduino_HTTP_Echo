/*
 This sketch echos distance to nearest surface using 
 HC-SR04 distance sensor and sends it using Arduino 
 Wiznet Ethernet shield to a webserver to be processed 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * HC-SR04's Echo and Trigger connected to Arduino PIN 11 and 12 respectively
 
 Created 03 Nov 2013
 by Kaspars Lāns
 */

// Import Ethernet and SPI libraries
#include <SPI.h>
#include <Ethernet.h>

// Define Echo and Trigger PIN constants
#define echoPin 11
#define trigPin 12


// Set controller's MAC address
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Set controller's static IP address to use if DHCP fails
// to assign IP address
IPAddress ip(192,168,0,120);

// Set web server's IP address. Comment this line
// and uncomment the following if you need to use
// DNS instead of IP
IPAddress server(192,168,0,101);
//char server[] = "my-domain.com";

// Initialize the Ethernet client
EthernetClient client;

// Setup routine which only runs once before loop()
void setup() {

  // Set the data rate in bits per second (baud) for serial data transmission
  Serial.begin(9600);

  // Wait for serial port to connect
  while (!Serial) {

  }

  // Set Echo PIN to digital input
  pinMode(echoPin, INPUT);

  // Set Trigger PIN to digital output
  pinMode(trigPin, OUTPUT);

  // Initiate the Ethernet connection using DHCP
  if (Ethernet.begin(mac) == 0) {

    // Initiate the Ethernet connection static IP
    // if DHCP IP assign failed
    Ethernet.begin(mac, ip);
  }

  // Wait a second for the Ethernet shield to initialize
  delay(1000);
}

// Main routine runs over and over again forever
void loop()
{
  // Define integers for saving duration and distance 
  int duration, distance;

  // Set the Trigger pin to HIGH starting ultrasonic wave
  digitalWrite(trigPin, HIGH);

  // Wait one second
  delayMicroseconds(1000);

  // Set the Trigger pin to LOW ending ultrasonic wave
  digitalWrite(trigPin, LOW);

  // Get the duration of the echo of the ultrasonic wave
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance to surface in centimeters
  distance = (duration / 2) / 29.1;

  // Process out of range case
  if (distance <= 0 || distance >= 200){

    // Sent 0 to web server
    send(0); 
  } 
  else {

    // Send distance to web server
    send(distance);
  }
  
  // Wait one second before next loop() routine
  delayMicroseconds(1000);
}

// Routine for sending distance to web server
void send(int distance) {

  //Serial.println("connecting...");

  // Connect to WEB server
  if (client.connect(server, 80)) {

    // Send distance value to input.php script using GET
    client.print("GET /input.php?measurument=");
    client.print(distance);
    client.println();

    // Send the HTTP version
    client.println("HTTP/1.1\r\n");

    // Send the HOST address. Comment this line
    // and uncomment the following if you need to use
    // DNS instead of IP
    client.println("Host: 192.168.0.101\r\n");
    //client.println("Host: my-domain.com\r\n");
    client.println();

    // Send User-Agent
    client.println("User-Agent: Arduino\r\n");

    // Finish sending HTTP headers
    client.println();

  } 
  else {
    // Process cases where the connection to
    // the web server could not be established
  }

  // Terminate web client
  client.stop();

}

