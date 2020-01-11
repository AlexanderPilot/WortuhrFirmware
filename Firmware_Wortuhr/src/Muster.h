#pragma once

#include "Arduino.h"
#include "Configurations.h"

class Muster
{
    private:
        pixel_t IstMatrix[12][12];
        pixel_t SollMatrix[12][12];
        pixel_t ArbsMatrix[12][12];

        uint8_t TimeMatrixAct[12][12];
        uint8_t TimeMatrixFut[12][12];


        // Funktionen nur zum Debuggen:
        void printTimeMatrixTestDebug( uint8_t *iMatrix );
    
    public:
        //Zeigt verschiedene Muster an
        //Übergabe Parameter m für welches Muster es sich hierbei handelt
        //Rückgabeparameter zeigt an ob die Funktion fertig ist
        uint8_t BuildMusterMatrix(pixel_t *Matrix, int typ, int pos);


        // Getter uns setter Funktionen für die Klassenmember
        pixel_t* getIstMatrix();
        pixel_t* getSollMatrix();
        pixel_t* getArbsMatrix();
        uint8_t* getTimeMatrixFut();

        void setIstMatrix( pixel_t *iMatrix );
        void setSollMatrix( pixel_t *iMatrix );
        void setArbsMatrix( pixel_t *iMatrix );
        void setTimeMatrixFut( uint8_t *t_Matrix );


        // Einstellen der Zeitmatrix
        void setTimeMatrix( uint8_t *t_Matrix, uint8_t hou, uint8_t min );


        // Laden der Zeitmatrix auf die Ausgabematrix ohne weiterer Übergangseffekte
        void setSimpleTimeNoEffects( uint8_t *t_Matrix, pixel_t *iMatrix, pixel_t color );
};
