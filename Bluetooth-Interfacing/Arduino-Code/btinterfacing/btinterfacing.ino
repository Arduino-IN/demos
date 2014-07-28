#include "lookup.h"

int pin = 13;
int buzzer = 8;

String morseString = ""; // We store our input data here
int done = 0;  // Have we received all the data ?

void setup() {
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
  delay(2);
}

void loop() {

  while (Serial.available())
  {
    char temp = (char) Serial.read();
    if (temp == '\n')done = 1;
    morseString += temp;
  }
  if (morseString != "" && done == 1) {
    //Serial.println(morseString);
    printMorse(morseString);
    morseString = "";
    done = 0;
  }
}

void dot()
{
  digitalWrite(pin, HIGH);
  tone(8, 196, 40);
  delay(40);
  //delay(250);
  noTone(8);
   digitalWrite(pin, LOW);
  delay(40);
 
  //delay(250);
}

void dash()
{
  digitalWrite(pin, HIGH);
  tone(8, 262, 120);
  delay(120);
  //delay(1000);
  noTone(8);
  digitalWrite(pin, LOW);
  delay(40);
  //delay(250);
}

void printMorse(String morse)
{
  for (int i = 0; i < morse.length(); ++i) {
    int j = 0;
    if (morse[i] == '\n') break;
    while (morse[i] != regulartext[j]) {
      ++j;
      if (j == 49) break;
    }
    if (j == 49) break;
    //Serial.println(j);
    Serial.print(regulartext[j]);
    Serial.print(morsecode[j]);
    for (int k = 0 ; k < morsecode[j].length(); ++k) {
      if (morsecode[j][k] == '.') {
        Serial.print("dot");
        dot();
      }
      else if (morsecode[j][k] == '-') {
        Serial.print("dash");
        dash();
      }
    }
    delay(80 );
    Serial.println();
  }
}
