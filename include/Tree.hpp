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