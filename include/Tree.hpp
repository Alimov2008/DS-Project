#include <iostream>

using namespace std;
class Node
{
public:
    int data;
    Node* left;
    Node* right;
    
    int height;

    Node(int value){
        data=value;
        left=right=nullptr;
        height=1;
    }
};

class Tree
{
private:
    Node* root;
public:
    Tree(){
        root=nullptr;
    }
    
    int getHeight(Node* n){
        if (n==nullptr) 
        {
            return 0;
        }
        return n->height;
    }

    int getBalance(Node* n){
        if (n==nullptr) 
        {
            return 0;
        }
        return getHeight(n->left)-getHeight(n->right);
    }

    Node* rightR(Node* y){
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    Node* leftR(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    
};
