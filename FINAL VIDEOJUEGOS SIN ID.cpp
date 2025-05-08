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
    const char* getTitulo() const {
		return titulo; 
	}
    const char* getDescrip() const {
		return descrip; 
	}
    float getCosto() const {
		return costo; 
	}
    virtual bool esVideojuego() const = 0;
};

class Videojuego : public Producto {
private:
    vector<Parche*> parches;
public:
    Videojuego(const char* nomb, const char* des, float pesos)
        : Producto(nomb, des, pesos) {}
    bool esVideojuego() const override {
		return true; 
	}
	vector<Parche*> getParches() {
		return parches;
	}
	void addParche(Parche* parchesito){
		for(const auto& p : parches) {
			if (parchesito->getPadre() == p) {
				parches.push_back(parchesito);
			}
		}
	}
};

class Parche : public Producto {
private:
    Producto* padre;
public:
    Parche(const char* nomb, const char* des, float pesos, Producto* padre_) : Producto(nomb, des, pesos), padre(padre_) {}
    Producto* getPadre() const {
		return padre; 
	}
    bool esVideojuego() const override {
		return false; 
	}
	void addDependencia(Producto* padre){
		this->padre = padre;
	}
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
	void addProducto(Producto* nuevo) {
		productos.push_back(nuevo);
	}
	vector<Producto*> getParchesDeUnVJ(Videojuego* videogame){
		Videojuego* vj = dynamic_cast<Videojuego*>(videogame);
		return vj->getParches();
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
	vector<Videojuego*> vjMasCaros() {
		if(productos.empty()){
			return;
		}
		vector<Videojuego*>games;
		for(const auto& p : productos) {
			if(dynamic_cast<Videojuego*>(p)){
				Videojuego* vj = dynamic_cast<Videojuego*>(p);
				games.push_back(vj);
			}
		}
		sort(games.begin(), games.end(), [](const auto& a, const auto& b){return a->getCosto() > b->getCosto();});
		float aux = games[0]->getCosto();
		int cont = 0;
		for(const auto g : games) {
			if(aux == g->getCosto()){
				cout<<g-getTitulo()<<endl;
				cont++;
			}
		}
		games.resize(cont);
		return games;
	}
	vector<Videojuego*> vjMasParches() {
		if(productos.empty()){
			return;
		}
		vector<Videojuego*>games;
		for(const auto& p : productos) {
			if(dynamic_cast<Videojuego*>(p)){
				Videojuego* vj = dynamic_cast<Videojuego*>(p);
				games.push_back(vj);
			}
		}
		sort(games.begin(), games.end(), [](const auto& a, const auto& b){return a->getParches().size() > b->getParches().size();});
		float aux = games[0]->getParches().size();
		int cont = 0;
		for(const auto g : games) {
			if(aux == g->getParches().size()){
				cout<<g-getTitulo()<<endl;
				cont++;
			}
		}
		games.resize(cont);
		return games;
	}
	void guardarEnTxt(string dirTxt) {
		ofstream archivo (dirTxt);
		if(archivo.fail()){
			return;
		}
		if(productos.empty()){
			return;
		}
		vector<Videojuego*>vjmascaros;
		vector<Videojuego*>vjmasparches;
		vjmascaros = vjMasCaros();
		vjmasparches = vjMasParches();
		archivo<<"Videojuegos de mayor costo: "<<endl;
		for(const auto& vj : vjmascaros){
			archivo<<"Titulo: "<<vj->getTitulo()<<" Costo: $"<<vj->getCosto()<<endl;
		}
		archivo<<"Videojuegos con más parches: "<<endl;
		for(const auto& vj : vjmasparches){
			archivo<<"Titulo: "<<vj->getTitulo()<<" Cantidad de parches: "<<vj->getParches().size()<<endl;
		}
		archivo.close();
	}
};
