#include <Arduino.h>
#include <EEPROM.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

#define BIP 11
#define RELAY 12

char keys[ROWS][COLS]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}};

byte rowPins[ROWS] = {5, 4, 3, 2};                                              // пины подключенных строк
byte colPins[COLS] = {9, 8, 7, 6};                                              // пины подключенных столбцов

Keypad keypad=Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
String pass;                                                                    //код по умолчанию
String summ;                                                                    //введенный код
int wrong=0;                                                                    //количество ошибочых вводов кода (для блокировки)
int shet=0;                                                                     //количество набранных символов
int shetch=0;                                                                   //количество набранных символов для смены пароля
int change=0;                                                                   //флаг проверки кода для смены пароля
int dochange=0;                                                                 //флаг производится смена пароля

void setup(){
   Serial.begin(9600);
   pinMode(BIP, OUTPUT);
   pinMode(RELAY, OUTPUT);
   digitalWrite(RELAY, HIGH);
                                                                                //только при первоначальной прошивке чтобы задать пароль
   EEPROM.write(1, '0');
   EEPROM.write(2, '0');
   EEPROM.write(3, '0');
   EEPROM.write(4, '0');
                                                                                //присваиваем паролю значения из энергонезависимой памяти
   pass = "";
   pass = pass+char(EEPROM.read(1));
   pass = pass+char(EEPROM.read(2));
   pass = pass+char(EEPROM.read(3));
   pass = pass+char(EEPROM.read(4));
                                                                                //эти строки нужны только для отладки
   Serial.println(char(EEPROM.read(1)));                                        //проговариваем первый знак пароля в порт
   Serial.println(char(EEPROM.read(2)));                                        //проговариваем второй знак пароля в порт
   Serial.println(char(EEPROM.read(3)));                                        //проговариваем третй знак пароля в порт
   Serial.println(char(EEPROM.read(4)));                                        //проговариваем четвертый знак пароля в порт
   Serial.print("Pass "); Serial.println(pass);                                 //проговариваем пароль в порт
}

void loop(){
   char key = keypad.getKey();
  if(key){
   Serial.println(key);
     tone(BIP, 400); delay(60); noTone(BIP);
     shet=shet+1;                                                               //увеличиваем на единицу счет количества символов
      summ=summ+key;
      Serial.print("Pass "); Serial.println(summ);
     switch(dochange){                                                          //ветвление на ввод нового пароля (case 1) и остальной код (case 0)
    case 0:                                                                     //блок для обычной работы - проверка правильности ввода пароля
  if(key=='#'){
    summ="";
    shet=0;
    Serial.println("# for RESET");
    tone(BIP, 300); delay(120); noTone(BIP);
  };

  if(key=='*'){
    summ="";
    shet=0;
    Serial.println("* for ENTER");
    tone(BIP, 700); delay(120); noTone(BIP);
  };

  if(summ==pass  && change==0){                                                 //если правильный код и не было запроса на смену кода
    Serial.println("PASS OK");
    summ="";
    shet=0;
    wrong=0;
    tone(BIP, 1200); delay(100); tone(BIP, 800); delay(100); tone(BIP, 1200); delay(100); tone(BIP, 1600); noTone(BIP);
    digitalWrite(RELAY, LOW);
    delay(15000);                                                               //время пока открыто реле
    digitalWrite(RELAY, HIGH);
  };

  if (summ=="ABCD"){                                                            //комбинация для смены кода
    Serial.println("Change pass go test");
    summ="";
    shet=0;
    wrong=0;
    change=1;
    tone(BIP, 400); delay(100); tone(BIP, 800); delay(100); tone(BIP, 400); delay(100); tone(BIP, 800); noTone(BIP);
  };

  if (summ==pass  && change==1){                                                //если правильный код и был запрос на смену кода
    Serial.println("Pass ok go change pass");
    summ="";
    shet=0;
    wrong=0;
    dochange=1;
    key = keypad.getKey();
    tone(BIP, 1200); delay(100); tone(BIP, 800); delay(100); tone(BIP, 1200); delay(100); tone(BIP, 1200); noTone(BIP);
  };

  if (wrong==0 && shet==4  && change==1){                                       //для смены пароля - если введено полное количество знаков кода и он ошибочный формируем звуковой сигнал
    summ="";
    shet=0;
    wrong=wrong+1;
    Serial.println("Wrong_Pass");
    tone(BIP, 400); delay(500); noTone(BIP);
  };

  if (wrong==1 && shet==4  && change==1){                                       //для смены пароля - если два раза ошибочный код формируем два звуковых сигнала
    summ="";
    shet=0;
    wrong=wrong+1;
    Serial.println("Attention!_2xWrong_Pass for change pas");
    tone(BIP, 400); delay(500); tone(BIP, 400); delay(500); noTone(BIP);
  };

  if (wrong==2 && shet==4  && change==1){                                       //для смены пароля - если три раза ошибочный код выходим из режима смены пароля
    summ="";
    shet=0;
    wrong=0;
    change=0;
    Serial.println("Attention!_3xWrong_Pass for change pas - cancel change pass");
    tone(BIP, 400); delay(500); tone(BIP, 400); delay(500); tone(BIP, 400); delay(500); noTone(BIP);
  };

  if (shet==4  && change==0){                                                   //если введено полное количество знаков кода и он ошибочный - формируем звуковой сигнал
    summ="";
    shet=0;
    wrong=wrong+1;
    Serial.println("Wrong_Pass");
    tone(BIP, 200); delay(500); tone(BIP, 200); delay(500); noTone(BIP);
  };

  if (wrong==2  &&  shet==0  && change==0){                                     //если два раза ошибочный код формируем два звуковых сигнала
    Serial.println("Attention!_2xWrong_Pass");
    tone(BIP, 200); delay(500); tone(BIP, 200); delay(500); tone(BIP, 200); delay(500); noTone(BIP);
  };

  if (wrong==3  &&  shet==0  && change==0){                                     //если три раза ошибочный код - формируем три звуковых сигнала
    Serial.println("Attention!_3xWrong_Pass");
    tone(BIP, 200); delay(500); tone(BIP, 200); delay(500); tone(BIP, 200); delay(500); tone(BIP, 200); delay(500); noTone(BIP);
  };

  if (wrong==4  && change==0){                                                  //если четыре реза ошибочный код блокируем замок на 5 секунд
    summ="";
    shet=0;
    wrong=0;
    Serial.println("Block!_4xWrong_Pass");
    tone(BIP, 200); delay(500); tone(BIP, 200); delay(500); tone(BIP, 200); delay(500); tone(BIP, 200); delay(500); tone(BIP, 200); delay(30000); noTone(BIP);
   };
  break;

  case 1:                                                                       //ВТОРАЯ ЧАСТЬ блок для смены пароля || начало блока для смены пароля и его записи в энергонезависимую память
    if (key=='#'){                                                              //если введен символ # сбрасываем код
    shetch=0;
    summ="";
    Serial.println("# is not an option, reset");
    tone(BIP, 200); delay(500); noTone(BIP);
  }

  else if (key=='*'){                                                           //если введен символ * сбрасываем код
    shetch=0;
    summ="";
    Serial.println("* is not an option, reset");
    tone(BIP, 200); delay(500); noTone(BIP);
  }

  else if (shetch==0 && (key)){                                                 //начинаем менять пароль 1й символ
    Serial.print("NewPass_symbol_one "); Serial.println(key);
    tone(BIP, 1400); delay(500); noTone(BIP);
    shetch=1;                                                                   //увеличиваем на единицу счет количества символов нового пароля
    EEPROM.write(1, key);
    Serial.println(char(EEPROM.read(1)));                                       //проговариваем второй знак пароля в порт
 }

  else if (shetch==1 && (key)){                                                 //начинаем менять пароль 2й символ
    Serial.print("NewPass_symbol_two "); Serial.println(key);
    EEPROM.write(2, key);
    tone(BIP, 1400); delay(500); noTone(BIP);
    shetch=2;                                                                   //увеличиваем на единицу счет количества символов нового пароля
    Serial.println(char(EEPROM.read(2)));                                       //проговариваем первый знак пароля в порт
 }

  else if (shetch==2 && (key)){                                                 //начинаем менять пароль 3й символ
    Serial.print("NewPass_symbol_three "); Serial.println(key);
    EEPROM.write(3, key);
    tone(BIP, 1400); delay(500); noTone(BIP);
    shetch=3;                                                                   //увеличиваем на единицу счет количества символов нового пароля
    Serial.println(char(EEPROM.read(3)));                                       //проговариваем третий знак пароля в порт
 }

  else if (shetch==3 && (key)){                                                 //начинаем менять пароль 4й символ
    Serial.print("NewPass_symbol_four "); Serial.println(key);
    EEPROM.write(4, key);
    tone(BIP, 1400); delay(500); noTone(BIP);
    Serial.println(char(EEPROM.read(4)));                                       //проговариваем четвертый знак пароля в порт
      String passnew = "";                                                      //вводим переменную, содержащую новый введенный код
      passnew = passnew+char(EEPROM.read(1));
      passnew = passnew+char(EEPROM.read(2));
      passnew = passnew+char(EEPROM.read(3));
      passnew = passnew+char(EEPROM.read(4));

  if (passnew==pass){                                                           //если новый код равен старому
    shetch=0;                                                                   //запрашиваем другой новый код
    summ="";
    Serial.println("NewPass equal old pass, Reset");
    tone(BIP, 1400); delay(500); tone(BIP, 1400); delay(500); tone(BIP, 1400); delay(500); noTone(BIP);
 }
  else if (passnew=="ABCD"){                                                    //если новый код равен комбинации для смены пароля
    shetch=0;                                                                   //запрашиваем другой новый код
    summ="";
    Serial.println("NewPass equal ABCD, Reset");
    tone(BIP, 1400); delay(500); tone(BIP, 1400); delay(500); tone(BIP, 1400); delay(500); noTone(BIP);
 }else{                                                                         //присваиваем паролю значения из энергонезависимой памяти
     pass = "";
     pass = pass+char(EEPROM.read(1));
     pass = pass+char(EEPROM.read(2));
     pass = pass+char(EEPROM.read(3));
     pass = pass+char(EEPROM.read(4));
                                                                                //вывод в порт пароль для отладки
   Serial.println("Pass read test: ");
   Serial.println(char(EEPROM.read(1)));                                        //проговариваем первый знак пароля в порт
   Serial.println(char(EEPROM.read(2)));                                        //проговариваем второй знак пароля в порт
   Serial.println(char(EEPROM.read(3)));                                        //проговариваем третй знак пароля в порт
   Serial.println(char(EEPROM.read(4)));                                        //проговариваем четвертый знак пароля в порт
   Serial.print("Pass "); Serial.println(pass);                                 //проговариваем пароль в порт

   dochange=0;                                                                  //далее выходим из цикла "case 1"
   change=0;
   shet=0;
   shetch=0;
   summ="";
  tone(BIP, 1400); delay(500); tone(BIP, 1000); delay(500); tone(BIP, 600); delay(500); noTone(BIP);
  break;
  }};}}}
