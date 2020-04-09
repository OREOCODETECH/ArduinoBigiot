/*
  Copyright 2020 Li JiaYong

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <ESP8266WiFi.h>
#include "ArduinoBigiot.h"

#ifndef STASSID
#define STASSID "your-ssid"//Your WiFi SSID
#define STAPSK  "your-password"//Your WiFi Passord
#endif
String DEVICEID = "deviceid";//Your Device ID,view at https://www.bigiot.net/User/listDev.html
String APIKEY = "apikey";//Your Device APIKEY,view at https://www.bigiot.net/User/listDev.html
const char* host = "www.bigiot.net";//Host domain,default is www.bigiot.net
const uint16_t port = 8181;//Port
#define LED_PIN 2//Your LED PIN
const char* ssid     = STASSID;
const char* password = STAPSK;

WiFiClient client;
ArduinoBigiot iot;

void handlePlay() {//Create a function for command "play"
  digitalWrite(LED_PIN, LOW);//Turn the LED on (Note that LOW is the voltage level
  Serial.println("Nice day ! ( > v < )");
}

void handleStop() {//Create a function for command "stop"
  digitalWrite(LED_PIN, HIGH);//Turn the LED off by making the voltage HIGH
  Serial.println("Good night!( = v = )");
}

void handleOffOn() {//Create a function for command "offOn"
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));//Invert GPIO state
  Serial.println("  Switch ! ( O v O )");
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(LED_PIN, OUTPUT);//Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_PIN, HIGH);//Turn the LED off by making the voltage HIGH
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);//Set WiFi to station mode
  WiFi.begin(ssid, password);//Connect WiFi
  while (WiFi.status() != WL_CONNECTED) {//Wait until WiFi connected
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Connecting to ");
  Serial.print(host);
  Serial.print(":");
  Serial.println(port);
  while (!client.connect(host, port)) {//Connect host and wait until host connected
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Host connected");
  Serial.println("Iot services init");
  iot.begin(client, DEVICEID, APIKEY);//Bind client, DEVICEID, APIKEY
  iot.checkout();//Checkout device before checkin device
  iot.checkin();//Checkin device
  iot.offOn(handleOffOn);//Bind "offOn" command to "handleOffOn" function
  iot.play(handlePlay);//Bind "play" command to "handlePlay" function
  iot.stop(handleStop);//Bind "stop" command to "handleStop" function
  Serial.println("Iot services started");
}

void loop() {
  iot.handleiot();//Keep device alive and process data
  delay(50);
}
