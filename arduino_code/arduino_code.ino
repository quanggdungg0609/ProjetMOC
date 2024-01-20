#define LED_PIN 2
#define PANNE_LED_PIN 4
#define BOUTON_PIN 3
#define TRIG_PIN 9
#define ECHO_PIN 10




// etat
bool panne = false;
bool marche = true;

unsigned long lastButtonPressTime = 0;
const unsigned long buttonResetTime = 10000; 

long duration,cm;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  // register le led 
  pinMode(LED_PIN, OUTPUT);
  pinMode(PANNE_LED_PIN, OUTPUT);
  // register le bouton
  pinMode(BOUTON_PIN, INPUT_PULLUP);
  // register capteur ultrason

  pinMode(TRIG_PIN,OUTPUT);
  digitalWrite(TRIG_PIN,LOW);
  pinMode(ECHO_PIN,INPUT);
}

void loop() {
  if(panne){
    Serial1.write("PANNE\n");
    Serial1.flush();
  }
  
  // put your main code here, to run repeatedly:
  if(Serial1.available()){
    String msg = Serial1.readStringUntil("\n");
    msg.trim();
    if(msg.equals("ON")){
      Serial.println(msg);
      marche = true;
    }
    if (msg.equals("OFF")){
      Serial.println(msg);
      marche = false;
    }
  }
    
    // change Etat
  if (digitalRead(BOUTON_PIN) == 0) {
    // verifie si est dans le temps reset
    if (millis() - lastButtonPressTime < buttonResetTime) {
      // Si le bouton est appuye pendant le temps de reset
      Serial.println("Resetting...");
      marche = true;
      panne = false;
      Serial1.write("RESET\n");
    } else {
      marche = !marche;
      // Envoie message a esp32 pour mise jour etat capteur
      if (!marche) {
        Serial.println("OFF...");
        Serial1.write("OFF\n");
        Serial1.flush();
      } else {
        Serial.println("ON...");
        Serial1.write("ON\n");
        Serial1.flush();
      }
      // sauvegard le dernier moment appuye butoon
      lastButtonPressTime = millis();
    }
  }
    /*
    // change Etat
    if(digitalRead(BOUTON_PIN)==0){
      marche = !marche;
      //envoyer message a ESP32 pour etat du systeme
      if(!marche){
        Serial1.write("OFF\n");
        Serial1.flush();
        //Serial.println("OFF");
      }else{
        Serial1.write("ON\n");
        Serial1.flush();
        //Serial.println("ON");
      }
    }
    */


  if (!panne && marche){
    // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    pinMode(ECHO_PIN, INPUT);
    duration = pulseIn(ECHO_PIN, HIGH);
    // Serial.println(duration);
    cm = duration/29/2;
      
    // si object est <30cm mise a jour sur la base donnee
    if(cm <30){
      String distance_str = String(cm);
      String message = "distance_"+distance_str+"_cm\n";
      Serial.println(message);
      Serial1.println(message);
      Serial1.flush();
    }
  }
  
  if (panne){
    digitalWrite(PANNE_LED_PIN, HIGH);
  }else{
    digitalWrite(PANNE_LED_PIN, LOW);
  }

  if (marche){
    digitalWrite(LED_PIN, HIGH);
  }else{
    digitalWrite(LED_PIN, LOW);
  }

  // Simulation panne apres 10 seconds
  if (millis() - lastButtonPressTime > buttonResetTime + 10000) {
    panne = true;
  }

  delay(1000);
}
