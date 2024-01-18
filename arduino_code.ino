#define LED_PIN 2
#define BOUTON_PIN 3
#define TRIG_PIN 9
#define ECHO_PIN 10

#define MESURE_TIMEOUT 25000UL
#define SOUND_SPEED 340/1000

// etat
bool panne = false;
bool marche = true;

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

  // change Etat
  if(digitalRead(BOUTON_PIN)==0){
    marche = !marche;
    //envoyer message a ESP32 pour etat du systeme
    if(!marche){
      Serial1.write("OFF\n");
      //Serial.println("OFF");
    }else{
      Serial1.write("ON\n");
      //Serial.println("ON");
    }
  }

  

  if (!panne && marche){
    digitalWrite(TRIG_PIN,HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN,LOW);

    long mesure = pulseIn(ECHO_PIN,1,MESURE_TIMEOUT);
    float distance = mesure/2.0 * SOUND_SPEED / 100;
    String distance_str = String(distance,3);
    String message = "distance_"+distance_str+"_cm";

    Serial.println(message);
    Serial1.println(message);
  }
  if (marche){
    digitalWrite(LED_PIN, HIGH);
  }else{
    digitalWrite(LED_PIN, LOW);
  }
  delay(500);
}



void changeEtat(void){
  marche=!marche;
}