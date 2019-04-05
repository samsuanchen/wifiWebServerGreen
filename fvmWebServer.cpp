// fvmWebServer.h
#include <fvm.h>
#include <ESP.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Preferences.h>
extern FVM F;
Preferences preferences; // create an instance of Preferences library
WebServer server(80);
String head = "<html><head>\
<meta name='viewport' content='width=device-width, initial-scale=1, maximum-scale=1, user-scalable=0'>\
<meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>\
<title>WebServer</title></head><body><h2>connected to ";
String body = " </h2><FORM action='/' method='post'>";
String tail = "<INPUT type='submit' value='send'></FORM><pre>";
String giveSSID = head + "192.168.4.1"
+ body + " <INPUT type='text' name='ssid' placeholder='ssid' size=7> <INPUT type='text' name='pswd' placeholder='pswd' size=7>"
+ tail;
String html=giveSSID;
String ssid = "";
String pswd = "";
String inp = "";
void trySoftAP() {
  Serial.print( "\nConnect to WiFi Access Point 'fvm' to enter ssid and pswd at" );
  WiFi.softAP( "fvm" );
  Serial.print( "\nIP Address: ");
  Serial.print( WiFi.softAPIP() );
  html = giveSSID;
}
int getWiFi( String ssid, String pswd ){ int n = 9;
  Serial.print( "\nConnecting " + ssid + " as Web Server 'fvm' to process Web page at 'fvm.local'" );
  WiFi.mode( WIFI_STA ); WiFi.begin( ssid.c_str(), pswd.c_str() );
  Serial.print( "\nCounting down for at most " + String(n) + " secends.  " );
  while ( WiFi.status() != WL_CONNECTED && n ) delay(1000), Serial.print(n--);
  if( n ){ Serial.print( "\nConnected to " + ssid );
    IPAddress ip = WiFi.localIP();
    char lcdBuffer[20];
    sprintf(lcdBuffer, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    Serial.print("\nGot IP Address "); Serial.print(WiFi.localIP());
    Serial.printf("\n%s",lcdBuffer);
    if(ssid != preferences.getString("ssid"))
      preferences.putString("ssid", ssid.c_str()), Serial.print( "\nssid saved" );
    if(pswd != preferences.getString("pswd"))
      preferences.putString("pswd", pswd.c_str()), Serial.print( "\npswd saved." );
    html = head + ssid + "<br>IP Address: " + String(lcdBuffer) + "<br>DS name: " + ssid + ".local" + body + 
             "<INPUT type='checkbox' name='other' value='other ssid'> use another ssid to connect WiFi<br> " + 
             "<INPUT type='text' name='inp' size=30 value='" + 
               "wb_init 0 0 128 160 img wb_drawImage 27 output : x for 500 ms 27 toggle next ; 9 x" +
             "'>"
         + tail;
    if (MDNS.begin("fvm")) Serial.print( "\nDS Name: fvm.local/ (MDNS responder started)" );
  } else Serial.print( "\nNo WiFi connection to " + ssid );
  return n; // no WiFi connection if n == 0.
}
void _getWiFi(){           // ##### 2.1. primitive function _connectWiFi to call
/*PRINTF("before flushing\n");
  F.flush();
  PRINTF("after  flushing\n");
  F.T->oEnd = F.T->tob;
  int i=F.dPick(0);
  X s;
  s.i=i;
  PRINTF("\ns.i=0x%d", s.i);
  PRINTF("\ns.s=\"%s\"", s.s);
  String ssid = String(s.s);
  PRINTF("\nssid=\"%s\"", ssid.c_str());
  i=F.dPick(0);
  PRINTF("\npswd=0x%d", i);
  X p;
  p.i=i;
  PRINTF("\np.i=0x%d", p.i);
  PRINTF("\np.s=\"%s\"",p.s);
  String pswd = String(p.s);
  PRINTF("\npswd=\"%s\"", pswd.c_str());
  F.dPush( getWiFi( ssid, pswd ) ); 
 */
}
void _send(){ html += F.T->tob;
    server.send(200, "text/html",  html);
}
void wifi(){
  html=giveSSID;
  F.T->html = &html;
  preferences.begin("iotsharing", false); // Start namespace "iotsharing" in Read/Write
  ssid = preferences.getString("ssid"), pswd = preferences.getString("pswd");
  int n=0;
  if( ssid != NULL && pswd != NULL ) n=getWiFi( ssid, pswd );
  server.on("/", []() {
    int n=server.args();
    for(int i=0; i<n; i++){
      Serial.print( "\n server.arg('" + server.argName(i) + "') = '" + server.arg(i) + "'" );
    }
    if (server.hasArg("ssid")) {
      ssid = server.arg("ssid"); if (server.hasArg("pswd")) pswd = server.arg("pswd");
      if( ! getWiFi(ssid, pswd) ) Serial.print( "!!! invalid ssid '" + ssid + "' and pswd '" + pswd +"' !!!" );
    }
    if (server.hasArg("other")){
      Serial.print("trying SoftAP();\n");
      trySoftAP();
    }
    if (server.hasArg("inp")){
      inp = server.arg("inp");
      F.initEval((char*)inp.c_str()); // ##### 4.2. eval input string as F script
    }
    server.send(200, "text/html",  html);
  });
  if(n==0) trySoftAP();
  server.begin();
  Serial.print("\nserver.begin();\n");
  server.send(200, "text/html",  html);
}
void fvmHandleClient(){
	server.handleClient();
}
