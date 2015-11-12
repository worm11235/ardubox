#include <EEPROM.h>
#include <Stepper.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#define TYPE_CHAR 1
#define TYPE_INT 2
#define TYPE_FLOAT 3

void put(char ch) {
  char *str = new char[2];
  str[0] = ch;
  str[1] = '\0';
  Serial.print(str);
}

void configStepper ();

void rotateStepper();

void rollWheel();

const char msgList_1[]  = "Select action: Config(c), Read Analog(a), Distance detect(d), Stepper rotate(r).";
const char msgList_2[]  = "Select the item you tant to config:Date(d), Stepper(s):";
//const char msgList_3[] PROGMEM  = "Select the item you tant to config:Date(d), Stepper(s):";

Stepper *stp = NULL;

byte preg = 1;

struct Registry {
  byte id;
  byte pid;
  const char *msg;
  int caseVal;
  byte caseType;
  void(*action)();
};

const struct Registry regs[] = {
  {1, 0, msgList_1, 0, TYPE_CHAR, NULL},
  {2, 1, msgList_2, 'c', TYPE_CHAR, NULL},
  {3, 2, NULL, 's', 0, configStepper},
  {4, 1, NULL, 'r', 0, rotateStepper},
  {5, 1, NULL, 'f', 0, fullSpeed},
  {6, 1, NULL, 'w', 0, forward},
  {7, 1, NULL, 'b', 0, backward},
  {8, 1, NULL, 'a', 0, turnLeft},
  {9, 1, NULL, 'd', 0, turnRight},
};

//TODO: Modify this every time you change registry list.
const int NUM_OF_REG = 9;

const struct Registry *findById (byte id) {
  for (int i = 0; i < NUM_OF_REG; i++) {
    if (regs[i].id == id) {
      return &regs[i];
    }
  }
    return NULL;
}

const struct Registry *findByPidAndValue(byte pid, int value) {
  //Serial.println(pid);
  //Serial.println(value);
  for (int i = 0; i < NUM_OF_REG; i++) {
    if (regs[i].pid == pid && regs[i].caseVal == value) {
      return &regs[i];
    }
    /*Serial.println("Not match");
    Serial.println(regs[i].id);
    Serial.println(regs[i].caseVal);*/
  }
    return NULL;
}

void wait() {
  while(Serial.available() <= 0) ;
}


// pins for the LEDs:
const int redPin = 3;
const int greenPin = 5;
const int bluePin = 6;
int inputPin = 4; // define ultrasonic signal receiver pin  ECHO to D4
int outputPin = 5; // define ultrasonic signal transmitter pin  TRIG to D5
//Stepper myStepper(100, 8, 9, 10, 11);
void setup() {
  // 设置转速，单位r/min
  //myStepper.setSpeed(60);
  // initialize serial:
  Serial.begin(115200);
  // make the pins outputs:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(inputPin, INPUT);
  pinMode(outputPin, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  while(true) {
    int val = Serial.read();
    if ('[' == val) {
      break;
    } 
  }
  while(true) {
    const Registry *reg = findById(preg);
    if (NULL != reg) {
      if (NULL != reg->action) {
        reg->action();
        preg = 1;
        continue;
      }
      Serial.println(reg->msg);
      int val;
      if (TYPE_CHAR == reg->caseType) {
        wait();
        val = Serial.read();
      } else {
        wait();
        val = Serial.parseInt();
      }
      Serial.println(val);
      const Registry *sreg = findByPidAndValue(reg->id, val);
      if (NULL != sreg) {
        preg = sreg->id;
      } else {
        //Serial.println("Not found");
      }
    } else {
      //Serial.println("Not found2");
    }
  }
}
