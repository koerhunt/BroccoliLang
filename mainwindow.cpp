#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "stdio.h"
#include "iostream"

#include <QFile>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include "lexical.h"
#include "shared_lex.h"

using namespace std;

//funciones
void limpiarTabla(QTableWidget*);

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

    //analisis
    connect(ui->aclexico, &QAction::triggered, this, &MainWindow::analisisLexico);


}

MainWindow::~MainWindow()
{
    delete ui;
}

//====== ANALISIS

void MainWindow::analisisLexico(){

    //guardar cambios
    MainWindow::guardar();

    //verificacion
    if(working_file!=""){

        //cargar archivo al lexico
        loadStdFile(working_file.toStdString());

        //inicializa Ltoken
        Ltoken = 0;

        //limpar
        limpiarTabla(ui->tableLex);

        while(Ltoken!=-1){
            lex_entry *l = dameToken();

            if(l!=nullptr){

                ui->tableLex->insertRow(ui->tableLex->rowCount());

                ui->tableLex->setItem(ui->tableLex->rowCount()-1,0,new QTableWidgetItem(QString::number(l->token)));
                ui->tableLex->setItem(ui->tableLex->rowCount()-1,1,new QTableWidgetItem(QString::fromStdString(l->lexema)));
                ui->tableLex->setItem(ui->tableLex->rowCount()-1,2,new QTableWidgetItem(QString::fromStdString(l->granema)));


            }

        }

        ui->tableLex->horizontalHeader()->setStretchLastSection(true);

        closeFile();

    }

}

//====== FIN DE ANALISIS

//====== OPERACIONES CON ARCHIVO Y EDITOR

//cargar archivo al editor
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

//guardar archivo (save as)
void MainWindow::guardarComo(){

    //pide al usuario un archivo
    working_file = QFileDialog::getSaveFileName(this, tr("Guardar"),
                               "/home/shikami/sin titulo.bcli",
                               tr("All Files (*.bcli)"));

    MainWindow::guardar();

}

//guardar archivo
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

//====== FIN OPERACIONES CON ARCHIVO Y EDITOR

//==== UTILERIA
void limpiarTabla(QTableWidget *tb){
    int i = tb->rowCount();
    for(int j=0;j<=i;j++){
        tb->removeRow(0);
    }
}



