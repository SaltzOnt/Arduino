#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Nextion.h>
SoftwareSerial nextion(2, 3);// TX - 2   RX - 3
Nextion myNextion(nextion, 9600);
const uint8_t In1 = 8;
const uint8_t In2 = 9;

unsigned long time1;
unsigned long time2;
unsigned long time3;
int brightness = 20;
int bright;
int z1, z2, z3;
int water = A0;

void setup() {
    Serial.begin(9600);
    pinMode(A0, INPUT);
    digitalWrite(0, HIGH);
    myNextion.init();
    pinMode(In1, OUTPUT);
    pinMode(In2, OUTPUT);
    pinMode(In1, LOW);
    pinMode(In2, LOW);
    z1 = 50;
    z2 = 50;
    z3 = 50;
    int bright = map(brightness, z1, 1024, z2, z3);
    String dim = "dim=" + String(bright);
    myNextion.sendCommand(dim.c_str());
}

void(* resetFunc) (void) = 0;
// resetFunc();

  void Rain(){
    water = analogRead(A0);
    Serial.println(water);
    if(water > 300){
      resetFunc();
    }
    }


  void pA(){
      pinMode(In1, HIGH);
      time1 = millis();
    while(millis() - time1 < 1800000){     // 1 зона 30 минут
      loop();
      Rain();
    }
      pinMode(In1, LOW);
    }

  void pB(){
      pinMode(In1, HIGH);
      time1 = millis();
    while(millis() - time1 < 3600000){     // 1 зона 1 час
      loop();
      Rain();
    }
      pinMode(In1, LOW);
    }

  void p2(){                // pC - почему-то не воспринимается IDE, выводит строки сис. ошибок
      pinMode(In1, HIGH);
      time1 = millis();
    while(millis() - time1 < 7200000){     // 1 зона 2 часа
      loop();
      Rain();
    }
      pinMode(In1, LOW);
    }

/////////////////////////

  void pD(){
      pinMode(In2, HIGH);
      time2 = millis();
    while(millis() - time2 < 1800000){     // 2 зона 30 минут
      loop();
      Rain();
    }
      pinMode(In2, LOW);
    }

  void pE(){
      pinMode(In2, HIGH);
      time2 = millis();
    while(millis() - time2 < 3600000){     // 2 зона 1 час
      loop();
      Rain();
    }
      pinMode(In2, LOW);
    }

  void pF(){
      pinMode(In2, HIGH);
      time2 = millis();
    while(millis() - time2 < 7200000){     // 2 зона 2 час
      loop();
      Rain();
    }
      pinMode(In2, LOW);
    }

/////////////////////////

  void pG(){
      pinMode(In2, HIGH);
      pinMode(In1, HIGH);
      time3 = millis();
    while(millis() - time3 < 1800000){     // 3 зона 30 минут
      loop();
      Rain();
    }
      pinMode(In2, LOW);
      pinMode(In1, LOW);
    }

  void pH(){
      pinMode(In2, HIGH);
      pinMode(In1, HIGH);
      time3 = millis();
    while(millis() - time3 < 3600000){     // 3 зона 1 час
      loop();
      Rain();
    }
      pinMode(In2, LOW);
      pinMode(In1, LOW);
    }

  void pI(){
      pinMode(In2, HIGH);
      pinMode(In1, HIGH);
      time3 = millis();
    while(millis() - time3 < 7200000){     // 3 зона 2 час
      loop();
      Rain();
    }
      pinMode(In2, LOW);
      pinMode(In1, LOW);
    }

///////////По таймеру///////////

  void pJ(){
      pinMode(In1, HIGH);
      time3 = millis();
    while(millis() - time3 < 1800000){     // 1 зона 30 минут
      loop();
      Rain();
    }
      pinMode(In1, LOW);
    }

  void pK(){
      pinMode(In1, HIGH);
      time3 = millis();
    while(millis() - time3 < 3600000){     // 1 зона 1 час
      loop();
      Rain();
    }
      pinMode(In1, LOW);
    }

  void pL(){
      pinMode(In1, HIGH);
      time3 = millis();
    while(millis() - time3 < 7200000){     // 1 зона 2 часа
      loop();
      Rain();
    }
      pinMode(In1, LOW);
    }

////////////////////////

  void pM(){
      pinMode(In2, HIGH);
      time3 = millis();
    while(millis() - time3 < 1800000){     // 2 зона 30 минут
      loop();
      Rain();
    }
      pinMode(In2, LOW);
    }

  void pN(){
      pinMode(In2, HIGH);
      time3 = millis();
    while(millis() - time3 < 3600000){     // 2 зона 1 час
      loop();
      Rain();
    }
      pinMode(In2, LOW);
    }

  void pO(){
      pinMode(In2, HIGH);
      time3 = millis();
    while(millis() - time3 < 7200000){     // 2 зона 2 часа
      loop();
      Rain();
    }
      pinMode(In2, LOW);
    }

////////////////////////

  void pP(){
      pinMode(In1, HIGH);
      pinMode(In2, HIGH);
      time3 = millis();
    while(millis() - time3 < 1800000){     // 3 зона 30 минут
      loop();
      Rain();
    }
      pinMode(In1, LOW);
      pinMode(In2, LOW);
    }

  void pQ(){
      pinMode(In1, HIGH);
      pinMode(In2, HIGH);
      time3 = millis();
    while(millis() - time3 < 3600000){     // 3 зона 1 час
      loop();
      Rain();
    }
      pinMode(In1, LOW);
      pinMode(In2, LOW);
    }

  void pR(){
      pinMode(In1, HIGH);
      pinMode(In2, HIGH);
      time3 = millis();
    while(millis() - time3 < 7200000){     // 3 зона 2 часа
      loop();
      Rain();
    }
      pinMode(In1, LOW);
      pinMode(In2, LOW);
    }

///////////Конец по таймеру/////////////



void loop() {
  if(nextion.available()>0){
          String str;
          while(nextion.available()){
            str+=char(nextion.read()); delay(10);
          }
            for(int i=0; i<str.length(); i++){

///////////////////////////////////////////////

          // 30 минут 1 зона
                if(memcmp(&str[i],"pA" ,2)==0){
                      pA();
                  }
                if(memcmp(&str[i],"p1A" ,2)==0){  pinMode(In1, LOW);
                  }

          // 1 час 1 зона
                if(memcmp(&str[i],"pB" ,2)==0){
                      pB();
                  }
                if(memcmp(&str[i],"p2B" ,2)==0){  pinMode(In1, LOW);
                  }

          // 2 час 1 зона
                if(memcmp(&str[i],"pC" ,2)==0){
                      p2();
                  }
                if(memcmp(&str[i],"p3C" ,2)==0){  pinMode(In1, LOW);
                  }

///////////////////////////////////////////////

          /// 30 минут 2 зона
                if(memcmp(&str[i],"pD" ,2)==0){
                      pD();
                  }
                if(memcmp(&str[i],"p4D" ,2)==0){  pinMode(In2, LOW);
                  }

          /// 1 час 2 зона
                if(memcmp(&str[i],"pE" ,2)==0){
                      pE();
                  }
                if(memcmp(&str[i],"p5E" ,2)==0){  pinMode(In2, LOW);
                  }

          /// 2 час 2 зона
                if(memcmp(&str[i],"pF" ,2)==0){
                      pF();
                  }
                if(memcmp(&str[i],"p6F" ,2)==0){  pinMode(In2, LOW);
                  }

///////////////////////////////////////////////

          //// 30 минут 3 зона
                if(memcmp(&str[i],"pG" ,2)==0){
                      pG();
                  }
                if(memcmp(&str[i],"p7G" ,2)==0){  pinMode(In2, LOW); pinMode(In1, LOW);
                  }

          //// 1 час 3 зона
                if(memcmp(&str[i],"pH" ,2)==0){
                      pH();
                  }
                if(memcmp(&str[i],"p8H" ,2)==0){  pinMode(In2, LOW); pinMode(In1, LOW);
                  }

          //// 2 час 3 зона
                if(memcmp(&str[i],"pI" ,2)==0){
                      pI();
                  }
                if(memcmp(&str[i],"p9I" ,2)==0){  pinMode(In2, LOW); pinMode(In1, LOW);
                  }

//////////////////////////////////////////////
        // Конец простого полива //
//////////////////////////////////////////////
        // Управление яркостью //
                if(memcmp(&str[i],"p-" ,2)==0){
                    z1=20; z2=20; z3=20;
                    int bright = map(brightness, z1, 1024, z2, z3);
                    String dim = "dim=" + String(bright);
                    myNextion.sendCommand(dim.c_str());
                  }
                if(memcmp(&str[i],"p+" ,2)==0){
                    z1=100; z2=100; z3=100;
                    int bright = map(brightness, z1, 1024, z2, z3);
                    String dim = "dim=" + String(bright);
                    myNextion.sendCommand(dim.c_str());
                  }
             // Конец //

                // Сброс всех зон //
                if(memcmp(&str[i],"pZ" ,2)==0){
                      resetFunc();  }
                // Конец //

//////////Начало полива по таймеру//////////

          // 30 минут 1 зона
                if(memcmp(&str[i],"pJ" ,2)==0){
                      pJ();
                  }

          // 1 час 1 зона
                if(memcmp(&str[i],"pK" ,2)==0){
                      pK();
                  }

          // 2 час 1 зона
                if(memcmp(&str[i],"pL" ,2)==0){
                      pL();
                  }

//////////////////////////////////////////////

          // 30 минут 2 зона
                if(memcmp(&str[i],"pM" ,2)==0){
                      pM();
                  }

          // 1 час 2 зона
                if(memcmp(&str[i],"pN" ,2)==0){
                      pN();
                  }

          // 2 час 2 зона
                if(memcmp(&str[i],"pO" ,2)==0){
                      pO();
                  }

//////////////////////////////////////////////

          // 30 минут 3 зона
                if(memcmp(&str[i],"pP" ,2)==0){
                      pP();
                  }

          // 1 час 3 зона
                if(memcmp(&str[i],"pQ" ,2)==0){
                      pQ();
                  }

          // 2 час 3 зона
                if(memcmp(&str[i],"pR" ,2)==0){
                      pR();
                  }

//////////Конец полива по таймеру///////////


          }
      }

}
