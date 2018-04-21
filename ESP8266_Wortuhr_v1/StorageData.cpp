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

void eraseConfig( eepromData cfg ) {
  // Reset EEPROM bytes to '0' for the length of the data structure
  EEPROM.begin(4095);
  for (int i = cfgStart ; i < (int) sizeof(cfg) ; i++) {
    EEPROM.write(i, 0);
  }
  delay(200);
  EEPROM.commit();
  EEPROM.end();
}


void saveConfig( eepromData cfg ) {
  // Save configuration from RAM into EEPROM
  EEPROM.begin(4095);
  EEPROM.put( cfgStart, cfg );
  delay(200);
  EEPROM.commit();                      // Only needed for ESP8266 to get data written
  EEPROM.end();                         // Free RAM copy of structure
}

void loadConfig( eepromData cfg ) {
  // Loads configuration from EEPROM into RAM
  EEPROM.begin(4095);
  EEPROM.get( cfgStart, cfg );
  EEPROM.end();
}
