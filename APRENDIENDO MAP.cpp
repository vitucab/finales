#include <iostream>
#include <map>
using namespace std;

int main(int argc, char *argv[]) {
	map<string, int> AlumnoMedio;
	AlumnoMedio["Juan"] = 10;
	AlumnoMedio["Juan"] += 5;
	cout<<"Juan: " << AlumnoMedio["Juan"] << endl;
	AlumnoMedio["Pepito"]=10;
	for(const auto& p : AlumnoMedio){
		cout<<p.first<<" - "<<p.second<<endl;
	}
	//para c++ 17
	for(const auto& [nombre, nota] : AlumnoMedio){
		cout<<nombre.first<<" - "<<nota.second<<endl;
	}
	map<string, pair<int, int>>
	return 0;
}
