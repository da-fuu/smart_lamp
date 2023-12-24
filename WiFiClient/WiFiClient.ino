#define lamp 4

#include <ESP8266WiFi.h>
#include <GyverTimer.h>

const String ans = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

byte lampstate = 0;
WiFiServer server(80);
GTimer Timer;


void setup() {
  WiFi.begin("WIFINAME", "PASSWORD");
//  Serial.begin(9600);
  pinMode(lamp, OUTPUT);
  digitalWrite(lamp, 0);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  server.begin();
//  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    if (Timer.isReady()) {
      digitalWrite(lamp, 0);
      lampstate = 0;
    }
    delay(1);
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  String req = client.readStringUntil('\r');
  client.flush();
  client.print(ans);
  req = req.substring(5, 8);
  byte data = req.toInt();
//  Serial.println(data);

  if (data == 0) {
    if (lampstate == 0) {
      digitalWrite(lamp, 1);
      lampstate = 1;
    }
    else {
      digitalWrite(lamp, 0);
      if (lampstate == 2) Timer.stop();
      lampstate = 0;
    }
  }
  else {
    digitalWrite(lamp, 1);
    Timer.setTimeout(data * 1000);
    lampstate = 2;
  }

  delay(1);
}
