#include "Lora.h"

bool reponseComplete;     /// vrai si '/n' reçu de la carte LRWAN
String reponseLora = "";  /// Réponse envoyé par la carte LRWAN

Lora::Lora(PinName _rx, PinName _tx):
Serial3(_rx,_tx)
{
   Serial3.begin(115200);
}

void Lora::EnvoyerAT(String commande)
{
  Serial.println("envoi de la commande AT");
  Serial3.println(commande);
}

int Lora::available(void)
{
    return Serial3.available() ;
}

void Lora::Initialise()
{
  Serial.println("Lora Initialisé");
}

char Lora::LireCar()
{
    return (char) Serial3.read();
}

void Lora::EcrireCar(char car)
{
    Serial3.write(car);
}
/**
 *  @brief : Evénement en réception sur la liaison série vers LORA
 *           reconstitution d'une trame complète
 */



