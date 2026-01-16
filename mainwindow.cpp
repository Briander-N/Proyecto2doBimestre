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
#include<fstream>
using namespace std;

struct Juego{
	int id;
	string nombre;
	string categoria;
	string desarrollador;
	float precio;
	int anioPublicacion;
};

const string archivoJuegos = "juegosRegistro.txt";


void guardarJuego(Juego& j) {
    ofstream archivo(archivoJuegos, ios::app);
    if (archivo.is_open()) {
        archivo<<j.id<<";"
        <<j.nombre<<";"
        <<j.categoria<<";"
        <<j.desarrollador<<";"
        <<j.precio<<";"
        <<j.anioPublicacion<<"\n";

    }else {
        cerr<<"No se pudo abrir el archivo para guardar\n";
    }
}

void guardarTodosJuegos(vector<Juego>& juegos){
	ofstream archivo(archivoJuegos);
	if (archivo.is_open()){
		for (int i=0; i<juegos.size(); i++){
			archivo<<juegos[i].id<<";"
			<<juegos[i].nombre<<";"
			<<juegos[i].categoria<<";"
            <<juegos[i].desarrollador<<";"
			<<juegos[i].precio<<";"
			<<juegos[i].anioPublicacion<<"\n";
		}
		archivo.close();
	} else {
		cerr<<"No se pudo abrir el archivo\n";
	}
}

vector<Juego> cargarJuegos(){
	vector<Juego> juegos;
	ifstream archivo(archivoJuegos);
    if (archivo.is_open()) {
        string linea;
        while (getline(archivo,linea)) {
            if (linea.empty()) {
                continue;
            }
            vector<string> campos;
            size_t pos = 0;
            while ((pos = linea.find(";")) != string::npos) {
                campos.push_back(linea.substr(0,pos));
                linea.erase(0,pos+1);
            }
            campos.push_back(linea);
            if (campos.size() == 6) {
                Juego j;
                j.id = stoi(campos[0]);
                j.nombre = campos[1];
                j.categoria = campos[2];
                j.desarrollador = campos[3];
                j.precio = stof(campos[4]);
                j.anioPublicacion = stoi(campos[5]);
                juegos.push_back(j);
            }
        }
    }
    return juegos;	
}


void registrarJuego(){
	Juego nuevo;
	cout<<"Ingresa los datos de registro a continuacion"<<endl;
	cout<<"ID: ";
	cin>>nuevo.id;
	cin.ignore();
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
		cout<<"Precio: $";
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
	guardarJuego(nuevo);
	
	cout<<"Juego registrado exitosamente\n";
}

void mostrarJuegos(){
    vector<Juego> juegos = cargarJuegos();
	if(juegos.empty()){
		cout << "\nNo hay juegos registrados.\n";
		return;
	}

	cout << "\n--- Lista de Juegos ---\n";
	for(int i = 0; i < juegos.size(); i++){
		cout << "ID: " << juegos[i].id << endl;
		cout << "Nombre: " << juegos[i].nombre << endl;
		cout << "Categoria: " << juegos[i].categoria << endl;
		cout << "Desarrollador: " << juegos[i].desarrollador << endl;
		cout << "Precio: $" << juegos[i].precio << endl;
		cout << "Anio de publicacion: " << juegos[i].anioPublicacion << endl;
		cout << "----------------------\n";
	}
}

void eliminarJuego(){
	vector<Juego> juegos = cargarJuegos();
	if(juegos.empty()){
		cout<<"\nNo hay juegos registrados.\n";
		return;
	}
	cout<<"\n--- Eliminar Juego ---\n";
	
	int idBuscado;
	cout<<"Ingrese el id del juego a eliminar: ";
	cin>>idBuscado;
	bool encontrado = false;
	for (int i=0; i<juegos.size();i++){
		if (juegos[i].id == idBuscado){
			cout<<"Eliminando el juego : "<<juegos[i].nombre<<endl; 
			encontrado = true;
			juegos.erase(juegos.begin() + i);
			guardarTodosJuegos(juegos);
			cout<<"Juego eliminado correctamente"<<endl;
			break;
		}
	}
	if (!encontrado){
		cout<<"No se encontro un juego con la ID ingresada"<<endl;
	}
}

int main(){
	vector<Juego> juegos;
	int opcion;
	do{
		cout<<"===Gestion de Videojuegos de Steam===\n";
		cout<<"1. Registrar Juego"<<endl;
		cout<<"2. Ver juegos"<<endl;
		cout<<"3. Modificar juego"<<endl;
		cout<<"4. Eliminar juego"<<endl;
		cout<<"0. Salir"<<endl;
		
		cout<<"Ingresa una opcion: ";
		cin>>opcion;
		switch (opcion){
			case 1:
				registrarJuego();
				break;
			case 2:
				mostrarJuegos();
				break;
			case 3:
				
				break;
			case 4:
				eliminarJuego();
				break;
			case 0:
				cout<<"Saliendo..."<<endl;
				break;
			default:
				cout<<"Opcion no valida";
				break;
		}
    }while (opcion != 0);
    return 0;
}

