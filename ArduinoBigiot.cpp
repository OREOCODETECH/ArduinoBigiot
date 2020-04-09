/*
  Copyright 2020 Li JiaYong

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ArduinoBigiot.h"

ArduinoBigiot::ArduinoBigiot() {
}

void ArduinoBigiot::empty() {
}

void ArduinoBigiot::begin(Client& espClient, String di, String ak) {
#ifdef IOT_DEBUG
  Serial.printf_P(PSTR("begin()\n"), iotdata.control.c_str());
#endif
  _client = &espClient;
  deviceId = di;
  apiKey = ak;
  _client->setTimeout(CLIENT_TIMEOUT);
#ifdef IOT_DEBUG
  Serial.printf_P(PSTR("exit begin()\n"), iotdata.control.c_str());
#endif
  for (int i = 0; i < FUN_NUMB; i++) {
    recallArr[i] = &empty;
  }
}

void ArduinoBigiot::checkin() {
  checkin(deviceId, apiKey);
}

void ArduinoBigiot::checkin(String di, String ak) {
  if (!(_client->connected())) {
    return;
  }
  _client->println((String)"{\"M\":\"checkin\",\"ID\":\"" + di + "\",\"K\":\"" + ak + "\"}");
}

void ArduinoBigiot::update(String val) {
  update(userId, val);
}

void ArduinoBigiot::update(String id, String val) {
  if (!(_client->connected())) {
    return;
  }
  _client->println((String)"{\"M\":\"update\",\"ID\":\"" + id + "\",\"V\":\"" + val + "\"}");
}

void ArduinoBigiot::say(String val) {
  say(userId, val);
}

void ArduinoBigiot::say(String id, String val) {
  if (!(_client->connected())) {
    return;
  }
#ifdef IOT_DEBUG
  Serial.printf_P(PSTR("send:%s\n"), ((String)"{\"M\":\"say\",\"ID\":\"" + id + "\",\"C\":\"" + val + "\"}").c_str());
#endif
  _client->println((String)"{\"M\":\"say\",\"ID\":\"" + id + "\",\"C\":\"" + val + "\"}");
}

void ArduinoBigiot::say_sign(String val, String sign) {
  say_sign(userId, val, sign);
}

void ArduinoBigiot::say_sign(String id, String val, String sign) {
  if (!(_client->connected())) {
    return;
  }
  _client->println((String)"{\"M\":\"say\",\"ID\":\"" + id + "\",\"C\":\"" + val + "\",\"SIGN\":\"" + sign + "\"");
}

void ArduinoBigiot::alert(String val) {
  alert(userId, val);
}

void ArduinoBigiot::alert(String id, String val) {
  if (!(_client->connected())) {
    return;
  }
  _client->println((String)"{\"M\":\"alert\",\"ID\":\"" + id + "\",\"C\":\"" + val + "\"}");
}

void ArduinoBigiot::checkout() {
  if (!(_client->connected())) {
    return;
  }
  _client->println((String)"{\"M\":\"checkout\",\"ID\":\"" + deviceId + "\",\"K\":\"" + apiKey + "\"}");
}

void ArduinoBigiot::setUserId(String id) {
  userId = id;
}

void ArduinoBigiot::heartBeat(int interval) {
  static long next = millis() + interval * 1000;
  if (millis() > next) {
    if (!(_client->connected())) {
      return;
    }
    next = millis() + interval * 1000;
#ifdef IOT_DEBUG
    Serial.printf_P(PSTR("heartBeat:%d\n"), millis());
#endif
    _client->println((String)"{\"M\":\"beat\"}");
  }
}

void ArduinoBigiot::handleiot() {
  heartBeat(HEARTBEAT_INTERVAL);
  if (!(_client->available())) {
    return;
  }
  iotdata.received = _client->readStringUntil('\n');
  iotdata.control = "";
#ifdef IOT_DEBUG
  Serial.printf_P(PSTR("data: % s\n"), iotdata.received.c_str());
#endif
  if (iotdata.received.indexOf("\"M\":\"say\"") == -1 || iotdata.received.indexOf("\"C\":\"") == -1) {
#ifdef IOT_DEBUG
    Serial.printf_P(PSTR("No control content found\n"));
#endif
    return;
  }
  unsigned int basic = iotdata.received.indexOf("\"C\":\"");
  iotdata.control = iotdata.received.substring(basic + 5, iotdata.received.indexOf("\"", basic + 5));
#ifdef IOT_DEBUG
  Serial.printf_P(PSTR("controlData:%s\n"), iotdata.control.c_str());
#endif
  (*recallArr[12])();
  if (iotdata.control == "offOn") {
    (*recallArr[0])();
  } else if (iotdata.control == "play") {
    (*recallArr[1])();
  } else if (iotdata.control == "stop") {
    (*recallArr[2])();
  } else if (iotdata.control == "minus") {
    (*recallArr[3])();
  } else if (iotdata.control == "up") {
    (*recallArr[4])();
  } else if (iotdata.control == "plus") {
    (*recallArr[5])();
  } else if (iotdata.control == "left") {
    (*recallArr[6])();
  } else if (iotdata.control == "pause") {
    (*recallArr[7])();
  } else if (iotdata.control == "right") {
    (*recallArr[8])();
  } else if (iotdata.control == "backward") {
    (*recallArr[9])();
  } else if (iotdata.control == "down") {
    (*recallArr[10])();
  } else if (iotdata.control == "forward") {
    (*recallArr[11])();
  } else {
    (*recallArr[13])();
  }
}

void ArduinoBigiot::bindData(void (*recall)()) {
#ifdef IOT_DEBUG
  Serial.printf_P(PSTR("bindData()\n"), iotdata.control.c_str());
#endif
  recallArr[12] = recall;
}

void ArduinoBigiot::bindNotFound(void (*recall)()) {
  recallArr[13] = recall;
}

void ArduinoBigiot::offOn(void (*recall)()) {
  recallArr[0] = recall;
}

void ArduinoBigiot::play(void (*recall)()) {
  recallArr[1] = recall;
}

void ArduinoBigiot::stop(void (*recall)()) {
  recallArr[2] = recall;
}

void ArduinoBigiot::minus(void (*recall)()) {
  recallArr[3] = recall;
}

void ArduinoBigiot::up(void (*recall)()) {
  recallArr[4] = recall;
}

void ArduinoBigiot::plus(void (*recall)()) {
  recallArr[5] = recall;
}

void ArduinoBigiot::left(void (*recall)()) {
  recallArr[6] = recall;
}

void ArduinoBigiot::pause(void (*recall)()) {
  recallArr[7] = recall;
}

void ArduinoBigiot::right(void (*recall)()) {
  recallArr[8] = recall;
}

void ArduinoBigiot::backward(void (*recall)()) {
  recallArr[9] = recall;
}

void ArduinoBigiot::down(void (*recall)()) {
  recallArr[10] = recall;
}

void ArduinoBigiot::forward(void (*recall)()) {
  recallArr[11] = recall;
}

String ArduinoBigiot::getData() {
  return iotdata.received;
}

String ArduinoBigiot::getContent() {
  return iotdata.control;
}
