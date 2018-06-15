
#include "capteurenvironnemental.h"

CapteurEnvironnemental::CapteurEnvironnemental(HTS221Sensor *_capteur, int _nbMesures) :
    cumulTemperatures(0),
    cumulHumidite(0),
    compteurMesures(0),
    nbMesures(_nbMesures)
{
    capteur = _capteur;
    capteur->Enable();
    Serial.println("Capteur initialisé"); 
    
}

bool CapteurEnvironnemental::EffectuerMesure()
{
    bool retour = false;
    float humidite;
    float temperature;
    
    capteur->GetHumidity(&humidite);
    capteur->GetTemperature(&temperature);
    compteurMesures++;
    cumulTemperatures += temperature;
    cumulHumidite += humidite;
    
    Serial.print("humidité : ");
    Serial.print(humidite,2);
    Serial.print(" - temperature : ");
    Serial.print(temperature,2); 
    Serial.print(" - Compteur : ");
    Serial.println(compteurMesures); 
    
    if(compteurMesures == nbMesures)
        retour = true;
    
    return retour;
    
}

void CapteurEnvironnemental::ObtenirCumul(byte cumul[])
{
    float humidite = cumulHumidite / compteurMesures;
    float temperature = cumulTemperatures / compteurMesures;
    
    cumulHumidite = 0;
    cumulTemperatures = 0;
    compteurMesures = 0;
    
    int indice;
    byte *ptrT = (byte*)&temperature;
    byte *ptrH = (byte*)&humidite;
    
    for(indice = 0 ; indice <4 ; indice++)
    {
        cumul[indice] = *ptrT;
        ptrT++;
        cumul[indice+4] = *ptrH;
        ptrH++;
    }
    
}

