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
        int getId() {
            return id;
        }
};

class Parche : public Producto {
    private:
        vector<Producto*>dependencias;
    public:
        Parche(const char* nomb, const char* des, float pesos, int nro): Producto(nomb, des, pesos, nro) {}
        void addDependencia(Producto* nuevo) {
            dependencias.push_back(nuevo);
        }
        vector<Producto*> getDependencias() {
            return dependencias;
        }
};

class Videojuego : public Producto {
    private:
        vector<Parche*>parches;
    public:
        Juego(const char* nomb, const char* des, float pesos, int nro): Producto(nomb, des, pesos, nro) {}
        void addParche(Parche* nuevo){
            parches.push_back(nuevo);
        }
        vector<Parche*> getParches() {
            return parches;
        }
};

class Gestor {
    private:
        vector<Producto*>productos;
    public:
        void addProducto(Producto* nuevo) {
            productos.push_back(nuevo);
        }
        void guardarProducto(string dirP, Producto* prod){
            ofstream archivo (dirP, ios::binary | ios::app);
            if(archivo.fail()){
                return;
            }
            int id = prod->getId();
            archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
            int tam = strlen(prod->getTitulo()) +1 ;
            archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
            archivo.write(prod->getTitulo(), tam);
            tam = strlen(prod->getDescrip()) +1 ;
            archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
            archivo.write(prod->getDescrip(), tam);
            float costo = prod->getCosto();
            archivo.write(reinterpret_cast<const char*>(&costo), sizeof(float));
            archivo.close();
        }
        void guardarRelacion(string dirR, Producto* parchesito){
            ofstream archivo (dirR, ios::binary | ios::app);
            if(archivo.fail()){
                return;
            }
            Parche* parche = dynamic_cast<Parche*>(parchesito);
            int id = parche->getId();
            archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
            int cant = parche->getDependencias().size();
            archivo.write(reinterpret_cast<const char*>(&cant), sizeof(int));
            for(const auto p : parche->getDependencias()){
                id = p->getId();
                archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
            }
            archivo.close();
        }
        void guardarTodo(string dirVJ, string dirPar, string dirR, string dirDep){
            if(productos.empty()){
                return;
            }
            for(const auto& p : productos){
                if(dynamic_cast<Videojuego*>(p)){
                    guardarProducto(dirVJ, p);
                    guardarRelacion(dirR, p);
                } else {
                    guardarProducto(dirPar, p);
                    guardarRelacion(dirDep, p);
                }
            }
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
            sort(games.begin(), games.end(), [](const auto& a, const auto b){return a->getCosto() > b->getCosto();});
            float aux = games[0]->getCosto();
            int cont = 0;
            for(const auto g : games) {
                if(aux == p->getCosto()){
                    cout<<p-getTitulo()<<endl;
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
            sort(games.begin(), games.end(), [](const auto& a, const auto b){return a->getParches().size() > b->getParches().size();});
            float aux = games[0]->getParches().size();
            int cont = 0;
            for(const auto g : games) {
                if(aux == p->getParches().size()){
                    cout<<p-getTitulo()<<endl;
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