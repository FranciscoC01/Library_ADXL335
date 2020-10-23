#pragma once

#ifndef ADXL335_H
#define ADXL335_H
class Adxl335 {
    private:
        //==Variable de Inicio
        int muestreo = 1000;           //en milisegundos 
        int pinx, piny, pinz;          
        //==Variable de proceso
        int outx, outy, outz;                      
        float aX, aY, aZ;
        float pitch, roll;
        //==Variable de control 
        float alpha;                   //es el mismo para los 3 ejes
        float ganancia = 1;
        float offX, offY, offZ;
        //==Variable de control de tiempo 
        unsigned long ahora = 0;
        unsigned long ultLec = 0;
    public:
        //metodo constructor
        Adxl335(int pinx, int piny, int pinz, float alpha, int muestreo = 1000);
        
        //metodos
        void filtroGOff(float ganancia, float offX = 0.0, float offY = 0.0, float offZ = 0.0);
        void calibrar();
        void actualizar();
        
        int getPitch();
        int getRoll();

        float getAx();
        float getAy();
        float getAz();
};
#endif /* ADXL335_H */
