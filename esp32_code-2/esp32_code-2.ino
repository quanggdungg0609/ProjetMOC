#include <WiFi.h>
#include <Firebase_ESP_Client.h> 
//#include <FirebaseESP32.h> // lecture Firebase
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include "time.h"


// WIFI setting
#define SSID "Extend"
#define MDP "Dinhsonque"


// Firebase Setting
#define API_KEY "AIzaSyBLtP_06qLJh0U4gyyIzeUrQVY7obKhn6g"
#define DATABASE_URL "https://projet-lse-group-2-default-rtdb.europe-west1.firebasedatabase.app"
#define USER_EMAIL "groupe2@mail.com"
#define USER_PASSWORD "123456"

// ntp setting for get time
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * 1;
const int   daylightOffset_sec = 3600 * 0;

FirebaseData fbdo; // données
FirebaseConfig config; // config
FirebaseAuth auth; 
String uuid="2bd6419e-c040-4310-abcf-2e7226820d2f";
String model="ESP32-ARDUINO-LEONARDO";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200);
 // Wifi setup
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(SSID, MDP);

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());

  //Firebase setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Register to Firebase si n'a pas registe
  Serial.println("Verifie data....");
  if(isPatchExist(uuid)){
    Serial.println("Exist");
  }else{
    Serial.println("La carte n'existe pas sur la base de donnee");
    addDeviceToFireBase();
  }

  // mise a jour la date commencer fonction
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    String text = "NaN";
    if(Firebase.RTDB.setString(&fbdo,"/"+uuid+"/date-commencer",text)){
      Serial.println("Date de debut de mise en service ajouter");
    }
  }else{
    char buffer[50]; // buffer pour sauvegar le temp actuel
    sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
          timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
          timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    if(Firebase.RTDB.setString(&fbdo,"/"+uuid+"/date-commencer",buffer)){
      Serial.println("Date de debut de mise en service ajouter");
    }
  }
  
 

}
String date;
void loop() {
  /*
  if (Firebase.ready()) {
   /*
   if(Firebase.RTDB.get(&fbdo,"esp32-123456/date-start")){
    Serial.print("Data: ");
    Serial.println(fbdo.stringData());
   }

  
   if(Serial2.available()>0){
      String msg=Serial2.readStringUntil('\n');
      if (msg.equals("ON")){
        if(Firebase.RTDB.setString(&fbdo,"esp32-123456/etat","ON")){
          Serial.println("Update Etat");
        }
      }
      if (msg.equals("OFF")){
        if(Firebase.RTDB.setString(&fbdo,"esp32-123456/etat","OFF")){
          Serial.println("Update Etat");
        }
      }
      if (verifyString(msg, "distance")){
        Serial.println("dedans");
        Serial.println(msg);
      }
    }
  }
  delay(1000);
  /*
  
  */
}

bool verifyString(String msg, String mot){
  if (msg.indexOf(mot) != -1) {
    return true;
  } 
  return false;
}

bool isPatchExist(String p){
  String path ="/"+p;
  if(Firebase.RTDB.get(&fbdo, path)){
    return true;
  }else{
    return false;
  }
}

void addDeviceToFireBase(){
  Serial.println("Registe a la base de donnee ...");
  FirebaseJson updateDevice;
  FirebaseJson data;
  data.add("temp","");
  data.add("dist","");
  updateDevice.add("date-commencer","");
  updateDevice.add("model", model);
  updateDevice.add("num-serie",model+"-"+uuid);
  updateDevice.add("etat-capteur","ON");
  updateDevice.add("etat-hardware","OK");
  updateDevice.add("data",data);


  if(Firebase.RTDB.updateNode(&fbdo,"/"+uuid,&updateDevice)){
    Serial.println("["+ uuid+ "] est bien registre");
  }
}
