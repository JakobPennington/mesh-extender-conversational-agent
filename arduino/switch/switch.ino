#include <Console.h>
#include <FileIO.h>
#include <TimeLib.h>
#include <Time.h>

/*
 * A simple program to read a switch connected to a Mesh Extender
 */

// Define variables
const int ledPin = 13;
const int switchPin = 3;
int switchState = 0;
int newSwitchState = 0;

void setup() {
  // Initialise pins
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);
  digitalWrite(switchPin, HIGH);

  // Set up Bridge to pass data to Linux board
  Bridge.begin();
}

void loop() {
  newSwitchState = digitalRead(switchPin);

  // Only act when the state of the switch changes
  String dataString = "";
  if (newSwitchState != switchState) {
    if (newSwitchState == HIGH) {
      dataString.concat("0");
      digitalWrite(ledPin, LOW);
    } else {
      dataString.concat("1");
      digitalWrite(ledPin, HIGH); 
    } 

    dataString.concat(",");
    dataString.concat(getTimeStamp());

    // Write the data to the output file
    File outputFile = FileSystem.open("/tmp/switch_values", FILE_APPEND);
    if(outputFile){
      outputFile.println(dataString);
      outputFile.close();
    } else {
      // Log error?
    }

    // Store the state of the switch
    switchState = newSwitchState;
  }
  delay(1000);
}

String getTimeStamp(){
  String timeStamp;
  timeStamp.concat(day());
  timeStamp.concat("/");
  timeStamp.concat(month());
  timeStamp.concat("/");
  timeStamp.concat(year());
  timeStamp.concat(" ");
  timeStamp.concat(hour());
  timeStamp.concat(":");
  timeStamp.concat(minute());
  timeStamp.concat(":");
  timeStamp.concat(second());
  return timeStamp;
}
