#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
using namespace std;

class Producto {
    protected:
        char titulo[100];
        char descrip[200];
        float costo;
        int id;
    public:
        Producto(const char* nomb, const char* des, float pesos, int nro) : costo(pesos), id(nro){
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
        virtual bool esVideojuego() const = 0;
};

class Videojuego : public Producto {
    private:
        vector<Parche*> parches;
    public:
    Videojuego(const char* nomb, const char* des, float pesos, int nro): Producto(nomb, des, pesos, nro) {}
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
	}
};

class Parche : public Producto {
    private:
        Producto* padre;
    public:
        Parche(const char* nomb, const char* des, float pesos, int nro): Producto(nomb, des, pesos, nro) {}
        Producto* getPadre() {
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
        vector<Producto*>productos;
    public:
        void addProducto(Producto* nuevo) {
            productos.push_back(nuevo);
        }
		vector<Producto*> getParchesDeUnVJ(Videojuego* videogame){
			Videojuego* vj = dynamic_cast<Videojuego*>(videogame);
			return vj->getParches();
		}
        void guardarBinario(string dir){
            ofstream archivo (dir, ios::binary | ios::app);
            if(archivo.fail()){
                return;
            }
            for(const auto& p : productos) {
                int id = p->getId();
                archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
                int tam = strlen(prod->getTitulo()) +1 ;
                archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
                archivo.write(prod->getTitulo(), tam);
                tam = strlen(prod->getDescrip()) +1 ;
                archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
                archivo.write(prod->getDescrip(), tam);
                float costo = prod->getCosto();
                archivo.write(reinterpret_cast<const char*>(&costo), sizeof(float));
                if(!p->esVideojuego()){
                    int idPadre = dynamic_cast<Parche*>(p)->getPadre()->getId();
                    archivo.write(reinterpret_cast<const char*>(&idPadre), sizeof(int));
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

int main(int argc, char *argv[]) {
	
	return 0;
}

//consigna 1: como ventajas la recursividad nos permite tener un codigo mas limpio y legible
//ademas de ser la solucion directa a problemas recursivos, evita codigo repetitivo o bucles complicados. 
// en cuanto a desventajas requiere un mayor uso de memoria, en algunos casos genera menos
//eficiencia, es facil cometer un error en la definicion de la funcion.
//consigna 2: c++ necesita herencia para implementar el polimorfismo porque
//el mismo se logra cuando varias clases comparten una padre/base y a traves
// de punteros se llaman a metodos virtuales que se comportarán diferentes
// dependiendo el tipo real del objeto.
