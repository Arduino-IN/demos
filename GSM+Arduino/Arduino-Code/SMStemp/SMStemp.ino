#include<SoftwareSerial.h>
SoftwareSerial GPRS(5, 6);

int inputPinTempSensor = 0;
float tempCelsius = 0 ;
float thresholdTemp = 45;

void setup()
{
  GPRS.begin(9600);               // the GPRS baud rate
  Serial.begin(9600);             // the Serial port of Arduino baud rate.
  delay(500);
}

void loop() {
  getTemperature();		// Read the temperature & convert to Celsius
  if ( tempCelsius > thresholdTemp ) {	// If the temperature crosses the threshold, send the SMS
    sendSMS();
    Serial.println("Sent SMS!");
  }
  delay(10);
}


void getTemperature()
{
  int rawVoltage = analogRead(inputPinTempSensor);
  float milliVolts = (rawVoltage / 1024.0) * 5000;    // Convert raw voltage to temperature in mV
  tempCelsius = milliVolts / 10;
  Serial.print(tempCelsius);
  Serial.print(" degrees Celsius");
}

void sendSMS() {
  GPRS.println("AT+CMGF=1");    // Set modem to text mode
  delay(2000);
  GPRS.write("AT+CMGS=");      // Start composing message
  GPRS.write(0x22);            // hex equivalent of double-quote '"'
  GPRS.write("+91xxxxxxxxx");	// the number to send the SMS to
  GPRS.write(0x22);
  GPRS.write(0x0D);            // hex equivalent of Carraige return    
  GPRS.write(0x0A);            // hex equivalent of newline
  delay(2000);
  GPRS.print("Temperature is ");  // Send the text message to the GSM module
  GPRS.println(tempCelsius);
  GPRS.write(0x0D);            // hex equivalent of Carraige return    
  GPRS.write(0x0A);            // hex equivalent of newline  
  delay(1000);
  GPRS.write(26);              // equivalent of CTRL-Z
  delay(3000);

}
