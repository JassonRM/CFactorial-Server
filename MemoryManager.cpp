//
// Created by Jasson Rodríguez Méndez on 5/4/18.
//


#include <iostream>
#include <rapidjson/document.h>

#include "MemoryManager.h"

MemoryManager::MemoryManager(int size) {
    typeTable = std::map<std::string, int>();
    typeTable["int"] = 4;
    typeTable["long"] = 8;
    typeTable["char"] = 1;
    typeTable["float"] = 4;
    typeTable["double"] = 8;
    typeTable["reference"] = 4;
    this->size = size;
    memory = new char[size];
}

rapidjson::Document* MemoryManager::request(rapidjson::Document* jsonRequest) {
    std::string requestType = (*jsonRequest)["Request"].GetString();
    rapidjson::Document *response;
    if(requestType == "New Variable"){
        response = assign(jsonRequest);
    }
    std::cout<<"Memory after operation: "<<requestType<<std::endl;
    for(auto it = index.cbegin(); it != index.cend(); ++it){
        std::cout<<it->first<<" : "<<it->second.type + " " + it->second.identifier<<std::endl;
    }

    return response;
}

rapidjson::Document* MemoryManager::assign(rapidjson::Document *variable) {
    Variable *newVariable = new Variable();
    newVariable->identifier = (*variable)["Identifier"].GetString();
    newVariable->type = (*variable)["Type"].GetString();
    newVariable->size = typeTable[newVariable->type];
    newVariable->references = 1;

    if(index.empty() and size > newVariable->size){
        saveValue(variable, 0, newVariable->type);
        index[0] = *newVariable;
    }else{
        for(std::map<int, Variable>::iterator it = index.begin(); it != index.end(); ++it){
            if(std::next(it, 1)->first - (it->first + it->second.size) > newVariable->size || (std::next(it, 1) == index.end() && this->size - (it->first + it->second.size) > newVariable->size)){
                saveValue(variable, it->first + it->second.size, newVariable->type);
                index[it->first + it->second.size] = *newVariable;
                break;
            }
        }
    }

    rapidjson::Document* response = new rapidjson::Document();
    rapidjson::Document::AllocatorType& allocator = response->GetAllocator();
    response->SetObject();
    response->AddMember("result", 1, allocator);
    return response;
}

void MemoryManager::saveValue(rapidjson::Document *variable, int address, std::string type) {
    if(type == "int"){
        int value = (*variable)["Value"].GetInt();
        memory[address] = value;
    }else if(type == "long"){
        long value = (*variable)["Value"].GetInt64();
        memory[address] = value;
    }else if(type == "char"){
        char value = (*variable)["Value"].GetString()[0];
        memory[address] = value;
    }else if(type == "float"){
        float value = (*variable)["Value"].GetFloat();
        memory[address] = value;
    }else if(type == "double") {
        double value = (*variable)["Value"].GetDouble();
        memory[address] = value;
    }else if(type == "reference") {
        int value = (*variable)["Value"].GetInt();
        memory[address] = value;
    }
}