#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
//#include "Timer.h"
#include "FS.h"
#include "Wire.h"
bool f = 1;
//Timer t;
int num = 0;
#include "global.h"

// Include the HTML, STYLE and Script "Pages"

#include "Page_Admin.h"
#include "Page_Market.h"
#include "Page_Action.h"
#include "Page_Script.js.h"
#include "Page_Style.css.h"
#include "Page_Information.h"
#include "Page_General.h"
#include "Page_NetworkConfiguration.h"


bool connectOk = 0;

extern "C" {
#include "user_interface.h"
}

Ticker ticker;


os_timer_t myTimer;


//*** Normal code definition here ...

#define LED_PIN 2
#define buttonPin 12
int flag1;
//ADC_MODE (ADC_VCC); //Режим АЦП - замеряем вольтаж питания
float voltage1;
String result1;
String dataset;
long long timez;
WiFiClient client;

long eventTime;


String chipID;

String SendHttp(String y) //функция отправки http get запроса, принимает url
{
  HTTPClient http;
  http.begin(y);
  String response = "error";
  short int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK)
  {
    Serial.print("HTTP response code ");
    Serial.println(httpCode);
    response = http.getString();
    Serial.println(response);
  }
  else
  {
    Serial.println("Error in HTTP request");
  }
  return response;
  http.end();
}

void ResetAll(){ //чистка eeprom
  EEPROM.begin(512);
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 512; i++){
  EEPROM.write(i, 0);
  }
  EEPROM.end();
  ESP.reset();
}

void deepSleep(){ //режим глубокого сна(пока не используется)
     ESP.deepSleep(0, WAKE_RF_DEFAULT); 
     ESP.reset();
}



void update(){ //http прошивка микроконтроллера
  
  server.send ( 200, "text/plain", "Ok" );
  t_httpUpdate_return ret = ESPhttpUpdate.update("https://github.com/hackuniversity/" + server.arg(0) + ".bin");

        switch(ret) {
            case HTTP_UPDATE_FAILED:
                Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                break;

            case HTTP_UPDATE_NO_UPDATES:
                Serial.println("HTTP_UPDATE_NO_UPDATES");
                break;

            case HTTP_UPDATE_OK:
                Serial.println("HTTP_UPDATE_OK");
                break;
        }
}

String ipToString(IPAddress ip) { //преобразует ip в строку
  String s = "";
  for (int i = 0; i < 4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}

bool CFG_saved = false;
void setup() {
  Wire.begin();
  pinMode(12,INPUT);
  SPIFFS.begin(); 
  //timer = micros();
  int WIFI_connected = false;
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Hello");
  display.display();
  delay(500);
  pinMode(12,INPUT);
  //**** Загрузка конфигурации сети 
  EEPROM.begin(512);
  CFG_saved = ReadConfig();

  if (CFG_saved)  //если есть конфигурации сети
  {    
      // Подключение к точке досутпа
      Serial.println("Booting");
      
      WiFi.mode(WIFI_STA);
        flag1=1;
      //WiFi.begin(config.ssid.c_str(), config.password.c_str());
      WiFi.begin(config.ssid.c_str(), config.password.c_str());
      printConfig();
      WIFI_connected = WiFi.waitForConnectResult();
      if(WIFI_connected!= WL_CONNECTED ){
        Serial.println("Connection Failed! activating to AP mode...");
      }
      else 
      {
        Serial.println(ipToString(WiFi.localIP()));
        connectOk = 1;
      }
  }

  if ( (WIFI_connected!= WL_CONNECTED) or !CFG_saved){
    // Конфигурации по умолчанию
    Serial.println("Setting AP mode default parameters");
    config.ssid = "IoT2";       
    config.password = "" ; 
    WiFi.mode(WIFI_AP);  
    WiFi.softAP(config.ssid.c_str());
    Serial.print("Wifi ip:");Serial.println(WiFi.softAPIP());
    flag1=0;
   }
   

    // ***Запуск http сервера
    server.on ( "/", []() {
      Serial.println("admin.html");
      server.send_P ( 200, "text/html", PAGE_AdminMainPage); 
    }  );
  
    server.on ( "/favicon.ico",   []() {
      Serial.println("favicon.ico");
      server.send( 200, "text/html", "" );
    }  );
  
    server.on ( "/config.html", send_network_configuration_html );
    server.on ( "/info.html", []() {
      Serial.println("info.html");
      server.send_P ( 200, "text/html", PAGE_Information );
    }  );
    server.on ( "/update", update);
  
    //server.on ( "/appl.html", send_application_configuration_html  );
    server.on ( "/general.html", send_general_html  );
    //  server.on ( "/example.html", []() { server.send_P ( 200, "text/html", PAGE_EXAMPLE );  } );
    server.on ( "/style.css", []() {
      Serial.println("style.css");
      server.send_P ( 200, "text/plain", PAGE_Style_css );
    } );
    server.on ( "/action.html", []() {
      Serial.println("action");
      server.send_P ( 200, "text/html", PAGE_Action );
    } );
    server.on ( "/microajax.js", []() {
      Serial.println("microajax.js");
      server.send_P ( 200, "text/plain", PAGE_microajax_js );
    } );
    server.on ( "/market", []() {
      Serial.println("market");
      server.send_P ( 200, "text/html", PAGE_Market );
    } );
    server.on ( "/admin/values", send_network_configuration_values_html );
    server.on ( "/check", sendText );
    server.on ( "/admin/connectionstate", send_connection_state_values_html );
    server.on ( "/admin/infovalues", send_information_values_html );
    //server.on ( "/admin/applvalues", send_application_configuration_values_html );
    server.on ( "/admin/generalvalues", send_general_configuration_values_html);
    server.on ( "/admin/devicename",     send_devicename_value_html);
  
    server.onNotFound ( []() {
      Serial.println("Page Not Found");
      server.send ( 400, "text/html", "Page not Found" );
    }  );
    server.begin();
    Serial.println( "HTTP server started" );

    
timez = millis();

}


void loop() {
  
  // всегда готовы к прошивке
  server.handleClient();

   // обновление watchdog таймера
   customWatchdog = millis();

}

