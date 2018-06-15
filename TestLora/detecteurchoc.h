#ifndef DETECTEURCHOC_H
#define DETECTEURCHOC_H

#include <LSM303AGR_ACC_Sensor.h>

class DetecteurChoc
{
private:
    LSM303AGR_ACC_Sensor *capteur;
    float sensibilite;
    byte pleineEchelle;

    byte CalculerDeclencheur();

public:
    DetecteurChoc(LSM303AGR_ACC_Sensor *_capteur, float _sensibilite);
};

#endif // DETECTEURCHOC_H

