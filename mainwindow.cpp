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

#include "sintactical.h"

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
    connect(ui->acsintx, &QAction::triggered, this, &MainWindow::analisisSintactico);


}

MainWindow::~MainWindow()
{
    delete ui;
}

//====== ANALISIS

void MainWindow::analisisSintactico(){


    //guardar cambios
    MainWindow::guardar();

    //verificacion
    if(working_file==""){
        return;
    }

    //cargar archivo al lexico
    loadStdFile(working_file.toStdString());

    //limpiar pila
    while(!ExecucionStack.empty()){
        ExecucionStack.pop();
    }

    //prepara pila de ejecucion
    ExecucionStack.push('$'); //$ [fin de fichero]
    ExecucionStack.push(1);   //produccion program

    //reinicializar ltoken
    Ltoken=0;

    //while
    while(Ltoken!=-1){

        lex_entry *l = dameToken();

        //Analizar estado de terminacion, si lo hay
        if(Ltoken==-1){
            if(ExecucionStack.top()=='$'){
                ExecucionStack.pop();
                QMessageBox msgBox;
                msgBox.setText("Analisis completado correctamente! la sintaxis es correcta");
                msgBox.exec();
            }else{
                while(!ExecucionStack.empty()){
                    ExecucionStack.pop();
                }
                QMessageBox msgBox;
                msgBox.setText("Analisis terminado incorrectamente, se llego al fin de fichero y no termino de analizar");
                msgBox.exec();
                break;
            }
            return;
        }

        //regresa elemento correspondiente a sintactico (>=1000)
        int Stoken = relacionaAlex(Ltoken,*l);

        //obtiene columna
        int rcol = Stoken-1000;

        bool found = false;

        while(!found){

            //imprimirStack(ExecucionStack);

            if(ExecucionStack.top()>=1000){

                //es elemento terminal
                if(ExecucionStack.top()==Stoken){
                    ExecucionStack.pop();
                    found = true;
                }else{
                    //es accion (traduccion
                    if(ExecucionStack.top()>=2000&&ExecucionStack.top()<=2500){
                        //cout<<"ACCION ENCONTRADA: "<<ExecucionStack.top()<<" analizando con -> "<<Stoken<<endl;
                        int accion = ExecucionStack.top();
                        ExecucionStack.pop();
                    }else{
                        cout<<"ERROR DE SINTAXIS, no coinciden los tokens"<<endl;
                        cout<<"En pila: "<<ExecucionStack.top()<<endl;
                        cout<<"Encontrado: "<<Stoken<<endl;
                        break;
                    }
                }

            }else{
                int elem = MATRIZ_PREDICTIVA[ExecucionStack.top()-1][rcol];

                if(elem!=-1){

                    //elimina la produccion
                    ExecucionStack.pop();

                    //insertar producciones a la pila
                    for(int i=10; i>=0;i--){

                        int elem2 = MATRIZ_DE_PRODUCCIONES[(elem-1)][i];

                        //vacio
                        if(elem2==-1){
                            break;
                        }else{
                            //ignorar el relleno
                            if(elem2!=0){
                                ExecucionStack.push(elem2);
                            }
                        }
                    }

                }else{
                    //error de sintaxis
                    cout<<"ERR --> "<<elem<<endl;
                    QMessageBox msgBox;
                    msgBox.setText("Error de sintaxis");
                    msgBox.exec();
                    break;
                }

            }


        }


    }

    closeFile();
}

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



