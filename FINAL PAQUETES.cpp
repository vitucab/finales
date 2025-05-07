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

friend ostream& operator<<(ostream& os, const Paquete& p) {
    os << p.nombre;
    return os;
}

class Gestor {
    private:
        vector<Paquete*> paquetitos;
    public:
        void cargarArchivos() {
            ifstream paquetes("paquetes.dat", ios::binary);
            ifstream dependencias("dependencias.dat", ios::binary);
            if(paquetes.fail() || dependencias.fail()) {
                return;
            }
            char codigo[100], nombre[100], codigopaqdep[100];
            int version;
            while(paquetes.read(codigo, sizeof(codigo))) {
                paquetes.read(nombre, sizeof(nombre));
                paquetes.read(reinterpret_cast<char*>(&version), sizeof(int));
                Paquete* nuevo = new Paquete(codigo, nombre, version);
                paquetitos.push_back(nuevo);
            }
            while(dependencias.read(codigo, sizeof(codigo))) {
                dependencias.read(codigopaqdep, sizeof(codigopaqdep));
                for(const auto& p : paquetitos) {
                    if(strcmp(p->getCodigo(), codigo) == 0) {
                        for(const auto& d : paquetitos) {
                            if(strcmp(d->getCodigo(), codigopaqdep) == 0) {
                                p->addDependencia(d);
                            }
                        }
                    }
                }
            }
            paquetes.close();
            dependencias.close();
        }
        //actividad 1
        vector<Paquete*> obtenerTodasLasDependencias(const char* codigo) {
            Paquete* p = buscarPaquete(codigo);
            vector<Paquete*> dependencias;
            if(p){
                obtenerDependenciasRecursivas(p, dependencias);
            }
            return dependencias;
        }
        void obtenerDependenciasRecursivas(Paquete* p, vector<Paquete*>& dependencias) {
            for(Paquete* dep : p->getDependencias()) {
                dependencias.push_back(dep);
                obtenerDependenciasRecursivas(dep, dependencias);
            }
        }
        Paquete* buscarPaquete(const char* codigo) {
            for(const auto& p : paquetitos) {
                if(strcmp(p->getCodigo(), codigo) == 0) {
                    return p;
                }
            }
            return nullptr;
        }
        //actividad 2
        void escribirDependencias(ostream& os, Paquete* p, int nivel) {
            for(Paquete* dep : p->getDependencias()) {
                for(int i = 0; i < nivel; i++) {
                    os << '\t';                
                }
                os << dep->getNombre() << endl;
                escribirDependencias(os, dep, nivel + 1);
            }
        }
        //actividad 3
        //actividad a
        vector<Paquete*> paqueteMasDependenciasNT() {
            vector<pair<Paquete*, int>> conteo;
            for(const auto& p : paquetitos) {
                conteo.push_back({p, p->getDependencias().size()});
            }
            sort(conteo.begin(), conteo.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
            vector<Paquete*> resultado;
            for(const auto& c : conteo) {
                resultado.push_back(c.first);
            }
            resultado.resize(5);
            return resultado;
        }
        //actividad b
        vector<Paquete*> paquetesSinDependencias() {
            vector<Paquete*> sinDependencias;
            for(const auto p : paquetitos) {
                if(p->getDependencias().empty()) {
                    sinDependencias.push_back(p);
                }
            }
            return sinDependencias;
        }
};

int main(int argc, char *argv[]) {
	
	return 0;
}