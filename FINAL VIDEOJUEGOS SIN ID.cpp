#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
using namespace std;

class Producto {
protected:
    char titulo[100];
    char descrip[200];
    float costo;
public:
    Producto(const char* nomb, const char* des, float pesos) : costo(pesos) {
        strcpy(titulo, nomb);
        strcpy(descrip, des);
    }
    const char* getTitulo() const { return titulo; }
    const char* getDescrip() const { return descrip; }
    float getCosto() const { return costo; }
    virtual bool esVideojuego() const = 0;
    virtual ~Producto() {}
};

class Videojuego : public Producto {
private:
    vector<Parche*> parches;
public:
    Videojuego(const char* nomb, const char* des, float pesos)
        : Producto(nomb, des, pesos) {}
    bool esVideojuego() const override { return true; }

    void addParche(Parche* p) {
        parches.push_back(p);
    }
};

class Parche : public Producto {
private:
    Producto* padre;
public:
    Parche(const char* nomb, const char* des, float pesos, Producto* padre_)
        : Producto(nomb, des, pesos), padre(padre_) {}
    Producto* getPadre() const { return padre; }
    bool esVideojuego() const override { return false; }
};

class Gestor {
private:
    vector<Producto*> productos;

    int getIndiceDeProducto(Producto* p) {
        for (size_t i = 0; i < productos.size(); ++i) {
            if (productos[i] == p)
                return i;
        }
        return -1; // No encontrado
    }

public:
    void addVideojuego(Videojuego* nuevo) {
        productos.push_back(nuevo);
    }

    void addParche(Parche* nuevo) {
        productos.push_back(nuevo);
        Producto* padre = nuevo->getPadre();
        Videojuego* videojuego = dynamic_cast<Videojuego*>(padre);
        if (videojuego) {
            videojuego->addParche(nuevo);
        }
    }

    void guardarBinario(const string& dir) {
        ofstream archivo(dir, ios::binary);
        if (archivo.fail()) return;

        int total = productos.size();
        archivo.write(reinterpret_cast<const char*>(&total), sizeof(int));

        for (Producto* p : productos) {
            bool esVj = p->esVideojuego();
            archivo.write(reinterpret_cast<const char*>(&esVj), sizeof(bool));

            int tam = strlen(p->getTitulo()) + 1;
            archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
            archivo.write(p->getTitulo(), tam);

            tam = strlen(p->getDescrip()) + 1;
            archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
            archivo.write(p->getDescrip(), tam);

            float costo = p->getCosto();
            archivo.write(reinterpret_cast<const char*>(&costo), sizeof(float));

            if (!esVj) {
                int idxPadre = getIndiceDeProducto(dynamic_cast<Parche*>(p)->getPadre());
                archivo.write(reinterpret_cast<const char*>(&idxPadre), sizeof(int));
            }
        }
        archivo.close();
    }
};
