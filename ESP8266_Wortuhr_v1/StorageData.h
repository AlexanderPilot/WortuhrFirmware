/*
 * StorageData.h
 *
 *  Created on: 01.04.2018
 *      Author: vitali
 */

#ifndef STORAGEDATA_H_
#define STORAGEDATA_H_

#include <EEPROM.h>
#include "globals.h"


	void eraseConfig( eepromData cfg );
	void saveConfig ( eepromData cfg );
	void loadConfig ( eepromData cfg );




#endif /* STORAGEDATA_H_ */
