#pragma once

#include "Arduino.h"
#include "Configurations.h"

class Muster
{
    private:
        pixel_t IstMatrix[12][12];
        pixel_t SollMatrix[12][12];
        pixel_t ArbsMatrix[12][12];
    
    public:
        //Zeigt verschiedene Muster an
        //Übergabe Parameter m für welches Muster es sich hierbei handelt
        //Rückgabeparameter zeigt an ob die Funktion fertig ist
        uint8_t BuildMusterMatrix(pixel_t *Matrix, int typ, int pos);
        pixel_t* getIstMatrix();
        pixel_t* getSollMatrix();
        pixel_t* getArbsMatrix();
        void setIstMatrix( pixel_t *iMatrix );
        void setSollMatrix( pixel_t *iMatrix );
        void setArbsMatrix( pixel_t *iMatrix );

};