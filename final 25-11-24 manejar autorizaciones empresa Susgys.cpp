#include <iostream>
using namespace std;

class Cargo {
	protected:
		char nombC[100];
		vector<char*> permisos;
	public:
		Cargo (const char* nombCargo){
			strcpy(this->nombC, nombCargo);
		}
		char* getNombC() const {
			return nombC;
		}
		bool tienePermiso(const char* permiso); //si el cargo tiene permiso
		void addPermiso(const char* permiso);
		vector<char*> getPermisos() const {
			return permisos;
		}
};

bool Cargo::tienePermiso(const char* permiso) {
	for (auto & p : permisos) { //cambio de const char* p a auto & p
		if(strcmp(p, permiso)==0){
			return true;
		} else return false;
	}
}

void Cargo::addPermiso(const char* permiso) {
	if(!tienePermiso(permiso)) {
		char* newPermiso = new char[strlen(permiso)+1];
		strcpy(newPermiso,permiso);
		permisos.push_back(newPermiso);
	}
}

class Usuario {
	protected:
		char nombU[200];
		int cod;
		vector <Cargo*> cargos;
	public:
		Usuario(const char* nombUsuario, int codUsuario){
			strcpy(this->nombU, nombUsuario);
			this->cod=codUsuario;
		}
		char* getNombU() const {
			return nombU;
		}
		int getCod() const {
			return cod;
		}
		void addCargo(const Cargo& cargo) {
			cargos.push_back(cargo);
		}
		vector<Cargo>& getCargos() const {
			return cargos;
		}
		bool tienePermiso(const char* permiso);
		void guardar(const char* nombArch);
};

bool Usuario::tienePermiso(const char* permiso) {
	for (auto & cargo : cargos){
		if(cargo->tienePermiso(permiso)){
			return true;
		} else return false;
	}
}

void Usuario::guardar(const char* nombArch){
	ofstream archivo;
	archivo.open(nombArch,ios::binary);
	if(archivo.fail()){
		return;
	}
	archivo.write(nombU, sizeof(nombU);
	archivo.write(reinterpret_cast<char*>(&cod), sizeof(cod));
	for(const auto & cargo : cargos) {
		archivo.write(cargo->getNombC(), sizeof(cargo->getNombC()));
		const vector<char*>&permisos = cargo->getPermisos();
		for(const auto & permiso : permisos) {
			size_t longitud = strlen(permiso)+1;
			archivo.write(permiso,longitud);
		}
	}
	archivo.close();
}

class Empresa {
	private:
		vector<Usuario*> usuarios;
		vector<Cargo*> cargos;
	public:
		Empresa() {};
		void addUsuario(const Usuario& usuario, const char* NombA) {
			usuarios.push_back(usuario);
			usuarios.guardar(NombA);
		}
		void addCargo2(const Cargo& cargo) {
			cargos.push_back(cargo);
		}
		bool verificarPermiso(int codUsuario, const char* permisoBusca);
		vector<Cargo*> getCargoMasPermisos();
		vector<string> getPermisosRepetidos();
		vector<Usuario*> getUsuarioMasPermisos();
};

bool Empresa::verificarPermiso(int codUsuario, const char* permisoBusca){
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

vector<Cargo*> Empresa::getCargoMasPermisos() {
		size_t max = 0;
		vector<Cargo*> cargosmaspermisos;
		for (auto & cargo : cargos){
			size_t aux = cargo->getPermisos().size();
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

vector<string> Empresa::getPermisosRepetidos() {
	vector<char*> permisosrepe;
	if(cargos.empty()){
		return permisosrepe;
	}
	map<string, int> m;
	for(auto & cargo : cargos){
		for(auto & permi : cargo->getPermisos()){
			m[permi->getPermisos()]++;
		}
	}
	int aux=cargos.size();
	for(auto & [permiso,num] : m){
		if(num==aux) {
			permisosrepe.push_back(permiso);
		}
	}
	return permisosrepe;
}

vector<Usuario*> Empresa::getUsuarioMasPermisos(){
	size_t max = 0;
	vector<Usuario*> usuariosmaspermisos;
	for(auto & usuario : usuarios){
		size_t aux = 0;
		for(auto & cargo : usuario->getCargos()){
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
