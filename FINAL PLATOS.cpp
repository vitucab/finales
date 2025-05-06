#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
using namespace std;

class Comida {
    protected:
        char nombre[100];
        int id;
    
    public:
        Comida(const char* nomb, int nro) : id(nro) {
            strcpy(nombre,nomb);
        }
        int getId() {
            return id;
        }
        const char* getNombre() {
            return nombre;
        }
        virtual double getCosto() = 0;
};

class Plato: public Comida {
    private:
        vector<Comida*>ingredientes;
    public:
        Plato(const char* nomb, int nro): Comida(nomb, nro) {}
        void ingresarIng(Comida* nueva){
            ingredientes.push_back(nueva);
        }
        double getCosto() {
            double costoTotal = 0;
            for(const auto& i : ingredientes){
                costoTotal = costoTotal+i->getCosto();
            }
            return costoTotal;
        }
        vector<Comida*> getIngredientes(){
            return ingredientes;
        }
};

class Ingrediente: public Comida {
    private:
        double costo;
    public:
        Ingrediente(int price, const char* nomb, int nro): Comida(nomb,nro), costo(price){}
        double getCosto() {
            return costo;
        }
};

class Gestor {
    private:
        vector<Comida*>platubis;
    public:
        void cargarComidas(Comida* nueva) {
            platubis.push_back(nueva);
        }
        void guardarPlatos(string dirP, Plato* plato){
            ofstream archivo (dirP, ios::binary | ios::app);
            if(archivo.fail()){
                return;
            }
            int tam = strlen(plato->getNombre()) +1 ;
            archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
            archivo.write(plato->getNombre(), tam);
            int id = plato->getId();
            archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
            double total = plato->getCosto();
            archivo.write(reinterpret_cast<const char*>(&total), sizeof(int));
            archivo.close();
        }
        void guardarIngredientes(string dirI, Ingrediente* ing){
            ofstream archivo (dirI, ios::binary | ios::app);
            if(archivo.fail()){
                return;
            }
            int tam = strlen(ing->getNombre()) +1 ;
            archivo.write(reinterpret_cast<const char*>(&tam), sizeof(int));
            archivo.write(ing->getNombre(), tam);
            int id = ing->getId();
            archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
            double total = ing->getCosto();
            archivo.write(reinterpret_cast<const char*>(&total), sizeof(int));
            archivo.close();
        }
        void guardarRelacion(string dirR, Plato* platos){
            ofstream archivo (dirR, ios::binary | ios::app);
            if(archivo.fail()){
                return;
            }
            int id = plato->getId();
            archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
            int cant = platos->getIngredientes().size();
            archivo.write(reinterpret_cast<const char*>(&cant), sizeof(int));
            for(const auto p : platos->getIngredientes()){
                id = p->getId();
                archivo.write(reinterpret_cast<const char*>(&id), sizeof(int));
            }
            archivo.close();
        }
        void guardarTodo(string dirP, string dirI, string dirR){
            if(platubis.empty()){
                return;
            }
            for(const auto& p : platubis){
                if(dynamic_cast<Plato*>(p)){
                    Plato* puntPlato = dynamic_cast<Plato*>(p);
                    guardarPlatos(dirP, puntPlato);
                    guardarRelacion(dirR, puntPlato);
                } else {
                    guardarIngredientes(dirI, p);
                }
            }
        }
        void ordenarMenorCosto(vector<Plato*>&platis){
            if(platis.empty()){
                return;
            }
            sort(platis.begin(), platis.end(), [](const auto& a, const auto& b){return a->getCosto()<b-getCosto();});
        }
        void top5Barats(dirA){
            ofstream archivo(dirA);
            if(archivo.fail()){
                return;
            }
            vector<Plato*>platos5barats = platubis;
            ordenarMenorCosto(platos5barats);
            platos5barats.resize(5);
            for(int i=0; i<5; i++){
                archivo<<platos5barats[i]->getNombre()<<" : "<<platos5barats[i]->getCosto()<<endl;
            }
            archivo.close();
        }
        bool tieneHarina(Comida* comida){
            Ingrediente* ing = dynamic_cast<Ingrediente*>(comida);
            if(ing) {
                return strcmp(ing->getNombre(), "harina") == 0;
            }
            Plato* plato = dynamic_cast<Plato*>(comida);
            if(plato){
                for(const auto& i : plato->getIngredientes()){
                    if(tieneHarina(i))
                        return true;
                }
            }
            return false;
        }
        void guardarPlatoSinHarina(string dirA){
            ofstream archivo(dirA);
            if(archivo.fail()){
                return;
            }
            for(const auto& p : platubis){
                if(!contieneHarina(p)){
                    archivo<<p->getNombre()<<endl;
                }
            }
            archivo.close();
        }
};

int main(int argc, char *argv[]) {
	
	return 0;
}

//consigna 1: si se puede usar polimorfismo y es una gran ventaja ya que nos
//agiliza y simplifica el codigo permitiendonos reutilizar metodos en las
//distintas clases hijas sin tener que crear nuevas.
//consigna 2: la herencia es la capacidad de crear nuevas clases basandose
// en otras clases ya definidas.
// la composicion es una clase que tiene objetos de otras clases como atributos
//es una forma flexiva de construir objetos complejos y evita problemas de jerarquia
// la reutilizacion de codigo nos permite tener un codigo limpio, sin duplicidad,
//y nos deja ampliar atributos y metodos o modificarlos.
//se relacionan en que evitan repetir logica, aprovechar lo ya escrito y 
//mantener el diseño limpio y mantenible.