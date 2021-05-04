#include <TroykaOLED.h>
TroykaOLED myOLED(0x3C);
long int timer;
long int last_press;
byte last_mode = 2;
byte last_pos;
byte pos = 6;
byte mode = 1;
int button1Pin=7;
int button2Pin=8;
int nextButton=4;
int tumblerpin=2;
int sbrospin=11;
int fplayer=6;
int splayer=0;
int led1pin=9;
int led2pin=10;
String answer = "привет";
void setup() {
  myOLED.begin();
  myOLED.clearDisplay();
  myOLED.setCoding(Encoding::UTF8);
  pinMode(splayer, INPUT_PULLUP);
  pinMode(tumblerpin, INPUT_PULLUP);
  pinMode(nextButton, INPUT_PULLUP);
 // pinMode(5, INPUT_PULLUP);
  pinMode(tumblerpin, INPUT_PULLUP);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(sbrospin, INPUT_PULLUP);
  pinMode(led1pin, OUTPUT);
  pinMode(led2pin, OUTPUT);
  Serial.begin(9600);
  timer = millis();
}
void loop() {
  if (Serial.available() > 0) {
    myOLED.clearDisplay();
        answer = Serial.readString();
        myOLED.setFont(font12x10);
        myOLED.print(answer, OLED_CENTER, 20);
      }
  //byte mode = digitalRead(tumblerpin);
  if (mode == 0) {
    if (last_mode != mode)
    {
      last_mode = mode;
      Serial.print("mode="); //режим работы
      Serial.println(mode); //0 - ручной, 1-автоматический (для отключения светодиодов)
    }
    value_X = analogRead(1);
    value_Y = analogRead(0);
    int sensorVal = digitalRead(button1Pin);
    int sensorVal1 = digitalRead(button2Pin);
    int sbros = digitalRead(sbrospin);
    if (value_Y > 600 && value_X < 600 && value_X > 200) pos = 1;
    else if (digitalRead(2) == LOW) pos = 2;
    else if (value_X > 600 && value_Y < 600 && value_Y > 200) pos = 3;
    else if (value_X < 200 && value_Y < 600 && value_Y > 200) pos = 4;
    if (pos == 1 && pos != last_pos) //положение джойстика для перехода к следующему вопросу
    {
      last_pos = pos;
      Serial.println("next");
    }
    else if (pos == 2 && pos != last_pos) //положение джойстика для показа ответа
    {
      myOLED.clearDisplay();
      last_pos = pos;
      Serial.println("answer");
      if (Serial.available() > 0) {
        answer = Serial.readString();
        myOLED.setFont(alphabet6x8);
        myOLED.print(answer, OLED_CENTER, 10);
      }
    }
    else if (pos == 3 && pos != last_pos) //положение джойстика для прибавления балла первому игроку
    {
      last_pos = pos;
      Serial.println("first player");
    }
    else if (pos == 4 && pos != last_pos) //положение джойстика для прибавления балла второму игроку
    {
      last_pos = pos;
      Serial.println("second player");
    }
    if (sensorVal == sensorVal1) {
      digitalWrite(led1pin, LOW);
      digitalWrite(led2pin, LOW);
    }
    else if (sensorVal == LOW && sensorVal1 == HIGH) {
      Serial.println("button=1"); //нажата первая кнопка
      while (sbros != LOW) {
        digitalWrite(led2pin, HIGH);
        digitalWrite(led1pin, LOW);
        sbros = digitalRead(sbrospin);
      }
      Serial.println("sbros"); //нажата кнопка сброса
      digitalWrite(led2pin, LOW);
    }
    else if (sensorVal == HIGH && sensorVal1 == LOW) {
      Serial.println("button=2");//нажата вторая кнопка
      while (sbros != LOW) {
        digitalWrite(led2pin, LOW);
        digitalWrite(led1pin, HIGH);
        sbros = digitalRead(sbrospin);
      }
      Serial.println("sbros");//нажата кнопка сброса
      digitalWrite(led1pin, LOW);
    }
  }
  else if (mode == 1)
  {
    if (last_mode != mode) {
      last_mode = mode;
      Serial.print("mode=");
      Serial.println(mode);
    }
    int sensorVal = digitalRead(button1Pin);
    int sensorVal1 = digitalRead(button2Pin);
    if (digitalRead(nextButton) == LOW) pos = 1;
    else if (digitalRead(fplayer) == LOW) pos = 2;
    else if (digitalRead(splayer) == LOW) pos = 3;
    if (pos == 1 && pos != last_pos) //положение джойстика для перехода к следующему вопросу
    {
      last_pos = pos;
      Serial.println("next");
      last_press=millis();
    }
    else if (pos == 2 && pos != last_pos) //положение джойстика для прибавления балла первому игроку
    {
      last_pos = pos;
      Serial.println("first player");
      last_press=millis();
      Serial.println("next");
    }
    else if (pos == 3 && pos != last_pos) //положение джойстика для прибавления балла второму игроку
    {
      last_pos = pos;
      Serial.println("second player");
      last_press=millis();
      Serial.println("next");
    }
    if (millis()-last_press>2000)
    {
      last_pos=4;
      pos=4;
    }
    if (sensorVal == sensorVal1) {
      digitalWrite(led1pin, LOW);
      digitalWrite(led2pin, LOW);
    }
    else if (sensorVal == LOW && sensorVal1 == HIGH) {
      last_press = millis();
      Serial.println("button=1"); //нажата первая кнопка
      while (timer - last_press < 3000 && digitalRead(fplayer)!=LOW && digitalRead(splayer)!=LOW) {
        digitalWrite(led2pin, HIGH);
        digitalWrite(led1pin, LOW);
        timer = millis();
      }
      digitalWrite(led2pin, LOW);
    }
    else if (sensorVal == HIGH && sensorVal1 == LOW) {
      last_press = millis();
      Serial.println("button=2");//нажата вторая кнопка
      while (timer - last_press < 3000 && digitalRead(fplayer)!=LOW  && digitalRead(splayer)!=LOW) {
        digitalWrite(led2pin, LOW);
        digitalWrite(led1pin, HIGH);
        timer = millis();
      }
      digitalWrite(led1pin, LOW);
    }
  }
}
