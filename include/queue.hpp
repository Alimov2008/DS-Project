#include <iostream>
#include "Node.hpp"
using namespace std;


class Queue
{
private:
    Node* front;
    Node* rear;
public:
    Queue(){
        front=rear=nullptr;
    };

    bool isEmpthy(){
        return (front==nullptr);
    }

    void enqueue(int value){
        Node* temp=new Node(value);

        if (rear==nullptr)
        {
            front=rear=temp;
            return;
        }
        rear->next=temp;
        rear=temp;
    }
    void dequeue(){
        if (isEmpthy())
        {
            cout<<"Queue is already empthy"<<endl;
            return;
        }
        
        
    }
};
