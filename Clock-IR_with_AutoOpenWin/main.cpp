#include <Arduino.h>
#include "CyberLib.h"
#include "TM1637.h"
#include <IRstD.h>
#include <SoftwareSerial.h>
#include "RedMP3.h"

#define CLK 11   // пин 11
#define DIO 12   // пин 12
#define ON 1
#define OFF 0
#define MAXMILLIS 4294967295
#define MP3_RX 7
#define MP3_TX 8

TM1637 tm1637(CLK,DIO);
IRrecvstd irrecvstd(4);
decode_resultsstd res_std;
MP3 mp3(MP3_RX, MP3_TX);

unsigned long timme;
unsigned long timelapsed = 0;
int chas = 0;
int minuta = 0;
int secunda = 0;

int8_t index  = 0x01;    //Первая песня на SD карте
int8_t volume = 0x1a;    //0~0x1e (регулируемый уровень громкости - 30)
const uint8_t In1 = 2;
const uint8_t In2 = 3;
unsigned long timer;

uint8_t B1_flag      = ReadEEPROM_Byte(1);
uint8_t B1chas_vkl   = ReadEEPROM_Byte(2);
uint8_t B1minut_vkl  = ReadEEPROM_Byte(3);
uint8_t val_shim     = ReadEEPROM_Byte(4);
uint8_t flag_disp    = 0;
uint8_t flag_zel_led = 0;

int buzzerPin            = 5;
int8_t TimeDisp[]        = {0x00,0x00,0x00,0x00};
int8_t BudDisp[]         = {0x00,0x00,0x00,0x00};
unsigned char ClockPoint = 1;


void setup(){
  tm1637.set(val_shim);
  tm1637.init();
  pinMode(buzzerPin, OUTPUT);
  D7_Out;
  D7_Low;
  D10_Out;

  if(B1_flag) D10_High;
  else D10_Low;
  Serial.begin(57600);
  irrecvstd.enableIRInstd();
  mp3.setVolume(volume);
}


void disp(){
     if(!flag_disp){
        ClockPoint = (~ClockPoint) & 0x01;

        if(ClockPoint)tm1637.point(POINT_ON);
        else tm1637.point(POINT_OFF);

        TimeDisp[0] = chas / 10;
        TimeDisp[1] = chas % 10;
        TimeDisp[2] = minuta / 10;
        TimeDisp[3] = minuta % 10;
        tm1637.display(TimeDisp);
      }else{
        tm1637.point(POINT_ON);
        BudDisp[0] = B1chas_vkl / 10;
        BudDisp[1] = B1chas_vkl % 10;
        BudDisp[2] = B1minut_vkl / 10;
        BudDisp[3] = B1minut_vkl % 10;
        tm1637.display(BudDisp);
      }
 }


void buzz(){
   if(B1_flag){
 /*    analogWrite(buzzerPin, 30);
      delay_ms(100);
      analogWrite(buzzerPin, 50);
      delay_ms(100);  */
      mp3.playWithVolume(index,volume);
      pinMode(In1, HIGH);
    }
   analogWrite(buzzerPin, 0);
   delay_ms(100);
 }

void wakeUp(){
  if(chas==19 && minuta==1){
    pinMode(In2, HIGH);
  }else{
    pinMode(In2, LOW);
  }
}


void loop(){
   if(irrecvstd.decodestd(&res_std)){
       if(res_std.valuestd == 16718055){ // Прибавление часов.Up
           analogWrite(buzzerPin, 255);
           chas++;
           if(chas > 23) chas = 0;
           disp();
           wakeUp();
           analogWrite(buzzerPin, 0);
         }

       if(res_std.valuestd == 16730805){ // Убавление часов.Down
           analogWrite(buzzerPin, 255);
           chas--;
           if(chas < 1) chas = 0;
           disp();
           wakeUp();
           analogWrite(buzzerPin, 0);
         }

       if(res_std.valuestd == 16734885){ // Прибавление минут.Right
           analogWrite(buzzerPin, 255);
           minuta++;
           if(minuta > 59) minuta = 0;
           disp();
           wakeUp();
           analogWrite(buzzerPin, 0);
         }

       if(res_std.valuestd == 16716015){ // Убавление минут.Left
           analogWrite(buzzerPin, 255);
           minuta--;
           if(minuta < 1 || minuta > 60) minuta = 0;
           disp();
           wakeUp();
           analogWrite(buzzerPin, 0);
         }

       if(flag_disp && res_std.valuestd == 16736925){ // Прибавление часов будильника.2
           analogWrite(buzzerPin, 255);
           B1chas_vkl++;
           if(B1chas_vkl > 23) B1chas_vkl = 0;
           WriteEEPROM_Byte(2, B1chas_vkl);
           disp();
           wakeUp();
           analogWrite(buzzerPin, 0);
         }

       if(flag_disp && res_std.valuestd == 16754775){ // Убавление часов будильника.8
           analogWrite(buzzerPin, 255);
           B1chas_vkl--;
           if(B1chas_vkl < 1 || B1chas_vkl > 24) B1chas_vkl = 0;
           WriteEEPROM_Byte(2, B1chas_vkl);
           disp();
           wakeUp();
           analogWrite(buzzerPin, 0);
         }

       if(flag_disp && res_std.valuestd == 16761405){ // Прибавление минут будильника.6
           analogWrite(buzzerPin, 255);
           B1minut_vkl++;
           if(B1minut_vkl > 59) B1minut_vkl = 0;
           WriteEEPROM_Byte(3, B1minut_vkl);
           disp();
           wakeUp();
           analogWrite(buzzerPin, 0);
         }

       if(flag_disp && res_std.valuestd == 16720605){ // Убавление минут будильника.4
           analogWrite(buzzerPin, 255);
           B1minut_vkl--;
           if(B1minut_vkl < 1 || B1minut_vkl > 60) B1minut_vkl = 0;
           WriteEEPROM_Byte(3, B1minut_vkl);
           disp();
           wakeUp();
           analogWrite(buzzerPin, 0);
         }


       if(res_std.valuestd == 16726215){ // Вкл/Откл будильник.OK
           analogWrite(buzzerPin, 255);
           B1chas_vkl = ReadEEPROM_Byte(2);
           B1minut_vkl = ReadEEPROM_Byte(3);

           if(B1_flag){
                B1_flag = 0;
                D10_Low;
                mp3.pause();
                index++;
                pinMode(In1, LOW);
                if(index>7){
                  index=0x01;
                }
             }else{
               B1_flag = 1;
               D10_High;
             }
             disp();
             wakeUp();
           WriteEEPROM_Byte(1, B1_flag);
           analogWrite(buzzerPin, 0);
         }


       if(res_std.valuestd == 16712445){ // Вывод будильника на дисплей.5
           analogWrite(buzzerPin, 255);
           flag_zel_led = 0;

           if(flag_disp){
                flag_disp = 0;
                D7_Low;
             }else{
               flag_disp = 1;
               D7_High;
             }
           disp();
           wakeUp();
           analogWrite(buzzerPin, 0);
         }


       if(res_std.valuestd == 16769565){ // Подсветка +.3
           analogWrite(buzzerPin, 255);
           val_shim++;
           if(val_shim > 7) val_shim = 7;
           WriteEEPROM_Byte(4, val_shim);
           tm1637.set(val_shim);
           disp();
           wakeUp();
           delay_ms(10);
           analogWrite(buzzerPin, 0);
         }

       if(res_std.valuestd == 16753245){ // Подсветка -.1
           analogWrite(buzzerPin, 255);
           val_shim--;
           if(val_shim < 1 || val_shim > 254) val_shim = 0;
           WriteEEPROM_Byte(4, val_shim);
           tm1637.set(val_shim);
           disp();
           wakeUp();
           delay_ms(10);
           analogWrite(buzzerPin, 0);
         }


  /*     if(res_std.valuestd == 551519865){ // +10 минут отсрочка
           analogWrite(buzzerPin, 255);
           B1minut_vkl = B1minut_vkl + 10;

           if(B1minut_vkl > 59){
                B1minut_vkl = 10;
                B1chas_vkl++;
                if(B1chas_vkl>23)  B1chas_vkl=0;
             }
           delay_ms(5);
           analogWrite(buzzerPin, 0);
           flag_disp = 1;
           D7_High;
           disp();
           delay_ms(300);
           flag_disp = 0;
           D7_Low;
         }           */

       Serial.println(res_std.valuestd); // Удалить после программирования пульта
       irrecvstd.resumestd();
       res_std.valuestd = 0;
    }

  unsigned long currtime = millis();
  if(currtime > timme) timelapsed = (currtime - timme);
  else timelapsed = (MAXMILLIS - timme + currtime);

  if(timelapsed > 999){
        timme = currtime;

        if(flag_disp){
            flag_zel_led++;
            if(flag_zel_led > 59){
                flag_zel_led = 0;
                flag_disp = 0;
                D7_Low;
                analogWrite(buzzerPin, 255);
                delay_ms(5);
                analogWrite(buzzerPin, 0);
              }
          }

        secunda++;
        if(secunda > 59){
           secunda=0;
           minuta++;

           if(minuta > 59){
               minuta=0;
               chas++;
               if(chas>23)  chas=0;
             }
         }


        if(chas == 0 && minuta == 0 && secunda == 10){
           Serial.print('T');
           Serial.print('\n');
           delay_ms(200);
         }
        disp();
        wakeUp();
     }


  if(B1_flag){
      if(chas == B1chas_vkl && minuta == B1minut_vkl){
           buzz();
        }

      if(chas == B1chas_vkl && minuta == B1minut_vkl && secunda == 59){
           B1minut_vkl = B1minut_vkl + 10;
           if(B1minut_vkl > 59){
                B1minut_vkl = 10;
                B1chas_vkl++;
                if(B1chas_vkl>23)  B1chas_vkl=0;
             }
        }
    }

  if(Serial.available()>4){
    byte descript[5];
    char ot_irserver[10] = {0,};

    if (Serial.read()=='Y'){
      for (byte i=0; i < 4; i++){
           descript[i] = Serial.read();
        }

    byte i = 0;

    if((descript[0] =='+') && (descript[1] =='=') && (descript[2] =='Z')){
      switch (descript[3]){
         case 'T':
         delay_ms(3);
         while(Serial.available()){
                  delay_ms(1);
                  ot_irserver[i] = Serial.read();

                  if(ot_irserver[i] == ':'){
                     ot_irserver[i] = ' ';
                   }

                  if(ot_irserver[i] == '\n'){
                     ot_irserver[i] = 0;
                     break;
                   }
                  i++;
               }

         sscanf(ot_irserver, "%d%d%d", &chas, &minuta, &secunda);

         for(int i=0; i < 64; i++){
            Serial.read();
          }

         break;
         default:
         break;
       }
     }else{
        for(byte i=0; i < 128; i++){
           Serial.read();
         }
      }
     }
   }
} //END LOOP
