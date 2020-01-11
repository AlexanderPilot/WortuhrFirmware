/***************************************************************************
 * Funktionen für die Interpretation der Uhrzeit in Bitmuster für LED-Matrix (Rendern)
 *
 **************************************************************************/


/* Einbinden von Headerdateien */
#include "Renderer.h"


/* Funktionen */
Renderer::Renderer()
{
      
}

void Renderer::setTime(byte Hours, byte Minutes, byte Language, word Matrix[12])
{
      DE_ESIST; 
      switch(Minutes / 5)
      {
            case 0:
                  //Glatte Stunde
                  setHours(Hours, true, Language, Matrix);
                  break;
            case 1:
                  //5 nach
                  DE_FUENF;
                  DE_NACH;
                  setHours(Hours, false, Language, Matrix);
                  break;
            case 2:
                  //10 nach
                  DE_ZEHN;
                  DE_NACH;
                  setHours(Hours, false, Language, Matrix);
                  break;
            case 3:
                  //viertel nach
                  if ((Language == LANGUAGE_DE_SW) || (Language == LANGUAGE_DE_SA))
                  {
                        DE_VIERTEL;
                        setHours(Hours + 1, false, Language, Matrix);
                  }
                  else
                  {
                        DE_VIERTEL;
                        DE_NACH;
                        setHours(Hours, false, Language, Matrix);
                  }
                  break;
            case 4:
                  //20 nach
                  if (Language == LANGUAGE_DE_SA)
                  {
                  DE_ZEHN;
                  DE_VOR;
                  DE_HALB;
                  setHours(Hours + 1, false, Language, Matrix);
                  }
                  else
                  {
                  DE_ZWANZIG;
                  DE_NACH;
                  setHours(Hours, false, Language, Matrix);
                  }
                  break;
            case 5:
                  //5 vor halb
                  DE_FUENF;
                  DE_VOR;
                  DE_HALB;
                  setHours(Hours + 1, false, Language, Matrix);
                  break;
            case 6:
                  //halb
                  DE_HALB;
                  setHours(Hours + 1, false, Language, Matrix);
                  break;
            case 7:
                  //5 nach halb
                  DE_FUENF;
                  DE_NACH;
                  DE_HALB;
                  setHours(Hours + 1, false, Language, Matrix);
                  break;
            case 8:
                  //20 vor
                  if (Language == LANGUAGE_DE_SA)
                  {
                        DE_ZEHN;
                        DE_NACH;
                        DE_HALB;
                        setHours(Hours + 1, false, Language, Matrix);
                  }
                  else
                  {
                        DE_ZWANZIG;
                        DE_VOR;
                        setHours(Hours + 1, false, Language, Matrix);
                  }
                  break;
            case 9:
                  //viertel vor
                  if ((Language == LANGUAGE_DE_SW) || (Language == LANGUAGE_DE_BA) || (Language == LANGUAGE_DE_SA))
                  {
                        DE_DREIVIERTEL;
                        setHours(Hours + 1, false, Language, Matrix);
                  }
                  else
                  {
                        DE_VIERTEL;
                        DE_VOR;
                        setHours(Hours + 1, false, Language, Matrix);
                  }
                  break;
            case 10:
                  //10 vor
                  DE_ZEHN;
                  DE_VOR;
                  setHours(Hours + 1, false, Language, Matrix);
                  break;
            case 11:
                  //5 vor
                  DE_FUENF;
                  DE_VOR;
                  setHours(Hours + 1, false, Language, Matrix);
                  break;
            default: break;
      }
}

  
void Renderer::setHours(byte Hours, boolean glatt, byte Language, word Matrix[12])
{
      Serial.print("Wert von glatt: ");
      Serial.println(glatt);
      if(glatt == true)
      {
            DE_UHR;
      }
      switch(Hours)
      {
            case 0:
            case 12:
            case 24:
                  DE_H_ZWOELF;
                  break;
            case 1:
            case 13:
                  if (glatt)
                  {
                        DE_H_EIN;
                  } 
                  else
                  {
                        DE_H_EINS;
                  }
                  break;
            case 2:
            case 14:
                  DE_H_ZWEI;
                  break;
            case 3:
            case 15:
                  DE_H_DREI;
                  break;
            case 4:
            case 16:
                  DE_H_VIER;
                  break;
            case 5:
            case 17:
                  DE_H_FUENF;
                  break;
            case 6:
            case 18:
                  DE_H_SECHS;
                  break;
            case 7:
            case 19:
                  DE_H_SIEBEN;
                  break;
            case 8:
            case 20:
                  DE_H_ACHT;
                  break;
            case 9:
            case 21:
                  DE_H_NEUN;
                  break;
            case 10:
            case 22:
                  DE_H_ZEHN;
                  break;
            case 11:
            case 23:
                  DE_H_ELF;
                  break;
            default: break;
      }
}

void Renderer::setCorners(byte Minutes, word Matrix[12])
{
      if( (Minutes%5) == 1 )
      {
            (*Matrix) |= 0b100000000000;
      }
      if( (Minutes%5) == 2 )
      {
            *Matrix |= 0b100000000001;
      }
      if( (Minutes%5) == 3 )
      {
            *Matrix |= 0b100000000001;
            *(Matrix+11) |= 0b000000000001;
      }
      if( (Minutes%5) == 4 )
      {
            *Matrix |= 0b100000000001;
            *(Matrix+11) |= 0b100000000001;
      }
}

void Renderer::cleanWordsMode(byte language, word Matrix[12])
{
      switch (language)
      {
            case LANGUAGE_DE_DE:
            case LANGUAGE_DE_SW:
            case LANGUAGE_DE_BA:
            case LANGUAGE_DE_SA:
                  Matrix[0] &= 0b0010001111111111; // ES IST weg
                  break;
            case LANGUAGE_EN:
                  Matrix[0] &= 0b0010011111111111; // IT IS weg
                  break;
      }
}

void Renderer::setAllScreenOn(word Matrix[12])
{
      for( byte i = 0; i <= (NUM_COLUMN); i++)
      {
            Matrix[i] |= 0b1111111111111111;
      }
}

void Renderer::clearScreen(word Matrix[12])
{
      for( byte i = 0; i <= (NUM_COLUMN); i++)
      {
            Matrix[i] = 0;
      }
}
