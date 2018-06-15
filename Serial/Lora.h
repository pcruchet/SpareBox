#ifndef LORA_H
#define LORA_H

#include <Arduino.h>
#include <HardwareSerial.h>

#define HAVE_HWSERIAL3

extern bool reponseComplete;     /// vrai si '/n' reçu de la carte LRWAN
extern String reponseLora;  /// Réponse envoyé par la carte LRWAN


class Lora
{
  private:
    HardwareSerial Serial3;
  public:
    Lora(PinName _rx, PinName _tx);
    void EnvoyerAT(String commande);
    int available(void);
    char LireCar();
    void EcrireCar(char c);
    void Initialise();
};

extern Lora accesLora;

#endif
