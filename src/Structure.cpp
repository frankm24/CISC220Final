//
// Created by Maya on 11/4/2025.
//
#include "../include/Structure.hpp"

#include "Cell.hpp"


Structure::Structure() {
    id = "";
    loc = 0;
    size = 0;
    members = {};
}

Structure::Structure(int sz) {
    id = "";
    loc = 0;
    size = sz;
    members = {};
}

Structure::Structure(int sz, int lc) {
    id = "";
    loc = lc;
    size = sz;
    members = {};
}

Structure::~Structure() {
    for (int i = 0; i < members.size(); i++) {
        delete members[i];
    }
}

string Structure::getId() {
    return id;
}

// Heap class
Heap::Heap() {
    id = "heap";
    size = 0;
    data = {};
    members = {};
}
Heap::Heap(int sz) {
    id="heap";
    size = sz;
    data = {};
    for (int i = 0; i < sz; i++) {
        data.push_back("7");
        members.push_back(new Cell("random data", this, false, loc+i));
    }
}

Heap::Heap(int sz, vector<string> dta) {
    id="heap";
    size = sz;
    for (string dataPt : dta) {
        data.push_back(dataPt);
    }
    for (int i = 0; i < size; i++) {
        members.push_back(new Cell(data[i], this, false, loc+i));
    }
}

Heap::Heap(int sz, int lc, vector<string> dta) {
    id="heap";
    loc = lc;
    size = sz;
    for (string dataPt : dta) {
        data.push_back(dataPt);
    }
    for (int i = 0; i < size; i++) {
        members.push_back(new Cell(data[i], this, false, loc+i));
    }
}
//Can't tell if heap needs its own destructor or it is all handeled by the Structure destructor

//Graph
Graph::Graph() {
    id="graph";
    numNodes = 0;
    data = {};
    members = {};
}

Graph::Graph(int sz) {
    id="graph";
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
            members.push_back(new Cell(to_string(data[i][j]), this, false, (loc+j)+i*numNodes)); // Changed!
        }
    }
}

Graph::Graph(int sz, vector<vector<int>> dta) {
    id="graph";
    //trusting the user to input a perfect square
    long long root = static_cast<long long>(sqrt(static_cast<double>(sz)));
    size = sz;
    numNodes = root;
    for (vector<int> dataLst : dta) {
        data.push_back(dataLst);
    }
    for (int i = 0; i < root; i++) {
        for (int j = 0; j < root; j++) {
            members.push_back(new Cell(to_string(data[i][j]), this, false, (loc+j)+i*numNodes)); // Changed!
        }
    }
}

Graph::Graph(int sz, int lc, vector<vector<int>> dta) {
    id="graph";
    loc = lc;
    //trusting the user to input a perfect square
    long long root = static_cast<long long>(sqrt(static_cast<double>(sz)));
    size = sz;
    numNodes = root;
    for (vector<int> dataLst : dta) {
        data.push_back(dataLst);
    }
    for (int i = 0; i < root; i++) {
        for (int j = 0; j < root; j++) {
            members.push_back(new Cell(to_string(data[i][j]), this, false, (loc+j)+i*numNodes)); // Changed!
        }
    }
}

// Same as above about destructors

Matrix::Matrix() {
    id="matrix";
    size = 0;
    data = {};
    n = 0;
    m = 0;
    pointers = {};
    members = {};
}

Matrix::Matrix(int sz) {
    id="matrix";
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
        members.push_back(new Cell(pointers[i], this, false, stoi(arrLoc.substr(2),nullptr,16)+i));
        for (int j = 0; j < m; j++) {
            data[i].push_back("random data");
            members.push_back(new Cell(data[i][j], this, false, loc+i));
        }
    }
}

Matrix::Matrix(int sz, int n, int m, vector<vector<string>> dta, vector<string> ptrs) {
    id="matrix";
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
            members.push_back(new Cell(pointers[i], this, false, loc+i));
            for (int j = 0; j < m; j++) {
                members.push_back(new Cell(data[i][j], this, false, stoi(pointers[i].substr(2),nullptr,16)+j));
                // somehow make sure this exists in the proper location
        }
    }
}

Matrix::Matrix(int sz,int lc, int n, int m, vector<vector<string>> dta, vector<string> ptrs) {
    id="matrix";
    loc = lc;
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
    }
    for (string pointer : ptrs) {
        pointers.push_back(pointer);
    }
    for (int i = 0; i < n; i++) {
            members.push_back(new Cell(pointers[i], this, false, loc+i));
            for (int j = 0; j < m; j++) {
                members.push_back(new Cell(data[i][j], this, false, stoi(pointers[i].substr(2),nullptr,16)+j));
                // somehow make sure this exists in the proper location
            }
        }
}

// Same thing with constructor


BST::BST() {
    id="bst";
    size = 0;
    numNodes = 0;
    members = {};
    nodes = {};
    root = "";
}

BST::BST(int sz) {
    id="bst";
    if (sz%3!=0) {
        cout<<"error not a possible size"<<endl;
        return;
    }
    size = sz;
    numNodes = size/3;
    nodes = {};
    nodes.push_back(new BSTNode("root data", loc, "0x!!", "0x!!"));
    for (int x = 0; x<3; x++) {
        members.push_back(nodes[0]->members[x]);
    }
    for (int i = 1; i < numNodes; i++) {
        //BLARING ISSUE WITH THIS LINE IT ONLY GETS LOCATION FOR LEFT - I will come back and fix it
        nodes.push_back( new BSTNode("random data",stoi(nodes[i-1]->left,nullptr,16),"0x!!", "0x!!"));
        for (int x = 0; x<3; x++) {
            members.push_back(nodes[i]->members[x]);
        }
    }
}

BST::BST(int sz, vector<BSTNode*> nds, string rt) {
    id="bst";
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

BST::BST(int sz, int lc, vector<BSTNode*> nds, string rt) {
    id="bst";
    loc = lc;
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
    nodes[0]->loc = loc;
}

BSTNode::BSTNode() {
    id="bst";
    loc = 0;
    members = {};
    size = 3;
    data = "random data";
    left = "null";
    right = "null";
    members.push_back(new Cell(data, this, false, loc));
    members.push_back(new Cell(left, this, false, loc+1));
    members.push_back(new Cell(right, this, false, loc+2));
}
BSTNode::BSTNode(string dta) {
    id="bst";
    loc = 0;
    members = {};
    size = 3;
    data = dta;
    left = "null";
    right = "null";
    members.push_back(new Cell(data, this, false, loc));
    members.push_back(new Cell(left, this, false, loc+1));
    members.push_back(new Cell(right, this, false, loc+2));
}

BSTNode::BSTNode(string dta, int lc) {
    id="bst";
    loc = lc;
    members = {};
    size = 3;
    data = dta;
    left = "null";
    right = "null";
    members.push_back(new Cell(data, this, false, loc));
    members.push_back(new Cell(left, this, false, loc+1));
    members.push_back(new Cell(right, this, false, loc+2));
}

BSTNode::BSTNode(string dta, string lft, string rgt) {
    id="bst";
    loc = 0;
    members = {};
    size = 3;
    data = dta;
    left = lft;
    right = rgt;
    members.push_back(new Cell(data, this, false, loc));
    members.push_back(new Cell(left, this, false, loc+1));
    members.push_back(new Cell(right, this, false, loc+2));
}

BSTNode::BSTNode(string dta, int lc, string lft, string rgt) {
    id="bst";
    loc = lc;
    members = {};
    size = 3;
    data = dta;
    left = lft;
    right = rgt;
    members.push_back(new Cell(data, this, false, loc));
    members.push_back(new Cell(left, this, false, loc+1));
    members.push_back(new Cell(right, this, false, loc+2));
}

// Same as above about destructors

DLL::DLL() {
    id="dll";
    loc = 0;
    size = 0;
    numNodes = 0;
    members = {};
    nodes = {};
    first = "null";
    last = "null";
}

DLL::DLL(int sz) {
    id="dll";
    if (sz%3!=0) {
        cout<<"error not a possible size"<<endl;
        return;
    }
    size = sz;
    numNodes = size/3;
    nodes = {};
    string nextHex = convertToHex(rand()%256);// needs to be tested to be sure it always finds a location where there is space
    nodes.push_back(new DLLNode("root data", loc, "null", nextHex));
    for (int x = 0; x<3; x++) {
        members.push_back(nodes[0]->members[x]);
    }
    for (int i = 1; i < numNodes-1; i++) {
        //BLARING ISSUE WITH THIS LINE IT ONLY GETS LOCATION FOR LEFT - I will come back and fix it
        string prevHex = convertToHex(nodes[i-1]->loc);
        string nextHex = convertToHex(rand()%256);// needs to be tested to be sure it always finds a location where there is space

        nodes.push_back( new DLLNode("random data",stoi(nodes[i-1]->next.substr(2),nullptr,16),prevHex, nextHex));
        for (int x = 0; x<3; x++) {
            members.push_back(nodes[i]->members[x]);
        }
    }
    string prevHex = convertToHex(nodes[numNodes-2]->loc);
    nodes.push_back( new DLLNode("random data",stoi(nodes[numNodes-2]->next.substr(2),nullptr,16),prevHex, "null"));
    for (int x = 0; x<3; x++) {
        members.push_back(nodes[numNodes-1]->members[x]);
    }
}

DLL::DLL(int sz, vector<DLLNode*> nds, string frst, string lst) {
    id="dll";
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

DLL::DLL(int sz,int lc, vector<DLLNode*> nds, string frst, string lst) {
    id="dll";
    loc = lc;
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
    id="dll";
    loc = 0;
    members = {};
    size = 3;
    data = "random data";
    last = "null";
    next = "null";
    members.push_back(new Cell(last, this, false, loc));
    members.push_back(new Cell(data, this, false,loc+1));
    members.push_back(new Cell(next, this, false,loc+2));
}

DLLNode::DLLNode(string dta, string lst, string nxt) {
    id="dll";
    loc = 0;
    members = {};
    size = 3;
    data = dta;
    last = lst;
    next = nxt;
    members.push_back(new Cell(last, this, false, loc));
    members.push_back(new Cell(data, this, false,loc+1));
    members.push_back(new Cell(next, this, false,loc+2));
}

DLLNode::DLLNode(string dta,int lc, string lst, string nxt) {
    id="dll";
    loc = lc;
    members = {};
    size = 3;
    data = dta;
    last = lst;
    next = nxt;
    members.push_back(new Cell(last, this, false, loc));
    members.push_back(new Cell(data, this, false,loc+1));
    members.push_back(new Cell(next, this, false,loc+2));
}

// Same as above about destructors