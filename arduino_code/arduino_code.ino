#define LED_PIN 2
#define BOUTON_PIN 3
#define TRIG_PIN 9
#define ECHO_PIN 10




// etat
bool panne = false;
bool marche = true;

long duration,cm;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  // register le led 
  pinMode(LED_PIN, OUTPUT);
  // register le bouton
  pinMode(BOUTON_PIN, INPUT_PULLUP);
  // register capteur ultrason

  pinMode(TRIG_PIN,OUTPUT);
  digitalWrite(TRIG_PIN,LOW);
  pinMode(ECHO_PIN,INPUT);
}

void loop() {
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
    Serial.println(duration);
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
  if (marche){
    digitalWrite(LED_PIN, HIGH);
  }else{
    digitalWrite(LED_PIN, LOW);
  }
  delay(1000);
}
