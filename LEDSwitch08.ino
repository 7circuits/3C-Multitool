/* 
LED Application
FOR 1W LED ONLY
1. Turn ON with LOW Voltage
2. Switch to HIGH Voltage
3. Flash
4. OFF/STANDBY

FOR ATTINY85 
Button for HIGH signal in on Pin0
PWM output for MOSFET Gate on PIN1
*/

#include <avr/sleep.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// Bei LED Vorwiderstand 4,7R und C=1µF
// 2,402V (1/20) 2,41V(1/18) 2,44V(1/15) 3,28(20/1) 2,53(2/10) 2,69(5/10)
// 2,99V(20/1)
// Bei LED Vorwiderstand 1R und C=1µF
// (1/15)(20/0) 2,434V 3,252V
// mode low
int delPWM1 = 1;
int delPWM2 = 16; //2,42V
// mode middle
int delPWM3 = 4;
int delPWM4 = 9; //2,76V
// mode middle
int delPWM5 = 5;
int delPWM6 = 2; //3,32V

const int button1Pin = 0; //setting button pin
const int ledPin =  2;  // setting LED pin
int button1PushCounter1 = 0;    // counts the button pushes
int button1State1 = 1;    // tracks the button state
int lastButton1State1 = 0;    // last state of the button  
int button1State = 0;  // Button status
int lastButton1State = LOW;  //Button set to LOW
long lastDebounceTime1 = 0;  // Debounce status
long debounceDelay = 50;  // Debounce delay time

void setup() {
  pinMode(ledPin, OUTPUT);  // Set LED-Pin as output
  pinMode(button1Pin, INPUT);  // Sets button as input
  sbi(GIMSK,PCIE); // // turn on general PCINT
  sbi(PCMSK,PCINT0); // turn on chosen PCINTx
}

//mode one: low
int modeone() {
  digitalWrite(ledPin,HIGH);
  delay(delPWM1);
  digitalWrite(ledPin,LOW);
  delay(delPWM2);
}

//mode middle: middle
int modemiddle() {
  digitalWrite(ledPin,HIGH);
  delay(delPWM3);
  digitalWrite(ledPin,LOW);
  delay(delPWM4);
}

//mode two: full
int modetwo() {
  digitalWrite(ledPin,HIGH);
  delay(delPWM5);
  digitalWrite(ledPin,LOW);
  delay(delPWM6);
}

//mode three: strobe
int modethree() {
  digitalWrite(ledPin,HIGH);
  delay(15);
  digitalWrite(ledPin,LOW);
  delay(100);
}

//Sleep mode
int modefour() {
  digitalWrite(ledPin,LOW);
  delay(200);
  button1PushCounter1 = 1;
  system_sleep();
}

void loop() {
  int reading1 = digitalRead(button1Pin);
  button1State1 = digitalRead(button1Pin);
  
  if (reading1 != lastButton1State) {
    lastDebounceTime1 = millis();
  }
  
  if ((millis() - lastDebounceTime1) > debounceDelay) {

    if (reading1 != button1State) {
      button1State = reading1;
      
        if (button1State1 != lastButton1State1) {
          
          if (button1State1 == HIGH) {
            button1PushCounter1++;
            
            if (button1PushCounter1 == 6) {
              button1PushCounter1 = 1;}
          }
          else {
          }
        }
          lastButton1State1 = button1State1;
    }
  }
  lastButton1State = reading1;

//switching modes:
  
      if (button1PushCounter1 == 1) {  
          modeone();
    }
      if (button1PushCounter1 == 2) {  
          modemiddle();
    }
      if (button1PushCounter1 == 3) {  
           modetwo(); 
    }
      if (button1PushCounter1 == 4) {  
          modethree();
    }
      if (button1PushCounter1 == 5) {  
          modefour();
    }
}

void system_sleep() {
cbi(ADCSRA,ADEN); // Analog-Digitalwandler aus
set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode to power down
sleep_mode(); // System sleeps here
sbi(ADCSRA,ADEN);  // Switch Analog to Digital converter ON
}

ISR(PCINT0_vect) {
}
