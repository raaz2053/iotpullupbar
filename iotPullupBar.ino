const int sensorOut = A0; // Pin A0 to read analog input
int pressure; //To store analog value
const int trigPin = 2;  //D4
const int echoPin = 0;  //D3
long duration;
const int target = 10;
int distance;
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
const char *ssid = "IOTpullupBar";
const char *password = "makemefit";
ESP8266WebServer server(80);
int count;
#define Time 5
String condition;
#define maxd 50
#define mind 20
//#define condition "accomplished"
boolean flag;
void handleRoot() {

  server.send(200, "text/html", prepareHtmlPage());

}

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
  pressure = analogRead(sensorOut);  //value from Force Sensitive Resistance
  Serial.println(pressure);          //Print value
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  //Serial.print("Distance: ");
  //Serial.println(distance);
  delay(100);
  Serial.print("Count: \n");
  if (distance < mind) {
    if (flag == 0 && pressure > 500) {
      flag = 1;
      count = count + 1;
      delay(10);
    }
  }
  else if (distance > maxd) {
    flag = 0;
    delay(100);
  }
  if (count >= target) {
    condition = "accomplished. Congratulations";
  }
  else if (count < 5) {
    condition = "not accomplished. Try harder";
  }
  Serial.println(count);

  server.handleClient();

}

String prepareHtmlPage()
{
  String htmlPage =
    String("HTTP/1.1 200 OK\r\n") +
    "Content-Type: text/html\r\n" +
    "Connection: close\r\n" +  // the connection will be closed after completion of the response
    "Refresh: 5\r\n" +  // refresh the page automatically every 5 sec
    "\r\n" +
    "<h1>IOTpullupBar</h1>"
    "<!DOCTYPE HTML>" +
    "<html>" +
    "<title>" +
    "IOT pullup Bar" +
    "</title>" +
    "<body>" +
    "<p>" +
    "Workout count:  " + String(count) +
    "\r\n" +
    "</p>" +
    "Calories Burnt:" + String(count + 3) +
    "\r\n" +
    "<p>" +
    "Workout time in minutes:" + String(Time) +
    "<p>" +
    "Targeted goal is " + String(condition) +
    "."
    "</p>" +
    "</body>" +
    "</html>" +
    "\r\n";
  return htmlPage;
}
