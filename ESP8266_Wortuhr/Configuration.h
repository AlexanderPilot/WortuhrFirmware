/* Headerdatei für Konfigurationen */

/* Vermeidung Doppeldefinitionen */
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/* Serielle Geschwindigkeit */
#define SERIAL_SPEED 74880

/* Versionsstand */
#define FIRMWARE_VERSION "1.0"

/* Debug Botschaften */
#define DEBUG false

/* WiFi AP Name */
#define AP_SSID "karl"

/* WiFi AP Passwort */
#define AP_PW ""

/* WiFi AP IP-Addresse */
#define AP_IP_ADDR ""

/* Telnet Server Port */
#define TELNET_PORT 23

/* Clients auf Server */
#define MAX_SRV_CLIENTS 1

/* LED für Client-Verbindung zum Server */
#define LED_SVR_HAS_CLNT D6


#endif
