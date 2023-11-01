#include <HCSR04.h>


#include <Servo.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>

#include <Servo.h>
Servo motor ;
int MD_direction = 5; //d1
int MD_vitesse = 4; //d2 
int MG_vitesse = 0 ; //d3
int MG_direction = 2; //d4 
int vitesse_max=511;//1022 ;

  char ssid[] = "TOPNET_26E0"; // your network SSID (name)
char pass[] = "4sd6tlm6kv"; // your network password
WiFiClient client;
unsigned long myChannelNumber =1743911;
const char * myWriteAPIKey = "H6T4LHTVSIWQKIOO";

int trig=13 ;
int echo=12;
int Duration ;
int Distance ;

void voiture_avance(int vitesse)
{digitalWrite(MD_direction,HIGH); analogWrite(MD_vitesse,vitesse); 
digitalWrite(MG_direction,HIGH); analogWrite(MG_vitesse,vitesse); 
  }

  void voiture_recule(int vitesse)
{digitalWrite(MD_direction,LOW); analogWrite(MD_vitesse,vitesse); 
digitalWrite(MG_direction,LOW); analogWrite(MG_vitesse,vitesse); 
  }
  
void avance_droite(int vitesse)
{digitalWrite(MD_direction,HIGH); analogWrite(MD_vitesse,vitesse/2); 
digitalWrite(MG_direction,HIGH); analogWrite(MG_vitesse,vitesse); 
  }

  void avance_gauche(int vitesse)
{digitalWrite(MD_direction,HIGH); analogWrite(MD_vitesse,vitesse); 
digitalWrite(MG_direction,HIGH); analogWrite(MG_vitesse,vitesse/2); 
  }
     void voiture_arret()
{ analogWrite(MD_vitesse,0); analogWrite(MG_vitesse,0); 
  }
void setup()
{
  //motor.attach(9) ;//sd2
Serial.begin(115000); 

WiFi.begin(ssid, pass);
 ThingSpeak.begin(client);
 pinMode(MD_direction, OUTPUT);
  pinMode(MD_vitesse, OUTPUT);
  pinMode(MG_direction, OUTPUT);
  pinMode(MG_vitesse, OUTPUT);
pinMode(echo,INPUT) ;//envoi la distance de le triger ala carte
pinMode(trig,OUTPUT);// envoi le signal de la capteur a l 'echo


void loop()
{
digitalWrite(trig,LOW) ;
delayMicroseconds(2) ;
digitalWrite(trig,HIGH) ;
delayMicroseconds(10) ;
digitalWrite(trig,LOW) ;
Duration=pulseIn(echo,HIGH) ;// met la durée de l'impulsion de niveau HAUT dans la variable duration un chronométre sa sera charger   jusqua que " echo" low 
Distance=(Duration/2)*0.0324 ; //* pour convertir en cm
Serial.print("distance= ") ;
Serial.print(Distance) ;
Serial.println(" cm ") ;

if (Distance>20)// si la distance >20 la voiture avance 
{
voiture_avance(vitesse_max);

else if(Distance<=20) { // sinon la voiture fait un stop et le servo motor tourne 
voiture_arret();delay(2000);  
ThingSpeak.writeField(myChannelNumber,1,Distance,myWriteAPIKey); 

motor.write(0) ;
delay(1000) ;
motor.write(90);
if(Distance<=20){ // le servo motor tourne ala droite s'il ne trouve pas un obstacle il avance sinon il tourne ala gauche est fait le meme test sinon il recule
avance_droite(vitesse_max)
}
else 
motor.write(180);}
if(Distance<=20){
avance_gauche(vitesse_max);
}
else {
voiture_recule(vitesse_max);}
delay(1000) ;
}
