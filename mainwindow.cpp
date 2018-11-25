#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "stdio.h"
#include "iostream"

#include <QFile>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //acciones del menu
    connect(ui->mabrir, &QAction::triggered, this, &MainWindow::abrir);
    connect(ui->mguardar, &QAction::triggered, this, &MainWindow::guardar);
    connect(ui->mcerrar, &QAction::triggered, this, &MainWindow::cerrar);
    connect(ui->mguardarcomo, &QAction::triggered, this, &MainWindow::guardarComo);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//solts

void MainWindow::abrir()
{

    //cara la ruta del archivo
    working_file = QFileDialog::getOpenFileName(this,
                                                tr("Abrir archivo fuente"), "/home/shikami", tr("All Files (*.bcli)"));
    //valida que se selecciona un archivo
    if(working_file!=""){

        //limpia el editor
        ui->textEditor->clear();

        //abre el archivo, si no puede muestra un error
        QFile file(working_file);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox msgBox;
            msgBox.setText("No se pudo cargar el archivo");
            msgBox.exec();
        }

        //lee el contenido
        QByteArray abt = file.readAll();

        //coloca el contenido en el editor
        ui->textEditor->append(abt);

        //cierra el archivo
        file.close();

    }

}

void MainWindow::guardarComo(){

    //pide al usuario un archivo
    working_file = QFileDialog::getSaveFileName(this, tr("Guardar"),
                               "/home/shikami/sin titulo.bcli",
                               tr("All Files (*.bcli)"));

    MainWindow::guardar();

}

void MainWindow::guardar()
{

    //si no se esta editando un arhicvo existente
    //hace al usuario elegir uno
    if(working_file==""){
        working_file = QFileDialog::getSaveFileName(this, tr("Guardar"),
                                   "/home/shikami/sin titulo.bcli",
                                   tr("All Files (*.bcli)"));
    }

    //si selecciono un archivo valido
    if(working_file!=""){

        QFile file(working_file);

        //abre el archivo en modo escritura
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QMessageBox msgBox;
            msgBox.setText("No se pudo guardar el archivo");
            msgBox.exec();
        }else{

            //obtiene el texto del editor
            QString s = ui->textEditor->toPlainText();

            //escribe el contenido en el archivo
            file.write(s.toStdString().data());
        }

        file.close();

    }else{

        QMessageBox::warning(this, tr("Guardar"),
                                       tr("por favor selecciona un archivo valido"),
                                       QMessageBox::Save);
    }

}

void MainWindow::cerrar()
{
    cout<<"cerrando"<<endl;
}

