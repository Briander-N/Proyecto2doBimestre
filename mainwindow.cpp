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
	cout<<"Juego registrado exitosamente\n";
}

int main(){
	vector<Juego> juegos;
	cout<<"===Gestion de Videojuegos de Steam===\n";
	cout<<"---------------------------------\n";
	cout<<"\n---Registrar Juego---\n";
	registrarJuego(juegos);
	
	return 0;
}
