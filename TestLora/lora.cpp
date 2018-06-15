#include "lora.h"

/**
 * @brief Lora::EnvoyerCommandeAT
 * @param commande
 * @return Vrai si la reponse à la commande est bien "ok", faux si ce n'est pas le cas
 * @details Envoye une commande AT  vers le module Lora bloquant 
 *          jusqu'a arrivée du prompt de la réponse
 */
Lora::ETAT_RESEAU Lora::EnvoyerCommandeAT(String commande)
{
    commande += '\r';
    
    Serial.print("EnvoyerCommandeAT : ");
    Serial.println(commande);
    
    ETAT_RESEAU reponseOk = OCCUPE ;
    if(attenteReponse == false)
    {   
        char car;
        int indice = 0;
        do
        {
            car = commande.charAt(indice++);
            Serial3.write(car);         
        } while (car != '\r');
        
        attenteReponse = true;
        reponseOk = NOK;
        
        while(reponseComplete == false)
        {
            ScruterReponse();        
            if(reponseComplete && reponseLora.lastIndexOf("OK") != -1)
            {
                reponseOk = OK;
            }
            else
            {
                if(reponseComplete)
                    Serial.println(reponseLora);
            }
        }       
        reponseComplete = false;
        reponseLora = "";
        attenteReponse = false;
    }
    return reponseOk;
}

/**
 * @brief Lora::ScruterReponse
 * @param carFin caractère de fin de réception
 * @return Vrai si la reponse une est arrivée, faux sinon
 * @details Scrute la réponse du module Lora jusqu'au caractere de fin par défaut "#' et vide le tampon d'entrée
 */
void Lora::ScruterReponse(char carFin, int timeOut)
{
    char carLu;
    bool sortie = false;
    uint32_t temps;
    
    
    
    while (sortie == false)
    {
        if(Serial3.available()>0)
        {
            carLu = Serial3.read();
            if(reponseComplete == false) 
            {
                reponseLora += carLu;
                if(carLu == carFin )
                {
                    reponseComplete = true; 
                    sortie = true;          
                }
            }
        }
        
        if(timeOut != 0)
        {
            temps = rtc->getY2kEpoch();
            if(temps - debut == timeOut)
                sortie = true ;   
        }
    }
}

void Lora::Initialiser()
{
    /*
    ATE=1                                    // pour avoir de l'écho avec PuTTY
    AT+APPEUI=1122334455667788               // L'AppEui défini lors de la déclaration de l'objet
    AT+AK=112233445566778899AABBCCDDEEFF00   // L'AppKey défini  lors de la déclaration de l'objet
    AT+DC=0                                  // Désactive le rapport cyclique afin de pouvoir envoyer plusieurs fois de suite des données
    AT+DEFMOD=6                              // 6 = Mode pour LoraWAN   
    AT+CLASS=0                               // 0 pour la classe A
    AT+BAND=0                                // pour la bande de fréquence européenne 868 MHz
    AT+WDCT                                  // Sauvegarde de la configuration
    */
    
    Serial.println("Initialisation");
    ETAT_RESEAU  retour = EnvoyerCommandeAT("AT");
    Serial.println(retour);
    
    int indice;
    
    String commande = "AT+APPEUI=" ;
    String appeui = APPEUI ;  
    for(indice = 0 ; indice < LONGUEUR_APPEUI ; indice++)
        commande += String (appeui.charAt(indice),HEX);
    
    
    if(EnvoyerCommandeAT(commande) == Lora::OK)
    {
        Serial.println("APPEUI");
        
        String appkey = APPKEY ;
        commande = "AT+AK=" ;
        for(indice = 0 ; indice < LONGUEUR_APPKEY ; indice++)
            commande += String (appkey.charAt(indice),HEX);
        
        if(EnvoyerCommandeAT(commande) == Lora::OK)
        {
            Serial.println("APPKEY");
            if(EnvoyerCommandeAT("AT+DC=0") == Lora::OK) 
            {
                
                if(EnvoyerCommandeAT("AT+CLASS=0") == Lora::OK) 
                {
                    if(EnvoyerCommandeAT("AT+BAND=0") == Lora::OK) 
                    {
                        if(EnvoyerCommandeAT("AT+WDCT") == Lora::OK) 
                        {
                            Serial.println("Configuration enregistrée");
                        }   
                    }   
                    
                }          
            }
        }
    }
}


bool Lora::JoindreLora()
{
    bool retour = false;
    if(EnvoyerCommandeAT("AT+JOIN=1")== Lora::OK)
    {
        debut = rtc->getY2kEpoch();
        ScruterReponse('\r',30);
        if(reponseComplete == true && reponseLora.lastIndexOf("+JoinAccepted") != -1)
        {
            Serial.println(reponseLora);
            retour = true;
                    
        }
        else
        {
            Serial.println("delai ecoulé");
        }
        reponseComplete = false;
        reponseLora = "";
        attenteReponse = false;   
    }
    return retour;
}

void Lora::EnvoyerTrame(byte trame[], int nbOctet)
{
    int indice;
    String tramePourLora = "AT+SEND=2,";
    for(indice=0; indice < nbOctet ; indice++)
    {  
        if(trame[indice] < 0x10) // ajout 0 si nécessaire
            tramePourLora += String(0,HEX);
        
        tramePourLora += String(trame[indice],HEX);
    }          
    tramePourLora += ",1\r";
    
    if(EnvoyerCommandeAT(tramePourLora) == Lora::OK)
    {
        Serial.println("Trame envoyée");
    }
    else
    {
        Serial.println("Trame non envoyée");
    }
}


Lora::Lora(PinName _rx, PinName _tx, STM32RTC *_rtc):
    Serial3(_rx,_tx),
    rtc(_rtc)
{
    reponseComplete = false;
    reponseLora ="";
    attenteReponse = false;
    Serial3.begin(115200);
    
    
    debut = rtc->getY2kEpoch();
}

Lora::~Lora()
{
    
}

