//
// Created by Jasson Rodríguez Méndez on 5/4/18.
//

#ifndef CFACTORIAL_SERVER_MEMORYMANAGER_H
#define CFACTORIAL_SERVER_MEMORYMANAGER_H

#include <map>
#include <string>
#include <rapidjson/document.h>
#include "StructContainer.h"
#include "Variable.h"

/**
 * Maneja la memoria del programa
 */
class MemoryManager {
private:
    /**
     * Tipos de datos admitidos por el programa
     */
    std::map<std::string, int> typeTable;
    /**
     * Tamano de la memoria en bytes
     */
    int size;
    /**
     * Indice con todas las variables almacenadas en memoria
     */
    std::map<int, Variable*> index;
    /**
     * Memoria RAM
     */
    unsigned char* memory;

    std::map<std::string, StructContainer*> classes;

    /**
     * Asigna un espacio de memoria a una variable
     * @param variable Caracteristicas de la variable en formato JSON
     * @return Resultado de la operacion
     */
    rapidjson::Document* assign(rapidjson::Document *variable);
    rapidjson::Document* newStruct(rapidjson::Document *variable);
    rapidjson::Document* defineStruct(rapidjson::Document *variable);
    rapidjson::Document* getValue(rapidjson::Document *variable);
    rapidjson::Document* changeValue(rapidjson::Document *variable);
    rapidjson::Document* getAddress(rapidjson::Document *variable);
    rapidjson::Document* getType(rapidjson::Document *variable);
    rapidjson::Document* closeScope(rapidjson::Document *variable);
    rapidjson::Document* exists(rapidjson::Document *variable);
    rapidjson::Document* isStruct(rapidjson::Document *variable);
    rapidjson::Document* ramStatus(rapidjson::Document *variable);
    rapidjson::Document* newReference(rapidjson::Document *variable);
    rapidjson::Document* reset();

    /**
     * Guarda un valor en una direccion de memoria
     * @param variable Caracteristicas de la variable en formato JSON
     * @param address Direccion de memoria
     * @param type Tipo de la variable
     */
    void saveValue(rapidjson::Document *variable, int address, std::string type);

public:
    /**
     * Constructor del manejador de memoria
     * @param size Tamano de la memoria RAM a reservar
     */
    MemoryManager(int size);
    /**
     * Request para realizar operacion de memoria
     * @param jsonRequest Caracteristicas del request en formato JSON
     * @return Resultado en formato JSON
     */
    rapidjson::Document* request(rapidjson::Document* jsonRequest);
};


#endif //CFACTORIAL_SERVER_MEMORYMANAGER_H
