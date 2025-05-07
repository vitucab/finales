#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <map>

using namespace std;

class Alumno {
    protected:
        int cod;
        char nya[100];
        vector<int> notas;
    public:
        Alumno(int codigo,  char* ayn, vector<int> nott) : cod(codigo), notas(nott) {
            strcpy(nya, ayn);
        };
        virtual const char* getTipo() = 0;
		char* getNya() {
			return nya;
		}
        int getCod() {
			return cod;
		}
        virtual bool checkAprobo() = 0;
        int calcularProm();
        vector<int> getNotas() {
            return notas;
        }
};

int Alumno::calcularProm() {
    int aux = 0;
    for ( auto& nota : notas) {
        aux+=nota;
    }
    return aux/notas.size();
}

class Invitado : public Alumno {
    public:
        Invitado(int codigo,  char* ayn, vector<int> nott) : Alumno(codigo, ayn, nott) {};
        const char* getTipo() override {
            return "invitado";
        }
        bool checkAprobo();
};

bool Invitado::checkAprobo()  {
    return notas.size() == 1 && calcularProm()>60;
}

class Medio : public Alumno {
    public:
        Medio(int codigo,  char* ayn, vector<int> nott) : Alumno(codigo, ayn, nott) {};
        const char* getTipo() override {
            return "medio";
        }
         bool checkAprobo()  override;
};

bool Medio::checkAprobo()  {
    return notas.size() == 3 && calcularProm()>70;
}

class Premium : public Alumno {
    public:
        Premium(int codigo,  char* ayn, vector<int> nott) : Alumno(codigo, ayn, nott) {};
        const char* getTipo() override {
            return "premium";
        }
        bool checkAprobo()  override;
};

bool Premium::checkAprobo()  {
    if(notas.size()<5) {
        return false;
    }
    for ( auto& nota : notas) {
        if ( nota <= 70 ) {
            return false;
        }
    }
    return calcularProm()>80;
}

class CursoSys {
    protected:
        vector<Alumno*> alumnos;
    public:
        CursoSys() {};
        void addAlumno(Alumno* a);
        void guardarAlumno(char* NombA);
        vector<Alumno*> addAprobo();
        void cambiarPlan(Alumno& al,  char* plan);
        vector<Alumno*> getAlumnoMejorProm();
        map<string,int> getCantAlumnosXTipo();
};

void CursoSys::addAlumno(Alumno* a) {
    alumnos.push_back(a);
}

void CursoSys::guardarAlumno(char* NombA) {
    ofstream archivo;
    archivo.open(NombA,ios::binary);
    if(archivo.fail()){
        return;
    }
    for (  auto& a : alumnos ) {
        archivo.write(a->getTipo(), strlen(a->getTipo()) +1);
        archivo.write(reinterpret_cast< char*>(a->getCod()), sizeof(a->getCod()));
        archivo.write(a->getNya(),sizeof(a->getNya()));
        ////
        //OPCIONAL, DICE LA CANTIDAD DE NOTAS QUE HAY DEPENDIEDO EL TIPO DEL ALUMNO
        if(strcmp(a->getTipo(), "invitado") == 0 ) {
            archivo.write(reinterpret_cast<char*>(1), sizeof(1));
        } else if (strcmp(a->getTipo(), "medio") == 0) {
            archivo.write(reinterpret_cast<char*>(3), sizeof(3));
        } else if (strcmp(a->getTipo(), "premium") == 0) {
            archivo.write(reinterpret_cast<char*>(5), sizeof(5));
        }
        ////
        for (  auto& nota : a->getNotas()) {
        archivo.write(reinterpret_cast< char*>(&nota), sizeof(nota));
        }
    }
    archivo.close();
}

vector<Alumno*> CursoSys::addAprobo() {
    vector<Alumno*> aprobados;
    for ( auto& a : alumnos) {
        if (a->checkAprobo()) {
            aprobados.push_back(a);
        }
    }
    return aprobados;
}

void CursoSys::cambiarPlan(Alumno& al,  char* plan) {
    if ( strcmp(al.getTipo(), "invitado") == 0 ) {
        if ( strcmp(plan, "medio") == 0 ) {
            new Medio(al.getCod(), al.getNya(), {});
        } else if ( strcmp(plan, "premium") == 0 ) {
            new Premium(al.getCod(), al.getNya(), {});
        }
    }
}

vector<Alumno*> CursoSys::getAlumnoMejorProm() {
    int max = 0;
    vector<Alumno*> alumnosmejoresprom;
    for( auto& al : alumnos) {
        int aux = al->calcularProm();
        if(aux > max) {
            max = aux;
            alumnosmejoresprom.clear();
            alumnosmejoresprom.push_back(al);
        } else if (aux == max) {
            alumnosmejoresprom.push_back(al);
        }
    }
    alumnosmejoresprom.resize(5);
    return alumnosmejoresprom;
}

map<string,int> CursoSys::getCantAlumnosXTipo() {
    map<string, int> cantidadalumnos;
    for(  auto& a : alumnos ) {
        cantidadalumnos[a->getTipo()]++;
    }
    return cantidadalumnos;
}

int main() {
    /*
	CursoSys curso;
    Alumno* a1 = new Invitado(1, "Mariano de la facu", {75});
    Alumno* a2 = new Medio(2, "Garrafita", {80, 70, 75});
    Alumno* a3 = new Premium(3, "Ariana Grande", {85, 90, 80, 88, 92});
    curso.addAlumno(a1);
    curso.addAlumno(a2);
    curso.addAlumno(a3);
    // Guardar alumnos en un archivo binario
    curso.guardarAlumno("alumnos.dat");
    // Obtener alumnos que aprobaron
    vector<Alumno*> aprobados = curso.addAprobo();
    cout << "Alumnos aprobados:" << endl;
    for ( auto& a : aprobados) {
        cout << a->getNya() << endl;
    }
    return 0;
    */

    // Crear alumnos
    vector<int> notas_invitado = {75}; // Un invitado, con una sola nota
    Alumno* invitado = new Invitado(1, "JuanPerez", notas_invitado);

    vector<int> notas_medio = {80, 85, 90}; // Un alumno medio, con 3 notas
    Alumno* medio = new Medio(2, "AnaLopez", notas_medio);

    vector<int> notas_premium = {95, 92, 94, 98, 99}; // Un alumno premium, con 5 notas
    Alumno* premium = new Premium(3, "CarlosGarcia", notas_premium);

    // Crear el sistema de curso
    CursoSys curso;
    
    // Añadir alumnos al curso
    curso.addAlumno(*invitado);
    curso.addAlumno(*medio);
    curso.addAlumno(*premium);
    
    // Verificar los alumnos que aprobaron
    vector<Alumno*> aprobados = curso.addAprobo();
    std::cout << "Alumnos aprobados: \n";
    for (auto& alumno : aprobados) {
        std::cout << alumno->getNya() << " (" << alumno->getCod() << ") Tipo: " << alumno->getTipo() << "\n";
    }

    // Cambiar plan de un alumno (Invitado -> Premium)
    Alumno* nuevo_premium = curso.cambiarPlan(*invitado, "premium");
    curso.addAlumno(*nuevo_premium); // Añadimos el alumno con nuevo plan al sistema

    // Obtener los alumnos con mejor promedio
    vector<Alumno*> mejoresProm = curso.getAlumnoMejorProm();
    std::cout << "\nMejores alumnos por promedio: \n";
    for (auto& alumno : mejoresProm) {
        std::cout << alumno->getNya() << " - Promedio: " << alumno->calcularProm() << "\n";
    }

    // Obtener cantidad de alumnos por tipo
    map<string, int> cantidadPorTipo = curso.getCantAlumnosXTipo();
    std::cout << "\nCantidad de alumnos por tipo: \n";
    for (const auto& entry : cantidadPorTipo) {
        std::cout << entry.first << ": " << entry.second << "\n";
    }

    // Guardar los alumnos en un archivo binario
    char nombreArchivo[] = "alumnos.dat";
    curso.guardarAlumno(nombreArchivo);
    
    return 0;
}
