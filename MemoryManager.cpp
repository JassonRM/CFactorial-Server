//
// Created by Jasson Rodríguez Méndez on 5/4/18.
//


#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "MemoryManager.h"
#include "StructInstance.h"
#include "Reference.h"

MemoryManager::MemoryManager(int size) {
    typeTable = std::map<std::string, int>();
    typeTable["int"] = 4;
    typeTable["long"] = 8;
    typeTable["char"] = 1;
    typeTable["float"] = 4;
    typeTable["double"] = 8;
    typeTable["reference"] = 4;
    this->size = size;
    memory = new unsigned char[size];
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
    }else if(requestType == "RAM Status"){
        response = ramStatus(jsonRequest);
    }else if(requestType == "Reset"){
        response = reset();
    }else if(requestType == "Close Scope"){
        response = closeScope(jsonRequest);
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
        char value = (*variable)["Value"].GetString()[1];
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
    rapidjson::Document *response = new rapidjson::Document();
    rapidjson::Document::AllocatorType &allocator = response->GetAllocator();
    response->SetObject();

    if(variable->HasMember("Struct")){
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
        response->AddMember("Result", 0, allocator);
        return response;
    }else {
        for (std::map<int, Variable*>::iterator it = index.begin(); it != index.end(); ++it) {
            if (it->second->identifier == (*variable)["Identifier"].GetString()) {

                std::string type = it->second->type;
                int address = it->first;


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
        response->AddMember("Result", 0, allocator);
        return response;
    }
}

rapidjson::Document* MemoryManager::getAddress(rapidjson::Document *variable) {
    rapidjson::Document* response = new rapidjson::Document();
    rapidjson::Document::AllocatorType& allocator = response->GetAllocator();
    response->SetObject();
    for(std::map<int, Variable*>::iterator it = index.begin(); it != index.end(); ++it){
        if(it->second->identifier == (*variable)["Identifier"].GetString()){
            response->AddMember("Address", it->first, allocator);
            rapidjson::Value value(it->second->type.c_str(), it->second->type.size(), allocator);
            response->AddMember("Type", value, allocator);
            return response;
        }
    }
    response->AddMember("Result", 0, allocator);
    return response;
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


    for(std::map<int, MemberVariable*>::iterator it = structTemplate->variables.begin(); it != structTemplate->variables.end(); ++it){ //Assertion failed
        newStruct->variables[structAddress + it->first] = *it->second;
        saveValue(it->second->defaultValue, structAddress + it->first, it->second->type);
    }
    rapidjson::Document* response = new rapidjson::Document();
    rapidjson::Document::AllocatorType& allocator = response->GetAllocator();
    response->SetObject();
    response->AddMember("Result", 1, allocator);
    return response;
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

        rapidjson::Value jsonType(it->second->type.c_str(), it->second->type.size(), allocator);
        entry.AddMember("Type", jsonType, allocator);

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
            std::cout<<"Hay un struct"<<std::endl;
            entry.AddMember("Value", "null", allocator);
            StructInstance* structInstance = (StructInstance*) it->second;
            std::cout<<"Vacio? "<<structInstance->variables.empty()<<std::endl;
            response->PushBack(entry, allocator);
            int prevAddr = 0;
            for(std::map<int, Variable>::iterator iter = structInstance->variables.begin(); iter != structInstance->variables.end(); ++iter){
                std::cout<<"Imprimiendo los miembros"<<std::endl;
                rapidjson::Value memberEntry;
                memberEntry.SetObject();

                std::string identifier = it->second->identifier + "." + iter->second.identifier;
                rapidjson::Value id(identifier.c_str(), identifier.size(), allocator);
                memberEntry.AddMember("Identifier", id, allocator);

                memberEntry.AddMember("Address", address + prevAddr, allocator);

                prevAddr += it->first;

                memberEntry.AddMember("References", (int) iter->second.references.size(), allocator);

                std::string mtype = iter->second.type;


                rapidjson::Value jsonType(iter->second.type.c_str(), iter->second.type.size(), allocator);
                memberEntry.AddMember("Type", jsonType, allocator);

                if (mtype == "int") {
                    int value = memory[address + iter->first];
                    memberEntry.AddMember("Value", value, allocator);
                    std::cout<<"Se anadio valor "<<value<<std::endl;
                } else if (mtype == "long") {
                    int64_t value = memory[address + iter->first];
                    memberEntry.AddMember("Value", value, allocator);
                } else if (mtype == "char") {
                    char value = memory[address + iter->first];
                    memberEntry.AddMember("Value", value, allocator);
                } else if (mtype == "float") {
                    float value = memory[address + iter->first];
                    memberEntry.AddMember("Value", value, allocator);
                } else if (mtype == "double") {
                    double value = memory[address + iter->first];
                    memberEntry.AddMember("Value", value, allocator);
                } else if (mtype == "reference") {
                    int value = memory[address + iter->first];
                    memberEntry.AddMember("Value", value, allocator);
                }
                std::cout<<identifier<<std::endl;
                response->PushBack(memberEntry, allocator);
            }
            continue;
        }

        response->PushBack(entry, allocator);

    }

    return response;
}

rapidjson::Document* MemoryManager::reset() {
    index.clear();

    for(int b = 0; b < sizeof(memory); b++) {
        memory[b] = '\0';
    }

    classes.clear();

    rapidjson::Document* response = new rapidjson::Document();
    rapidjson::Document::AllocatorType& allocator = response->GetAllocator();
    response->SetObject();
    response->AddMember("Result", 1, allocator);
    return response;
}

rapidjson::Document* MemoryManager::newReference(rapidjson::Document *variable) {
    Reference *newVariable = new Reference();
    newVariable->identifier = (*variable)["Identifier"].GetString();
    newVariable->type = "Reference";
    newVariable->subtype = (*variable)["Type"].GetString();
    newVariable->size = 4;
    newVariable->references.push((*variable)["Scope"].GetInt());

    rapidjson::Document* address = new rapidjson::Document();
    rapidjson::Document::AllocatorType& addressAlloc = address->GetAllocator();
    address->SetObject();

    for(std::map<int, Variable*>::iterator it = index.begin(); it != index.end(); ++it){
        if(it->second->identifier == (*variable)["Identifier"].GetString()){
            address->AddMember("Value", it->first, addressAlloc);
            it->second->references.push((*variable)["Scope"].GetInt());
        }
    }



    if(index.empty() and size > newVariable->size){
        saveValue(address, 0, "int");
        index[0] = newVariable;
    }else{
        for(std::map<int, Variable*>::iterator it = index.begin(); it != index.end(); ++it){
            if(std::next(it, 1)->first - (it->first + it->second->size) > newVariable->size || (std::next(it, 1) == index.end() && this->size - (it->first + it->second->size) > newVariable->size)){
                saveValue(address, it->first + it->second->size, "int");
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

rapidjson::Document* MemoryManager::closeScope(rapidjson::Document *variable) {
    int scope = (*variable)["Scope"].GetInt();

    for(std::map<int, Variable*>::iterator it = index.begin(); it != index.end(); ++it){
        if(it->second->references.top() == scope){
            it->second->references.pop();
            if(it->second->references.empty()){
                index.erase(it->first);
            }
        }
    }
    rapidjson::Document* response = new rapidjson::Document();
    rapidjson::Document::AllocatorType& allocator = response->GetAllocator();
    response->SetObject();
    response->AddMember("Result", 1, allocator);
    return response;
}






