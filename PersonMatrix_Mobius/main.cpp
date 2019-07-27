#include <Arduino.h>
#include <RedMP3.h>
#include <DS3231.h>
#include <SoftwareSerial.h>
#include "LedControl.h"
#include <IRstD.h>

#define MP3_RX 7
#define MP3_TX 8

DS3231  rtc(SDA, SCL);
LedControl lc=LedControl(12,11,10,1);
IRrecvstd irrecvstd(2);
decode_resultsstd res_std;
Time binar;
MP3 mp3(MP3_RX, MP3_TX);

int i=0;
unsigned long time1;
int photorez = A0;

int8_t index  = 0x01;//Первая песня на SD карте
int8_t volume = 0x10;//0~0x1e (регулируемый уровень громкости - 30)
int8_t voiceFolder = 0x02;
int8_t musicFolder = 0x01;
long randomy;

void setup(){
  lc.shutdown(0,false); lc.setIntensity(0,0); // яркость, второй аргумент 0-15
  Serial.begin(57600);
  rtc.begin(); rtc.setTime(16, 38, 0); // Установить время
  irrecvstd.enableIRInstd();
  mp3.setVolume(volume);
  randomSeed(analogRead(0)); randomy = random(2,6);
  delay(300); mp3.playWithFileName(voiceFolder,randomy); delay(300);
}

void state(){
  byte eye[8]={
    B00111100,B01111110,B11111111,B11100111,B11100111,B11111111,B01111110,B00111100};
  lc.setRow(0,0,eye[0]); lc.setRow(0,1,eye[1]); lc.setRow(0,2,eye[2]);
  lc.setRow(0,3,eye[3]); lc.setRow(0,4,eye[4]); lc.setRow(0,5,eye[5]);
  lc.setRow(0,6,eye[6]); lc.setRow(0,7,eye[7]);
  i++;
  if(binar.hour==16 && binar.min==38 && binar.sec==23){
    randomy = random(6,10);
    delay(300);
    mp3.playWithFileName(voiceFolder,randomy);
    delay(300);
  }
}

void close(){
  byte close1[8]={
    B00111100,B01111110,B11111111,B11100111,B11100111,B11111111,B01111110,B00111100};
  byte close2[8]={
    B00000000,B00111100,B11111111,B11100111,B11100111,B11111111,B00111100,B00000000};
  byte close3[8]={
    B00000000,B00000000,B00111100,B11100111,B11100111,B00111100,B00000000,B00000000};
  byte close4[8]={
    B00000000,B00000000,B00000000,B11000011,B00111100,B00000000,B00000000,B00000000};
  byte close5[8]={
    B00000000,B00000000,B00111100,B11100111,B11100111,B00111100,B00000000,B00000000};
  byte close6[8]={
    B00000000,B00111100,B11111111,B11100111,B11100111,B11111111,B00111100,B00000000};
  lc.setRow(0,0,close1[0]); lc.setRow(0,1,close1[1]); lc.setRow(0,2,close1[2]);
  lc.setRow(0,3,close1[3]); lc.setRow(0,4,close1[4]); lc.setRow(0,5,close1[5]);
  lc.setRow(0,6,close1[6]); lc.setRow(0,7,close1[7]);
  delay(50);
  lc.setRow(0,0,close2[0]); lc.setRow(0,1,close2[1]); lc.setRow(0,2,close2[2]);
  lc.setRow(0,3,close2[3]); lc.setRow(0,4,close2[4]); lc.setRow(0,5,close2[5]);
  lc.setRow(0,6,close2[6]); lc.setRow(0,7,close2[7]);
  delay(50);
  lc.setRow(0,0,close3[0]); lc.setRow(0,1,close3[1]); lc.setRow(0,2,close3[2]);
  lc.setRow(0,3,close3[3]); lc.setRow(0,4,close3[4]); lc.setRow(0,5,close3[5]);
  lc.setRow(0,6,close3[6]); lc.setRow(0,7,close3[7]);
  delay(50);
  lc.setRow(0,0,close4[0]); lc.setRow(0,1,close4[1]); lc.setRow(0,2,close4[2]);
  lc.setRow(0,3,close4[3]); lc.setRow(0,4,close4[4]); lc.setRow(0,5,close4[5]);
  lc.setRow(0,6,close4[6]); lc.setRow(0,7,close4[7]);
  delay(60);
  lc.setRow(0,0,close5[0]); lc.setRow(0,1,close5[1]); lc.setRow(0,2,close5[2]);
  lc.setRow(0,3,close5[3]); lc.setRow(0,4,close5[4]); lc.setRow(0,5,close5[5]);
  lc.setRow(0,6,close5[6]); lc.setRow(0,7,close5[7]);
  delay(50);
  lc.setRow(0,0,close6[0]); lc.setRow(0,1,close6[1]); lc.setRow(0,2,close6[2]);
  lc.setRow(0,3,close6[3]); lc.setRow(0,4,close6[4]); lc.setRow(0,5,close6[5]);
  lc.setRow(0,6,close6[6]); lc.setRow(0,7,close6[7]);
  delay(50);
  lc.setRow(0,0,close1[0]); lc.setRow(0,1,close1[1]); lc.setRow(0,2,close1[2]);
  lc.setRow(0,3,close1[3]); lc.setRow(0,4,close1[4]); lc.setRow(0,5,close1[5]);
  lc.setRow(0,6,close1[6]); lc.setRow(0,7,close1[7]);
  delay(50);
}

void BinarClock(){
      lc.clearDisplay(0);
      binar = rtc.getTime();
      time1 = millis();
    while(millis() - time1 < 8000){
//-----------------hours-----------------
    if(binar.hour==0){lc.setColumn(0,1,B00000000);lc.setColumn(0,2,B00000000);}
    if(binar.hour==1){lc.setColumn(0,1,B00000000);lc.setColumn(0,2,B00000100);}
    if(binar.hour==2){lc.setColumn(0,1,B00000000);lc.setColumn(0,2,B00001000);}
    if(binar.hour==3){lc.setColumn(0,1,B00000000);lc.setColumn(0,2,B00001100);}
    if(binar.hour==4){lc.setColumn(0,1,B00000000);lc.setColumn(0,2,B00010000);}
    if(binar.hour==5){lc.setColumn(0,1,B00000000);lc.setColumn(0,2,B00010100);}
    if(binar.hour==6){lc.setColumn(0,1,B00000000);lc.setColumn(0,2,B00011000);}
    if(binar.hour==7){lc.setColumn(0,1,B00000000);lc.setColumn(0,2,B00011100);}
    if(binar.hour==8){lc.setColumn(0,1,B00000000);lc.setColumn(0,2,B00100000);}
    if(binar.hour==9){lc.setColumn(0,1,B00000000);lc.setColumn(0,2,B00100100);}
    if(binar.hour==10){lc.setColumn(0,1,B00000100);lc.setColumn(0,2,B00000000);}
    if(binar.hour==11){lc.setColumn(0,1,B00000100);lc.setColumn(0,2,B00000100);}
    if(binar.hour==12){lc.setColumn(0,1,B00000100);lc.setColumn(0,2,B00001000);}
    if(binar.hour==13){lc.setColumn(0,1,B00000100);lc.setColumn(0,2,B00001100);}
    if(binar.hour==14){lc.setColumn(0,1,B00000100);lc.setColumn(0,2,B00010000);}
    if(binar.hour==15){lc.setColumn(0,1,B00000100);lc.setColumn(0,2,B00010100);}
    if(binar.hour==16){lc.setColumn(0,1,B00000100);lc.setColumn(0,2,B00011000);}
    if(binar.hour==17){lc.setColumn(0,1,B00000100);lc.setColumn(0,2,B00011100);}
    if(binar.hour==18){lc.setColumn(0,1,B00000100);lc.setColumn(0,2,B00100000);}
    if(binar.hour==19){lc.setColumn(0,1,B00000100);lc.setColumn(0,2,B00100100);}
    if(binar.hour==20){lc.setColumn(0,1,B00001000);lc.setColumn(0,2,B00000000);}
    if(binar.hour==21){lc.setColumn(0,1,B00001000);lc.setColumn(0,2,B00000100);}
    if(binar.hour==22){lc.setColumn(0,1,B00001000);lc.setColumn(0,2,B00001000);}
    if(binar.hour==23){lc.setColumn(0,1,B00001000);lc.setColumn(0,2,B00001100);}
//------------------min-------------------
    if(binar.min==0){lc.setColumn(0,5,B00000000);lc.setColumn(0,6,B00000000);}
    if(binar.min==1){lc.setColumn(0,5,B00000000);lc.setColumn(0,6,B00000100);}
    if(binar.min==2){lc.setColumn(0,5,B00000000);lc.setColumn(0,6,B00001000);}
    if(binar.min==3){lc.setColumn(0,5,B00000000);lc.setColumn(0,6,B00001100);}
    if(binar.min==4){lc.setColumn(0,5,B00000000);lc.setColumn(0,6,B00010000);}
    if(binar.min==5){lc.setColumn(0,5,B00000000);lc.setColumn(0,6,B00010100);}
    if(binar.min==6){lc.setColumn(0,5,B00000000);lc.setColumn(0,6,B00011000);}
    if(binar.min==7){lc.setColumn(0,5,B00000000);lc.setColumn(0,6,B00011100);}
    if(binar.min==8){lc.setColumn(0,5,B00000000);lc.setColumn(0,6,B00100000);}
    if(binar.min==9){lc.setColumn(0,5,B00000000);lc.setColumn(0,6,B00100100);}
    if(binar.min==10){lc.setColumn(0,5,B00000100);lc.setColumn(0,6,B00000000);}
    if(binar.min==11){lc.setColumn(0,5,B00000100);lc.setColumn(0,6,B00000100);}
    if(binar.min==12){lc.setColumn(0,5,B00000100);lc.setColumn(0,6,B00001000);}
    if(binar.min==13){lc.setColumn(0,5,B00000100);lc.setColumn(0,6,B00001100);}
    if(binar.min==14){lc.setColumn(0,5,B00000100);lc.setColumn(0,6,B00010000);}
    if(binar.min==15){lc.setColumn(0,5,B00000100);lc.setColumn(0,6,B00010100);}
    if(binar.min==16){lc.setColumn(0,5,B00000100);lc.setColumn(0,6,B00011000);}
    if(binar.min==17){lc.setColumn(0,5,B00000100);lc.setColumn(0,6,B00011100);}
    if(binar.min==18){lc.setColumn(0,5,B00000100);lc.setColumn(0,6,B00100000);}
    if(binar.min==19){lc.setColumn(0,5,B00000100);lc.setColumn(0,6,B00100100);}
    if(binar.min==20){lc.setColumn(0,5,B00001000);lc.setColumn(0,6,B00000000);}
    if(binar.min==21){lc.setColumn(0,5,B00001000);lc.setColumn(0,6,B00000100);}
    if(binar.min==22){lc.setColumn(0,5,B00001000);lc.setColumn(0,6,B00001000);}
    if(binar.min==23){lc.setColumn(0,5,B00001000);lc.setColumn(0,6,B00001100);}
    if(binar.min==24){lc.setColumn(0,5,B00001000);lc.setColumn(0,6,B00010000);}
    if(binar.min==25){lc.setColumn(0,5,B00001000);lc.setColumn(0,6,B00010100);}
    if(binar.min==26){lc.setColumn(0,5,B00001000);lc.setColumn(0,6,B00011000);}
    if(binar.min==27){lc.setColumn(0,5,B00001000);lc.setColumn(0,6,B00011100);}
    if(binar.min==28){lc.setColumn(0,5,B00001000);lc.setColumn(0,6,B00100000);}
    if(binar.min==29){lc.setColumn(0,5,B00001000);lc.setColumn(0,6,B00100100);}
    if(binar.min==30){lc.setColumn(0,5,B00001100);lc.setColumn(0,6,B00000000);}
    if(binar.min==31){lc.setColumn(0,5,B00001100);lc.setColumn(0,6,B00000100);}
    if(binar.min==32){lc.setColumn(0,5,B00001100);lc.setColumn(0,6,B00001000);}
    if(binar.min==33){lc.setColumn(0,5,B00001100);lc.setColumn(0,6,B00001100);}
    if(binar.min==34){lc.setColumn(0,5,B00001100);lc.setColumn(0,6,B00010000);}
    if(binar.min==35){lc.setColumn(0,5,B00001100);lc.setColumn(0,6,B00010100);}
    if(binar.min==36){lc.setColumn(0,5,B00001100);lc.setColumn(0,6,B00011000);}
    if(binar.min==37){lc.setColumn(0,5,B00001100);lc.setColumn(0,6,B00011100);}
    if(binar.min==38){lc.setColumn(0,5,B00001100);lc.setColumn(0,6,B00100000);}
    if(binar.min==39){lc.setColumn(0,5,B00001100);lc.setColumn(0,6,B00100100);}
    if(binar.min==40){lc.setColumn(0,5,B00010000);lc.setColumn(0,6,B00000000);}
    if(binar.min==41){lc.setColumn(0,5,B00010000);lc.setColumn(0,6,B00000100);}
    if(binar.min==42){lc.setColumn(0,5,B00010000);lc.setColumn(0,6,B00001000);}
    if(binar.min==43){lc.setColumn(0,5,B00010000);lc.setColumn(0,6,B00001100);}
    if(binar.min==44){lc.setColumn(0,5,B00010000);lc.setColumn(0,6,B00010000);}
    if(binar.min==45){lc.setColumn(0,5,B00010000);lc.setColumn(0,6,B00010100);}
    if(binar.min==46){lc.setColumn(0,5,B00010000);lc.setColumn(0,6,B00011000);}
    if(binar.min==47){lc.setColumn(0,5,B00010000);lc.setColumn(0,6,B00011100);}
    if(binar.min==48){lc.setColumn(0,5,B00010000);lc.setColumn(0,6,B00100000);}
    if(binar.min==49){lc.setColumn(0,5,B00010000);lc.setColumn(0,6,B00100100);}
    if(binar.min==50){lc.setColumn(0,5,B00010100);lc.setColumn(0,6,B00000000);}
    if(binar.min==51){lc.setColumn(0,5,B00010100);lc.setColumn(0,6,B00000100);}
    if(binar.min==52){lc.setColumn(0,5,B00010100);lc.setColumn(0,6,B00001000);}
    if(binar.min==53){lc.setColumn(0,5,B00010100);lc.setColumn(0,6,B00001100);}
    if(binar.min==54){lc.setColumn(0,5,B00010100);lc.setColumn(0,6,B00010000);}
    if(binar.min==55){lc.setColumn(0,5,B00010100);lc.setColumn(0,6,B00010100);}
    if(binar.min==56){lc.setColumn(0,5,B00010100);lc.setColumn(0,6,B00011000);}
    if(binar.min==57){lc.setColumn(0,5,B00010100);lc.setColumn(0,6,B00011100);}
    if(binar.min==58){lc.setColumn(0,5,B00010100);lc.setColumn(0,6,B00100000);}
    if(binar.min==59){lc.setColumn(0,5,B00010100);lc.setColumn(0,6,B00100100);}
//----------------------------------------
    }
}

void sleep(){
  byte sleep[8]={
    B00000000,B00000000,B00000000,B11000011,B00111100,B00000000,B00000000,B00000000};
  lc.setRow(0,0,sleep[0]); lc.setRow(0,1,sleep[1]); lc.setRow(0,2,sleep[2]);
  lc.setRow(0,3,sleep[3]); lc.setRow(0,4,sleep[4]); lc.setRow(0,5,sleep[5]);
  lc.setRow(0,6,sleep[6]); lc.setRow(0,7,sleep[7]);
  if (irrecvstd.decodestd(&res_std)){
     if(res_std.valuestd == 16750695){  // 0 - включить бинарные часы
           BinarClock();
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
      }
}

void music(){
  mp3.setVolume(volume);
  mp3.playWithVolume(index,volume);
}

void loop(){
  binar = rtc.getTime();
  int valrez = analogRead(photorez);
  if(analogRead(photorez)>=700){sleep();}else{state();}
  if(i==450){close();i=0;}
     // Serial.println(valrez);
  if (irrecvstd.decodestd(&res_std)){
      Serial.println(res_std.valuestd);
     if(res_std.valuestd == 16750695){  // 0 - включить бинарные часы
           BinarClock();
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
     if(res_std.valuestd == 16726215){  // OK - включить музыку
           music();
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
     if(res_std.valuestd == 16756815){  // # - выключить музыку
           mp3.pause();
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
     if(res_std.valuestd == 16734885){  // > - след. песня
           mp3.nextSong();
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
     if(res_std.valuestd == 16716015){  // < - пред. песня
           mp3.previousSong();
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
     if(res_std.valuestd == 16718055){  // - прибавить громкость
           mp3.volumeUp();
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
     if(res_std.valuestd == 16730805){  // - убавить громкость
           mp3.volumeDown();
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
//---------------Неиспользуемые-------------------------
      if(res_std.valuestd == 16753245){  // 1
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
      if(res_std.valuestd == 16736925){  // 2
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
      if(res_std.valuestd == 16769565){  // 3
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
      if(res_std.valuestd == 16720605){  // 4
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
      if(res_std.valuestd == 16712445){  // 5
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
      if(res_std.valuestd == 16761405){  // 6
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
      if(res_std.valuestd == 16769055){  // 7
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
      if(res_std.valuestd == 16754775){  // 8
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
      if(res_std.valuestd == 16748655){  // 9
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
      if(res_std.valuestd == 16738455){  // *
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
//------------------Длительные нажатия-----------------
      if(res_std.valuestd == 4294967295){  // длительное нажатие любой кнопки
        delay(300);
        mp3.playWithFileName(voiceFolder,17);
        delay(300);
      irrecvstd.resumestd();
      res_std.valuestd = 0;
         }
   }


}
