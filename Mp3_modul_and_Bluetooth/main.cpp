//RXD - TX, TXD - RX, 5v   --- Bluetooth modul
//RX - D7, TX - D8, 5v     --- MP3 modul

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "RedMP3.h"

#define MP3_RX 7
#define MP3_TX 8
MP3 mp3(MP3_RX, MP3_TX);

int val;

int8_t index  = 0x01;//Первая песня на SD карте
int8_t volume = 0x1a;//0~0x1e (регулируемый уровень громкости - 30)

void setup(){
  Serial.begin(9600);
  mp3.setVolume(volume);
  mp3.playWithVolume(index,volume);
}

void loop(){
  if (Serial.available()){
      val = Serial.read();

      if (val == '1'){
        mp3.playWithVolume(index,volume);}

      if (val == '3'){
        mp3.nextSong();}

      if (val == '2'){
        mp3.previousSong();}

      if (val == '4'){
        mp3.pause();}

      if (val == '5'){
        mp3.play();}

    }
}
