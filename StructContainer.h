//
// Created by Jasson Rodríguez Méndez on 20/4/18.
//

#ifndef CFACTORIAL_SERVER_STRUCTCONTAINER_H
#define CFACTORIAL_SERVER_STRUCTCONTAINER_H

#include <string>
#include <map>

#include "Variable.h"
#include "MemberVariable.h"

class StructContainer {
public:
    std::string id;
    int size = 0;
    std::map<int, MemberVariable*> variables;
};


#endif //CFACTORIAL_SERVER_STRUCTCONTAINER_H
