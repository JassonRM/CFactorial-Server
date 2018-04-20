//
// Created by Jasson Rodríguez Méndez on 5/4/18.
//


#include <iostream>
#include <rapidjson/document.h>

#include "MemoryManager.h"
#include "StructInstance.h"

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
    }else if(requestType == "New Struct"){
        response = newStruct(jsonRequest);
    }else if(requestType == "Get Value"){
        response = getValue(jsonRequest);
    }else if(requestType == "Change Value"){
        response = changeValue(jsonRequest);
    }else if(requestType == "Is Variable"){
        response = exists(jsonRequest);
    }else if(requestType == "Define Struct"){
        response = defineStruct(jsonRequest);
    }else if(requestType == "Get Address"){
        response = getAddress(jsonRequest);
    }else if(requestType == "Get Type"){
        response = getType(jsonRequest);
    }else if(requestType == "Is Struct"){
        response  = isStruct(jsonRequest);
    }
    std::cout<<"Memory after operation: "<<requestType<<std::endl;
    for(auto it = index.cbegin(); it != index.cend(); ++it){
        std::cout<<it->first<<" : "<<it->second->type + " " + it->second->identifier<<std::endl;
    }

    return response;
}

rapidjson::Document* MemoryManager::assign(rapidjson::Document *variable) {
    Variable *newVariable = new Variable();
    newVariable->identifier = (*variable)["Identifier"].GetString();
    newVariable->type = (*variable)["Type"].GetString();
    newVariable->size = typeTable[newVariable->type];
    newVariable->references.push((*variable)["Scope"].GetInt());

    if(index.empty() and size > newVariable->size){
        saveValue(variable, 0, newVariable->type);
        index[0] = newVariable;
    }else{
        for(std::map<int, Variable*>::iterator it = index.begin(); it != index.end(); ++it){
            if(std::next(it, 1)->first - (it->first + it->second->size) > newVariable->size || (std::next(it, 1) == index.end() && this->size - (it->first + it->second->size) > newVariable->size)){
                saveValue(variable, it->first + it->second->size, newVariable->type);
                index[it->first + it->second->size] = newVariable;
                break;
            }
        }
    }

    rapidjson::Document* response = new rapidjson::Document();
    rapidjson::Document::AllocatorType& allocator = response->GetAllocator();
    response->SetObject();
    response->AddMember("Result", 1, allocator);
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



rapidjson::Document* MemoryManager::changeValue(rapidjson::Document *variable) {
    for(std::map<int, Variable*>::iterator it = index.begin(); it != index.end(); ++it){
        if(it->second->identifier == (*variable)["Identifier"].GetString()){
            saveValue(variable, it->first, it->second->type);
            break;
        }
    }
}

rapidjson::Document* MemoryManager::getValue(rapidjson::Document *variable) {
    if((*variable)["Struct"].GetString() != ""){
        for (std::map<int, Variable*>::iterator it = index.begin(); it != index.end(); ++it) {
            if (it->second->identifier == (*variable)["Struct"].GetString()) {

                StructInstance *structInstance = (StructInstance*) it->second;

                std::string type = it->second->type;

                int address = it->first;

                for(std::map<int, Variable>::iterator it = structInstance->variables.begin(); it != structInstance->variables.end(); ++it){
                    if(it->second.identifier == (*variable)["Identifier"].GetString()){
                        address += it->first;
                    }
                }


                rapidjson::Document *response = new rapidjson::Document();
                rapidjson::Document::AllocatorType &allocator = response->GetAllocator();
                response->SetObject();

                if (type == "int") {
                    int value = memory[address];
                    response->AddMember("Value", value, allocator);
                } else if (type == "long") {
                    int64_t value = memory[address];
                    response->AddMember("Value", value, allocator);
                } else if (type == "char") {
                    char value = memory[address];
                    response->AddMember("Value", value, allocator);
                } else if (type == "float") {
                    float value = memory[address];
                    response->AddMember("Value", value, allocator);
                } else if (type == "double") {
                    double value = memory[address];
                    response->AddMember("Value", value, allocator);
                } else if (type == "reference") {
                    int value = memory[address];
                    response->AddMember("Value", value, allocator);
                }

                return response;
            }
        }
    }else {
        std::cout<<"Valor de una variable"<<std::endl;
        for (std::map<int, Variable*>::iterator it = index.begin(); it != index.end(); ++it) {
            if (it->second->identifier == (*variable)["Identifier"].GetString()) {

                std::string type = it->second->type;
                int address = it->first;

                rapidjson::Document *response = new rapidjson::Document();
                rapidjson::Document::AllocatorType &allocator = response->GetAllocator();
                response->SetObject();

                rapidjson::Value typeValue(type.c_str(), type.size(), allocator);
                response->AddMember("Type", typeValue, allocator);

                if (type == "int") {
                    int value = memory[address];
                    response->AddMember("Value", value, allocator);
                } else if (type == "long") {
                    int64_t value = memory[address];
                    response->AddMember("Value", value, allocator);
                } else if (type == "char") {
                    char value = memory[address];
                    response->AddMember("Value", value, allocator);
                } else if (type == "float") {
                    float value = memory[address];
                    response->AddMember("Value", value, allocator);
                } else if (type == "double") {
                    double value = memory[address];
                    response->AddMember("Value", value, allocator);
                } else if (type == "reference") {
                    int value = memory[address];
                    response->AddMember("Value", value, allocator);
                }

                return response;
            }
        }
    }
}

rapidjson::Document* MemoryManager::getAddress(rapidjson::Document *variable) {
    for(std::map<int, Variable*>::iterator it = index.begin(); it != index.end(); ++it){
        if(it->second->identifier == (*variable)["Identifier"].GetString()){
            rapidjson::Document* response = new rapidjson::Document();
            rapidjson::Document::AllocatorType& allocator = response->GetAllocator();
            response->SetObject();
            response->AddMember("Address", it->first, allocator);
            return response;
        }
    }
}

rapidjson::Document* MemoryManager::getType(rapidjson::Document *variable) {
    for(std::map<int, Variable*>::iterator it = index.begin(); it != index.end(); ++it){
        if(it->second->identifier == (*variable)["Identifier"].GetString()){
            rapidjson::Document* response = new rapidjson::Document();
            rapidjson::Document::AllocatorType& allocator = response->GetAllocator();
            response->SetObject();
            rapidjson::Value value(it->second->type.c_str(), it->second->type.size(), allocator);
            response->AddMember("Type", value, allocator);
            return response;
        }
    }
}


rapidjson::Document* MemoryManager::exists(rapidjson::Document *variable) {
    rapidjson::Document* response = new rapidjson::Document();
    rapidjson::Document::AllocatorType& allocator = response->GetAllocator();
    response->SetObject();

    bool exists = false;

    for(std::map<int, Variable*>::iterator it = index.begin(); it != index.end(); ++it){
        if(it->second->identifier == (*variable)["Identifier"].GetString()){
            exists = true;
        }
    }
    response->AddMember("Result", exists, allocator);
    return response;
}

rapidjson::Document* MemoryManager::defineStruct(rapidjson::Document *variable) {
    StructContainer* container = new StructContainer;

    container->id = (*variable)["Identifier"].GetString();


    auto variables = (*variable)["Variables"].GetArray();

    int address = 0;
    for(int i = 0; i < variables.Size(); i++){
        MemberVariable *newVariable = new MemberVariable();
        newVariable->identifier = variables[i]["Identifier"].GetString();
        newVariable->type = variables[i]["Type"].GetString();
        newVariable->size = typeTable[newVariable->type];

        newVariable->references.push((*variable)["Scope"].GetInt());

        rapidjson::Document* value = new rapidjson::Document;
        rapidjson::Document::AllocatorType& allocator = value->GetAllocator();
        value->SetObject();
        value->AddMember("Value", variables[i]["Value"], allocator);

        newVariable->defaultValue = value;


        container->variables[container->size] = newVariable;
        container->size += newVariable->size;
    }

    classes[container->id] = container;

    rapidjson::Document* response = new rapidjson::Document();
    rapidjson::Document::AllocatorType& allocator = response->GetAllocator();
    response->SetObject();
    response->AddMember("Result", 1, allocator);
    return response;
}

rapidjson::Document* MemoryManager::newStruct(rapidjson::Document *variable) {
    std::cout<<"Creating struct----------------------"<<std::endl;

    StructInstance *newStruct = new StructInstance();
    newStruct->identifier = (*variable)["Identifier"].GetString();
    newStruct->type = (*variable)["Type"].GetString();

    StructContainer* structTemplate = classes.find(newStruct->type)->second;

    newStruct->size = structTemplate->size;
    newStruct->references.push((*variable)["Scope"].GetInt());

    int structAddress;

    if(index.empty() and size > newStruct->size){
        index[0] = newStruct;
        structAddress = 0;
    }else{
        for(std::map<int, Variable*>::iterator it = index.begin(); it != index.end(); ++it){
            if(std::next(it, 1)->first - (it->first + it->second->size) > newStruct->size || (std::next(it, 1) == index.end() && this->size - (it->first + it->second->size) > newStruct->size)){
                structAddress = it->first + it->second->size;
                index[structAddress] = newStruct;
                break;
            }
        }
    }

    for(std::map<int, MemberVariable*>::iterator it = structTemplate->variables.begin(); it != structTemplate->variables.end(); ++it){
        saveValue(it->second->defaultValue, structAddress + it->first, it->second->type);
    }
}


rapidjson::Document* MemoryManager::isStruct(rapidjson::Document *variable) {
    rapidjson::Document* response = new rapidjson::Document();
    rapidjson::Document::AllocatorType& allocator = response->GetAllocator();
    response->SetObject();

    bool exists = false;
    if(classes.count((*variable)["Identifier"].GetString()) == 1){
        exists = true;
    }

    response->AddMember("Result", exists, allocator);
    return response;
}

rapidjson::Document* MemoryManager::ramStatus(rapidjson::Document *variable) {
    rapidjson::Document* response = new rapidjson::Document();
    rapidjson::Document::AllocatorType& allocator = response->GetAllocator();
    response->SetArray();

    for(std::map<int, Variable*>::iterator it = index.begin(); it != index.end(); ++it){
        rapidjson::Value entry;
        entry.SetObject();

        rapidjson::Value id(it->second->identifier.c_str(), it->second->identifier.size(), allocator);
        entry.AddMember("Identifier", id, allocator);

        entry.AddMember("Address", it->first, allocator);

        entry.AddMember("References", (int) it->second->references.size(), allocator);

        std::string type = it->second->type;
        int address = it->first;

        if (type == "int") {
            int value = memory[address];
            entry.AddMember("Value", value, allocator);
        } else if (type == "long") {
            int64_t value = memory[address];
            entry.AddMember("Value", value, allocator);
        } else if (type == "char") {
            char value = memory[address];
            entry.AddMember("Value", value, allocator);
        } else if (type == "float") {
            float value = memory[address];
            entry.AddMember("Value", value, allocator);
        } else if (type == "double") {
            double value = memory[address];
            entry.AddMember("Value", value, allocator);
        } else if (type == "reference") {
            int value = memory[address];
            entry.AddMember("Value", value, allocator);
        } else {
            entry.AddMember("Value", ' ', allocator);
        }

//        response->PushBack(entry, allocator);

    }
}











