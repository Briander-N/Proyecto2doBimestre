#include<iostream>
#include<vector>
#include<string>
using namespace std;

struct Juego{
	string nombre;
	string categoria;
	string desarrollador;
	float precio;
	int anioPublicacion;
};

void registrarJuego(vector<Juego>& juegos){
	Juego nuevo;
	cin.ignore();
	cout<<"\nNombre: ";
	getline(cin, nuevo.nombre);
	cout<<"Categoria: ";
	getline(cin, nuevo.categoria);
	cout<<"Desarrollador: ";
	getline(cin, nuevo.desarrollador);
	cout<<"Precio: ";
	cin>>nuevo.precio;
	cout<<"Anio de publicacion: ";
	cin>>nuevo.anioPublicacion;
	cin.ignore();
	juegos.push_back(nuevo);
}

void mostrarJuegos(const vector<Juego>& juegos){
	if(juegos.empty()){
		cout << "\nNo hay juegos registrados.\n";
		return;
	}

	cout << "\n--- Lista de Juegos ---\n";
	for(int i = 0; i < juegos.size(); i++){
		cout << "Nombre: " << juegos[i].nombre << endl;
		cout << "Categoria: " << juegos[i].categoria << endl;
		cout << "Desarrollador: " << juegos[i].desarrollador << endl;
		cout << "Precio: " << juegos[i].precio << endl;
		cout << "Anio de publicacion: " << juegos[i].anioPublicacion << endl;
		cout << "----------------------\n";
	}
}

int main(){
	vector<Juego> juegos;
	cout<<"===Gestion de Videojuegos de Steam===\n";
	cout<<"---------------------------------\n";
	cout<<"\n---Registrar Juego---\n";
	registrarJuego(juegos);
	cout <<"---Mostrar Juegos---\n";
	mostrarJuegos(juegos);
	
	return 0;
}

