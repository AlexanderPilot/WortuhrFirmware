/***************************************************************************
 * Funktionen für die Einstellungsmöglichkeiten der Wortuhr
 *
 **************************************************************************/

/* Einbinden von Headerdateien */
#include "Settings.h"

/****************************************
 * Definition Objekte
 ***************************************/
Preferences preferences; //Infos: https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
WebServer server(80); //Webserver für OTA Update
Zeitmaster *pSettingsZeit;

/****************************************
 * Definition der static Variablen
 ***************************************/
byte Settings::_Language;
byte Settings::_Brightness;
pixel_t Settings::_Color;
byte Settings::_FadeMode;
byte Settings::_CornerStartLed;
boolean Settings::_CornersClockwise;
byte Settings::_StartPattern;
uint16_t Settings::_GmtTimeOffsetSec;
char Settings::_SSID_Array[32] = "OnLine";
char Settings::_PW_Array[64] = "Br8#Pojg56";

/* Style */
String style =
"<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
"input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
"#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
"#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
"form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

/* Login page */
String loginIndex = 
"<form name=loginForm>"
"<h1>Wordclock Firmware Update</h1>"
"<input name=userid placeholder='User ID'> "
"<input name=pwd placeholder=Password type=Password> "
"<input type=submit onclick=check(this.form) class=btn value=Login></form>"
"<script>"
"function check(form) {"
"if(form.userid.value=='admin' && form.pwd.value=='admin')"
"{window.open('/serverIndex')}"
"else"
"{alert('Error Password or Username')}"
"}"
"</script>" + style;
 
/* Server Index Page */
String serverIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
"<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
"<label id='file-input' for='file'>   Choose file...</label>"
"<input type='submit' class=btn value='Update'>"
"<br><br>"
"<div id='prg'></div>"
"<br><div id='prgbar'><div id='bar'></div></div><br></form>"
"<script>"
"function sub(obj){"
"var fileName = obj.value.split('\\\\');"
"document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
"};"
"$('form').submit(function(e){"
"e.preventDefault();"
"var form = $('#upload_form')[0];"
"var data = new FormData(form);"
"$.ajax({"
"url: '/update',"
"type: 'POST',"
"data: data,"
"contentType: false,"
"processData:false,"
"xhr: function() {"
"var xhr = new window.XMLHttpRequest();"
"xhr.upload.addEventListener('progress', function(evt) {"
"if (evt.lengthComputable) {"
"var per = evt.loaded / evt.total;"
"$('#prg').html('progress: ' + Math.round(per*100) + '%');"
"$('#bar').css('width',Math.round(per*100) + '%');"
"}"
"}, false);"
"return xhr;"
"},"
"success:function(d, s) {"
"console.log('success!') "
"},"
"error: function (a, b, c) {"
"}"
"});"
"});"
"</script>" + style;

/****************************************
 * Konstruktor mit Standardeinstellungen
 ***************************************/
Settings::Settings()
{
    _Language = LANGUAGE_DE_DE;
    _Brightness = 100;
    _Color.red = 30;
    _Color.green = 30;
    _Color.blue = 30;
    _FadeMode = 0;
    _CornerStartLed = 0;
    _CornersClockwise = 1;
    _StartPattern = 1;
    _GmtTimeOffsetSec = 3600;
}

/***************************************************************************
 * Setter Funktion für Setzen der Sprache
 * Übergabeparameter: Sprache
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::setLanguage(byte Language)
{
    if(_Language != Language)
    {
        writeLanguageToPreferences(Language);
        loadLanguageFromPreferences();
    }
}

/***************************************************************************
 * Getter Funktion fürs Auslesen der Sprache
 * Übergabeparameter: kein
 * Rückgabeparameter: Sprache
 **************************************************************************/
byte Settings::getLanguage()
{
    return _Language;
}

/***************************************************************************
 * Setter Funktion für Setzen der Helligkeit
 * Übergabeparameter: Helligkeitswert
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::setBrightnessPercent(byte Brightness)
{
    if(_Brightness != map(Brightness, 0, 100, 0, 255))
    {
        writeBrightnessToPreferences(map(Brightness, 0, 100, 0, 255));
        loadBrightnessFromPreferences();
    }
}

/***************************************************************************
 * Getter Funktion fürs Auslesen der Helligkeit
 * Übergabeparameter: kein
 * Rückgabeparameter: Helligkeitswert
 **************************************************************************/
byte Settings::getBrightnessPercent()
{
    return map(_Brightness, 0, 255, 0, 100);
}

/***************************************************************************
 * Setter Funktion für Setzen der Farbe
 * Übergabeparameter: Farbe (color_t)
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::setColor(pixel_t color)
{
    if (DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.println("Übergabe der Farbe - color");
    }
    if((_Color.red != color.red) || (_Color.green != color.green) || (_Color.blue != color.blue))
    {
        writeColorToPreferences(color);
        loadColorFromPreferences();
    }
}

/***************************************************************************
 * Setter Funktion für Setzen der Farbe
 * Übergabeparameter: Farbe (RGB)
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::setColor(byte red, byte green, byte blue)
{
    if((_Color.red != red) || (_Color.green != green) || (_Color.blue != blue))
    {
        writeColorToPreferences(red, green, blue);
        loadColorFromPreferences();
    }
}

/***************************************************************************
 * Getter Funktion fürs Auslesen der Farbe
 * Übergabeparameter: kein
 * Rückgabeparameter: Farbe (color_t)
 **************************************************************************/
pixel_t Settings::getColor()
{
    return _Color;
}

/***************************************************************************
 * Setter Funktion für Setzen des FadeMode
 * Übergabeparameter: FadeMode
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::setFadeMode(byte FadeMode)
{
    if(_FadeMode != FadeMode)
    {
        writeFadeModeToPreferences(FadeMode);
        loadFadeModeFromPreferences();
    }
}

/***************************************************************************
 * Getter Funktion fürs Auslesen des FadeMode
 * Übergabeparameter: kein
 * Rückgabeparameter: FadeMode
 **************************************************************************/
byte Settings::getFadeMode()
{
    return _FadeMode;
}

/***************************************************************************
 * Setter Funktion für Setzen der Start Eck-LED
 * Übergabeparameter: Start Eck-LED
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::setCornerStartLed(byte CornerStartLed)
{
    if(_CornerStartLed != CornerStartLed)
    {
        writeCornerStartLedToPreferences(CornerStartLed);
        loadCornerStartLedFromPreferences();
    }
    
}

/***************************************************************************
 * Getter Funktion fürs Auslesen der Start Eck-LED
 * Übergabeparameter: kein
 * Rückgabeparameter: Start Eck-LED
 **************************************************************************/
byte Settings::getCornerStartLed()
{
    return _CornerStartLed;
}

/***************************************************************************
 * Setter Funktion für Setzen der Drehrichtung der Eck-LEDs
 * Übergabeparameter: Drehrichtung
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::setCornersClockwise(boolean CornersClockwise)
{
    if(_CornersClockwise != CornersClockwise)
    {
        writeCornerClockwiseToPreferences(CornersClockwise);
        loadCornerClockwiseFromPreferences();
    }
}

/***************************************************************************
 * Getter Funktion fürs Auslesen der Drehrichtung der Eck-LEDs
 * Übergabeparameter: kein
 * Rückgabeparameter: Drehrichtung
 **************************************************************************/
boolean Settings::getCornersClockwise()
{
    return _CornersClockwise;
}

/***************************************************************************
 * Setter Funktion für Setzen des Startmusters
 * Übergabeparameter: Startmuster
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::setStartPattern(byte StartPattern)
{
    if(_StartPattern != StartPattern)
    {
        writeStartpatternToPreferences(StartPattern);
        loadStartpatternFromPreferences();
    }
}

/***************************************************************************
 * Getter Funktion fürs Auslesen des Startmusters
 * Übergabeparameter: kein
 * Rückgabeparameter: Startmuster
 **************************************************************************/
byte Settings::getStartPattern()
{
    return _StartPattern;
}

/***************************************************************************
 * Setter Funktion für Setzen des GMT Zeitoffsets
 * Übergabeparameter: GMT Zeitoffset
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::setGmtTimeOffsetSec(uint16_t GmtTimeOffsetSec)
{
    if(_GmtTimeOffsetSec != GmtTimeOffsetSec)
    {
        writeGmtOffsetToPreferences(GmtTimeOffsetSec);
        loadGmtOffsetFromPreferences();
    }
}

/***************************************************************************
 * Getter Funktion fürs Auslesen des GMT Zeitoffsets
 * Übergabeparameter: kein
 * Rückgabeparameter: GMT Zeitoffset
 **************************************************************************/
uint16_t Settings::getGmtTimeOffsetSec()
{
    return _GmtTimeOffsetSec;
}

/***************************************************************************
 * Prüfen ob gültige WiFi Daten zur Verfügung stehen
 * Übergabeparameter: kein, da werte direkt aus den Preferences gelesen wird
 * Rückgabeparameter: Info ob gültige Daten zur verfügung stehen
 **************************************************************************/
bool Settings::getWifiSettingsAvailable()
{
    bool wifiavailable = false;
    loadSsidFromPreferences();
    loadPasswordFromPreferences();
    if(strlen(_SSID_Array) != 0 && strlen(_PW_Array) != 0)
    {
        wifiavailable = true;
    }
    return wifiavailable;
}

void Settings::setWifiSSID(char* ssid)
{
    uint8_t i = 0;
    while(*(ssid+i) != '\n')
    {
        _SSID_Array[i] = *(ssid+i);
        i++;
    }
    while(i<64)
    {
        _SSID_Array[i] = '\0';
        i++;
    }
    writeSsidToPreferences(_SSID_Array);
}

void Settings::setWifiPW(char *pw)
{
    uint8_t i = 0;
    while(*(pw+i) != '\n')
    {
        _PW_Array[i] = *(pw+i);
        i++;
    }
    while(i<64)
    {
        _PW_Array[i] = '\0';
        i++;
    }
    writePasswordToPreferences(_PW_Array);
}

/***************************************************************************
 * Auslesen der WiFi SSID  aus Settings
 * Übergabeparameter: kein, da der Wert direkt aus der Klassenvariable gelesen wird
 * Rückgabeparameter: char* auf Array mit WiFi SSID
 **************************************************************************/
char* Settings::getWifiSSID()
{
    return _SSID_Array;
}

/***************************************************************************
 * Auslesen des WiFi Passworts aus Settings
 * Übergabeparameter: kein, da der Wert direkt aus der Klassenvariable gelesen wird
 * Rückgabeparameter: char* auf Array mit WiFi Passwort
 **************************************************************************/
char* Settings::getWifiPW()
{
    return _PW_Array;
}


/***************************************************************************
 * Prüfen ob Einstellungen in Prefrences vorhanden sind
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: Info ob Daten verfügbar sind
 **************************************************************************/
bool Settings::allDataAvailable()
{
    bool dataAvailable = false;
    preferences.begin("settings", false);
    //prüfen ob alle relevanten Keys angelegt sind
    if(preferences.isKey("language") && preferences.isKey("brightness") && preferences.isKey("colorred") && \
        preferences.isKey("colorgreen") && preferences.isKey("colorblue") &&  preferences.isKey("fademode") && \
         preferences.isKey("cornerstartled") && preferences.isKey("cornerclockwise") && preferences.isKey("startpattern") && \
          preferences.isKey("gmtoffset") && preferences.isKey("wifissid") && preferences.isKey("wifipw"))
    {
        dataAvailable = true;
    }
    if(DEBUG_SETTINGS == 0)
    {
        Serial.print("Settings.cpp - ");
        Serial.println("Auswertung Daten in Preferences");
        Serial.println("Daten vorhanden:");
        Serial.print("Sprache: ");
        Serial.println(preferences.isKey("language"));
        Serial.print("Helligkeit: ");
        Serial.println(preferences.isKey("brightness"));
        Serial.print("Farbe rot: ");
        Serial.println(preferences.isKey("colorred"));
        Serial.print("Farbe grün: ");
        Serial.println(preferences.isKey("colorgreen"));
        Serial.print("Farbe blau: ");
        Serial.println(preferences.isKey("colorblue"));
        Serial.print("FadeMode: ");
        Serial.println(preferences.isKey("fademode"));
        Serial.print("Start Eck-LED: ");
        Serial.println(preferences.isKey("cornerstartled"));
        Serial.print("Drehrichtung: ");
        Serial.println(preferences.isKey("cornerclockwise"));
        Serial.print("Startmuster: ");
        Serial.println(preferences.isKey("startpattern"));
        Serial.print("GMT Zeitoffset: ");
        Serial.println(preferences.isKey("gmtoffset"));
        Serial.print("WiFi SSID: ");
        Serial.println(preferences.isKey("wifissid"));
        Serial.print("WiFi Passwort: ");
        Serial.println(preferences.isKey("wifipw"));
    }
    preferences.end();
    return dataAvailable;
}

/***************************************************************************
 * Auslesen aller Einstellungen aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: Farbe als pixel_t struct
 **************************************************************************/
void Settings::loadDataFromPreferences()
{
    this->loadLanguageFromPreferences();
    this->loadBrightnessFromPreferences();
    this->loadColorFromPreferences();
    this->loadFadeModeFromPreferences();
    this->loadCornerStartLedFromPreferences();
    this->loadCornerClockwiseFromPreferences();
    this->loadStartpatternFromPreferences();
    this->loadGmtOffsetFromPreferences();
    this->loadSsidFromPreferences();
    this->loadPasswordFromPreferences();
    if(DEBUG_SETTINGS == 1)
    {
        Serial.println("Geladene Einstellungen:");
        Serial.print("Sprache: ");
        Serial.println(_Language);
        Serial.print("Helligkeit: ");
        Serial.println(_Brightness);
        Serial.print("Farbe - rot: ");
        Serial.println(_Color.red);
        Serial.print("Farbe - green: ");
        Serial.println(_Color.green);
        Serial.print("Farbe - blue: ");
        Serial.println(_Color.blue);
        Serial.print("FadeMode: ");
        Serial.println(_FadeMode);
        Serial.print("Start Eck-LED: ");
        Serial.println(_CornerStartLed);
        Serial.print("Drehrichtung Eck-Leds: ");
        Serial.println(_CornersClockwise);
        Serial.print("Startmuster: ");
        Serial.println(_StartPattern);
        Serial.print("GMT Offset: ");
        Serial.println(_GmtTimeOffsetSec);
        Serial.print("WiFi SSID: ");
        Serial.println(_SSID_Array);
        Serial.print("WiFi PW: ");
        Serial.println(_PW_Array);
        Serial.println("");
    }
}

/***************************************************************************
 * Schreiben aller Einstellungen in Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Klassenobjekten gelesen werden
 * Rückgabeparameter: kein, da die Objekte direkt in die Preferences geschrieben werden
 **************************************************************************/
void Settings::writeDataToPreferences()
{
    this->writeLanguageToPreferences(_Language);
    this->writeBrightnessToPreferences(_Brightness);
    this->writeColorToPreferences(_Color);
    this->writeFadeModeToPreferences(_FadeMode);
    this->writeCornerStartLedToPreferences(_CornerStartLed);
    this->writeCornerClockwiseToPreferences(_CornersClockwise);
    this->writeStartpatternToPreferences(_StartPattern);
    this->writeGmtOffsetToPreferences(_GmtTimeOffsetSec);
    this->writePasswordToPreferences(_PW_Array);
    this->writeSsidToPreferences(_SSID_Array);
}

/***************************************************************************
 * Zurücksetzen der Preferences
 * Übergabeparameter: kein
 * Rückgabeparameter: kein
 **************************************************************************/
void Settings::clearPreferences()
{
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.println("Löschen der Daten in Preferences");
    }
    preferences.begin("settings", false);
    preferences.clear();
    preferences.end();
}

/***************************************************************************
 * Initialisierung und Start von WiFi
 * Übergabeparameter: kein
 * Rückgabeparameter: kein
 **************************************************************************/
void Settings::startWifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(WIFI_HOSTNAME);
    WiFi.begin(getWifiSSID(), getWifiPW());
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Connecting to SSID: ");
        Serial.print(getWifiSSID());
    }
    while (WiFi.status() != WL_CONNECTED)
    {
        if(DEBUG_SETTINGS == 1)
        {
            Serial.print('.');
        }
        delay(500);
    }
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("successful with IP: ");
        Serial.println(WiFi.localIP());
    }
}

void getTime()
{
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    Serial.print("Day of week: ");
    Serial.println(&timeinfo, "%A");
    Serial.print("Month: ");
    Serial.println(&timeinfo, "%B");
    Serial.print("Day of Month: ");
    Serial.println(&timeinfo, "%d");
    Serial.print("Year: ");
    Serial.println(&timeinfo, "%Y");
    Serial.print("Hour: ");
    Serial.println(&timeinfo, "%H");
    Serial.print("Hour (12 hour format): ");
    Serial.println(&timeinfo, "%I");
    Serial.print("Minute: ");
    Serial.println(&timeinfo, "%M");
    Serial.print("Second: ");
    Serial.println(&timeinfo, "%S");

    Serial.println("Time variables");
    char timeHour[3];
    strftime(timeHour,3, "%H", &timeinfo);
    Serial.println(timeHour);
    char timeWeekDay[10];
    strftime(timeWeekDay,10, "%A", &timeinfo);
    Serial.println(timeWeekDay);
    Serial.println();
}

/***************************************************************************
 * Starten der Uhrzeitabfrage über NTP
 * Übergabeparameter: kein
 * Rückgabeparameter: kein
 **************************************************************************/
void Settings::startNtp()
{
    //Konfiguration des NTP Servers
    configTime(_GmtTimeOffsetSec, 3600, NTP_SERVER_NAME);
    if(DEBUG_SETTINGS == 1)
    {
        Serial.println("Konfiguration NTP Server eingerichtet");
    }
    delay(500);
}

/***************************************************************************
 * Starten des Over The Air Update Servers
 * Übergabeparameter: kein
 * Rückgabeparameter: kein
 **************************************************************************/
void Settings::startOTA()
{


/*use mdns for host name resolution*/
  if (!MDNS.begin(SERVER_HOSTNAME)) //http://wordclock.local
  { 
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []()
  {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []()
  {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []()
  {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []()
  {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) //start with max available size
      {
        Update.printError(Serial);
      }
    }
    else if (upload.status == UPLOAD_FILE_WRITE) /* flashing firmware to ESP*/
    {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
      {
        Update.printError(Serial);
      }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
      if (Update.end(true)) //true to set the size to the current progress
      {
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      }
      else
      {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
}

/***************************************************************************
 * Automatischer Reconnect zum Wifi
 * Übergabeparameter: kein
 * Rückgabeparameter: kein
 **************************************************************************/
void Settings::WifiAutoReconnect()
{
    if(getWifiSettingsAvailable() == true)
    {
        if(WiFi.status() != WL_CONNECTED)
        {
            
        }
    }
}

/***************************************************************************
 * Automatischer Reconnect zum Wifi
 * Übergabeparameter: kein
 * Rückgabeparameter: kein
 **************************************************************************/
void Settings::NtpTimeUpdate()
{
    
}

/***************************************************************************
 * handle over the air updates
 * Übergabeparameter: kein
 * Rückgabeparameter: kein
 **************************************************************************/
void Settings::handleOTA()
{
    server.handleClient();
}

/***************************************************************************
 * Auslesen der Sprache aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _Language gespeichert werden
 **************************************************************************/
void Settings::loadLanguageFromPreferences()
{
    preferences.begin("settings", true);
    _Language = preferences.getUChar("language");
    preferences.end();
}

/***************************************************************************
 * Schreiben der Sprache in Preferences
 * Übergabeparameter: Sprache
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeLanguageToPreferences(byte language)
{
    if(DEBUG_SETTINGS == 1)
    {
        Serial.print("Settings.cpp - ");
        Serial.print("Schreibe Sprache ");
        Serial.print(language);
        Serial.println(" in die Preferences");
    }
    preferences.begin("settings", false);
    preferences.putUChar("language", language);
    preferences.end();
}

/***************************************************************************
 * Auslesen der Helligkeit aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _Brightness gespeichert werden
 **************************************************************************/
void Settings::loadBrightnessFromPreferences()
{
    preferences.begin("settings", true);
    _Brightness = preferences.getUChar("brightness");
    preferences.end();
}

/***************************************************************************
 * Schreiben der Helligkeit in Preferences
 * Übergabeparameter: Helligkeitswert
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeBrightnessToPreferences(byte brightness)
{
    preferences.begin("settings", false);
    preferences.putUChar("brightness", brightness);
    preferences.end();
}

/***************************************************************************
 * Auslesen der Farbe aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _Color gespeichert werden
 **************************************************************************/
void Settings::loadColorFromPreferences()
{
    preferences.begin("settings", true);
    _Color.red = preferences.getUChar("colorred");
    _Color.green = preferences.getUChar("colorgreen");
    _Color.blue = preferences.getUChar("colorblue");
    preferences.end();
}

/***************************************************************************
 * Schreiben der Farbe in Preferences
 * Übergabeparameter: Farbe als pixel_t struct
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeColorToPreferences(pixel_t color)
{
    preferences.begin("settings", false);
    preferences.putUChar("colorred", color.red);
    preferences.putUChar("colorgreen", color.green);
    preferences.putUChar("colorblue", color.blue);
    preferences.end();
}

/***************************************************************************
 * Schreiben der Farbe in Preferences
 * Übergabeparameter: Farbe als RGB
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeColorToPreferences(byte red, byte green, byte blue)
{
    preferences.begin("settings", false);
    preferences.putUChar("colorred", red);
    preferences.putUChar("colorgreen", green);
    preferences.putUChar("colorblue", blue);
    preferences.end();
}

/***************************************************************************
 * Auslesen des FadeMode aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _FadeMode gespeichert werden
 **************************************************************************/
void Settings::loadFadeModeFromPreferences()
{
    preferences.begin("settings", true);
    _FadeMode = preferences.getUChar("fademode");
    preferences.end();
}

/***************************************************************************
 * Schreiben des FadeMode in Preferences
 * Übergabeparameter: FadeMode
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeFadeModeToPreferences(byte fademode)
{
    preferences.begin("settings", false);
    preferences.putUChar("fademode", fademode);
    preferences.end();
}

/***************************************************************************
 * Auslesen des Start Eck-LED aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _CornerStartLed gespeichert werden
 **************************************************************************/
void Settings::loadCornerStartLedFromPreferences()
{
    preferences.begin("settings", true);
    _CornerStartLed = preferences.getUChar("cornerstartled");
    preferences.end();
}

/***************************************************************************
 * Schreiben der Start Eck-LED in Preferences
 * Übergabeparameter: Start Eck-LED
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeCornerStartLedToPreferences(byte cornerstartled)
{
    preferences.begin("settings", false);
    preferences.putUChar("cornerstartled", cornerstartled);
    preferences.end();
}

/***************************************************************************
 * Auslesen der Drehrichtung der Eck-LED aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _CornersClockwise gespeichert werden
 **************************************************************************/
void Settings::loadCornerClockwiseFromPreferences()
{
    preferences.begin("settings", true);
    _CornersClockwise = preferences.getBool("cornerclockwise");
    preferences.end();
}

/***************************************************************************
 * Schreiben der Drehrichtung der Eck-LEDs in Preferences
 * Übergabeparameter: Drechrichtung Eck-LED
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeCornerClockwiseToPreferences(bool cornersclockwise)
{
    preferences.begin("settings", false);
    preferences.putBool("cornerclockwise", cornersclockwise);
    preferences.end();
}

/***************************************************************************
 * Auslesen des Startmusters aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _StartPattern gespeichert werden
 **************************************************************************/
void Settings::loadStartpatternFromPreferences()
{
    preferences.begin("settings", true);
    _StartPattern = preferences.getUChar("startpattern");
    preferences.end();
}

/***************************************************************************
 * Schreiben des Startmusters in Preferences
 * Übergabeparameter: Startmuster
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeStartpatternToPreferences(byte startpattern)
{
    preferences.begin("settings", false);
    preferences.putUChar("startpattern", startpattern);
    preferences.end();
}

/***************************************************************************
 * Auslesen des GMT Offsets aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _GmtTimeOffsetSec gespeichert werden
 **************************************************************************/
void Settings::loadGmtOffsetFromPreferences()
{
    preferences.begin("settings", true);
    _GmtTimeOffsetSec = preferences.getUShort("gmtoffset");
    preferences.end();
}

/***************************************************************************
 * Schreiben des Startmusters in Preferences
 * Übergabeparameter: Startmuster
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeGmtOffsetToPreferences(uint16_t gmtoffset)
{
    preferences.begin("settings", false);
    preferences.putUShort("gmtoffset", gmtoffset);
    preferences.end();
}

/***************************************************************************
 * Auslesen der WiFi SSID aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _GmtTimeOffsetSec gespeichert werden
 **************************************************************************/
void Settings::loadSsidFromPreferences()
{
    preferences.begin("settings", true);
    preferences.getBytes("wifissid", _SSID_Array, 32);
    preferences.end();
}

/***************************************************************************
 * Schreiben der Wifi SSID in Preferences
 * Übergabeparameter: SSID Name
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writeSsidToPreferences(char* ssid)
{
    preferences.begin("settings", false);
    preferences.putBytes("wifissid", ssid, 32);
    preferences.end();
}

/***************************************************************************
 * Auslesen des WiFi Passworts aus Preferences
 * Übergabeparameter: kein, da die Werte direkt aus den Preferences gelesen werden
 * Rückgabeparameter: kein, da die Werte direkt in die Klassenvariable _GmtTimeOffsetSec gespeichert werden
 **************************************************************************/
void Settings::loadPasswordFromPreferences()
{
    preferences.begin("settings", true);
    preferences.getBytes("wifipw", _PW_Array, 32);
    preferences.end();
}

/***************************************************************************
 * Schreiben des Wifi PW in Preferences
 * Übergabeparameter: Passwort
 * Rückgabeparameter: kein, da die Werte direkt in den Preferences abgelegt werden
 **************************************************************************/
void Settings::writePasswordToPreferences(char* password)
{
    preferences.begin("settings", false);
    preferences.putBytes("wifipw", password, 32);
    preferences.end();
}
