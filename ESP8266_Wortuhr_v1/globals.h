#ifndef GLOBALS_H
#define GLOBALS_H


// Helligkeit in 0-100%, wobei empfangen wird 0 - 99999 + '\0'
extern unsigned int helligkeit;
// Farben in RGB-Format --> in hex
extern char farbe[6];
// Farben aufgel�st nach einzelnen Anteilen --> Rot, Gr�n, Blau
struct colorRGB{
	int red;
	int green;
	int blue;
};
extern colorRGB farbanteile;
// Platz f�r Ideen
extern char befehl[6];
// Zum einlesen von neuen Befehlen
extern char bufferReader[7];

#endif