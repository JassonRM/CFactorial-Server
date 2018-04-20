//
// Created by Jasson Rodríguez Méndez on 13/4/18.
//

#ifndef CFACTORIAL_SERVER_VARIABLE_H
#define CFACTORIAL_SERVER_VARIABLE_H

#include <string>
#include <stack>

/**
 * Contenedor de las caracteristicas de la variable
 */
struct Variable {
public:
    /**
     * Identificador de la variable
     */
    std::string identifier;
    /**
     * Tamano en memoria de la variable
     */
    int size;
    /**
     * Tipo de la variable
     */
    std::string type;
    /**
     * Cantidad de veces que es referencida
     */
    std::stack<int> references;
};

#endif //CFACTORIAL_SERVER_VARIABLE_H
