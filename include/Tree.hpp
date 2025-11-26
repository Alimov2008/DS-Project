#ifndef TREE_HPP
#define TREE_HPP

#include "Tree_node.hpp"
using namespace std;

template <typename T>
class Tree 
{
private:
    Node<T>* root;
public:
    Tree(){
        root=nullptr;
     }

    
    int getHeight(Node<T>* n){
        if (n==nullptr) 
        {
            return 0;
        }
        return n->height;
    }

    int getBalance(Node<T>* n){
        if (n==nullptr) 
        {
            return 0;
        }
        return getHeight(n->left)-getHeight(n->right);
    }

    Node<T>* rightR(Node<T>* y){
        Node<T>* x = y->left;
        Node<T>* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    Node<T>* leftR(Node<T>* x) {
        Node<T>* y = x->right;
        Node<T>* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    Node<T>* insert(Node<T>* node,const T& value){
        if (node==nullptr)
        {
            return new Node<T>(value);
        }
        
        if (value<node->data)
        {
            node->left=insert(node->left,value);
        }
        else if (node->data<value){
            node->right=insert(node->right,value);
        }
        else{
            return node;
        }

        node->height=max(getHeight(node->left)+1,getHeight(node->right));
        int balance=getBalance(node);
        
        //LL
        if (balance > 1 && value < node->left->data)
            return rightR(node);

        //RR
        if (balance < -1 && node->right->data < value)
            return leftR(node);

        //LR
        if (balance > 1 && node->left->data < value) {
            node->left = leftR(node->left);
            return rightR(node);
        }

        //RL
        if (balance < -1 && value < node->right->data) {
            node->right = rightR(node->right);
            return leftR(node);
        }        

        return node;
    }

    void insert(const T& value){
            root=insert(root,value);
        }

    void inorder(Node<T>* node) {
        if (node == nullptr){
            return;
        }
        inorder(node->left);
        cout << node->data <<" ";
        inorder(node->right);
    }
    
    void inorder() {
        inorder(root);
        cout<<endl;
    }

    bool exists(int value){
        return exists(root,value);
    }

    bool exists(Node<T>* node,int value){
        if (node == nullptr) {
            return false;
        }
        if (node->data == value) {
            return true;
        }
        if (value < node->data) {
            return searchRecursive(node->left, value);
        } else {
            return searchRecursive(node->right, value);
        }
    }

    Node<T>* search(Node<T>* node,const T& value){
        if (node == nullptr || node->data == value)
        return node;

        if (value < node->data)
            return search(node->left, value);
        else
            return search(node->right, value);
    }

    Node<T>* search(const T& value){
        return search(root, value);
    }

};

#endif