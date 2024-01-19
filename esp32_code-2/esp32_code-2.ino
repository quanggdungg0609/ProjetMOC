#include <WiFi.h>
#include <Firebase_ESP_Client.h> 
//#include <FirebaseESP32.h> // lecture Firebase
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include "time.h"

//TODO: Gerer la callback pour gerer etat la capteur quand etat sur firebase est changee


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

FirebaseData stream; //stream 
FirebaseData fbdo; // données
FirebaseConfig config; // config
FirebaseAuth auth; 
String uuid="2bd6419e-c040-4310-abcf-2e7226820d2f";
String model="ESP32-ARDUINO-LEONARDO";

bool etatOnFirebase;
bool etatCapteur;

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
  fbdo.setBSSLBufferSize(2048, 1024);
  stream.setBSSLBufferSize(2048, 1024);
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // begin a stream pour ecouter change avec etat de capteur sur firebase
  stream.keepAlive(5, 5, 1);
  Firebase.RTDB.beginStream(&stream, "/"+uuid+"/etat-capteur");
  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);

  // Register to Firebase si n'a pas registe
  Serial.println("Verifie data....");
  if(!isPatchExist(uuid)){
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


  // Mise en service 
  if(Firebase.RTDB.setString(&fbdo,"/"+ uuid +"/etat-capteur","ON")){
    Serial.println("Mise en service");
  }

}

void loop() {
  if (Firebase.ready()) {
    if(Serial2.available()>0){
      String msg=Serial2.readStringUntil('\n');
      handleMsg(msg);
    }
  }
}

///// Need to Improvement
// gerer le status quand etat capteur est changer sur firebase
void streamCallback(FirebaseStream data){
  printResult(data); // see addons/RTDBHelper.h
  Serial.println();
}

void streamTimeoutCallback(bool timeout){
  if(timeout){
    Serial.println("stream timeout, resuming...");
  }
  if(!stream.httpConnected()){
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
  }
}
/////////////////////////////////



bool verifyString(String str, String mot){
  /*
    Verifie que si la chaine donnee est contient le mot
    @param str: la chaine caractere
    @param mot: le mot pour verifie si la chaine est contient
    return:
      true si contient sinon false
  */
  if (str.indexOf(mot) != -1) {
    return true;
  } 
  return false;
}



bool isPatchExist(String p){
  /*
    @param p : le path pour verifie si exist
    return: true si exist sinon false
  */
  String path ="/"+p;
  if(Firebase.RTDB.get(&fbdo, path)){
    return true;
  }else{
    return false;
  }
}

void handleMsg(String msg){
  /*
  Gerer les message ont recus par capteur
  */
  if (msg.equals("ON")){
      if(Firebase.RTDB.setString(&fbdo,"/"+ uuid +"/etat-capteur","ON")){
        Serial.println("Update Etat");
      }
    }

  if (msg.equals("OFF")){
    if(Firebase.RTDB.setString(&fbdo,"/"+ uuid+ "/etat-capteur","OFF")){
      Serial.println("Update Etat");
    }
  }

  // si la carte arduino envoyer la distance
  if (verifyString(msg, "distance")){
    String data =processData(msg);
    if(data != ""){
      // mise a jour data sur firebase
      if(Firebase.RTDB.setString(&fbdo,"/"+ uuid +"/data/dist",data)){
        Serial.println("update distance");
      }
    }        
  }
}

void addDeviceToFireBase(){
  /*
    Ajouter la carte dans la base de donnee firebase
  */
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

String processData(const String data){
  int startPos = data.indexOf('_') + 1; // Tìm vị trí bắt đầu của X
  int endPos = data.indexOf('_', startPos); // Tìm vị trí kết thúc của X

  if (startPos != -1 && endPos != -1) {
    return data.substring(startPos, endPos);
  }
}