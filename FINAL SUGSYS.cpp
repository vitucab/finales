#include <iostream>
using namespace std;

class Cargos {
	private:
		char nombC[100];
		vector<string>permisos;
	public:
		Cargos(const char* nomb) {
			strcpy(nombC, nomb);
		}
		const char* getNombCargo() {
			return nombC;
		}
		void addPermiso(const char* permiso){
			if(!tienePermiso(permiso)){
				char* newPermiso = new char[strlen(permiso)+1];
				strcpy(newPermiso, permiso);
				permisos.push_back(permiso);
			}
		}
		vector<string> getPermisos() {
			return permisos;
		}
		bool tienePermiso(const char* permiso){
			for(const auto& p : permisos) {
				if(strcmp(p, permiso)==0) {
					return true;
				} else {
					return false;
				}
			}
		}
};

class Usuario {
	private:
		char nombre[200];
		int cod;
		vector<Cargos*> cargos;
	public:
		Usuario(const char* nomb, int c) : cod(c) {
			strcpy(nombre, nomb);
		}
		const char* getNombUsuario() {
			return nombre;
		}
		int getCod() {
			return cod;
		}
		vector<Cargos*> getCargos() {
			return cargos;
		}
		void addCargo(Cargo* cargo) {
			cargos.push_back(cargo);
		}
		bool tienePermiso(const char* permiso){
			for(const auto& c : cargos){
				if(cargo->tienePermiso(permiso)){
					return true;
				} else {
					return false;
				}
			}
		}

class Gestor {
	private:
		vector<Usuario*> usuarios;
		vector<Cargo*> cargos;
	public:
		Gestor() {};
		void addUsuario(const Usuario& usuario) {
			usuarios.push_back(usuario);
		}
		void addCargo2(const Cargo& cargo) {
			cargos.push_back(cargo);
		}
		//actividad 1
		void guardarUsuarios(const char* dirU) {
			ofstream archivo;
			archivo.open(dir, ios::binary || ios::app);
			if(archivo.fail()){
				return;
			}
			for(const auto& u : usuarios){
				int tam = strlen(u->getNombUsuario()) +1;
				archivo.write(reinterpret_cast<char*>(&tam), sizeof(int);
				archivo.write(u->getNombUsuario(), tam);
				archivo.write(reinterpret_cast<char*>(&cod), sizeof(int);
				tam = u->getCargos().size();
				archivo.write(reinterpret_cast<char*>(&tam), sizeof(int);
				for(const auto& c : u->getCargos()){
					tam = strlen(c->getNombCargo())+1;
					archivo.write(u->getNombCargo(), sizeof(tam);
				}
			}
			archivo.close();
		}
		void guardarCargos(const char* dirC) {
			ofstream archivo;
			archivo.open(dir, ios::binary || ios::app);
			if(archivo.fail()){
				return;
			}
			for(const auto& c : cargos) {
				int tam = strlen(c->getNombCargo())+1;
				archivo.write(reinterpret_cast<char*>(&tam), sizeof(int);
				archivo.write(u->getNombCargo(),tam);
				tam = u->getPermisos().size();
				archivo.write(reinterpret_cast<char*>(&tam), sizeof(int);
				for(const auto&  p : c->getPermisos()){
					tam = p.size();
					archivo.write(reinterpret_cast<char*>(&tam), sizeof(int);
					archivo.write(p.c_str(),tam);
				}
			}
			archivo.close();
		}
		void guardarTodo(const char* dirU, const char* dirC) {
			guardarUsuarios(dirU);
			guardarCargos(dirC);
		}
		//actividad 2
		bool verificarPermiso(int codUsuario, const char* permisoBusca){
			for(const Usuario& usuario : usuarios){
				if(usuario->getCod() == codUsuario){
					for(auto & cargo : usuario->getCargos()){
						for(auto & permiso : cargo->getPermisos()){
							if(strcmp(permiso,permisoBusca) == 0){
								return true;
							}
						}
					}
				}
			}
			return false;
		}
		//actividad 3
		//actividad a
		vector<Cargo*> getCargoMasPermisos() {
			int max = 0;
			vector<Cargo*> cargosmaspermisos;
			for (const auto& cargo : cargos){
				int aux = cargo->getPermisos().size();
				if(aux > max){
					max = aux;
					cargosmaspermisos.clear();
					cargosmaspermisos.push_back(cargo);
				} else if(aux == max){
					cargosmaspermisos.push_back(cargo);
				}
			}
			cargosmaspermisos.resize(5);		
			return cargosmaspermisos;
		}
		//actividad b
		vector<string> getPermisosRepetidos() {
			vector<string> permisosrepe;
			if(cargos.empty()){
				return {};
			}
			map<string, int> m;
			for(const auto& cargo : cargos){
				for(const auto& permi : cargo->getPermisos()){
					m[permi]++;
				}
			}
			for(const auto& p : m){
				if(p.second >= 2){
					permisosrepe.push_back(p.first);
				}
			}
			return permisosrepe;
		}
		//actividad c
		vector<Usuario*> getUsuarioMasPermisos(){
			int max = 0;
			vector<Usuario*> usuariosmaspermisos;
			for(const auto& usuario : usuarios){
				int aux = 0;
				for(const auto& cargo : usuario->getCargos()){
					aux += cargo->getPermisos().size();
				}
				if(aux > max){
					max = aux;
					usuariosmaspermisos.clear();
					usuariosmaspermisos.push_back(usuario);
				} else if (aux == max){
					usuariosmaspermisos.push_back(usuario);
				}
			}
			usuariosmaspermisos.resize(5);
			return usuariosmaspermisos;
		}
};

int main(int argc, char *argv[]) {
	
	return 0;
}

