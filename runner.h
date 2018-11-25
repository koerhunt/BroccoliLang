#include "shared.h"
#include "traductor.h"

#include <qinputdialog.h>
#include <qstring.h>

#ifndef RUNNER_H
#define RUNNER_H


// EJECUCION DE PROGRAMA
static int cp = 1; //contador del programa

static bool inited = false;

//Devuelve el valor en string de la variable, constante o temp
cadena_tipo recuperarValor(int addr){
    if(addr>=3500&&addr<5550){

        SimbolosRowPtr a = buscarAddrEnTDS(addr);
        if(a->apram==nullptr){
            return "null";
        }

        switch(a->type){
            case ENTERO:
            {
                int *c = (int*)a->apram;

                cadena_tipo str = std::to_string(*c);
                return str;
            }
            case FLOTANTE:
            {
                float *c = (float*)a->apram;

                cadena_tipo str = std::to_string(*c);
                return str;
            }
            case BOOLEANO:
            {
                bool *c = (bool*)a->apram;

                cadena_tipo str = std::to_string(*c);
                if(str==""){
                    return "0";
                }else{
                    return str;
                }
            }
            case CARACTER:
            {
                char *c = (char*)a->apram;

                cadena_tipo str = std::to_string(*c);
                return str;
            }
            case CADENA:
            {

            }
        }
    }else{
        if(addr>=7000&&addr<8000){
            ConstantesRowPtr a = buscarAddrEnTDC(addr);
            if(a->apram==nullptr){
                return "null";
            }

            switch(a->type){
            case ENTERO:
            {
                int *c = (int*)a->apram;

                cadena_tipo str = std::to_string(*c);
                return str;
            }
            break;
            case FLOTANTE:
            {
                float *c = (float*)a->apram;

                cadena_tipo str = std::to_string(*c);
                return str;
            }
            break;
            case BOOLEANO:
            {
                bool *c = (bool*)a->apram;

                cadena_tipo str = std::to_string(*c);
                return str;
            }
            break;
            case CARACTER:
            {
                char *c = (char*)a->apram;

                cadena_tipo str = std::to_string(*c);
                return str;
            }
            break;
            case CADENA:
            {
                char *c = (char*)a->apram;
                cadena_tipo str;
                while(*c!=0x00){
                    str.append(c);
                    *c++;
                }
                return str;
            }
            break;
            }
        }
    }
}


tipos identificaTipo(int addr){
    if(addr>=3500&&addr<5550){
        SimbolosRowPtr a = buscarAddrEnTDS(addr);
        return a->type;
    }else{
        if(addr>=5550&&addr<7000){
            //es temp
            TempRowPtr a = buscarAddrEnTmp(addr);
            return a->type;
        }else{
            if(addr>=7000&&addr<=8000){
                ConstantesRowPtr a = buscarAddrEnTDC(addr);
                return a->type;
            }
        }

    }
}

int recuperarInt(int addr){
    if(addr>=3500&&addr<5550){
        SimbolosRowPtr a = buscarAddrEnTDS(addr);
        return *(int*)a->apram;
    }else{
        if(addr>=5550&&addr<7000){
            TempRowPtr a = buscarAddrEnTmp(addr);
            return *(int*)a->apram;
        }else{
            if(addr>=7000&&addr<=8000){
                ConstantesRowPtr a = buscarAddrEnTDC(addr);
                return *(int*)a->apram;
            }
        }

    }
}

float recuperarFloat(int addr){
    if(addr>=3500&&addr<5550){
        SimbolosRowPtr a = buscarAddrEnTDS(addr);
        return *(float*)a->apram;
    }else{
        if(addr>=5550&&addr<7000){
            TempRowPtr a = buscarAddrEnTmp(addr);
            return *(float*)a->apram;
        }else{
            if(addr>=7000&&addr<=8000){
                ConstantesRowPtr a = buscarAddrEnTDC(addr);
                return *(float*)a->apram;
            }
        }

    }
}

bool recuperarBool(int addr){
    if(addr>=3500&&addr<5550){
        SimbolosRowPtr a = buscarAddrEnTDS(addr);
        return *(bool*)a->apram;
    }else{
        if(addr>=5550&&addr<7000){
            TempRowPtr a = buscarAddrEnTmp(addr);
            return *(bool*)a->apram;
        }else{
            if(addr>=7000&&addr<=8000){
                ConstantesRowPtr a = buscarAddrEnTDC(addr);
                return *(bool*)a->apram;
            }
        }

    }
}

char recuperarChar(int addr){
    if(addr>=3500&&addr<5550){
        SimbolosRowPtr a = buscarAddrEnTDS(addr);
        return *(char*)a->apram;
    }else{
        if(addr>=5550&&addr<7000){
            TempRowPtr a = buscarAddrEnTmp(addr);
            return *(char*)a->apram;
        }else{
            if(addr>=7000&&addr<=8000){
                ConstantesRowPtr a = buscarAddrEnTDC(addr);
                return *(char*)a->apram;
            }
        }

    }
}


//guarda el valor tx en la variable
void guardarValor(QString tx, int addr){

    SimbolosRowPtr var = buscarAddrEnTDS(addr);

    switch(var->type){
        case ENTERO:
        {
            //praseint
            int num = std::stoi(tx.toStdString());
            int *nump = (int*)malloc(4);

            *nump = num;

            unsigned char *pt;
            pt = (unsigned char*)nump;

            var->apram = pt;
        }
        break;
        case FLOTANTE:
        {
            //prasefloat
            float num = std::stof(tx.toStdString());
            float *nump = (float*)malloc(4);

            *nump = num;

            unsigned char *pt;
            pt = (unsigned char*)nump;

            var->apram = pt;
        }
        break;

    }


}

//guardar temp
void asignarTemp(int addr, unsigned char* valpt, tipos t){

    TempRowPtr nodo = buscarAddrEnTmp(addr);
    if(nodo==nullptr){
        //declarar
        TempRowPtr n = new TempRow();
        n->addr=addr;
        n->type=t;
        n->apram = valpt;

        if(temporales==nullptr){
            temporales = n;
        }else{
            tmpfinal->next2 = n;
        }
        tmpfinal = n;
    }else{
        //declarar
        nodo->type=t;
        nodo->apram = valpt;
    }
}

//inicializar
void inicializarValores(){

    if(!inited){


        //inicializar tabla de variables en 0s
        if(TDS!=nullptr){

            SimbolosRowPtr node = TDS;

            do{

                switch(node->type){
                    case ENTERO:
                    {
                        //praseint
                        int *nump = (int*)malloc(4);
                        *nump=0;
                        unsigned char *pt = (unsigned char*)nump;
                        node->apram = pt;
                        break;
                    }
                    case FLOTANTE:
                    {
                        //prasefloat
                        float *nump = (float*)malloc(4);
                        *nump=0.0;
                        unsigned char *pt = (unsigned char*)nump;
                        node->apram = pt;
                        break;
                    }
                    case BOOLEANO:
                    {
                        //prasebool
                        bool *nump = (bool*)malloc(1);
                        *nump=false;
                        unsigned char *pt = (unsigned char*)nump;
                        node->apram = pt;
                        break;
                    }
                    case CARACTER:
                    {
                        //prasefloat
                        char *nump = (char*)malloc(1);
                        *nump=0x00;
                        unsigned char *pt = (unsigned char*)nump;
                        node->apram = pt;
                        break;
                    }
                    case CADENA:
                    {
                        //TODO PARSE STRING
                        break;
                    }
                 }

                node = node->next2;
            }while(node!=nullptr);
        }

        //inicializar tabla de constantes
        if(TDC!=nullptr){

            ConstantesRowPtr node = TDC;

            do{

                switch(node->type){
                    case ENTERO:
                    {

                        //praseint
                        int num = std::stoi(node->desc);
                        int *nump = (int*)malloc(4);
                        *nump = num;
                        unsigned char *pt;
                        pt = (unsigned char*)nump;
                        node->apram = pt;
                        break;


                    }
                    case FLOTANTE:
                    {
                        //prasefloat
                        float *nump = (float*)malloc(4);
                        *nump = std::stof(node->desc);
                        unsigned char *pt = (unsigned char*)nump;
                        node->apram = pt;
                        break;
                    }
                    case BOOLEANO:
                    {
                        //prasebool
                        bool *nump = (bool*)malloc(1);

                        //interpretar valor
                        if(node->desc=="true"){
                            *nump = 1;
                        }else{
                            if(node->desc=="false"){
                                *nump = 0;
                            }else{
                                *nump = std::stoi(node->desc);
                            }
                        }

                        unsigned char *pt = (unsigned char*)nump;
                        node->apram = pt;
                        break;
                    }
                    case CARACTER:
                    {
                        //prasechar
                        char *nump = (char*)malloc(1);
                        *nump = node->desc.at(1);
                        unsigned char *pt = (unsigned char*)nump;
                        node->apram = pt;
                        break;
                    }
                    case CADENA:
                    {
                        //prasestd
                        string *c = &node->desc;
                        unsigned char pt = (unsigned char)*c->data();

    //                    node->apram = ;
                        break;
                    }
                 }

                node = node->next2;
            }while(node!=nullptr);
        }




    }

    inited = true;
}


//Ejecuta el codigo de operacion
void ejecutarCuadruplo(CuadruploPtr cuadruplo){
    switch (cuadruplo->cop) {
        case INPUT:
        {
            bool ok;
            QString text = QInputDialog::getText(0,"Ingrese valor","Ingrese un valor: ",QLineEdit::Normal,"", &ok);
            if (ok && !text.isEmpty()){
                guardarValor(text, cuadruplo->resl);
            }else{
                guardarValor("0", cuadruplo->resl);
            }
            cp++;
        }
        break;
        case OUTPUT:
        {
            cadena_tipo cad = recuperarValor(cuadruplo->resl);
//            outputConsole->appendPlainText(QString::fromStdString(cad));
            cp++;
            break;
        }
        case ASIG:
        {

        //busca variable y actualiza apuntador
        SimbolosRowPtr pt2 = buscarAddrEnTDS(cuadruplo->resl);

        if(cuadruplo->op1>=3500&&cuadruplo->op1<5550){
            SimbolosRowPtr a = buscarAddrEnTDS(cuadruplo->op1);
            *pt2->apram = *a->apram;
        }else{
            if(cuadruplo->op1>=5550&&cuadruplo->op1<7000){
                TempRowPtr a = buscarAddrEnTmp(cuadruplo->op1);
                *pt2->apram = *a->apram;
            }else{
                if(cuadruplo->op1>=7000&&cuadruplo->op1<=8000){
                    ConstantesRowPtr a = buscarAddrEnTDC(cuadruplo->op1);
                    *pt2->apram= *a->apram;
                }
            }

        }
            cp++;
        }
        break;
        case OR:
        {
           int op1 = recuperarBool(cuadruplo->op1);
           int op2 = recuperarBool(cuadruplo->op2);

           bool resl = op1 || op2;

           //prasebool
           bool *nump = (bool*)malloc(1);
           *nump=resl;
           unsigned char *pt = (unsigned char*)nump;
           asignarTemp(cuadruplo->resl,pt,BOOLEANO);
           cp++;
        }
        break;
        case AND:
        {
            tipos t = identificaTipo(cuadruplo->op1);

            switch(t){
                case BOOLEANO:
                {
                    bool op1 = recuperarBool(cuadruplo->op1);
                    bool op2 = recuperarBool(cuadruplo->op2);

                    bool resl = op1 && op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;

                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);

                }
            }

            cp++;
        }
        break;
        case DIF:
        {
            bool op1 = recuperarBool(cuadruplo->op1);
            bool resl = !op1;

            //prasebool
            bool *nump = (bool*)malloc(1);
            *nump=resl;
            unsigned char *pt = (unsigned char*)nump;
            asignarTemp(cuadruplo->resl,pt,BOOLEANO);
            cp++;
        }
        break;
        case PLUS:
        {
            tipos t = identificaTipo(cuadruplo->op1);
            switch(t){
                case ENTERO:
                {

                    int op1 = recuperarInt(cuadruplo->op1);
                    int op2 = recuperarInt(cuadruplo->op2);

                    int resl = op1 + op2;

                    int *nump = (int*)malloc(4);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,ENTERO);


                }
                break;
                case FLOTANTE:
                {

                    float op1 = recuperarFloat(cuadruplo->op1);
                    float op2 = recuperarFloat(cuadruplo->op2);

                    float resl = op1 + op2;

                    float *nump = (float*)malloc(4);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,ENTERO);


                }
            }
            cp++;
        break;
        }
        case MINUS:
        {
            tipos t = identificaTipo(cuadruplo->op1);
            switch(t){
                case ENTERO:
                {

                    int op1 = recuperarInt(cuadruplo->op1);
                    int op2 = recuperarInt(cuadruplo->op2);

                    int resl = op1 - op2;

                    int *nump = (int*)malloc(4);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,ENTERO);


                }
                break;
                case FLOTANTE:
                {

                    float op1 = recuperarFloat(cuadruplo->op1);
                    float op2 = recuperarFloat(cuadruplo->op2);

                    float resl = op1 - op2;

                    float *nump = (float*)malloc(4);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,ENTERO);


                }
            }
        cp++;
        break;
        }
        case MULT:
        {
            tipos t = identificaTipo(cuadruplo->op1);
            switch(t){
                case ENTERO:
                {

                    int op1 = recuperarInt(cuadruplo->op1);
                    int op2 = recuperarInt(cuadruplo->op2);

                    int *nump = (int*)malloc(4);
                    *nump=op1 * op2;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,ENTERO);

                }
                break;
                case FLOTANTE:
                {

                    float op1 = recuperarFloat(cuadruplo->op1);
                    float op2 = recuperarFloat(cuadruplo->op2);

                    float resl = op1 * op2;

                    float *nump = (float*)malloc(4);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,ENTERO);


                }
            }
            cp++;
        break;
        }
        case DIV:
        {
            tipos t = identificaTipo(cuadruplo->op1);
            switch(t){
                case ENTERO:
                {

                    int op1 = recuperarInt(cuadruplo->op1);
                    int op2 = recuperarInt(cuadruplo->op2);

                    int resl = op1 / op2;

                    int *nump = (int*)malloc(4);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,ENTERO);


                }
                break;
                case FLOTANTE:
                {

                    float op1 = recuperarFloat(cuadruplo->op1);
                    float op2 = recuperarFloat(cuadruplo->op2);

                    float resl = op1 / op2;

                    float *nump = (float*)malloc(4);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,ENTERO);


                }
            }
            cp++;
        break;
        }
        case MODULUS:
        {
            tipos t = identificaTipo(cuadruplo->op1);
            switch(t){
                case ENTERO:
                {

                    int op1 = recuperarInt(cuadruplo->op1);
                    int op2 = recuperarInt(cuadruplo->op2);

                    int resl = op1 % op2;

                    int *nump = (int*)malloc(4);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,ENTERO);


                }
                break;
            }
            cp++;
        break;
        }
        case EQUALS:
        {
            tipos t = identificaTipo(cuadruplo->op1);
            switch(t){
                case ENTERO:
                {

                    int op1 = recuperarInt(cuadruplo->op1);
                    int op2 = recuperarInt(cuadruplo->op2);

                    bool resl = op1 == op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
                break;
                case FLOTANTE:
                {

                    float op1 = recuperarFloat(cuadruplo->op1);
                    float op2 = recuperarFloat(cuadruplo->op2);

                    bool resl = op1 == op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
                break;
                case CARACTER:
                {

                    char op1 = recuperarChar(cuadruplo->op1);
                    char op2 = recuperarChar(cuadruplo->op2);

                    bool resl = op1 == op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
                break;
                case BOOLEANO:
                {

                    char op1 = recuperarBool(cuadruplo->op1);
                    char op2 = recuperarBool(cuadruplo->op2);

                    bool resl = op1 <= op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
            }
            cp++;
        }
        break;
        case NOTEQUALS:
        {
            tipos t = identificaTipo(cuadruplo->op1);
            switch(t){
                case ENTERO:
                {

                    int op1 = recuperarInt(cuadruplo->op1);
                    int op2 = recuperarInt(cuadruplo->op2);

                    bool resl = op1 != op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
                break;
                case FLOTANTE:
                {

                    float op1 = recuperarFloat(cuadruplo->op1);
                    float op2 = recuperarFloat(cuadruplo->op2);

                    bool resl = op1 != op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
                break;
                case CARACTER:
                {

                    char op1 = recuperarChar(cuadruplo->op1);
                    char op2 = recuperarChar(cuadruplo->op2);

                    bool resl = op1 != op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
                break;
                case BOOLEANO:
                {

                    char op1 = recuperarBool(cuadruplo->op1);
                    char op2 = recuperarBool(cuadruplo->op2);

                    bool resl = op1 <= op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
            }
            cp++;
        }
        break;
        case LESSTHAN:
        {
            tipos t = identificaTipo(cuadruplo->op1);
            switch(t){
                case ENTERO:
                {

                    int op1 = recuperarInt(cuadruplo->op1);
                    int op2 = recuperarInt(cuadruplo->op2);

                    bool resl = op1 < op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }break;
                case FLOTANTE:
                {

                    float op1 = recuperarFloat(cuadruplo->op1);
                    float op2 = recuperarFloat(cuadruplo->op2);

                    bool resl = op1 < op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
                break;
                case CARACTER:
                {

                    char op1 = recuperarChar(cuadruplo->op1);
                    char op2 = recuperarChar(cuadruplo->op2);

                    bool resl = op1 < op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
            }
            cp++;
        }
        break;
        case LESSOREQUALSTHAN:
        {
            tipos t = identificaTipo(cuadruplo->op1);
            switch(t){
                case ENTERO:
                {

                    int op1 = recuperarInt(cuadruplo->op1);
                    int op2 = recuperarInt(cuadruplo->op2);

                    bool resl = op1 <= op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
                break;
                case FLOTANTE:
                {

                    float op1 = recuperarFloat(cuadruplo->op1);
                    float op2 = recuperarFloat(cuadruplo->op2);

                    bool resl = op1 <= op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
                break;
                case CARACTER:
                {

                    char op1 = recuperarChar(cuadruplo->op1);
                    char op2 = recuperarChar(cuadruplo->op2);

                    bool resl = op1 <= op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
            }
            cp++;
        }
        break;
        case GREATERTHAN:
        {
            tipos t = identificaTipo(cuadruplo->op1);
            switch(t){
                case ENTERO:
                {

                    int op1 = recuperarInt(cuadruplo->op1);
                    int op2 = recuperarInt(cuadruplo->op2);

                    bool resl = op1 > op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
                break;
                case FLOTANTE:
                {

                    float op1 = recuperarFloat(cuadruplo->op1);
                    float op2 = recuperarFloat(cuadruplo->op2);

                    bool resl = op1 > op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
                break;
                case CARACTER:
                {

                    char op1 = recuperarChar(cuadruplo->op1);
                    char op2 = recuperarChar(cuadruplo->op2);

                    bool resl = op1 > op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
            }
            cp++;
        }
        break;
        case GREATEROREQUALSTHAN:
        {
            tipos t = identificaTipo(cuadruplo->op1);
            switch(t){
                case ENTERO:
                {

                    int op1 = recuperarInt(cuadruplo->op1);
                    int op2 = recuperarInt(cuadruplo->op2);

                    bool resl = op1 >= op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
                break;
                case FLOTANTE:
                {

                    float op1 = recuperarFloat(cuadruplo->op1);
                    float op2 = recuperarFloat(cuadruplo->op2);

                    bool resl = op1 >= op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
                break;
                case CARACTER:
                {

                    char op1 = recuperarChar(cuadruplo->op1);
                    char op2 = recuperarChar(cuadruplo->op2);

                    bool resl = op1 >= op2;

                    //prasebool
                    bool *nump = (bool*)malloc(1);
                    *nump=resl;
                    unsigned char *pt = (unsigned char*)nump;
                    asignarTemp(cuadruplo->resl,pt,BOOLEANO);
                }
            }
            cp++;
        }
        case GOTO:{
            cp = cuadruplo->resl;
        }
        break;
        case GOTOFALSO:
        {
            if(recuperarBool(cuadruplo->op1)==false){
                cp = cuadruplo->resl;
            }else{
                cp++;
            }
        }
        break;
        case GOTOVERDADERO:
        {
            if(recuperarBool(cuadruplo->op1)==true){
                cp = cuadruplo->resl;
            }else{
                cp++;
            }
        }
        break;
    }
}

//Avanza un paso en la ejecucion
int ejecutarStep(){

    inicializarValores();//si ya se inicializaron no pasa nada

    CuadruploPtr cuadruplo;

    cuadruplo = BuscarCuadruplo(cp);
    if(cuadruplo!=nullptr){
        ejecutarCuadruplo(cuadruplo);
        return cuadruplo->key-1;
    }else{
        inited = false;
        return -1;
    }

}

//TODO recuperar Cadena

#endif // RUNNER_H
