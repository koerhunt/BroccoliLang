#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //aciones del menu
    void abrir();
    void guardar();
    void guardarComo();
    void cerrar();
    void analisisLexico();

    void analisisSintactico();
private:
    Ui::MainWindow *ui;
    QString working_file;
};

#endif // MAINWINDOW_H
