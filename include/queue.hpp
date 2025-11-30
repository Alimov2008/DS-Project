#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <string>
#include <vector>
#include <iostream>

class Queue {
private:
    struct Node {
        std::string studentID;
        Node* next;
        Node(const std::string& id) : studentID(id), next(nullptr) {}
    };
    
    Node* front;
    Node* rear;
    int size;

public:
    Queue() : front(nullptr), rear(nullptr), size(0) {}
    
    ~Queue() {
        clear();
    }
    
    void enqueue(const std::string& studentID) {
        Node* newNode = new Node(studentID);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }
    
    std::string dequeue() {
        if (isEmpty()) {
            return "";
        }
        
        Node* temp = front;
        std::string studentID = temp->studentID;
        front = front->next;
        
        if (front == nullptr) {
            rear = nullptr;
        }
        
        delete temp;
        size--;
        return studentID;
    }
    
    std::string peek() const {
        if (isEmpty()) {
            return "";
        } else {
            return front->studentID;
        }
    }
    
    bool isEmpty() const {
        return front == nullptr;
    }
    
    int getSize() const {
        return size;
    }
    
    bool contains(const std::string& studentID) const {
        Node* current = front;
        while (current != nullptr) {
            if (current->studentID == studentID) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    std::vector<std::string> getAllStudents() const {
        std::vector<std::string> students;
        Node* current = front;
        while (current != nullptr) {
            students.push_back(current->studentID);
            current = current->next;
        }
        return students;
    }
    
    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

#endif