#include <Console.h>
#include <FileIO.h>
#include <TimeLib.h>

/*
 * A program to read a switch connected to a Dragino
 */

// Define variables
int ledPin = 13;
int switchPin = 3;
int switchState = 0;
int newSwitchState = 0;

boolean changed = false;

void setup() {
  // Initialise pins as outputs
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);
  digitalWrite(switchPin, HIGH);

  Bridge.begin();
}

void loop() {
  newSwitchState = digitalRead(switchPin);

  // Only act when the state of the switch changes
  String dataString = "";
  if(newSwitchState != switchState){
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
      // Do nothing, for now.
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
