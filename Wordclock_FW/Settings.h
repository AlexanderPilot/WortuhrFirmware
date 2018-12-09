/* Header für Einstellungen */

/* Vermeidung Doppeldefinitionen */
#pragma once

#include "Arduino.h"
#include <EEPROM.h>
#include "Renderer.h"
#include "Configurations.h"

class Settings
{
      public:
            Settings();
            
            byte getLanguage();
            void setLanguage(byte language);
            
            boolean getCornersClockwise();
            void setCornersClockwise(boolean clockwise);
            
            byte getBrightness();
            void setBrightness(byte brightness);
            /*
            void loadFromEEPROM();
            void saveToEEPROM();
            */
      private:
            byte _Language;
            boolean _CornersClockwise;
            byte _Brightness;
};

