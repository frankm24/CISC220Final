//
// Created by Maya on 11/4/2025.
//
#include "../include/Structure.hpp"


Structure::Structure() {
    size = 0;
    members = {};
}

Structure::~Structure() {
    for (int i = 0; i < members.size(); i++) {
        delete members[i];
    }
}

Structure* Structure::generateStructure() {
    Structure* new_struct = new Structure();
    return new_struct;
}


// Heap class
Heap::Heap() {
    size = 0;
    data = {};
    members = {};
}
Heap::Heap(int sz) {
    size = sz;
    data = {};
    for (int i = 0; i < sz; i++) {
        data.push_back("7");
        members.push_back(new Cell("random data", this, false));
    }
}

Heap::Heap(int sz, vector<string> dta) {
    size = sz;
    for (string dataPt : dta) {
        data.push_back(dataPt);
    }
    for (int i = 0; i < size; i++) {
        members.push_back(new Cell(data[i], this, false));
    }
}
//Can't tell if heap needs its own destructor or it is all handeled by the Structure destructor

//Graph
Graph::Graph() {
    numNodes = 0;
    data = {};
    members = {};
}

Graph::Graph(int sz) {
    long long root = static_cast<long long>(sqrt(static_cast<double>(sz)));
    if (root * root != sz) {
        cout<<"error not a perfect square"<<endl;
        return;
    }
    size = sz;
    numNodes = root;
    data = {};
    for (int i = 0; i < root; i++) {
        for (int j = 0; j < root; j++) {
            if (i == j) data[i][j]=0;
            else data[i][j]=1;
            members.push_back(new Cell(to_string(data[i][j]), this, false)); // Changed!
        }
    }
}

Graph::Graph(int sz, vector<vector<int>> dta) {
    //trusting the user to input a perfect square
    long long root = static_cast<long long>(sqrt(static_cast<double>(sz)));
    size = sz;
    numNodes = root;
    for (vector<int> dataLst : dta) {
        data.push_back(dataLst);
    }
    for (int i = 0; i < root; i++) {
        for (int j = 0; j < root; j++) {
            members.push_back(new Cell(to_string(data[i][j]), this, false)); // Changed!
        }
    }
}

// Same as above about destructors

Matrix::Matrix() {
    size = 0;
    data = {};
    n = 0;
    m = 0;
    pointers = {};
    members = {};
}

Matrix::Matrix(int sz) {
    m = 0;
    for (int mTest = 3; mTest<=sz/3; mTest++) {
        for (int nTest = 3; nTest<=sz/3; nTest++) {
            if ((sz-nTest)%mTest == 0) {
                n = (sz-nTest)/mTest; m = mTest;
                if (n!=nTest) cout<<"n mismatch something wrong"<<endl;
            }
        }
    }
    if (m==0) {
        cout<<"error not a possible size"<<endl;
        return;
    }
    size = sz;
    data = {};
    pointers = {};
    for (int i = 0; i < n; i++) {
        data.push_back({});
        string arrLoc = "0x##"; // missing logic to find available space
        pointers.push_back(arrLoc);
        members.push_back(new Cell(pointers[i], this, false));
        for (int j = 0; j < m; j++) {
            data[i].push_back("random data");
            members.push_back(new Cell(data[i][j], this, false));
        }
    }
}

Matrix::Matrix(int sz, int n, int m, vector<vector<string>> dta, vector<string> ptrs) {
    if ((m*n)+n!=sz) {
        cout<<"sizes don't match, expected "<<(m*n)+n<<endl;
        return;
    }
    this->m = m;
    this->n = n;
    size = sz;
    data = {};
    for (vector<string> dataLst : dta) {
        data.push_back(dataLst);
    }    for (string pointer : ptrs) {
        pointers.push_back(pointer);
    }    for (int i = 0; i < n; i++) {
        members.push_back(new Cell(pointers[i], this, false));
        for (int j = 0; j < m; j++) {
            members.push_back(new Cell(data[i][j], this, false));
            // somehow make sure this exists in the proper location
        }
    }
}

// Same thing with constructor


BST::BST() {
    size = 0;
    numNodes = 0;
    members = {};
    nodes = {};
    root = "";
}

BST::BST(int sz) {
    if (sz%3!=0) {
        cout<<"error not a possible size"<<endl;
        return;
    }
    size = sz;
    numNodes = size/3;
    nodes = {};
    for (int i = 0; i < numNodes; i++) {
        nodes.push_back( new BSTNode("random data","0x!!", "0x!!"));
        for (int x = 0; x<3; x++) {
            members.push_back(nodes[i]->members[x]);
        }
    }
}

BST::BST(int sz, vector<BSTNode*> nds, string rt) {
    if (sz%3!=0) {
        cout<<"error not a possible size"<<endl;
        return;
    }
    root = rt;
    size = sz;
    numNodes = size/3;
    members = {};
    for (BSTNode* node : nds) {
        nodes.push_back(node);
    }    for (int i = 0; i < numNodes; i++) {
        for (int x = 0; x<3; x++) {
            members.push_back(nodes[i]->members[x]);
        }
    }
}

BSTNode::BSTNode() {
    members = {};
    size = 3;
    data = "random data";
    left = "null";
    right = "null";
    members.push_back(new Cell(data, this, false));
    members.push_back(new Cell(left, this, false));
    members.push_back(new Cell(right, this, false));
}


BSTNode::BSTNode(string dta) {
    members = {};
    size = 3;
    data = dta;
    left = "null";
    right = "null";
    members.push_back(new Cell(data, this, false));
    members.push_back(new Cell(left, this, false));
    members.push_back(new Cell(right, this, false));
}

BSTNode::BSTNode(string dta, string lft, string rgt) {
    members = {};
    size = 3;
    data = dta;
    left = lft;
    right = rgt;
    members.push_back(new Cell(data, this, false));
    members.push_back(new Cell(left, this, false));
    members.push_back(new Cell(right, this, false));
}

// Same as above about destructors

DLL::DLL() {
    size = 0;
    numNodes = 0;
    members = {};
    nodes = {};
    first = "null";
    last = "null";
}

DLL::DLL(int sz) {
    if (sz%3!=0) {
        cout<<"error not a possible size"<<endl;
        return;
    }
    size = sz;
    numNodes = size/3;
    nodes = {};
    for (int i = 0; i < numNodes; i++) {
        nodes.push_back( new DLLNode("random data","0x!!", "0x!!"));
        for (int x = 0; x<3; x++) {
            members.push_back(nodes[i]->members[x]);
        }
    }
}

DLL::DLL(int sz, vector<DLLNode*> nds, string frst, string lst) {
    if (sz%3!=0) {
        cout<<"error not a possible size"<<endl;
        return;
    }
    first = frst;
    last = lst;
    size = sz;
    numNodes = size/3;
    members = {};
    for (DLLNode* node : nds) {
        nodes.push_back(node);
    }
    for (int i = 0; i < numNodes; i++) {
        for (int x = 0; x<3; x++) {
            members.push_back(nodes[i]->members[x]);
        }
    }
}

DLLNode::DLLNode() {
    members = {};
    size = 3;
    data = "random data";
    last = "null";
    next = "null";
    members.push_back(new Cell(last, this, false));
    members.push_back(new Cell(data, this, false));
    members.push_back(new Cell(next, this, false));
}

DLLNode::DLLNode(string dta, string lst, string nxt) {
    members = {};
    size = 3;
    data = dta;
    last = lst;
    next = nxt;
    members.push_back(new Cell(last, this, false));
    members.push_back(new Cell(data, this, false));
    members.push_back(new Cell(next, this, false));
}

// Same as above about destructors

