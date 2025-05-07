#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class DeMesa {
protected:
    int nro, cantidad_litros, anio_produccion, precio, espera;
    char tipo;
public:
    DeMesa(int num, int cantlitros, int anioprod, char tip, int price) : nro(num), cantidad_litros(cantlitros), anio_produccion(anioprod), tipo(tip), precio(price) {
        this->espera = 1;
    }
    int calcularTotal();
    int getNro() {
        return nro;
    }
    int getCantLitros() {
        return cantidad_litros;
    }
    int getAnioProd() {
        return anio_produccion;
    }
    char getTipo() {
        return tipo;
    }
    int getPrecio() {
        return precio;
    }
    int getEspera() {
        return espera;
    }
};

int DeMesa::calcularTotal() {
    return this->precio * this->cantidad_litros;
}

class Premium : public DeMesa{
private:
    int espera;
public:
    Premium(int num, int cantlitros, int anioprod, char tip, int price) : DeMesa(num, cantlitros, anioprod, 'p', 200){
        this->espera = 2;
    }
};

class Especial : public DeMesa {
private:
    int espera;
public:
    Especial(int num, int cantlitros, int anioprod, char tip, int price) : DeMesa(num, cantlitros, anioprod, 'e', 320) {
        this->espera = 3;
    }
};

class Empresa {
private:
vector<DeMesa*> vinos;
public:
    Empresa();
    void leerArchivo(char* nombA);
    pair<int, int> anioDeterminado(int aniod);
    bool sePuedeVender(DeMesa* v, int anioa);
    void escribirTexto(char* nombA, char* nombB);
    vector<pair<int,int>> masLitrosEn5();
    int elAnioMasVino();
    int elAnioMasMonto();
}

void Empresa::leerArchivo(char* nombA){
    ifstream archivo;
    archivo.open(nombA, ios::binary);
    if(archivo.fail()){
        return;
    }
    int nro, cantidad, anio;
    char tipo;
    while(archivo.read(reinterpret_cast<char*>(&nro), sizeof(nro))){
        archivo.read(&tipo, sizeof(tipo));
        archivo.read(reinterpret_cast<char*>(&cantidad_litros), sizeof(cantidad_litros));
        archivo.read(reinterpret_cast<char*>(&anio_produccion), sizeof(anio_produccion));
        switch(tipo){
            case 'm': vinos.push_back(new DeMesa); break;
            case 'p': vinos.push_back(new Premium); break;
            case 'e': vinos.push_back(new Especial); break;
        }
    }
    archivo.close();
}

pair<int, int> Empresa::anioDeterminado(int aniod) {
    int montototal = 0, litrostotal = 0;
    for(const auto& v : vinos) {
        if( v->sePuedeVender(v, aniod) ) {
            litrostotal += v->getCantLitros();
            montototal += v->calcularTotal();
        }
    }
    return {litrostotal, montototal};
}

bool Empresa::sePuedeVender(DeMesa* v, int anioa) {
    if( ( anioa - v->getAnioProd() ) >= v->getEspera() ) {
        return true;
    } else return false;
}

void Empresa::escribirTexto(char* nombA, char* nombB) {
    leerArchivo(nombA);
    ofstream archivo2;
    archivo2.open(nombB);
    if(archivo2.fail()) {
        return;
    }

}

void Empresa::archivoDeTexto(char* nombA, char* nombT) {
    leerArchivo(nombA);
    ofstream archivo2;
    archivo2.open(nombT);
    if(archivo2.fail()){
        return;
    }
    archivo2<<"Año - Litros - Monto"<<endl;
    for(int i = 2020 ; i <= 2023; i++) {
        auto [litros, monto] = anioDeterminado(i);
        archivo2<<i<<" - "<<litros<<" - "<<monto<<endl;
    }
    archivo2.close();
}

vector<pair<int,int>> Empresa::masLitrosEn5() {
    map<int,int> aux;
    for( const auto& v : vinos ) {
        aux[v.anio_produccion] += v.cantidad_litros;
    }
    vector<pair<int,int>> los5;
    sort(los5.begin(), los5.end(), [](const auto &a, const auto &b) { return a.second > b.second; });
    los5.resize(5);
    return los5;
}

int Empresa::elAnioMasVino() {
    int maxA = 0, maxL = 0;
    for(int i = 2020; i <= 2025; i++) {
        auto [litros,_]=anioDeterminado(i);
        if(litros > maxL){
            maxL = litros;
            maxA = i;
        }
    }
    return maxA;
}

int Empresa::elAnioMasMonto() {
    int maxA = 0, maxM = 0;
    for(int i = 2020; i <= 2025; i++) {
        auto [_,monto]=anioDeterminado(i);
        if(monto > maxM){
            maxM = monto;
            maxA = i;
        }
    }
    return maxA;
}

template <class T>
class Cola {
public:
    Cola();
    void add(T dato);
    T obtener();
    bool estaVacia() const; //no la pide pero la necesito
private:
    struct Nodo {
        T dato;
        Nodo* sig;
    }
    Nodo* frente;
    Nodo* final;
    int size;
}

bool Cola::estaVacia() const {
    return frente == nullptr;
}

void Cola::add(T dato) {
    Nodo* nuevoNodo = new Nodo{dato, nullptr};
    if(estaVacia()){
        frente=final=nuevoNodo;
    } else {
        final->sig = nuevoNodo;
        final = nuevoNodo;
    }
    size++;
}

T Cola::obtener() {
    if(estaVacia()) {
        return T{};
    }
    Nodo* aux = frente;
    T dato = frente->dato;
    frente = frente->sig;
    if(frente == nullptr){
        final=nullptr;
    }
    delete aux;
    size--;
    return dato;
}