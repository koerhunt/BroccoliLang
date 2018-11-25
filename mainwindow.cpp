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

    working_file = QFileDialog::getOpenFileName(this,
                                                tr("Abrir archivo fuente"), "/home/shikami", tr("All Files (*.bcli)"));

    if(working_file!=""){

        ui->textEditor->clear();

        QFile file(working_file);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox msgBox;
            msgBox.setText("No se pudo cargar el archivo");
            msgBox.exec();
        }

        QByteArray abt = file.readAll();
        ui->textEditor->append(abt);

        file.close();

    }

}

void MainWindow::guardarComo(){

    working_file = QFileDialog::getSaveFileName(this, tr("Guardar"),
                               "/home/shikami/sin titulo.bcli",
                               tr("All Files (*.bcli)"));

    MainWindow::guardar();

}

void MainWindow::guardar()
{

    if(working_file==""){
        working_file = QFileDialog::getSaveFileName(this, tr("Guardar"),
                                   "/home/shikami/sin titulo.bcli",
                                   tr("All Files (*.bcli)"));
    }

    if(working_file!=""){
        QFile file(working_file);
    }else{

        int ret = QMessageBox::warning(this, tr("Guardar"),
                                       tr("por favor selecciona un archivo valido"),
                                       QMessageBox::Save);
    }

    //working_file.resize(0);

    QFile file(working_file);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox msgBox;
        msgBox.setText("No se pudo guardar el archivo");
        msgBox.exec();
    }else{
        QString s = ui->textEditor->toPlainText();
        file.write(s.toStdString().data());
    }

    file.close();

}

void MainWindow::cerrar()
{
    cout<<"cerrando"<<endl;
}

