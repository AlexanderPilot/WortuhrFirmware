#pragma once

#include "WS2812B.h"	//Einstellungen der LEDs über AppInterpreter
#include "DS3231.h"		//Einstellungen der RTC über AppInterpreter


class AppInterpreter
{
private:
	// Define signal properties
	#define START_SIGN '+'
	#define END_SIGN   '$'
public:
	//Konstruktor
	AppInterpreter();

	// Initialisierung des AppInterpreters
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