/***************************************************************************
 * Erstellt verschiedene Muster für start oder einfach zum spaß
 *
 **************************************************************************/

#include "Muster.h"

/***************************************************************************
 * Constants defines
 **************************************************************************/
#define l_on (100, 100, 100)
#define l_off (0, 0, 0)


/***************************************************************************
 * Bildet den Istzustand der Uhr ab
 * Übergabeparameter: kein
 * RückgabeParameter: Referenz auf die IstMatrix
 **************************************************************************/
pixel_t* Muster::getIstMatrix( )
{
    return &IstMatrix[0][0]; 
}

/***************************************************************************
 * Bildet den Istzustand der Uhr ab
 * Übergabeparameter: Matrix die Übernommen werden soll als Ist
 * RückgabeParameter: kein
 **************************************************************************/
void Muster::setIstMatrix( pixel_t *iMatrix )
{
    pixel_t *ptr = &IstMatrix[0][0];
    for( int i = 0; i < 144; i++)
    {
        *(ptr+i) = *(iMatrix+i);
    }
}

/***************************************************************************
 * Bildet den Istzustand der Uhr ab
 * Übergabeparameter: kein
 * RückgabeParameter: Referenz auf die SollMatrix
 **************************************************************************/
pixel_t* Muster::getSollMatrix( )
{
    return &SollMatrix[0][0];
}

void Muster::setSollMatrix( pixel_t *iMatrix )
{
    pixel_t *ptr = &SollMatrix[0][0];
    for( int i = 0; i < 144; i++)
    {
        *(ptr+i) = *(iMatrix+i);
    }
}

/***************************************************************************
 * Bildet den Istzustand der Uhr ab
 * Übergabeparameter: kein
 * RückgabeParameter: Referenz auf die ArbsMatrix
 **************************************************************************/
pixel_t* Muster::getArbsMatrix( )
{
    return &ArbsMatrix[0][0];
}

void Muster::setArbsMatrix( pixel_t *iMatrix )
{
    pixel_t *ptr = &ArbsMatrix[0][0];
    for( int i = 0; i < 144; i++)
    {
        *(ptr+i) = *(iMatrix+i);
    }
}

/***************************************************************************
 * Erstelle Zufallsfarben
 **************************************************************************/
pixel_t createRandomColor( void )
{
    return {(uint8_t)random(0,80),(uint8_t)random(0,80),(uint8_t)random(0,80)};
}


/***************************************************************************
 * Funktion zum erstellen von Mustern
 * Übergabeparameter:
 *           Referenz auf die zu verändernde Matrix: pixel *Matrix
 *           Welche Animation gewünscht ist (siehe Liste): int typ
 *           Welche Position in der Animation: int pos
 * Animationen:
 *           typ 1: Je zwei horizontal und vertikal laufende Linien
 *           typ 2: Viereck definierter Farbe (pos e [0-5])
 *           typ 3: Viereck mit Zufallsfarben (pos e [0-5])
 **************************************************************************/
uint8_t Muster::BuildMusterMatrix( pixel_t *Matrix, int typ, int pos)
{
    // Matrix der Form: pixel_t myMatrix[j][i];
    uint8_t iRet = 1;
    pixel_t iMem = {0,0,0};

    if( (typ == 1) && (pos >= 0) && (pos <= 5) ) 
    {
        int upper=(6+pos), lower=(5-pos);

        for(int j = 0; j < 12; j++)
        {
            for(int i = 0; i < 12 ;i++)
            {
                if( (i==lower) || (i==upper) || (j==lower) || (j==upper) )
                {
                    if( (j%2)==1 )
                    {
                        *(Matrix + 12*j + (11-i)) = {2,10,30};
                    }
                    else
                    {
                        *(Matrix + 12*j + i) = {2,10,30};
                    } 
                }
                else
                {
                    if( (j%2)==1 )
                    {
                        *(Matrix + 12*j + (11-i)) = {0,0,0};
                    }
                    else
                    {
                        *(Matrix + 12*j + i) = {0,0,0};
                    } 
                }
                
            }
        }
    }  
    
    else if( ((typ == 2)||(typ == 3)) && (pos >= 0) && (pos <= 5) ) 
    {
        int upper=(6+pos), lower=(5-pos);

        for(int j = 0; j < 12; j++)
        {
            for(int i = 0; i < 12 ;i++)
            {
                //Einstellung der Farbe: wenn typ==2 --> Def, wenn typ==3 --> Zufallsfarben
                if( typ == 3)
                {
                    iMem = createRandomColor();
                }
                else
                {
                    iMem = {2,10,30};
                }
                
                if( ((i==lower)&&(j<=upper)&&(j>=lower)) || ((i==upper)&&(j<=upper)&&(j>=lower)) || ((j==lower)&&(i<=upper)&&(i>=lower)) || ((j==upper)&&(i<=upper)&&(i>=lower)) )
                {
                    if( (j%2)==1 )
                    {
                        *(Matrix + 12*j + (11-i)) = iMem;
                    }
                    else
                    {
                        *(Matrix + 12*j + i) = iMem;
                    } 
                }
                else
                {
                    if( (j%2)==1 )
                    {
                        *(Matrix + 12*j + (11-i)) = {0,0,0};
                    }
                    else
                    {
                        *(Matrix + 12*j + i) = {0,0,0};
                    } 
                }
                
            }
        }
    }  
    else
    {
        iRet = 0; // Fehler, da diese Option nicht verfügbar ist
    }
    return iRet;
}