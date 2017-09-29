#include <DS3232RTC.h>        //http://github.com/JChristensen/DS3232RTC
#include <Time.h>             //http://playground.arduino.cc/Code/Time
#include <Wire.h>             //http://arduino.cc/en/Reference/Wire
#include <LowPower.h>

//LED connected to this pin
int led = 4;
// Use pin 2 as wake up pin
const int wakeUpPin = 2;

void wakeUp()
{
    // Just a handler for the pin interrupt.
}

void setup()
{
    // Configure wake up pin as input.
    // This will consumes few uA of current.
    pinMode(wakeUpPin, INPUT);
    
    //Configure led pin as output
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW); 
    
    //Alarm setup
    RTC.alarm(1);
    RTC.setAlarm(ALM1_MATCH_SECONDS, 7, 0, 0, 0);
    RTC.alarmInterrupt(1, true);
}

void loop() 
{
    // Allow wake up pin to trigger interrupt on low.
    attachInterrupt(0, wakeUp, LOW);
    
    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake up pin is low.
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
    
    // Disable external pin interrupt on wake up pin.
    detachInterrupt(0); 
    
    
    //Blink led
    int i;
    for (i = 0; i < 10; i++) {
      digitalWrite(led, HIGH);
      delay(500);
      digitalWrite(led, LOW);
      delay(500);
    }
    
    //Clears alarm flag
    RTC.alarm(1);
}
