#define RELAY 3         //lights
#define SENSOR A1       //photoresistor
#define TIME_ON 15      //15*8s=120s=2 minutes
#define TIME_WAIT 255   //225*8s=1800s=30 minutes
#define NIGHT_VALUE 800 //if sensor value is above this value then is night, else is day


//////////////watchdog///////////////////
#include <avr/sleep.h>
#include <avr/wdt.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
//////////////watchdog///////////////////

bool lights_on=true;       //check if lights are on
int time_on=0;             //time for the lights to stay on during night
int time_wait=0;           //time for the lights to stay off during night

void setup() {
  
  pinMode(OUTPUT,RELAY);
  pinMode(INPUT,SENSOR);
  //////////////watchdog///////////////////
  setup_watchdog(9);      //set watchdog to give an interrupt after 8 seconds
  //////////////watchdog///////////////////
  
}

void loop() {

    if(analogRead(SENSOR)<NIGHT_VALUE){
      digitalWrite(RELAY,LOW);
      pinMode(RELAY,INPUT);
      lights_on=false;
      system_sleep();
    }else{
      if(lights_on){
        time_on++;
        if(time_on<TIME_ON){
          system_sleep();
        }else{
          lights_on=false;
          digitalWrite(RELAY,LOW);
          pinMode(RELAY,INPUT);
          time_on=0;
          system_sleep();
        }
      }else{
        time_wait++;
        if(time_wait<TIME_WAIT){
          system_sleep();
        }else{
          lights_on=true;
          pinMode(RELAY,OUTPUT);
          digitalWrite(RELAY,HIGH);
          time_wait=0;
          system_sleep();
        }
      }
    }
 
}

void system_sleep(){
  cbi(ADCSRA,ADEN);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  sleep_mode();

  sleep_disable();
  sbi(ADCSRA,ADEN);
}

void setup_watchdog(int ii){
  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;

  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE); 
}

ISR(WDT_vect) {
}
