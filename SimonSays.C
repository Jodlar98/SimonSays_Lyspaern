#include "Adafruit_Keypad.h"
#include "arduino-timer.h"
#define MAX_LEVEL 5
#define MAX_LED 8      // 0 indexed, 0 - 16

#define LIGHT_TIME 500  // ms LED will light
#define LIGHT_SPACE 250 // ms spacing after light

int sequence[MAX_LEVEL];

auto timer = timer_create_default(); // create a timer with default settings
unsigned long myTime;
unsigned long timerflag;

enum STATUS {
  INACTIVE,
  SHOWING,
  PLAYING,
  WON,
};

STATUS gameStatus = INACTIVE;
int level = 0;
int current_index = 0;
int velocity = 1000;

const byte ROWS = 3; //four rows
const byte COLS = 3; //three columns
char hexaKeys[ROWS][COLS] = {
  {'0', '1', '2'},
  {'3', '4', '5'},
  {'6', '7', '8'},

};

byte rowPins[ROWS] = {9, 10, 11}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {12, 13, 14}; //connect to the column pinouts of the keypad

Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  customKeypad.begin();
  pinMode(0,OUTPUT);
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  
}

void loop(){
  
  
  switch (gameStatus) {
    case INACTIVE:
      timerflag = 0;
      level = 0;
      current_index = 0;
      generate_sequence();
      gameStatus = SHOWING;
      Serial.print("Level: ");        
      Serial.println(level);
      for (int i = 0; i < MAX_LEVEL; i++) {
        Serial.print(" LED: ");
        Serial.println(sequence[i]);  
      }

      break;

    case SHOWING:
      current_index = 0;
      for (int i = 0; i < level + 1; i++) {
        digitalWrite(sequence[i], HIGH);
        delay(LIGHT_TIME);
        blankLED();
        delay(LIGHT_SPACE);
      }
      gameStatus = PLAYING;
      break;

     case PLAYING:
      // TODO: Add "timeout" for pressing the next key
      
      customKeypad.tick();
      while (customKeypad.available()) {
          keypadEvent e = customKeypad.read();
          
          Serial.print("Key  :");
          Serial.println((int)e.bit.KEY-48);
          Serial.print("Sequence  :");
          Serial.println((int)sequence[current_index]);
          Serial.print("Index  :");
          Serial.println(current_index);
          timerflag = 0;
        if(e.bit.EVENT == KEY_JUST_PRESSED){
        if ((int)e.bit.KEY-48 == (int)sequence[current_index]) {
          current_index = current_index +1;
          delay(30);}
        else{
            flashLED();
            gameStatus = INACTIVE;
          }
        } 
      } 
          if (timerflag == 0) {
            myTime = millis();
            timerflag = 1;}
          if (millis()> myTime+10000) {
            flashLED();
            gameStatus = INACTIVE;}
        
        if (current_index > level) {
          level++;
          current_index = 0;
          if (level >= MAX_LEVEL) {
            gameStatus = WON;
          } else {
            gameStatus = SHOWING;          
          }
        }
      
        break;
      case WON:
        flashLED();
        flashLED();
        flashLED();
        flashLED();
        flashLED();
        flashLED();
        gameStatus = INACTIVE;
        break;
      }

}



void generate_sequence() {
  randomSeed(millis()); //in this way is really random!!!
  for (int i = 0; i < MAX_LEVEL; i++) {
    sequence[i] = random(0,MAX_LED);
  }
}

void blankLED() {
  digitalWrite(0,LOW);
  digitalWrite(1,LOW);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);  
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);}

void flashLED() {
    for (int i = 0; i < 9; i++) {
      digitalWrite(i, HIGH);
      delay(10);
      digitalWrite(i, LOW);}
      delay(1000);
}


  