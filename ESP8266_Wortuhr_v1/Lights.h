#ifndef LIGHTS_H
#define LIGHTS_H
#include "globals.h"

class Lights
{
private:
	// Define signal properties
	#define START_SIGN '+'
	#define END_SIGN   '$'
public:
	// Initialisierung der Lichter
	void setup();

	// Muss in der main hinter dem serverClients[i].read(); aufgerufen werden
	// Liesst die Zeichen und stellt die Helligkeit, Farbe und Weiteres ein
	bool comevatiation(char getChar);

	// Aufzurufen wenn comevatiation() ein true liefert
	// Setzt den Eingangsstring um
	void setUpCommand( void );

	// Loeschen eines Strings
	void deleteString(char a[]);
};


#endif
