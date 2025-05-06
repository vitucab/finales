#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
using namespace std;

class Videojuegos {
    protected:
        char titulo[100];
        char descrip[200];
        float costo;

    public:
        Videojuegos(const char* nomb, const char* des, float pesos) : costo(pesos){
            strcpy(titulo, nomb);
            strcpy(descrip, des);
        }
};

class Parches {

};

int main(int argc, char *argv[]) {
	
	return 0;
}