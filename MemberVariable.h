//
// Created by Jasson Rodríguez Méndez on 20/4/18.
//

#ifndef CFACTORIAL_SERVER_MEMBERVARIABLE_H
#define CFACTORIAL_SERVER_MEMBERVARIABLE_H

#include <rapidjson/document.h>
#include "Variable.h"

struct MemberVariable : Variable{
    rapidjson::Document* defaultValue;
};

#endif //CFACTORIAL_SERVER_MEMBERVARIABLE_H
