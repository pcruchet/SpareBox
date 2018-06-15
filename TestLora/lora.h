/**
  * @file lora.h
  * @author Philippe CRUCHET
  * @date 1 juin 2018
  * @details Assure la communication avec le réseau Lora-Wan avec les commandes AT
  * 
  */

#ifndef LORA_H
#define LORA_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <STM32RTC.h>



#define LONGUEUR_APPEUI 8
#define LONGUEUR_APPKEY 16
#define APPEUI "SpareBox"
#define APPKEY "SBox version 1.0"


class Lora
{
    
public:
    enum ETAT_RESEAU
    {
        OCCUPE = -1,
        NOK,
        OK   
    };
    
    Lora(PinName _rx, PinName _tx, STM32RTC *_rtc );
    virtual ~Lora();
    
    ETAT_RESEAU EnvoyerCommandeAT(String commande);
    void ScruterReponse(char carFin='#',int timeOut=0);
    bool JoindreLora();  
    void Initialiser();
    void EnvoyerTrame(byte trame[], int nbOctet = 10);
    
private:
    bool    reponseComplete;
    String  reponseLora;
    bool    attenteReponse;
    
    HardwareSerial Serial3;
    STM32RTC *rtc;
    uint32_t debut;
};

#endif // LORA_H
