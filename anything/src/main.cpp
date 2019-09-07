#include <Arduino.h>

unsigned long time_init;
unsigned long blink_init;
unsigned long blink_now;
int mode;
int led1;
int led2;
int led3;
int button;
int button_state;
int last_button;
int wait_bouncing;
int button_now;
int prev_mode;
int pot_pin;
float blink_rate;

void setup() {
  led1 = 13;
  led2 = 12;
  led3 = 11;
  button = 8;
  pot_pin = 0;
  
  pinMode(led1,OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(button, INPUT);
  pinMode(pot_pin, INPUT);
  
  
  mode = 0;
  
  Serial.begin(9600);

  wait_bouncing = 60;

button_state = LOW;
last_button = HIGH;

time_init = 0;
}

void all_off(){
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
  digitalWrite(led3,LOW);
}

void all_on(){
  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);
  digitalWrite(led3,HIGH);
}

void loop() {
  button_state = digitalRead(button);
  prev_mode = mode;

  if (button_state != last_button){
    time_init = millis();
  }
  
  if (millis() - time_init > 50){
    if (digitalRead(button) != button_now){
      button_now = digitalRead(button);

      if (button_state == HIGH){
        mode++;
      }
    }
  }
  last_button = button_state;
  switch(mode%4){
    case 0:
      all_off();
      break;
    case 1:
      all_on();
      break;
    case 2:
      if (prev_mode%4 != 2){
        blink_init = millis();
      }
      blink_now = millis();
      blink_rate = analogRead(pot_pin);
      if ((blink_now - blink_init) > blink_rate){
        digitalWrite(led1, !digitalRead(led1));
        digitalWrite(led2, !digitalRead(led2));
        digitalWrite(led3, !digitalRead(led3));
        blink_init = millis();
      }
      break;
    case 3:
      if (prev_mode%4 != 3){
        blink_init = millis();
        digitalWrite(led1,LOW);
        digitalWrite(led2,LOW);
        digitalWrite(led3,LOW);
      }
      blink_now = millis();
      if ((blink_now - blink_init) > 100){
        digitalWrite(led1, HIGH);
      }
      if ((blink_now - blink_init) > 200){
        digitalWrite(led1,LOW);
        digitalWrite(led2,HIGH);
      }
      if ((blink_now - blink_init) > 300){
        digitalWrite(led2,LOW);
        digitalWrite(led3,HIGH);
      }
      if ((blink_now - blink_init) > 400){
        digitalWrite(led3, LOW);
        digitalWrite(led2,HIGH);
      }
      if ((blink_now - blink_init) > 500){
        digitalWrite(led2, LOW);
        digitalWrite(led1,HIGH);
      }
      if ((blink_now - blink_init) > 600){
        digitalWrite(led1, LOW);
        blink_init = millis();
      }
      break;
  }
}