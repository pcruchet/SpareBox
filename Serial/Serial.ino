/*
  Pont serie
  fil entre PC10 et PA2
  fil entre PC11 et PA3
*/

#include <HardwareSerial.h>
#include "Lora.h"


Lora accesLora(PC_11, PC_10);
int led = 13;

bool complet;
String inputString = ""; 

void setup() 
{
  Serial.begin(115200);
  accesLora.Initialise();
  pinMode(led, OUTPUT);
  digitalWrite(led,HIGH);
  complet = false;

}

// the loop function runs over and over again forever
void loop() 
{
  
  char c;

  if (accesLora.available() > 0) 
  {
        c = accesLora.LireCar();
       
       Serial.print(c);
  }

 
  if(complet)
  {
      Serial.print(inputString);
      complet = false;
      inputString = "";
  }
 
  if (Serial.available() > 0) 
  {
        c = Serial.read();
        
        
        accesLora.EcrireCar(c);
        
        /*
        String commande = "At";
        
        commande += '\r';
        char at[255] ;
        commande.toCharArray(at,3);
        at[3] = 0;
        int i;

        
        for(i = 0 ; i < 3 ; i++)
        {
          Serial.println(commande.charAt(i),HEX);
          accesLora.EcrireCar(commande.charAt(i));  
        }
        */
  }

  //serialEvent();

}

/*

void serialEvent()
{
    while(accesLora.available() > 0)
    {
        char inChar = accesLora.LireCar();
        inputString += inChar;
        if(inChar == 0x20)
          complet = true;
    }
}
*/

