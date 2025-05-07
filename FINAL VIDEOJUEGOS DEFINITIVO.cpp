#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
using namespace std;

class Videojuego {
    protected:
        char titulo[100];
        char descrip[200];
        float costo;
    public:
    Videojuego(const char* nomb, const char* des, float pesos) : costo(pesos){
            strcpy(titulo, nomb);
            strcpy(descrip, des);
        }
        const char* getTitulo() {
            return titulo;
        }
        const char* getDescrip() {
            return descrip;
        }
        float getCosto() {
            return costo;
        }
};

class Parche : public Videojuego {

};