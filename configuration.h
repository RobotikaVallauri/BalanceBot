#ifndef conf_h#define conf_h
#endif

#define Enable_Serial
#define baud_rate 115200

#define Enable_Hotspot
#define Hotspot_SSID "BalanceBOT"
#define Hotspot_Password "12345678"

//#define Enable_External_Hotspot
#define External_Hotspot_SSID "mpcom"
#define External_Hotspot_Password "(MAX)*(matte)%"


#define Enable_webServer
#define webServer_port 80

#define Enable_WebSocketsServer
#define WebSocketsServer_port 81

#define Enable_DNSServer
#define DNSServer_port 53

#define Enable_mDNS
#define mDNS_Name "balancebot"


#define MPU_addr 0x68
#define GyrominVal 265
#define GyromaxVal 402

#define MillisBetweenSend 200


#ifdef Enable_DNSServer
#ifndef Enable_Hotspot
#error Impossibile Avviare il captive portal senza creare l'hotspot, attiva Enable_Hotspot
#endif
#endif
//Controllo attivazione sia hotspot sia wifi classico
#ifdef Enable_Hotspot  
#ifdef Enable_External_Hotspot
#error Impossibile Avviare sia l'hotspot che il wifi esterno, disattivere o Enable_Hotspot o Enable_External_Hotspot
#endif
#endif
