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

void Renderer::randomMatrix(word Matrix[11])
{
      for (byte i = 0; i < 11; i++)
      {
            Matrix[i] = random(65536);
      }
}

void Renderer::setTime(byte Hours, byte Minutes, byte Language, word Matrix[11])
{
      switch (Language)
      {
            //Sprache Deutsch
            case LANGUAGE_DE_DE:
            case LANGUAGE_DE_SW:
            case LANGUAGE_DE_BA:
            case LANGUAGE_DE_SA:
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
                  }
                  break;
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            case LANGUAGE_EN:
                  EN_ITIS;
                  
                  switch (Minutes / 5)
                  {
                        case 0:
                              // glatte Stunde
                              setHours(Hours, true, Language, Matrix);
                              break;
                        case 1:
                              // 5 nach
                              EN_FIVE;
                              EN_PAST;
                              setHours(Hours, false, Language, Matrix);
                              break;
                        case 2:
                              // 10 nach
                              EN_TEN;
                              EN_PAST;
                              setHours(Hours, false, Language, Matrix);
                              break;
                        case 3:
                              // viertel nach
                              EN_A;
                              EN_QUATER;
                              EN_PAST;
                              setHours(Hours, false, Language, Matrix);
                              break;
                        case 4:
                              // 20 nach
                              EN_TWENTY;
                              EN_PAST;
                              setHours(Hours, false, Language, Matrix);
                              break;
                        case 5:
                              // 5 vor halb
                              EN_TWENTY;
                              EN_FIVE;
                              EN_PAST;
                              setHours(Hours, false, Language, Matrix);
                              break;
                        case 6:
                              // halb
                              EN_HALF;
                              EN_PAST;
                              setHours(Hours, false, Language, Matrix);
                              break;
                        case 7:
                              // 5 nach halb
                              EN_TWENTY;
                              EN_FIVE;
                              EN_TO;
                              setHours(Hours + 1, false, Language, Matrix);
                              break;
                        case 8:
                              // 20 vor
                              EN_TWENTY;
                              EN_TO;
                              setHours(Hours + 1, false, Language, Matrix);
                              break;
                        case 9:
                              // viertel vor
                              EN_A;
                              EN_QUATER;
                              EN_TO;
                              setHours(Hours + 1, false, Language, Matrix);
                              break;
                        case 10:
                              // 10 vor
                              EN_TEN;
                              EN_TO;
                              setHours(Hours + 1, false, Language, Matrix);
                              break;
                        case 11:
                              // 5 vor
                              EN_FIVE;
                              EN_TO;
                              setHours(Hours + 1, false, Language, Matrix);
                              break;
                        }
            break;
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //case XY
     }
}


  
void Renderer::setHours(byte Hours, boolean glatt, byte Language, word Matrix[11])
{
      switch (Language)
      {
            _DEBUG_PRINTLN("Stunden schreiben");
            case LANGUAGE_DE_DE:
            case LANGUAGE_DE_SW:
            case LANGUAGE_DE_BA:
            case LANGUAGE_DE_SA:
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
                  }
                  break;
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            case LANGUAGE_EN:
                  if (glatt)
                  {
                        EN_OCLOCK;
                  }
                  
                  switch (Hours) {
                        case 0:
                        case 12:
                        case 24:
                              EN_H_TWELVE;
                              break;
                        case 1:
                        case 13:
                              EN_H_ONE;
                              break;
                        case 2:
                        case 14:
                              EN_H_TWO;
                              break;
                        case 3:
                        case 15:
                              EN_H_THREE;
                              break;
                        case 4:
                        case 16:
                              EN_H_FOUR;
                              break;
                        case 5:
                        case 17:
                              EN_H_FIVE;
                              break;
                        case 6:
                        case 18:
                              EN_H_SIX;
                              break;
                        case 7:
                        case 19:
                              EN_H_SEVEN;
                              break;
                        case 8:
                        case 20:
                              EN_H_EIGHT;
                              break;
                        case 9:
                        case 21:
                              EN_H_NINE;
                              break;
                        case 10:
                        case 22:
                              EN_H_TEN;
                              break;
                        case 11:
                        case 23:
                              EN_H_ELEVEN;
                              break;
            }
            break;
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

void Renderer::cleanWordsMode(byte language, word Matrix[16])
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
