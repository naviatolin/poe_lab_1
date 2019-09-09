#include <Arduino.h>

// initializing the variables
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
  // setting up the pins
  led1 = 13;
  led2 = 12;
  led3 = 11;
  button = 8;
  pot_pin = 0;
  
  // setting the pins as INPUT or OUTPUT
  pinMode(led1,OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(button, INPUT);
  pinMode(pot_pin, INPUT);
  
  
  // setting the mode that we are in as all_off
  mode = 0;
  
  Serial.begin(9600);

  // setting the debounce timer
  wait_bouncing = 50;

  // initializing the button states
  button_state = LOW;
  last_button = HIGH;

  time_init = 0;
}

void all_off(){
  """
  Sets all three LEDs to LOW
  """
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
  digitalWrite(led3,LOW);
}

void all_on(){
  """
  Sets all LEDs to HIGH
  """
  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);
  digitalWrite(led3,HIGH);
}

void loop() {
  """
  Checks for the button state and changes the LED mode.
  """
  //checks for the current button state 
  button_state = digitalRead(button);
  
  //assigns the current mode value to the prev_mode variable
  prev_mode = mode;


  if (button_state != last_button){
    time_init = millis(); //initialize the debounce timer
  }
  
  //changing the mode if the button has been pressed
  if (millis() - time_init > wait_bouncing){
    if (digitalRead(button) != button_now){
      button_now = digitalRead(button);

      if (button_state == HIGH){
        mode++;
      }
    }
  }

  // saving the last state of the button
  last_button = button_state;

  // setting the switch between modes
  switch(mode%4){
    case 0:
      // the off mode
      all_off();
      break;
    case 1:
      // the on mode
      all_on();
      break;
    case 2:
      //initializing the blink timer
      if (prev_mode%4 != 2){
        blink_init = millis();
      }
    
      blink_now = millis(); // checking the current time
      blink_rate = analogRead(pot_pin); //reading the potentiometer output
      if ((blink_now - blink_init) > blink_rate){
        // changing the led state to whatever it is not currently
        digitalWrite(led1, !digitalRead(led1)); 
        digitalWrite(led2, !digitalRead(led2));
        digitalWrite(led3, !digitalRead(led3));
        blink_init = millis();
      }
      break;
    case 3:
      if (prev_mode%4 != 3){
        // initializes the blink timer and turns the three LEDs off
        blink_init = millis();
        digitalWrite(led1,LOW);
        digitalWrite(led2,LOW);
        digitalWrite(led3,LOW);
      }

      blink_now = millis(); // checking the current time

      // running through the bouncing light sequence
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