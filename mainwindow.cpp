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
    ui->tableJuegos->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
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
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString linea = in.readLine();
            QStringList campos = linea.split(";");



            if (campos.size() != 6)
                continue;

            Juego juegocargar;
            juegocargar.id = campos[0].toInt();
            juegocargar.nombre = campos[1];
            juegocargar.categoria = campos[2];
            juegocargar.desarrollador = campos[3];
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

bool contieneSoloNumeros(const QString &texto)
{
    bool ok;
    texto.toDouble(&ok);
    return ok;
}


bool MainWindow::validarCampos()
{
    QString nombre = ui->lnENombre->text().trimmed(); //trimmed quita espacios en blanco al inicio y al final del texto
    QString desarrollador = ui->lnEDesarrollador->text().trimmed();
    QString categoria = ui->lnECategoria->text().trimmed();
    bool ok;
    double precio = ui->lnEPrecio->text().toDouble(&ok);

    if (nombre.isEmpty() || contieneSoloNumeros(nombre)) {
        QMessageBox::warning(this, "Error", "El nombre no puede estar vacío ni ser solo números");
        ui->lnENombre->setFocus();
        return false;
    }

    if (desarrollador.isEmpty() || contieneSoloNumeros(desarrollador)) {
        QMessageBox::warning(this, "Error", "El desarrollador no puede estar vacío ni ser solo números");
        ui->lnEDesarrollador->setFocus();
        return false;
    }

    if (categoria.isEmpty() || contieneSoloNumeros(categoria)) {
        QMessageBox::warning(this, "Error", "La categoría no puede estar vacía ni ser solo números");
        ui->lnECategoria->setFocus();
        return false;
    }

    if (ui->lnEPrecio->text().toFloat() < 0 || ui->lnEPrecio->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "El precio no puede ser negativo o estar vacío");
        ui->lnEPrecio->setFocus();
        return false;
    }
    if (!ok) {
        QMessageBox::warning(this, "Error", "El precio debe ser un número válido");
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

void MainWindow::limpiarTabla()
{
    ui->tableJuegos->setRowCount(0);
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
    on_btnVer_clicked();
    QMessageBox::information(this, "Éxito", "Juego registrado correctamente");
}


void MainWindow::on_btnVer_clicked()
{
    limpiarTabla();
    juegos = cargarJuegos();

    if (juegos.empty()){
        QMessageBox::warning(this, "Error", "No hay juegos registrados!");
        return;
    }
    ui->tableJuegos->setRowCount(0);
    for(const auto& j : juegos){
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


void MainWindow::on_btnActualizar_clicked()
{
    bool ok;
    int idBuscado = QInputDialog::getInt(//Este genera el cuadro donde se puede ingresar el id
        this,//con esto lo ligamos al ui principal
        "Actualizar juego",
        "Ingrese el ID del juego a actualizar:",
        1,//Valor por defecto que aparece en el cuadro
        1,//Valor minimo que se pude ingresar
        1000,//Valor maximo que se puede ingresar
        1,//Valor de saltos que va a dar
        &ok
        );

    if (!ok) {
        return;
    }

    juegos = cargarJuegos();
    bool encontrado = false;

    for (auto &j : juegos) {
        if (j.id == idBuscado) {

            if (!validarCampos()) {
                return;
            }

            j.nombre = ui->lnENombre->text();
            j.categoria = ui->lnECategoria->text();
            j.desarrollador = ui->lnEDesarrollador->text();
            j.precio = ui->lnEPrecio->text().toFloat();
            j.anioPublicacion = ui->dbBAnioPublicacion->value();

            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        QMessageBox::warning(this, "Error", "No se encontró un juego con ese ID");
        return;
    }

    QFile file("juegosRegistro.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo para actualizar");
        return;
    }

    QTextStream out(&file);
    for (const auto &j : juegos) {
        out << j.id << ";"
            << j.nombre << ";"
            << j.categoria << ";"
            << j.desarrollador << ";"
            << QString::number(j.precio, 'f', 2) << ";"
            << j.anioPublicacion << "\n";
    }

    file.close();
    limpiarCampos();
    on_btnVer_clicked();
    QMessageBox::information(this, "Éxito", "Juego actualizado correctamente");
}



void MainWindow::on_btnEliminar_clicked()
{
    bool ok;
    int idBuscado = QInputDialog::getInt(
        this,
        "Eliminar juego",
        "Ingrese el ID del juego a eliminar:",
        1,
        1,
        1000,
        1,
        &ok
        );

    if (!ok) {
        return;
    }

    juegos = cargarJuegos();

    if (juegos.empty()) {
        QMessageBox::warning(this, "Error", "No hay juegos registrados");
        return;
    }

    bool encontrado = false;

    for (int i = 0; i < juegos.size(); i++) {
        if (juegos[i].id == idBuscado) {
            encontrado = true;

            QMessageBox::information(this, "Eliminando",
                                     QString("Eliminando el juego: %1").arg(juegos[i].nombre));

            juegos.erase(juegos.begin() + i);
            break;
        }
    }

    if (!encontrado) {
        QMessageBox::warning(this, "Error", "No se encontró un juego con ese ID");
        return;
    }

    QFile file("juegosRegistro.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo para guardar los cambios");
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < juegos.size(); i++) {
        out << juegos[i].id << ";"
            << juegos[i].nombre << ";"
            << juegos[i].categoria << ";"
            << juegos[i].desarrollador << ";"
            << QString::number(juegos[i].precio, 'f', 2) << ";"
            << juegos[i].anioPublicacion << "\n";
    }

    file.close();

    if (ui->tableJuegos->rowCount() > 0) {
        on_btnVer_clicked();
    }

    QMessageBox::information(this, "Éxito", "Juego eliminado correctamente");
}

