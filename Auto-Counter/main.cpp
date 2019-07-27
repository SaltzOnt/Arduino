#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
int echoPin=9;
int trigPin=8;
int duration, cm;
int num=0;
const int ON = 5;
const int OFF = 6;
int but1st = 0;
int but2st = 0;


void setup(){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ON, INPUT);
  pinMode(OFF, INPUT);
}

void loop(){
  but1st = digitalRead(ON);
  but2st = digitalRead(OFF);
    digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
  duration=pulseIn(echoPin, HIGH);
    cm=duration/58;
  Serial.println(cm);

  if(cm>6){
    num++;
    delay(1500); }
  if (but1st == HIGH) {
    num++;
    delay(500); }
  if (but2st == HIGH) {
    num--;
    delay(500); }

  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(48,0);
  display.setTextSize(5);
  display.print(num);
  display.display();
  delay(100);
}
