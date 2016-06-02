#include <Console.h>
#include <FileIO.h>
#include <TimeLib.h>
#include <Time.h>

/*
 * A simple program to read a switch connected to a Mesh Extender
 * 
 * Switch values are read and, when changed, written to a file with the 
 * timestamp. These are then read by meshmsresponder.
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

  /* Due to the pull-up resistor being on the switchPin, 
   * switchState is reversed to what would be expected 
   * High == switch is open, Low == switch is closed 
   */

  // Only act when the state of the switch changes
  String dataString = "";
  if (newSwitchState != switchState) {
    if (newSwitchState == HIGH) {
      // Switch is open
      dataString.concat("0");
      digitalWrite(ledPin, LOW);
    } else {
      // Switch is closed
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
  delay(500);
}

/*
 * Switch value, date and time are comma separated for easy processing by meshmsresponder
 */
String getTimeStamp(){
  // Timestamp format: 0,1/1/1970,00:01
  String timeStamp;
  timeStamp.concat(day());
  timeStamp.concat("/");
  timeStamp.concat(month());
  timeStamp.concat("/");
  timeStamp.concat(year());
  timeStamp.concat(",");
  timeStamp.concat(formatTime(hour()));
  timeStamp.concat(":");
  timeStamp.concat(formatTime(minute()));
  return timeStamp;
}

/*
 * TimeLib returns single digits for single digit times ie. 10:03 becomes 10:3
 * Concatenate a "0" in fromt of single digit numbers for better output.
 */
String formatTime(int number){
  if(number <= 9){
    String formattedNumber = "0";
    formattedNumber.concat(number);
    return formattedNumber;
  }
}

