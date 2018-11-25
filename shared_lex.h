#include "iostream"

#include <bits/stl_list.h>
#ifndef SHARED_H
#define SHARED_H


//Definiciones de tipos
typedef std::string tipo_cadena;

struct lex_entry{
  tipo_cadena granema;
  tipo_cadena lexema;
  int token;
};

#endif // SHARED_H
