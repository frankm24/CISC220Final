//
// Created by Maya on 11/4/2025.
//
#include "../include/Structure.hpp"


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
    switch (0) {
        case 0: //"BST"
            break;
        case 1: //"DLL"
            break;
        case 2: //"HEAP"
            break;
        case 3: //"MATRIX"
            break;
        case 4: //"GRAPH"
            break;
        default:
            break;
    }
    return new_struct;
}

