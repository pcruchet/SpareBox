#ifndef CAPTEURENVIRONNEMENTAL_H
#define CAPTEURENVIRONNEMENTAL_H

#include <HTS221Sensor.h>           // Pour le capteur Temperature + Humidité

class CapteurEnvironnemental
{
private:
    HTS221Sensor * capteur;
    float cumulTemperatures;    /// Somme des temperatures mesurées
    float cumulHumidite;        /// Somme des mesures d'humidite
    int  compteurMesures;       /// Comptage du nombre de mesures effectuées
    int  nbMesures;             /// Nombre de mesure à effectuer

public:
    CapteurEnvironnemental(HTS221Sensor *_capteur, int _nbMesures = 12);
    bool EffectuerMesure();
    void ObtenirCumul(byte cumul[]);
};

#endif // CAPTEURENVIRONNEMENTAL_H

