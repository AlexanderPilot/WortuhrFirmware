/***************************************************************************
 * In dieser Funktion wird die Zeit gesetzt und zurückgegeben
 *
 **************************************************************************/

/** Vermeidung Doppeldefinitionen **/
#pragma once

/** Einbinden von relevanten Bibliotheken **/
#include "Arduino.h"
#include "Configurations.h"

#define DEBUG_NTPTIME 0

struct strDateTime
{
  byte hour;
  byte minute;
  byte second;
  int year;
  byte month;
  byte day;
  byte dayofWeek;
  unsigned long epochTime;
  boolean valid;
};

class NTPtime {
  public:
    NTPtime(String NTPtime);
    strDateTime getNTPtime(float _timeZone, int _DayLightSaving);
    void printDateTime(strDateTime _dateTime);
    bool setSendInterval(unsigned long _sendInterval);  // in seconds
    bool setRecvTimeout(unsigned long _recvTimeout);    // in seconds

  private:
    bool _sendPhase;
    unsigned long _sentTime;
    unsigned long _sendInterval;
    unsigned long _recvTimeout;
    String _NTPserver;
    float _timeZone;

    strDateTime ConvertUnixTimestamp( unsigned long _tempTimeStamp);
    boolean summerTime(unsigned long _timeStamp );
    boolean daylightSavingTime(unsigned long _timeStamp);
    unsigned long adjustTimeZone(unsigned long _timeStamp, float _timeZone, int _DayLightSavingSaving);
    WiFiUDP UDPNTPClient;
};
