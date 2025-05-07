#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Viaje {
public:
    Viaje();
    virtual float calcularPrecio() = 0;
    virtual int calcularTiempo() = 0;
    virtual char* getNombre() {
        return destino;
    }
    virtual char* getTipo() {
        return tipo;
    }
    virtual float getCosto() {
        return costo;
    }
    virtual int getTiempo() {
        return tiempo;
    }
    virtual int getPrecio() {
        return precio;
    }
protected:
    char* destino[100], tipo[50];
    float precio, costo;
    int tiempo;
};

class Avion : public Viaje {
public:
    Avion(char* lugar, float costoi, int hs) {
        strcpy(destino, lugar);
        strcpy(tipo, "avion");
        this->costo = costoi;
        this->tiempo = hs;
    }
    float calcularPrecio() override;
    int calcularTiempo() override;
};

float Avion::calcularPrecio() {
    precio = (precio * 1.2);
    return precio;
}

int Avion::calcularTiempo() {
    tiempo += 2;
    return tiempo;
}

class Colectivo : public Viaje {
public:
    Colectivo(char* lugar, int km2, int hs) {
        strcpy(destino, lugar);
        strcpy(tipo, "bondi");
        this->km =  km2;
        this->costo = 8;
        this->tiempo = hs;
    }
    float calcularPrecio() override;
protected:
    int km;
};

float Colectivo::calcularPrecio() {
    precio = 8*km;
    return precio;
}

class Mixto : public Viaje {
public:
    Mixto(Colectivo* marianomoreno, Avion* aerosix, char* lugar) {
        strcpy(destino, lugar);
        strcpy(tipo, "mixto");
        ersa = marianomoreno;
        avi = aerosix;
        this->costo = ersa->getCosto() + avi->getCosto();
        this->tiempo = calcularTiempo();
    }
    float calcularPrecio() override;
    int calcularTiempo() override;
private:
Colectivo* ersa;
Avion* avi;
};

float Mixto::calcularPrecio() {
    precio = ersa->calcularPrecio() + avi->calcularPrecio();
    return precio;
}

int Mixto::calcularTiempo() {
    return ersa->calcularTiempo() + avi->calcularTiempo();
}

class Empresa {
public:
    void cargarViajes(Viaje* vv);
    void guardarViajes(Viaje* vv, char* nombA);
    vector<int> viajes5Baratos();
    Viaje* viajeMasTime();
    vector<Viaje*> viajeCiertoTime(int horita);
protected:
    vector<Viaje*> viajesuchos;
};

void Empresa::cargarViajes(Viaje* vv) {
    viajesuchos.push_back(vv);
}

void Empresa::guardarViajes(char* nombA) {
    ofstream archivo;
    archivo.open(nombA, ios::binary);
    if(archivo.fail()){
        return;
    }
    for (auto& v : viajesuchos) {
        archivo.write(v->getNombre(),sizeof(v->getNombre()));
        archivo.write(v->getTipo(), strlen(v->getTipo())+1);
        archivo.write(reinterpret_cast<char*>(v->getCosto()), sizeof(v->getCosto()));
        archivo.write(reinterpret_cast<char*>(v->getTiempo()), sizeof(v->getTiempo()));
        archivo.write(reinterpret_cast<char*>(v->getPrecio()), sizeof(v->getPrecio()));
    }
}

vector<int> Empresa::viajes5Baratos() {
    vector<Viaje*> baratos5;
    if (viajesuchos.empty()) {
        return nullptr;
    }
    sort(viajesuchos.begin(), viajesuchos.end(), [](const auto &a, const auto &b) { return a->calcularPrecio() < b->calcularPrecio(); });
    for (const auto& v : viajesuchos) {
        baratos5.push_back(v);
    }
    baratos5.resize(5);
    return baratos5;
}

Viaje* Empresa::viajeMasTime() {
    if (viajesuchos.empty()) {
        return nullptr;
    }
    sort(viajesuchos.begin(), viajesuchos.end(), [](const auto &a, const auto &b) { return a->getTiempo() > b->getTiempo(); });
    return viajesuchos[0];
}

vector<Viaje*> Empresa::viajeCiertoTime(int horita) {
    vector<Viaje*> viajesEspecificos;
    if (viajesuchos.empty()) {
        return nullptr;
    }
    copy(viajesuchos.begin(), viajesuchos.end(), back_inserter(viajesEspecificos), [horita] (const auto& v1) { return v1->getTiempo() == horita; });
    return viajesEspecificos;
}
 
template <class a, class g>
class Tupla {
public:
    Tupla();
    void insertar(a clave, g valor);
    g retornar(a clave);
    void eliminar(a clave);
private:
    struct Nodo {
        a clave;
        g valor;
        Nodo* sig;
    }
    Nodo* cabeza;
    bool encontrarNodo(a clave);
};

bool Tupla::encontrarNodo(a clave) {
    Nodo* actual = cabeza;
    while(actual){
        if(actual->clave == clave) {
            return true;
        }
        actual = actual->sig;
    }
    return false;
}

void Tupla::insertar(a clave, g valor) {
    if(encontrarNodo(clave)) {
        return nullptr;
    } else {
        Nodo* nuevoNodo = new Nodo(clave, valor);
        nuevoNodo->sig = cabeza;
        cabeza = nuevoNodo;
    }

}

g Tupla::retornar(a clave) {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        if (actual->clave == clave) {
            return actual->valor;
        }
        actual = actual->sig;
    }
}

void Tupla::eliminar(a clave) {
    Nodo* actual = cabeza;
    Nodo* anterior = nullptr;
    while (actual) {
        if(actual->clave == clave) {
            if(anterior) {
                cabeza= actual->sig;
            } else {
                anterior->sig = actual->sig;
            }
            delete actual;
            return;
        }
        anterior=actual;
        actual=actual->sig;
    }
}