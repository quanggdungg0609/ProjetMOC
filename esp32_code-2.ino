#include <WiFi.h>
#include <Firebase_ESP_Client.h> 
//#include <FirebaseESP32.h> // lecture Firebase
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define SSID "iPhone"
#define MDP "sonia2001"
#define API_KEY "AIzaSyBLtP_06qLJh0U4gyyIzeUrQVY7obKhn6g"
#define DATABASE_URL "https://projet-lse-group-2-default-rtdb.europe-west1.firebasedatabase.app"

#define USER_EMAIL "groupe2@mail.com"
#define USER_PASSWORD "123456"

FirebaseData fbdo; // données
FirebaseConfig config; // config
FirebaseAuth auth; 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200);
 //
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(SSID, MDP);

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}
String date;
void loop() {
  // put your main code here, to run repeatedly:

  if (Firebase.ready()) {
   /*
   if(Firebase.RTDB.get(&fbdo,"esp32-123456/date-start")){
    Serial.print("Data: ");
    Serial.println(fbdo.stringData());
   }*/

  
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
