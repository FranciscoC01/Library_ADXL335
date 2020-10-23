#include "Adxl335.h"
#include <Arduino.h>

//=============metodo=constructor=============

//->Adxl335() realiza la asignacion de los pines de lectura del modulo
//  un cuarto parametro opcional define el muestreo en milisegundos, 
//  por defecto en 1000
Adxl335::Adxl335(int pinx, int piny, int pinz, float alpha, int muestreo){

    this->pinx = pinx;
    this->piny = piny;
    this->pinz = pinz;

    if (alpha > 1.0){
        this->alpha = 1.0;
    }else if (alpha < 0.0){
        this->alpha = 0.0;
    }else{
        this->alpha = alpha;
    }

}

//================metodos=====================

//->filtroGOff() realiza la asignacion de la ganancia y el offset, por defecto
//  seteada la ganancia en 1 y todos los offset en 0.0
void Adxl335::filtroGOff(float ganancia, float offX, float offY, float offZ){
    
    this->ganancia = ganancia;

    this->offX = offX;
    this->offY = offY;
    this->offZ = offZ;
}

//->actualizar() realiza la actualizacion de los valores en el tiempo definido 
//  por muestreo, debe ir como minimo una vez en el loop
void Adxl335::actualizar(){
    ahora = millis();
    if (ahora - ultLec >= muestreo) {
        ultLec = ultLec + muestreo;
        outx = analogRead(pinx) * (1 - alpha) + outx * alpha;                  //Se filtra la entrada (filtro tipo IIR)
        aX = (6.0/(1023-0))*(outx-0)-3.0;                                      //out = ( maxOUT - minOUT / maxIN - minIN )*(IN-minIN)+minOUT ==== SE SIMPLIFICO CON LOS VALORES FIJOS
        aX = aX * ganancia + offX;                                             //Se aplica la ganancia y el offset

        outy = analogRead(piny) * (1 - alpha) + outy * alpha;
        aY = (6.0/(1023-0))*(outy-0)-3.0;
        aY = aY * ganancia + offY;

        outz = analogRead(pinz) * (1 - alpha) + outz * alpha;
        aZ = (6.0/(1023-0))*(outz-0)-3.0;
        aZ = aZ * ganancia + offZ;
    }
}

//->getPitch() realiza el calculo del pitch del modulo
int Adxl335::getPitch(){
    pitch = atan(aY/sqrt(pow(aZ,2)+pow(aX,2)));
    pitch = pitch * (180/PI);
    return (int)pitch;
}

//->getRoll() realiza el calculo del roll del modulo
int Adxl335::getRoll(){
    roll = atan(aX/sqrt(pow(aZ,2)+pow(aY,2)));
    roll = roll * (180/PI);
    return (int) roll; 
}

//->calibrar() funcion que realiza la calibracion del modulo durante 5 segundos
void Adxl335::calibrar(){
    long lecX = 0;
    long lecY = 0;
    long lecZ = 0;
    Serial.println("Deje el modulo quieto y en una superficie plana");
    for (byte i = 5; i > 0; i--){
        Serial.println("iniciando calibracion en: "+(String)i);
        delay(1000);
    }
    Serial.println("=====Iniciando calibracion=====");
    for (int i = 0; i <= 100; i++) {
        lecX = lecX + analogRead(pinx);
        lecY = lecY + analogRead(piny);
        lecZ = lecZ + analogRead(pinz);
        delay(50);
    }
    offX = (6.0/1023)*(lecX/100)-3.0;
    offY = (6.0/1023)*(lecY/100)-3.0;
    offZ = (6.0/1023)*(lecZ/100)-4.0;
    Serial.println("valor Offset X: " + (String)offX +" Offset Y: " + (String)offY +" Offset Z: " + (String)offZ );
    ahora = millis();
    Serial.println("=====Calibracion finalizada=====");
}

//================getters=====================

float Adxl335::getAx(){
  return aX;  
}
float Adxl335::getAy(){
  return aY;  
}
float Adxl335::getAz(){
  return aZ;  
}
