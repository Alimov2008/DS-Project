#include <iostream>
#include "Node.hpp"
using namespace std;

class LinkedList
{
private:
    Node* head;
public:
    LinkedList(){
        head=nullptr;
    }

    void insert_beginning(int value){
        Node* newNode=new Node(value);
        newNode->next=head;
        head=newNode;
    }

    void insert_end(int value){
        Node* newNode=new Node(value);
        if (head==nullptr){
            head=newNode;
            return;
        }
        Node* temp =head;
        while (temp->next!=nullptr)
        {
            temp=temp->next;
        }
        temp->next=newNode;
    }

    void inser_middle(int value,int position){
        if(position==0){
            insert_beginning(value);
            return;
        }
        Node* newNode=new Node(value);
        Node* temp=head;
        for (int i = 0; temp != nullptr && i<position-1; i++)
        {
            temp=temp->next;
        }
        if (temp==nullptr){
            cout<<"Position to insert is bigger than length of linked list"<<endl;
            return;
        }
        newNode->next=temp->next;
        temp->next=newNode;   
    }
 

    void delete_beginning(){
        if (head==nullptr)
        {
            cout<<"Linked list is already empthy"<<endl;
            return;
        }
        Node* temp=head;
        head=head->next;
        delete temp;
        cout<<"Front Node is deleted"<<endl;
    }

    void delete_end(){
        if (head==nullptr)
        {
            cout<<"Linked list is already empthy"<<endl;
            return;
        }

        if(head->next==nullptr){
            delete head;
            head=nullptr;
            cout<<"End Node is deleted"<<endl;
        }

        Node* temp=head;
        while (temp->next->next!=nullptr) 
        {
            temp=temp->next;
        }
        delete temp->next;
        temp->next=nullptr;
        cout<<"End Node is deleted"<<endl;
    }

    void delete_middle(int value){
        //delete by value
        if (head==nullptr)
        {
            cout<<"Linked list is already empthy"<<endl;
            return;
        }

        if (head->data==value)
        {
            Node* temp=head;
            head=head->next;
            delete temp;
            cout<<"Selected Node is deleted"<<endl;
            return;
        }
        Node* temp=head;
        while (temp->next->data!=value && temp->next!=nullptr)
        {
            temp=temp->next;
        }

        if (temp->next==nullptr)
        {
            cout<<"No matching value found in linked list"<<endl;
            return;
        }

        Node* toDelete=temp->next;
        temp->next=temp->next->next;
        delete toDelete;
        cout<<"Selected Node is deleted"<<endl;
    }

    bool is_present(int value){
        Node* temp=head;
        while (temp->next!=nullptr)
        {
            if(temp->data==value){
                return true;
            }
            temp=temp->next;
        }
        return false;
    }

    void display(){
        Node* temp=head;
        while (temp->next!=nullptr)
        {
            cout<<temp->data<<" -> " <<endl;
            temp=temp->next;
        }
        cout<<"null"<<endl;
    }
    
    int length(){
        int length=0;
        Node* temp=head;
        while (temp->next!=nullptr)
        {
            length++;
            temp=temp->next;
        }
        return length;
    }

    void reverse(){
        Node* previous=nullptr;
        Node* current=head;
        Node* next = nullptr;
        while (current!=nullptr)
        {
            next=current->next;
            current->next=previous;
            previous=current;
            current=next;
        }
        head=previous;
    }
};