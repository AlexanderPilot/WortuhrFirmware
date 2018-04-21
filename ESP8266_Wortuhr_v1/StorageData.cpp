/*
 * StorageData.cpp
 *
 *  Created on: 01.04.2018
 *      Author: vitali
 */

#include <EEPROM.h>
#include "globals.h"
#include "Arduino.h"

int cfgStart = 0;
int FlashSize = 4095;

// Reset EEPROM bytes to '0' for the length of the data structure
void eraseConfig(  ) {
  EEPROM.begin(FlashSize);
  for (int i = cfgStart ; i < (int) sizeof(configData) ; i++) {
    EEPROM.write(i, 0);
  }
  delay(200);
  EEPROM.commit();
  EEPROM.end();
}


void saveConfig(  ) {
  // Save configuration from RAM into EEPROM
  EEPROM.begin(FlashSize);
  EEPROM.put( cfgStart, configData );
  delay(200);
  EEPROM.commit();
  EEPROM.end();
}

void loadConfig(  ) {
  // Loads configuration from EEPROM into RAM
  EEPROM.begin(FlashSize);
  EEPROM.get( cfgStart, configData );
  delay(100);
  EEPROM.end();
}
