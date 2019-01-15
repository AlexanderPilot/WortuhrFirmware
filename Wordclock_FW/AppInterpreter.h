#pragma once

#include "Settings.h"    //Einstellungen ausschließlich über Settings Klasse

class AppInterpreter
{
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
        void setUpCommand();
    
        // Loeschen eines Strings
        void deleteString(char a[]);
};
