#include<SoftwareSerial.h>

// LED connected to pin 4 on Arduino
int LEDpin = 4;

// Create softserial object GSM, on pins 5/RX & 6/TX
SoftwareSerial GSM(5, 6);

void setup()
{
  pinMode(LEDpin, OUTPUT);
  GSM.begin(9600);               // the GSM baud rate
  Serial.begin(9600);             // the Serial port of Arduino baud rate.
  delay(500);
}

void loop() {
  checkSMS();
  delay(100);
}

void checkSMS() {
  char smsData[10];        // Buffer for SMS text
  while (GSM.available()) {  
    Serial.println("Got data over GSM serial");
    // sequentially check the characters for +CMT, which is received when a new SMS arrives
    // This only works when the GSM modem is set to directly send the SMS to serial port. 
    // AT command to set this mode is AT+CNMI=3,2,0,0
    if (GSM.read() == '+') {    
      if (GSM.read() == 'C') {
        if (GSM.read() == 'M') {
          if (GSM.read() == 'T') {
            Serial.println("Got a new SMS");
            while ( GSM.read() != '=');    // Read and discard all data until we encounter an '=' sign
            smsData[0] = GSM.read();      // Read the first character after '=' and save it to smsData
            smsData[1] = GSM.read();
            smsData[2] = GSM.read();
            smsData[3] = '\0';
            Serial.print("smsData :");    // Print the smsData
            Serial.print(smsData);
            if (smsData[0] == '1')      // If the data holds a '1' then turn the LED on, otherwise turn it off
              digitalWrite(LEDpin, HIGH);
            else                        
              digitalWrite(LEDpin, LOW);
          }
        }
      }
    }
  }
}

