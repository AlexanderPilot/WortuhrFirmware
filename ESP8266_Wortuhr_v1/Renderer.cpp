/* Funktionen Renderer */


/* Einbinden von Headerdateien */
#include "Renderer.h"


/* Funktionen */
Renderer::Renderer()
{
  
}

void Renderer::setTime(byte Hours, byte Minutes, byte Language, word Matrix[11])
{
  switch (Language)
  {
    //Sprache Deutsch
    case LANGUAGE_DE_DE:
      
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
          // 10 nach
          DE_ZEHN;
          DE_NACH;
          setHours(Hours, false, Language, Matrix);
          break;
        case 3:
          // viertel nach
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
          // 20 nach
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
          // 5 vor halb
          DE_FUENF;
          DE_VOR;
          DE_HALB;
          setHours(Hours + 1, false, Language, Matrix);
          break;
        case 6:
          // halb
          DE_HALB;
          setHours(Hours + 1, false, Language, Matrix);
          break;
        case 7:
          // 5 nach halb
          DE_FUENF;
          DE_NACH;
          DE_HALB;
          setHours(Hours + 1, false, Language, Matrix);
          break;
        case 8:
          // 20 vor
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
          // viertel vor
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
          // 10 vor
          DE_ZEHN;
          DE_VOR;
          setHours(Hours + 1, false, Language, Matrix);
          break;
        case 11:
          // 5 vor
          DE_FUENF;
          DE_VOR;
          setHours(Hours + 1, false, Language, Matrix);
          break;
        }
        break; 
     }
}


  
void Renderer::setHours(byte Hours, boolean glatt, byte Language, word Matrix[11])
{
  switch (Language)
  {
    Serial.println("Stunden schreiben");
    case LANGUAGE_DE_DE:
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
      if (glatt) {
        DE_H_EIN;
      } 
      else {
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
    }
    break;
  }
}

void Renderer::setCorners(byte Minutes, boolean CornersClockwise, word Matrix[11])
{
  if(CornersClockwise == true)
  {
    switch (Minutes % 5)
    {
      case 0:
        break;
      case 1:
        Matrix[10] |= 0b0010000000000000;  //LED oben links
        break;
      case 2:
        Matrix[10] |= 0b0010000000000000;
        Matrix[10] |= 0b0100000000000000;  //LED oben rechts
        break;
      case 3:
        Matrix[10] |= 0b0010000000000000;
        Matrix[10] |= 0b0100000000000000;
        Matrix[10] |= 0b1000000000000000;  //LED unten rechts
        break;
      case 4:
        Matrix[10] |= 0b0010000000000000;
        Matrix[10] |= 0b0100000000000000;
        Matrix[10] |= 0b1000000000000000;
        Matrix[10] |= 0b0001000000000000;  //LED unten links
        break;
    }
  }
  else
  {
    switch (Minutes % 5)
    {
      case 0:
        break;
      case 1:
        Matrix[10] |= 0b0010000000000000;  //LED oben links
        break;
      case 2:
        Matrix[10] |= 0b0010000000000000;
        Matrix[10] |= 0b0001000000000000;  //LED unten links
        break;
      case 3:
        Matrix[10] |= 0b0010000000000000;
        Matrix[10] |= 0b0001000000000000;
        Matrix[10] |= 0b1000000000000000;  //LED unten rechts
        break;
      case 4:
        Matrix[10] |= 0b0010000000000000;
        Matrix[10] |= 0b0001000000000000;
        Matrix[10] |= 0b1000000000000000;
        Matrix[10] |= 0b0100000000000000;  //LED oben rechts
        break;
    }
  }
}


void Renderer::setAllScreenOn(word Matrix[11])
{
  for( byte i = 0; i < 11; i++)
  {
    Matrix[i] |= 0b1111111111111111;
  }
}

void Renderer::clearScreen(word Matrix[11])
{
  for( byte i = 0; i < 11; i++)
  {
    Matrix[i] = 0;
  }
}


