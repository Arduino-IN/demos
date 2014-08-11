
/*
 * CapitiveSense Library courtesy
 * This sketch is put together for use with the daft punk online console at http://www.najle.com/idaft/idaft/
 * This sketch relies on the Capacitive Sensor library here : http://playground.arduino.cc/Main/CapacitiveSensor
 * 
 * Ankit Daftery, 8th August 2014
 *
 */


#include <CapacitiveSensor.h>


CapacitiveSensor   cs_0 = CapacitiveSensor(4, 0);
CapacitiveSensor   cs_1 = CapacitiveSensor(4, 1);
CapacitiveSensor   cs_2 = CapacitiveSensor(4, 2);
CapacitiveSensor   cs_3 = CapacitiveSensor(4, 3);
CapacitiveSensor   cs_5 = CapacitiveSensor(4, 5);
CapacitiveSensor   cs_6 = CapacitiveSensor(4, 6);
CapacitiveSensor   cs_7 = CapacitiveSensor(4, 7);
CapacitiveSensor   cs_8 = CapacitiveSensor(4, 8);

long threshold = 6000;		// Experimental

void setup()
{
 
}

void loop()
{

  // Read all the sensors 

  long total0 =  cs_0.capacitiveSensor(30);
  Serial.println(total0);
  long total1 =  cs_1.capacitiveSensor(30);
  long total2 =  cs_2.capacitiveSensor(30);
  long total3 =  cs_3.capacitiveSensor(30);

  long total5 =  cs_5.capacitiveSensor(30);
  long total6 =  cs_6.capacitiveSensor(30);
  long total7 =  cs_7.capacitiveSensor(30);
  long total8 =  cs_8.capacitiveSensor(30);

  // Check if any of the sensors were pressed and generate the corresponding keypress on the computer
  if (isPressed(total0)) Keyboard.write('a');
  delay(10);
  if (isPressed(total1)) Keyboard.write('q');
  delay(10);
  if (isPressed(total2)) Keyboard.write('s');
  delay(10);
  if (isPressed(total3)) Keyboard.write('w');
  delay(10);
  if (isPressed(total5)) Keyboard.write('d');
  delay(10);
  if (isPressed(total6)) Keyboard.write('e');
  delay(10);
  if (isPressed(total7)) Keyboard.write('r');
  delay(10);
  if (isPressed(total8)) Keyboard.write('f');

}

/*
 * Check if the electrode / pad has been touched
 * Touch gives a distinct difference in reading, which we can use to check
 *
 */
bool isPressed(long total) {
  if (total > threshold) return true;
  return false;

}
