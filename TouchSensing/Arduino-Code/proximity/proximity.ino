

/*

Modified by @ankitdaf on 8th August 2014.
Includes experimental settings for proximity, but checking for touch has been left out.

Copyright (c) 2010 bildr community
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


#include "mpr121.h" // Contains the register addresses for configuration
#include <Wire.h>

const int numReadings = 10;     // Taking 10 measurements per reading to avoid noise
int readings[numReadings];      // for storing readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int oldaverage = 0;

int threshold = 710; // Experimental, worked in my case
int ledpinR = 10;    // LED connected to digital pin 10
int ledpinB = 9;    // LED connected to digital pin 9
int ledpinG = 11;    // LED connected to digital pin 11

#define SENSORS       12    // We will be using all 12 electrodes
#define TOU_THRESH    0x1F  // Experimental, but doesn't matter much for this application
#define REL_THRESH    0x1A
#define PROX_THRESH   0x3f
#define PREL_THRESH   0x3c

void setup() {
  Wire.begin(); // Initiate I2C communication with MPR121
  pinMode(ledpinR, OUTPUT); // Set the LEDs as output and switch them off
  pinMode(ledpinG, OUTPUT);
  pinMode(ledpinB, OUTPUT);
  analogWrite(11, 0);
  analogWrite(10, 0);
  analogWrite(9, 0);
  delay(1000);

  // Initialize the reading array with zeroes to avoid garbage

  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;

  // set-up the Serial and I2C/Wire connections
  Serial.begin(115200);
  Wire.begin();

  // set the registers on the capacitive sensing IC
  setupCapacitiveRegisters();
}

void loop() {
  // subtract the last reading:
  total = total - readings[index];
  // read from the sensor:

  int temp = get_proximity();
  if ((temp > 300) && (temp < 760))    {
    readings[index] = temp;
  }

  // add the reading to the total:
  total = total + readings[index];

  // advance to the next position in the array:
  index = index + 1;

  // if we're at the end of the array...
  if (index >= numReadings)
    // ...wrap around to the beginning:
    index = 0;

  // calculate the average:
  average = total / numReadings;

  if (average < threshold - 5 && average > 300) {
    lightUp(average);
  }
  else {
    //     Serial.println("off");
    analogWrite(11, 0);
    analogWrite(10, 0);
    analogWrite(9, 0);
  }
  delay(10);
}




/**
 * setupCapacitiveRegisters Updates all of configurations on the MP121 capacitive sensing IC. This includes
 *   setting levels for all filters, touch and proximity sensing activation and release thresholds, debounce,
 *   and auto-configurations options. At the end it activates all of the electrodes.
 */
void setupCapacitiveRegisters() {

  set_register(0x5A, ELE_CFG, 0x00);

  // prox sensing rising
  set_register(0x5A, PROX_MHDR, 0x3F);
  set_register(0x5A, PROX_NHDAR, 0x5F);
  set_register(0x5A, PROX_NCLR, 0x04);    // Should probably consider rate of approach, but trial and error for now
  set_register(0x5A, PROX_FDLR, 0x00);

  // prox sensing falling
  set_register(0x5A, PROX_MHDF, 0x01);
  set_register(0x5A, PROX_NHDAF, 0x01);
  set_register(0x5A, PROX_NCLF, 0x1F);  // Take Fewer noise samples to adjust so as to make it more sensitive ?
  set_register(0x5A, PROX_NDLF, 0x04);

  // Section D - Set the touch filter Configuration
  set_register(0x5A, FIL_CFG, 0xC4);

  // Section E - Set proximity sensing threshold and release
  //    set_register(0x5A, PRO_T, PROX_THRESH);   // sets the proximity sensor threshold
  //  set_register(0x5A, PRO_R, PREL_THRESH);   // sets the proximity sensor release

  // Section F - Set proximity sensor debounce
  set_register(0x59, PROX_DEB, 0x50);  // PROX debounce


  // Section G - Set Auto Config and Auto Reconfig for prox sensing
  set_register(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V
  set_register(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
  set_register(0x5A, ATO_CFGT, 0xB5);  // Target = 0.9*USL = 0xB5 @3.3V
  set_register(0x5A, ATO_CFG0, 0x0B);

  // Additional registers to be set for better proximity sensing
  set_register(0x5A, 0x5C, 0x3F);
  set_register(0x5A, 0x6B, 0x7F);

  // Section H - Start listening to all electrodes and the proximity sensor
  set_register(0x5A, ELE_CFG, 0x30);
}

/**
 * set_register Sets a register on a device connected via I2C. It accepts the device's address,
 *   register location, and the register value.
 * @param address The address of the I2C device
 * @param r       The register's address on the I2C device
 * @param v       The new value for the register
 */
void set_register(int address, unsigned char r, unsigned char v) {
  Wire.beginTransmission(address);
  Wire.write(r);
  Wire.write(v);
  Wire.endTransmission();
}


void lightUp(int average) {
  if (average >= threshold) average = threshold;
  if (average < 400) average = 400;
  int lightValue = map(average, 300, threshold - 5, 254, 10);
  analogWrite(11, lightValue);
}

int* read_reg(int address, int register_no, int no_of_bytes) {
  int rxbuffer[no_of_bytes];
  Wire.beginTransmission(address);
  Wire.write(register_no);
  Wire.endTransmission(false);
  Wire.requestFrom(address, no_of_bytes);
  for (int i = 0; i < no_of_bytes; ++i) {

    if (Wire.available()) {
      rxbuffer[i] = Wire.read();
      //Serial.println(rxbuffer[i]);
    }
  }
  Wire.endTransmission();
  return rxbuffer;
}

/*

A method to read the promixity sensing registers

*/
int get_proximity() {
  int* prox;
  prox = read_reg(0x5A, 0x1C, 2);
  int proximity = ( prox[1] << 8) + prox[0];
  //  Serial.print(threshold);
  //  Serial.print(",");
  Serial.println(proximity);

  return proximity;
}
