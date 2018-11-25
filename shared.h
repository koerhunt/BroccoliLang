#include "iostream"

#include <bits/stl_list.h>
#ifndef SHARED_H
#define SHARED_H


//Definiciones de tipos

//Definiciones de tipo
typedef std::string cadena_tipo;

struct lex_entry{
  cadena_tipo granema;
  cadena_tipo lexema;
  int token;
};


//Valores para los tipos de datos
enum tipos{
    ENTERO=2501,
    FLOTANTE,
    CARACTER,
    CADENA,
    BOOLEANO
};

//Valores para los codigos de operacion
enum cops{
    FONDO_FALSO = -1,
    INPUT = 3000,
    OUTPUT,
    ASIG,
    OR,
    AND,
    DIF,
    PLUS,
    MINUS,
    MULT,
    DIV,
    MODULUS,
    EQUALS,
    NOTEQUALS,
    LESSTHAN,
    LESSOREQUALSTHAN,
    GREATERTHAN,
    GREATEROREQUALSTHAN,
    GOTO,
    GOTOFALSO,
    GOTOVERDADERO
};

//nodo de Estructura de cuadruplos
struct cuadruplo {
    int key;
    cops cop;
    int op1;
    int op2;
    int resl;
    struct cuadruplo *next2;
};

//ESTRUCTURAS
//nodo de Estructura de de tabla de simbolos
struct simbolosRow{
    tipos type;
    int count;
    cadena_tipo desc;
    unsigned char *apram;
    int *apnext;
    struct simbolosRow *next2;
};

struct constantesRow{
    tipos type;
    int count;
    cadena_tipo desc;
    unsigned char *apram;
    struct constantesRow *next2;
};

struct tempRow{
    tipos type;
    int addr;
    unsigned char *apram;
    struct tempRow *next2;
};

//Definiciones de tipos de estructuras
typedef struct simbolosRow SimbolosRow;
typedef SimbolosRow *SimbolosRowPtr;

typedef struct cuadruplo Cuadruplo;
typedef Cuadruplo *CuadruploPtr;

typedef struct constantesRow ConstantesRow;
typedef ConstantesRow *ConstantesRowPtr;

typedef struct tempRow TempRow;
typedef TempRow *TempRowPtr;


//raiz de la estructura
static CuadruploPtr cuadruplos;

//Ultimo elemento de la lista de cuadruplos
static CuadruploPtr FinalCuadruplo;

//elemento final de la tabla de simbolos
static SimbolosRowPtr FinalElemTS;

//raiz de la tabla de simbolos
static SimbolosRowPtr TDS;

//raiz de la tabla de constantes
static ConstantesRowPtr TDC;

//raiz de la tabla de constantes
static ConstantesRowPtr FinalElemTC;

//raiz de la tabla temporal
static TempRowPtr temporales,tmpfinal;

#endif // SHARED_H
