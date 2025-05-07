#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
using namespace std;

class Paquete {
    private:
        char codigo[100];
        char nombre[100];
        int version;
        vector<Paquete*>dependencias;
    public:
        Paquete(const char* cod, const char* nomb, int v) : version(v) {
            strcpy(codigo, cod);
            strcpy(nombre, nomb);
        }
        const char* getCodigo() {
            return codigo;
        }
        const char* getNombre() {
            return nombre;
        }
        int getVersion() {
            return version;
        }
        void addDependencia(Paquete* nuevo){
            dependencias.push_back(nuevo);
        }
        vector<Paquete*> getDependencias() {
            return dependencias;
        }
};

int main(int argc, char *argv[]) {
	
	return 0;
}