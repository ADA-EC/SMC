//Include the SoftwareSerial library
#include "SoftwareSerial.h"

const int ledPin = 13; // the pin that the LED is attached to
String incomingMsg;      // a variable to read incoming serial data into
char terminator = '\r'; //line terminator
String newline = "\r\n";

SoftwareSerial bluetooth(2, 3); //TX, RX (Bluetooth)

void setup() {
  //Initialize serial comm
  bluetooth.begin(38400);
  bluetooth.setTimeout(500);
  
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);

  
}

void loop() {
  // see if there's incoming serial data:
  if (bluetooth.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingMsg = bluetooth.readStringUntil(terminator);
    incomingMsg.trim();
    if(incomingMsg == "") {
    } else if(incomingMsg == "mandatudo"){
      bluetooth.print("mando\r\n");
    } else if(incomingMsg == "leituraatual"){
      bluetooth.print("agora\r\n");
    } else {
      bluetooth.print("erro:'");
      bluetooth.print(incomingMsg);
      bluetooth.print("' nao eh valido\r\n");
    }
  }
}
