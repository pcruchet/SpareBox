/**
 ******************************************************************************
   @file    X_NUCLEO_IKS01A2_LSM303AGR_DataLog_Terminal.ino
   @author  AST
   @version V1.0.0
   @date    7 September 2017
   @brief   Arduino test application for the STMicrolectronics X-NUCLEO-IKS01A2
            MEMS Inertial and Environmental sensor expansion board.
            This application makes use of C++ classes obtained from the C
            components' drivers.
 ******************************************************************************
   @attention

   <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:
     1. Redistributions of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.
     2. Redistributions in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the documentation
        and/or other materials provided with the distribution.
     3. Neither the name of STMicroelectronics nor the names of its contributors
        may be used to endorse or promote products derived from this software
        without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
*/


// Includes.
#include <LSM303AGR_ACC_Sensor.h>
#include <LPS22HBSensor.h>

#define DEV_I2C Wire    //Or Wire
#define SerialPort Serial

// Components.
LSM303AGR_ACC_Sensor *Acc;
LPS22HBSensor *PressTemp;

void interruptFunction()
{
  uint8_t result;

  SerialPort.print("INT source reg :");

  Acc->ReadReg(LSM303AGR_ACC_INT1_SOURCE , &result);
  SerialPort.println(result, HEX);
  
  int32_t accelerometer[3];
  Acc->GetAxes(accelerometer);
  SerialPort.print("| Interrupt [mg]: ");
  SerialPort.print(accelerometer[0]);
  SerialPort.print(" ");
  SerialPort.print(accelerometer[1]);
  SerialPort.print(" ");
  SerialPort.print(accelerometer[2]);
  SerialPort.println(" |");

}


void InitialiserInterruption_LSM303()
{

  // ODR = 100Hz + mode LowPower + Axe Z
  Acc->WriteReg(LSM303AGR_ACC_CTRL_REG1, 0x57);
  //LSM303AGR_ACC_ZEN_ENABLED | y et x
  //LSM303AGR_ACC_ODR_DO_100Hz );



  // Pas de filtre programmé
  Acc->WriteReg(LSM303AGR_ACC_CTRL_REG2, 0x00);

  //AOI1=1 AOI1 activé
  Acc->WriteReg(LSM303AGR_ACC_CTRL_REG3, 0x40);

  //fs=2g
  Acc->WriteReg(LSM303AGR_ACC_CTRL_REG4, 0x00); //bdu à 1 pour le capteur de temperature
  //ou
  //fs=16g (faut taper comme une brute!!)
  //Acc->WriteReg(LSM303AGR_ACC_CTRL_REG4,0x30);

  // Interruption 1 vérouillé
  Acc->WriteReg(LSM303AGR_ACC_CTRL_REG5, 0x08);  //LSM303AGR_ACC_LIR_INT1_ENABLED


  //Déclenchement à 32 * 16mg soit 512mg (sensibilité du capteur) très sensible dans la plaque (0x00 à 0x7f) pour 2g
  Acc->WriteReg(LSM303AGR_ACC_INT1_THS,0x7F);     //changer le couple fs/ths en accord avec le cahier des charges
  //Acc->WriteReg(LSM303AGR_ACC_INT1_THS, 0x7f);

  //Durée de 0
  Acc->WriteReg(LSM303AGR_ACC_INT1_DURATION, 0x00);

  //AOI=0 6D=1 ZHIE,ZLIE,YHIE,YLIE,XHIE,XLIE=1 Détectection d'un mouvement dans les 6 directions
  Acc->WriteReg(LSM303AGR_ACC_INT1_CFG, 0x6F);

}

void setup() {
  // Led.
  pinMode(13, OUTPUT);

  // Initialize serial for output.
  SerialPort.begin(9600);

  // Initialize I2C bus.
  DEV_I2C.begin();

  // Initlialize components.
  Acc = new LSM303AGR_ACC_Sensor(&DEV_I2C);
  Acc->Enable();
  InitialiserInterruption_LSM303();
  pinMode(PC1, INPUT_PULLUP); //PF3
  attachInterrupt(PC1, interruptFunction, RISING); // FALLING // CHANGE //RISING //PF3

  PressTemp = new LPS22HBSensor(&DEV_I2C);
  PressTemp->Enable();
}



void loop() {
 
  delay(1000);
  float pressure, temperature;
  PressTemp->GetPressure(&pressure);
  PressTemp->GetTemperature(&temperature);

  SerialPort.print("Pres[hPa]: ");
  SerialPort.print(pressure, 2);
  SerialPort.print(" | Temp[C]: ");
  SerialPort.println(temperature, 2);
}
