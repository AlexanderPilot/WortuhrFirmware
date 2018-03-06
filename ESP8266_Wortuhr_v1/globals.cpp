using namespace std;
#include "globals.h"
#include "comevatiation.h"

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
unsigned int helligkeit = 0;
char farbe[6];
colorRGB farbanteile;
char befehl[6];
char bufferReader[7];
