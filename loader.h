#ifndef LOADER_H
#define LOADER_H
#define TAM 50
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include "traductor.h"
#include <fstream>
#include <string>
#include <stdlib.h>


cuadruplo *cuad;
constantesRow *constante;
SimbolosRowPtr *simbolos;

void vaciar(char temp[]);

void write(QString Filename){
    QFile mFile(Filename);
    QString ss;
    if(!mFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug()<<"no se puede escribir";
        return;
    }
    QTextStream out (&mFile);
    out << "#Const\n";
    if(TDC!=nullptr){
        ConstantesRowPtr node = TDC;
        do{
            out<<node->count;
            out<<",";
            out<<node->type;
            out<<",";
            out<<QString::fromStdString(node->desc);
            out<<"\n";
            node = node->next2;
        }while(node!=nullptr);
    }
    out << "#Vars\n";
    if(TDS!=nullptr){
        SimbolosRowPtr node = TDS;
        do{
            out<<node->count;
            out<<",";
            out<<node->type;
            out<<",";
            out<<QString::fromStdString(node->desc);
            out<<"\n";
            node = node->next2;
        }while(node!=nullptr);
    }
    out << "#Data\n";
    if(cuadruplos!=nullptr){
        CuadruploPtr node = cuadruplos;
        do{
            if(!traducir){
                out<<node->key;
                out<<",";
                out<<node->cop;
                out<<",";
                out<<node->op1;
                out<<",";
                out<<node->op2;
                out<<",";
                out<<node->resl;
                out<<"\n";
            }else{out<<node->key;
                out<<",";
                out<<node->cop;
                out<<",";
                out<<node->op1;
                out<<",";
                out<<node->op2;
                out<<",";
                out<<node->resl;
                out<<"\n";
            }
            node = node->next2;
        }while(node!=nullptr);
    }
    mFile.flush();
    mFile.close();
}

std::vector<std::string> leerLinea(std::string ln){

    std::vector<std::string> v;
    std::string palabra = "";
    for(int i = 0;i<ln.length();i++){
        char c = ln.at(i);
        if(c==','||c=='\n'){
            v.push_back(palabra);
            palabra = "";
        }else{
            palabra+=c;
        }
    }

    return v;

}


void CargaArchivo(QString filename){

    QFile file(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    int sc;
    int header = -1;
    while (!file.atEnd()) {

         QByteArray line = file.readLine();
         std::string st = line.toStdString();
         cout<<st<<endl;

         if(st.compare("#Const")==1){
             header = 1;
             continue;
         }

         if(st.compare("#Vars")==1){
             header = 2;
             continue;
         }

         if(st.compare("#Data")==1){
             header = 3;
             continue;
         }

         switch(header){
             case 1:{
                ConstantesRowPtr nodo = new ConstantesRow();
                std::vector<std::string> vec = leerLinea(st);

                nodo->count = std::stoi(vec[0]);
                nodo->type = (tipos)std::stoi(vec[1]);
                nodo->desc = vec[2];

                if(TDC==nullptr){
                    TDC = nodo;
                    FinalElemTC = nodo;
                }else{
                    FinalElemTC->next2 = nodo;
                    FinalElemTC = nodo;
                }
             }
             break;
             case 2:{
                SimbolosRowPtr nodo = new SimbolosRow();
                std::vector<std::string> vec = leerLinea(st);

                nodo->count = std::stoi(vec[0]);
                nodo->type = (tipos)std::stoi(vec[1]);
                nodo->desc = vec[2];

                if(TDS==nullptr){
                    TDS = nodo;
                    FinalElemTS = nodo;
                }else{
                    FinalElemTS->next2 = nodo;
                    FinalElemTS = nodo;
                }

             }
             break;
             case 3:{
                CuadruploPtr nodo = new Cuadruplo();
                std::vector<std::string> vec = leerLinea(st);

                nodo->key = std::stoi(vec[0]);
                nodo->cop = (cops)std::stoi(vec[1]);
                nodo->op1=  std::stoi(vec[2]);
                nodo->op2=  std::stoi(vec[3]);
                nodo->resl =  std::stoi(vec[4]);

                if(cuadruplos==nullptr){
                    cuadruplos = nodo;
                    FinalCuadruplo = nodo;
                }else{
                    FinalCuadruplo->next2 = nodo;
                    FinalCuadruplo = nodo;
                }

             }
             break;
         }

    }

}



#endif // LOADER_H
