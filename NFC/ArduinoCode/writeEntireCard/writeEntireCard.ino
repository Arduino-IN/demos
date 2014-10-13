/**************************************************************************/
/*!
    This example will wait for a Mifare Classic 1K card or tag, and
    will attempt to write to all the blocks in it.

    For a Mifare Classic card, the following steps are taken:

    - Authenticate block 4 (the first block of Sector 1) using
      the default KEYA of 0XFF 0XFF 0XFF 0XFF 0XFF 0XFF
    - If authentication succeeds, we can then write any of the
      4 blocks in that sector

*/
/**************************************************************************/

// Header files for using PN532 over SPI
#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"

// pin 10 is the slave select line
PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);

void printData(uint8_t* data) {
  for (int i = 0; i < 16; ++i)
  {
    Serial.print(data[i]);
    //          Serial.print(",");
  }
}


void setup(void) {
  Serial.begin(115200);
  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A Card ...");
}


void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");

    // The Mifare Classic 1K cards have 4 byte UIDs
    if (uidLength == 4)
    {
      Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

      Serial.println("Trying to authenticate block with default KEYA value");
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

      for (int i = 1; i < 16; ++i) {
        for (int j = 0; j < 3; ++j) {
          success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4 * i + j, 0, keya);
          if (success)
          {
            uint8_t data[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
            data[i] = i;
            success = nfc.mifareclassic_WriteDataBlock(4 * i + j, data);

            if (success) {
              Serial.print("Succesfully wrote ");
              Serial.print(i);
              Serial.print(" ");
              Serial.println(j);
              printData(data);
              Serial.println();
              delay(100);
            }
          }
          else
          {
            Serial.println("Ooops... unable to read the requested block.  Try another key?");
            delay(5000);
          }
        }
      }
    }
  }
}


