#include "detecteurchoc.h"
/**
 * @brief DetecteurChoc::CalculerDeclencheur
 * @return La valeur à mettre dans le registre INT1_THS_A
 * @details Modifie la valeur de l'attribut pleineEchelle qui détermine la gamme de sensibilité.
 *          A placer dans le  registre LSM303AGR_ACC_CTRL_REG4
 */
byte DetecteurChoc::CalculerDeclencheur()
{
    pleineEchelle = LSM303AGR_ACC_FS_2G;
    float quantum = (2 / 127.0);
    if(sensibilite > 2 && sensibilite <= 4)
    {
        pleineEchelle = LSM303AGR_ACC_FS_4G;
        quantum = (4 / 127.0);
    }
    else if(sensibilite > 4 && sensibilite <= 8)
    {
        pleineEchelle = LSM303AGR_ACC_FS_8G;
        quantum = (8 / 127.0);
    }
    else if(sensibilite > 8 && sensibilite <= 16)
    {
        pleineEchelle = LSM303AGR_ACC_FS_8G;
        quantum = (16 / 127.0);
    }

    return (byte) (sensibilite / quantum);
}

/**
 * @brief DetecteurChoc::DetecteurChoc
 * @param _capteur  pointeur sur le capteur accéléromètre
 * @param _sensibilite sensibilité pour la détection de choc la valeur est inférieur ou égale à 16g
 * @details Initialise le capteur pour une détection sur les 3 axes quelque soit le sens
 *          avec une fréquence de détetcetion à 100HZ en mode normal, sans filtre et déclenchement d'interruption.
 *
 */
DetecteurChoc::DetecteurChoc(LSM303AGR_ACC_Sensor *_capteur,float _sensibilite):
    capteur(_capteur),
    sensibilite(_sensibilite)
{
    byte declencheur = CalculerDeclencheur();

    capteur->Enable();
    // ODR = 100Hz + mode normal + Axes Z,Y,X
    capteur->WriteReg(LSM303AGR_ACC_CTRL_REG1, LSM303AGR_ACC_ODR_DO_100Hz | LSM303AGR_ACC_LPEN_DISABLED | LSM303AGR_ACC_ZEN_ENABLED |
                            LSM303AGR_ACC_YEN_ENABLED | LSM303AGR_ACC_XEN_ENABLED);
    // Pas de filtre programmé
    capteur->WriteReg(LSM303AGR_ACC_CTRL_REG2,LSM303AGR_ACC_HPM_NORMAL | LSM303AGR_ACC_HPCF_00 | LSM303AGR_ACC_FDS_BYPASSED |LSM303AGR_ACC_HPCLICK_DISABLED);
    //AOI1=1 AOI1 activé
    capteur->WriteReg(LSM303AGR_ACC_CTRL_REG3,LSM303AGR_ACC_I1_AOI1_ENABLED);
    // Sensibilité du capteur +/- 2g,  Haute résolution désactivé et Le registre de sortie n'est mis à jour qu'après la lecture du MSB et LSB

    capteur->WriteReg(LSM303AGR_ACC_CTRL_REG4,LSM303AGR_ACC_BDU_DISABLED | pleineEchelle | LSM303AGR_ACC_HR_DISABLED  );
    // Interruption 1 vérouillé jusqu'à la lecture du registre INT1_SRC
    capteur->WriteReg(LSM303AGR_ACC_CTRL_REG5,LSM303AGR_ACC_LIR_INT1_ENABLED);
    //Déclenchement à 127 * 16mg (8 bits pour une sensibilité +-2g) soit 2g (sensibilité du capteur)

    capteur->WriteReg(LSM303AGR_ACC_INT1_THS,declencheur);
    //Durée à 0
    capteur->WriteReg(LSM303AGR_ACC_INT1_DURATION,0x00);

    //AOI= OU 6D=1 ZHIE,ZLIE,YHIE,YLIE,XHIE,XLIE=1 Détectection d'un mouvement dans l'une des 6 directions
    capteur->WriteReg(LSM303AGR_ACC_INT1_CFG,LSM303AGR_ACC_AOI_OR | LSM303AGR_ACC_6D_ENABLED | LSM303AGR_ACC_ZHIE_ENABLED | LSM303AGR_ACC_ZLIE_ENABLED |
                            LSM303AGR_ACC_YHIE_ENABLED | LSM303AGR_ACC_YLIE_ENABLED | LSM303AGR_ACC_XHIE_ENABLED | LSM303AGR_ACC_XLIE_ENABLED );

    Serial.print("PleineEchelle ");
    Serial.print(pleineEchelle,HEX);
    Serial.print(" Declencheur ");
    Serial.println(declencheur,HEX);

}

