#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Juego.h"
#include <QMessageBox>
#include <QFile>
#include <QHeaderView>
#include <QInputDialog>
#include <algorithm>
#include <QTextStream>


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
    QFile file("juegosRegistro.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return 1; // si no existe el archivo, empieza en 1
    }
    QTextStream in(&file);
    int maxID = 0;
    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList campos = linea.split(";");
        if (campos.size() < 1)
            continue;
        int id = campos[0].toInt();
        if (id > maxID)
            maxID = id;
    }
    return maxID + 1;
}

std::vector<Juego>cargarJuegos(){
    std::vector<Juego> juegos;
    QFile file("juegosRegistro.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file); //Permite leer y escribir archivos QString, QFile y asi pos...xd
        while (!in.atEnd()) { //Esto lee hasta que se acabe el archivo
            QString linea = in.readLine(); //Lee una linea del archivo, osea como en python masomenos
            QStringList campos = linea.split(";"); //Es como un vector de QStrings



            if (campos.size() != 6)
                continue;

            Juego juegocargar;
            juegocargar.id = campos[0].toInt();
            juegocargar.nombre = campos[1];
            juegocargar.desarrollador = campos[2];
            juegocargar.categoria = campos[3];
            juegocargar.precio = campos[4].toDouble();
            juegocargar.anioPublicacion = campos[5].toInt();

            juegos.push_back(juegocargar);
        }
        return juegos;

    }else{
        QMessageBox::critical(nullptr, "Error", "No se pudo abrir el archivo"); //Se usa nullptr en lugar de this ya que es pura logica y no depende de la interfaz
        return juegos;
    }



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


void MainWindow::on_btnVer_clicked()
{
    std::vector<Juego>juegos = cargarJuegos();

    if (juegos.empty()){
        QMessageBox::warning(this, "Error", "No hay juegos registrados!");
        return;
    }
    ui->tableJuegos->setRowCount(0);
    for(auto j : juegos){
        int f = ui->tableJuegos->rowCount();
        ui->tableJuegos->insertRow(f);
        ui->tableJuegos->setItem(f, 0, new QTableWidgetItem(QString::number(j.id))); //Ya que no se aceptan int, solo strings, lo mismo para el resto de valores numericos
        ui->tableJuegos->setItem(f, 1, new QTableWidgetItem(j.nombre));
        ui->tableJuegos->setItem(f, 2, new QTableWidgetItem(j.desarrollador));
        ui->tableJuegos->setItem(f, 3, new QTableWidgetItem(j.categoria));
        ui->tableJuegos->setItem(f, 4, new QTableWidgetItem(QString::number(j.precio)));
        ui->tableJuegos->setItem(f, 5, new QTableWidgetItem(QString::number(j.anioPublicacion)));
    }


}

