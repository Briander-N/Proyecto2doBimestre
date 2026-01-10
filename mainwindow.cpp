/*#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
*/

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
	
	do{
		cout<<"\nNombre: ";
		getline(cin, nuevo.nombre);
		if(nuevo.nombre.empty()){
			cout<<"El nombre no puede estar vacio!!"<<endl;
		}
	}while(nuevo.nombre.empty());

	cout<<"Categoria: ";
	getline(cin, nuevo.categoria);
	cout<<"Desarrollador: ";
	getline(cin, nuevo.desarrollador);
	
	do{
		cout<<"Precio: ";
		cin>>nuevo.precio;
		if(nuevo.precio < 0){
			cout<<"Ingrese un valor valido!"<<endl;
		}
	}while(nuevo.precio < 0);
	
	do{
		cout<<"Anio de publicacion: ";
		cin>>nuevo.anioPublicacion;
		if(nuevo.anioPublicacion < 1950 || nuevo.anioPublicacion > 2026){
		    cout << "Ingresa un anio valido (1950 - 2026)"<<endl;
		}
	}while(nuevo.anioPublicacion < 1950 || nuevo.anioPublicacion > 2026);
	
	cin.ignore();
	juegos.push_back(nuevo);
	cout<<"Juego registrado exitosamente\n";
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
