
#include <STM32RTC.h>
#include <HTS221Sensor.h> 
#include <LSM303AGR_ACC_Sensor.h>

#include "lora.h"
#include "constantes.h"
#include "capteurenvironnemental.h"
#include "detecteurchoc.h"

STM32RTC rtc;
Lora accesLora(PC_11, PC_10, &rtc);
  
HTS221Sensor  *sensor1 ;
CapteurEnvironnemental *capteur1;
LSM303AGR_ACC_Sensor *accelerometre;
DetecteurChoc *capteur2;

int ouverture = PC13;                     /// Bouton USER pour l'instant afin de générer l'interruption d'ouverture
int led = 13;

EtatsSpareBox etatSysteme;
byte trame[10]= {0,0,0,0,0,0,0,0,0,0};

void setup() 
{
  
  Serial.begin(115200);
  Serial.println("Setup");

  

  rtc.begin(); 
  rtc.setTime(0,0,0);
  rtc.attachInterrupt(DeclencherMesures);
  rtc.setAlarmTime(0, 0, 0);
  rtc.enableAlarm(rtc.MATCH_MMSS);    // déclenchement toutes les minutes rtc.MATCH_SS -- rtc.MATCH_MMSS pour déclenchement toutes les heures

  accesLora.EnvoyerCommandeAT("at");

  Wire.begin();
  sensor1 = new HTS221Sensor(&Wire);
  capteur1 = new CapteurEnvironnemental(sensor1,6); // envoie Toutes les 4h56 avec 6 mesures.
  capteur1->EffectuerMesure();

  // Initialisation choc
  accelerometre = new LSM303AGR_ACC_Sensor(&Wire);
  capteur2 = new DetecteurChoc(accelerometre,2);    // detection des chocs supérieurs ou égales à 2g
 
  pinMode(PC1,INPUT_PULLUP);
  attachInterrupt(PC1, DetecterChoc, RISING);

 //Initialisation détection ouverture
  pinMode(ouverture,INPUT_PULLDOWN);
  attachInterrupt(ouverture, DetecterOuverture, FALLING);

  etatSysteme = SOMMEIL;

  pinMode(led, OUTPUT);
  digitalWrite(led,HIGH);
    
}

void loop() 
{
    switch(etatSysteme)
    {
      case SOMMEIL:
     
      break;
      
      case MESURES:
        if(capteur1->EffectuerMesure())
          etatSysteme = TRANSMISSION;
        else
          etatSysteme = SOMMEIL;
      break;
      
      case CHOC:
      break;
      
      case OUVERTURE: 
      break;
      
      case TRANSMISSION:     
        if(accesLora.JoindreLora() == true)
        {
            capteur1->ObtenirCumul(trame);
            accesLora.EnvoyerTrame(trame,10);
            delay(5000); // il faut attendre un peu, le temps que les données soient envoyées avant de RAZ le stm32
            if(accesLora.EnvoyerCommandeAT("ATZ") != Lora::OK) // Il faut réinitialiser pour envoyer à nouveau des données !!!
            {
              Serial.println("Erreur ATZ");
            } 
            etatSysteme = SOMMEIL;
        }
        
      break;
      default:
      break;
    }
}


void DeclencherMesures(void *data)
{
  if(etatSysteme == SOMMEIL)
      etatSysteme = MESURES;
}

void DetecterOuverture()
{
  trame[9] = 0xFF;
  etatSysteme = TRANSMISSION;
}

/**
 *  @brief Routine d'interruption suite à la détetection d'un choc
 */
void DetecterChoc()
{
    uint8_t result;
    trame[8] = 0xFF;
    accelerometre->ReadReg(LSM303AGR_ACC_INT1_SOURCE,&result);
    etatSysteme = TRANSMISSION;
}

