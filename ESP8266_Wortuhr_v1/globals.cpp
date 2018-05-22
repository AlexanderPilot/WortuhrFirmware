#include "globals.h"

#include "Lights.h"

/**
 * +-------------------------------------------------------------------------------------------+
 * | Ueber die globalen Variablen laesst sich Folgendes einstellen:                              |
 * | -> Farbe der Beleuchtung                                                                  |
 * | -> Helligkeit                                                                             |
 * | -> Weitere Sachen (Platz fuer Ideen)                                                       |
 * +-------------------------------------------------------------------------------------------+
 * | Die Aenderung der Variablen, und damit der Befehle erfolgt in comevatiation.h              |
 * |                                                                                           |
 * +-------------------------------------------------------------------------------------------+
 */
// Farben aufgelöst nach einzelnen Anteilen --> Rot, Grün, Blau

// Variablendeklaration:
unsigned int helligkeit = 100;
char farbe[6];
colorRGB farbanteile;
char befehl[6];
char bufferReader[8];
eepromData configData;
