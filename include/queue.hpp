#ifndef QUEUE_HPP
#define QUEUE_HPP

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

    bool isEmpty(){
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
        if (isEmpty()) {
            cout << "Queue is already empty" << endl;
            return;
        }
        Node* temp = front;
        front = front->next;
    
        if (front == nullptr) {
            rear = nullptr;
        }
    
        delete temp;
    }
};

#endif