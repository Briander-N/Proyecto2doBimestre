#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "Juego.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnRegistrar_clicked();

    void on_btnVer_clicked();

    void on_btnActualizar_clicked();

    void on_btnEliminar_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<Juego> juegos;
    int generarNuevoID();
    bool validarCampos();
    void limpiarCampos();
};
#endif // MAINWINDOW_H
