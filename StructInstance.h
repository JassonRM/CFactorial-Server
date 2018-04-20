//
// Created by Jasson Rodríguez Méndez on 20/4/18.
//

#ifndef CFACTORIAL_SERVER_STRUCTINSTANCE_H
#define CFACTORIAL_SERVER_STRUCTINSTANCE_H

#include <map>

#include "Variable.h"

struct StructInstance : Variable{
    std::map<int, Variable> variables;
};
#endif //CFACTORIAL_SERVER_STRUCTINSTANCE_H
