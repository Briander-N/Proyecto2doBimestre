#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Juego.h"
#include <QMessageBox>
#include <QFile>
#include <QHeaderView>
#include <QInputDialog>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dbBAnioPublicacion->setMinimum(1997);
    ui->dbBAnioPublicacion->setMaximum(2026);
    ui->dbBAnioPublicacion->setValue(2024);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::generarNuevoID()
{
    if (juegos.empty()) {
        return 1;
    }

    int maxID = 0;
    for (const auto& juego : juegos) {
        if (juego.id > maxID) {
            maxID = juego.id;
        }
    }

    return maxID + 1;
}

bool MainWindow::validarCampos()
{
    if (ui->lnENombre->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "El nombre no puede estar vacío");
        ui->lnENombre->setFocus();
        return false;
    }

    if (ui->lnEPrecio->text().toFloat() < 0) {
        QMessageBox::warning(this, "Error", "El precio no puede ser negativo");
        ui->lnEPrecio->setFocus();
        return false;
    }

    if (ui->dbBAnioPublicacion->value() < 1997 || ui->dbBAnioPublicacion->value() > 2026) {
        QMessageBox::warning(this, "Error", "El año debe estar entre 1997 y 2026");
        ui->dbBAnioPublicacion->setFocus();
        return false;
    }

    return true;
}

void MainWindow::limpiarCampos()
{
    ui->lnENombre->clear();
    ui->lnECategoria->clear();
    ui->lnEDesarrollador->clear();
    ui->lnEPrecio->clear();
    ui->dbBAnioPublicacion->setValue(2024);
    ui->lnENombre->setFocus();
}

void MainWindow::on_btnRegistrar_clicked()
{
    if (!validarCampos()) {
        return;
    }

    Juego nuevo;
    nuevo.id = generarNuevoID();
    nuevo.nombre = ui->lnENombre->text();
    nuevo.categoria = ui->lnECategoria->text();
    nuevo.desarrollador = ui->lnEDesarrollador->text();
    nuevo.precio = ui->lnEPrecio->text().toFloat();
    nuevo.anioPublicacion = ui->dbBAnioPublicacion->value();

    juegos.push_back(nuevo);

    QFile file("juegosRegistro.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QString linea = QString::number(nuevo.id) + ";" +
                        nuevo.nombre + ";" +
                        nuevo.categoria + ";" +
                        nuevo.desarrollador + ";" +
                        QString::number(nuevo.precio, 'f', 2) + ";" +
                        QString::number(nuevo.anioPublicacion) + "\n";
        file.write(linea.toUtf8());
        file.close();
    } else {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo para guardar");
        return;
    }
    limpiarCampos();
    QMessageBox::information(this, "Éxito", "Juego registrado correctamente");
}


/*
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include <locale>
#include <windows.h>
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
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	setlocale(LC_ALL, "");
	Juego nuevo;
	cout<<"Ingresa los datos de registro a continuación"<<endl;
	cout<<"ID: ";
	cin>>nuevo.id;
	cin.ignore();
	do{
		cout<<"\nNombre: ";
		getline(cin, nuevo.nombre);
		if(nuevo.nombre.empty()){
			cout<<"El nombre no puede estar vacío!!"<<endl;
		}
	}while(nuevo.nombre.empty());

	cout<<"Categoría: ";
	getline(cin, nuevo.categoria);
	cout<<"Desarrollador: ";
	getline(cin, nuevo.desarrollador);

	do{
		cout<<"Precio: $";
		cin>>nuevo.precio;
		if(nuevo.precio < 0){
			cout<<"Ingrese un valor válido!"<<endl;
		}
	}while(nuevo.precio < 0);

	do{
		cout<<"Anio de publicación: ";
		cin>>nuevo.anioPublicacion;
		if(nuevo.anioPublicacion < 1997 || nuevo.anioPublicacion > 2026){
		    cout << "Ingresa un anio válido (1997 - 2026)"<<endl;
		}
	}while(nuevo.anioPublicacion < 1997 || nuevo.anioPublicacion > 2026);

	cin.ignore();
	guardarJuego(nuevo);

	cout<<"Juego registrado exitosamente\n";
}

void mostrarJuegos(){
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	setlocale(LC_ALL, "");
    vector<Juego> juegos = cargarJuegos();
	if(juegos.empty()){
		cout << "\nNo hay juegos registrados.\n";
		return;
	}

	cout << "\n--- Lista de Juegos ---\n";
	for(int i = 0; i < juegos.size(); i++){
		cout << "ID: " << juegos[i].id << endl;
		cout << "Nombre: " << juegos[i].nombre << endl;
		cout << "Categoría: " << juegos[i].categoria << endl;
		cout << "Desarrollador: " << juegos[i].desarrollador << endl;
		cout << "Precio: $" << juegos[i].precio << endl;
		cout << "Año de publicación: " << juegos[i].anioPublicacion << endl;
		cout << "----------------------\n";
	}
}

void modificarJuego(){
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	setlocale(LC_ALL, "");
    vector<Juego> juegos = cargarJuegos();

    if(juegos.empty()){
        cout << "\nNo hay juegos registrados.\n";
        return;
    }

    int idBuscado;
    cout << "\nIngrese el ID del juego a modificar: ";
    cin >> idBuscado;
    cin.ignore();

    bool encontrado = false;

    for(int i = 0; i < juegos.size(); i++){
        if(juegos[i].id == idBuscado){
            encontrado = true;

            cout << "\nModificando juego\n";

            do{
                cout << "Nuevo nombre: ";
                getline(cin, juegos[i].nombre);
                if(juegos[i].nombre.empty()){
                    cout << "El nombre no puede estar vacío.\n";
                }
            }while(juegos[i].nombre.empty());

            do{
                cout << "Nueva categoría: ";
                getline(cin, juegos[i].categoria);
                if(juegos[i].categoria.empty()){
                    cout << "La categoría no puede estar vacía.\n";
                }
            }while(juegos[i].categoria.empty());

            do{
                cout << "Nuevo desarrollador: ";
                getline(cin, juegos[i].desarrollador);
                if(juegos[i].desarrollador.empty()){
                    cout << "El desarrollador no puede estar vacío.\n";
                }
            }while(juegos[i].desarrollador.empty());

            do{
                cout << "Nuevo precio: $";
                cin >> juegos[i].precio;
                if(juegos[i].precio < 0){
                    cout << "Ingrese un precio válido.\n";
                }
            }while(juegos[i].precio < 0);

            do{
                cout << "Nuevo año de publicación: ";
                cin >> juegos[i].anioPublicacion;
                if(juegos[i].anioPublicacion < 1950 || juegos[i].anioPublicacion > 2026){
                    cout << "Ingrese un año válido (1950 - 2026).\n";
                }
            }while(juegos[i].anioPublicacion < 1950 || juegos[i].anioPublicacion > 2026);

            guardarTodosJuegos(juegos);
            cout << "\nJuego modificado correctamente.\n";
            break;
        }
    }

    if(!encontrado){
        cout << "\nNo se encontró un juego con ese ID.\n";
    }
}


void eliminarJuego(){
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	setlocale(LC_ALL, "");
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
		cout<<"No se encontró un juego con la ID ingresada"<<endl;
	}
}

int main(){
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	setlocale(LC_ALL, "");
	vector<Juego> juegos;
	int opcion;
	do{
		cout<<"===Gestión de Videojuegos de Steam===\n";
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
				modificarJuego();
				break;
			case 4:
				eliminarJuego();
				break;
			case 0:
				cout<<"Saliendo..."<<endl;
				break;
			default:
				cout<<"Opción no valida";
				break;
		}
    }while (opcion != 0);
    return 0;
}
*/
