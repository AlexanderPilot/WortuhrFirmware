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
 * Bildet die Uhrzeit ab
 * Übergabeparameter: kein
 * RückgabeParameter: Referenz auf Zeitmatrix
 **************************************************************************/
uint8_t* Muster::getTimeMatrixFut()
{
    return &TimeMatrixFut[0][0]; 
}

void Muster::setTimeMatrixFut( uint8_t *iMatrix )
{
    uint8_t *ptr = &TimeMatrixFut[0][0];
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

/***********************************************************************************************************
 * Diese Funktion erstell eine 12 x 12 Zeitmatrix, die anzeigt welche LED leuchten muss,
 * damit die Zeit richtig angezeigt wird
 * 
 * Übergabeparameter:
 *          eine [12][12] matrix des Datentyps uint8_t (nur Referenz)
 *          Stunden aus der aktuellen Zeit, der Form: uint8_t stunden
 *          Minuten aus der akteellen Zeit, der Form: uint8_t minuten
 * 
 * RückgabeParameter:
 *          keine
 * 
***********************************************************************************************************/
void Muster::setTimeMatrix( uint8_t *iMatrix, uint8_t hou, uint8_t min )
{
    word myTimeMatrix[12];
    int i = 0, j = 0;

    // Hole Zeit in der Alternform und Zwischenspeicherung in myTimeMatrix[12]. Zuvor: Reseten der Zeitmatrix --> alles auf 0
    Renderer myClass = Renderer();
    myClass.clearScreen( myTimeMatrix );
    myClass.setTime( hou, min, 1, myTimeMatrix );
    myClass.setCorners( min, myTimeMatrix );

    // In die neue Form wandeln und in der uint8_t *iMatrix ablegen
    word mask[12] = {1,2,4,8,16,32,64,128,256,512,1024,2048};
    j = 0;
    i = 0;

    // Kopieren der Bitmaske in die Zeitmatrix
    while ( j < 12 )
    {
        while( i < 12 )
        {
            *(iMatrix + 12*j + i) = ((myTimeMatrix[j]&mask[11-i]) >> (11-i));
            i ++;
        }
        
        i = 0;
        j ++;
    }

    // Nur zum Debuggen:
    printTimeMatrixTestDebug( iMatrix );

}

/***********************************************************************************************************
 * Laden der Zeitmatrix auf die Ausgabematrix ohne weiterer Übergangseffekte. D.h. die erstellte Zeitmatrix (t_Matrix)
 * wid mit der gewünschten Farbe auf die Anzeigematrix (iMatrix) geschrieben. Die erstellte anzeigematrix kann dann
 * mittels Funktion (z.B.): mLedausgabe.setPixelToColorMatrix( meineMatrix ) wiedergegeben werden.
 * 
 * Übergabeparameter:
 *          Die zeitmatrix t_Matrix der Form: uint8_t Matrix[12][12]
 *          Die Anzeigematrix die später wiedergegeben werden kann, der Form: pixel_t Matrix[12][12]
 *          Die gewünschte Farbe, der Form: pixel_t color, oder direkt z.B.: {50, 0, 0} für rot
 * 
 * Rükgabeparameter:
 *          keine
 * 
***********************************************************************************************************/
void Muster::setSimpleTimeNoEffects( uint8_t *t_Matrix, pixel_t *iMatrix, pixel_t color )
{
    for(int i = 0; i < 144; i++)
    {
        if( *(t_Matrix+i) == 1)
        {
            *(iMatrix+i) = {color.red,color.green,color.blue};
        }
        else
        {
            *(iMatrix+i) = {0,0,0};
        }
    }
}




/*************************************************************************************************************
 * 
 * Ende der Hauptfunktionen der Klassen - ab hier lediglich Funktionen zum Debuggen
 * 
*************************************************************************************************************/

// Gibt die Zeitmatrix auf des seriellen Monitor aus
void Muster::printTimeMatrixTestDebug( uint8_t *iMatrix )
{
    //DEBUG
    Serial.println("-------------------------------------------------------------------------------------------------------"); 
    int j = 0;
    int i = 0;
    while(i<12)
    {
        while (j<12)
        {
            if(j!=11)
            {
                Serial.print(*(iMatrix+12*i+j)); Serial.print("  ");
            }
            else
            {
                Serial.println(*(iMatrix+12*i+j));
            }
            j++;
        }
        
        j=0;
        i++;
    }
    Serial.println("-------------------------------------------------------------------------------------------------------");
}
