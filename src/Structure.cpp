//
// Created by Maya on 11/4/2025.
//
#include "Structure.h"


Structure::Structure() {
    length = 0;
    type = "";
    members = nullptr;
}

Structure::Structure(string tp) {
    length = 0;
    type = tp;
    members = nullptr;
}

Structure::~Structure() {
    for (int i = 0; i < length; i++) {
        delete members[i];
    }
}

Structure* Structure::generateStructure(string type) {
    Structure* new_struct = new Structure(type);
    switch (type) {
        case "BST":
            // Code to execute if expression equals value1
            break; // Exits the switch statement
        case "DLL":
            break;
        case "HEAP":
            break;
        case "MATRIX":
            break;
        case "GRAPH":
            break;
        default:
            break;
    }
    return new_struct;
}

