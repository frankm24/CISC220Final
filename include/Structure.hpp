#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Cell.hpp"
#include "Board.hpp"
#include <boost/algorithm/string.hpp>
class DLLNode;
class BSTNode;
using namespace std;


class Structure {
    friend class Board;
    friend class Heap;
    friend class Graph;
    friend class BST;
    friend class DLL;
    friend class Matrix;
    friend class Node;
    friend class BSTNode;
    friend class DLLNode;
    friend class Cell;
    int loc;
    int size;
    string id;
    vector<Cell*> members;
public:
    Structure();
    Structure(int sz);
    Structure(int sz, int lc);
    virtual ~Structure();
    string getId();
};

class Heap : Structure {
    friend class Board;
    vector<string> data;
public:
    Heap();
    Heap(int sz);
    Heap(int sz, vector<string> dta);
    Heap(int sz, int lc, vector<string> dta);
    //~Heap();
};

class Graph : Structure {
    friend class Board;
    vector<vector<int>> data;
    int numNodes;
public:
    Graph();
    Graph(int sz); //must be an even square
    Graph(int sz, vector<vector<int>> dta);
    Graph(int sz, int lc, vector<vector<int>> dta);
    //~Graph();
};

class Matrix : Structure {
    friend class Board;
    int n;
    int m;
    vector<vector<string>> data;
    vector<string> pointers; // 0xformat
public:
    Matrix();
    Matrix(int sz);
    Matrix(int sz, int n, int m, vector<vector<string>> dta, vector<string> ptrs);
    Matrix(int sz, int lc, int n, int m, vector<vector<string>> dta, vector<string> ptrs);
    //~Matrix();
};

class BST : Structure {
    friend class Board;
    vector<BSTNode*> nodes;
    int numNodes;
    string root; // 0xformat
public:
    BST();
    BST(int sz);
    BST(int sz, vector<BSTNode*> nds, string rt);
    BST(int sz, int lc, vector<BSTNode*> nds, string rt);
};

class DLL : Structure {
    friend class Board;
    vector<DLLNode*> nodes;
    int numNodes;
    string first; // 0xformat
    string last; // 0xformat
public:
    DLL();
    DLL(int sz);
    DLL(int sz, vector<DLLNode*> nds, string frst, string lst);
    DLL(int sz, int lc, vector<DLLNode*> nds, string frst, string lst);
};

class Node : Structure {
    friend class Board;
    friend class BSTNode;
    friend class DLLNode;
    friend class BST;
    friend class DLL;
    // already has members
    string data;
public:
    //Node();
    //~Node();
};

class BSTNode : Node {
    friend class BST;
    friend class Board;
    string left; //0xformat
    string right; //0xformat
public:
    BSTNode();
    BSTNode(string dta);
    BSTNode(string dta, int lc);
    BSTNode(string dta, string lft, string rgt);
    BSTNode(string dta, int lc, string lft, string rgt);
    //~BSTNode();
};

class DLLNode : Node {
    friend class Board;
    friend class DLL;
    string last; //0xformat
    string next; //0xformat
public:
    DLLNode();
    DLLNode(string dta, string lst, string nxt);
    DLLNode(string dta, int lc, string lst, string nxt);
    //~DLLNode();
};
