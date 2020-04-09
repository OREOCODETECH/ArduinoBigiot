/*
  Copyright 2020 Li JiaYong

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef ArduinoBigiot_H
#define ArduinoBigiot_H
//#define IOT_DEBUG
#define HEARTBEAT_INTERVAL 50
#define CLIENT_TIMEOUT 100
#include "Arduino.h"
#include "ESP8266WiFi.h"
#define FUN_NUMB 14
class ArduinoBigiot {
  public:
    ArduinoBigiot();
    void begin(Client& espClient, String di, String ak);
    void checkin();
    void checkin(String di, String ak);
    void update(String val);
    void update(String id, String val);
    void say(String val);
    void say(String id, String val);
    void say_sign(String val, String sign);
    void say_sign(String id, String val, String sign);
    void alert(String val);
    void alert(String id, String val);
    void checkout();
    void setUserId(String id);
    void heartBeat(int interval);
    void handleiot();
    void bindData(void (*recall)());
    void bindNotFound(void (*recall)());
    void offOn(void (*recall)());
    void play(void (*recall)());
    void stop(void (*recall)());
    void minus(void (*recall)());
    void up(void (*recall)());
    void plus(void (*recall)());
    void left(void (*recall)());
    void pause(void (*recall)());
    void right(void (*recall)());
    void backward(void (*recall)());
    void down(void (*recall)());
    void forward(void (*recall)());
    String getData();
    String getContent();
    struct {
      String received;
      String control;
    } iotdata = {"", ""};
  private:
    String userId = "ALL";
    static void empty();
    void (*recallArr[FUN_NUMB])();
    Client* _client;
    String deviceId, apiKey;
};
#endif
