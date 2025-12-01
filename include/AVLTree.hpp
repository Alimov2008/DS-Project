#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include "Book.hpp"
#include "Queue.hpp"
#include <string>
#include <vector>
#include <iostream>

struct AVLNode {
    Book book;
    Queue waitingQueue;
    AVLNode* left;
    AVLNode* right;
    int height;
    
    AVLNode(const Book& b) : book(b), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    AVLNode* root;
    
    int getHeight(AVLNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }
    
    int getBalance(AVLNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return getHeight(node->left) - getHeight(node->right);
    }
    
    int max(int a, int b) {
        if (a > b) {
            return a;
        } else {
            return b;
        }
    }
    
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        
        return x;
    }
    
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        
        y->left = x;
        x->right = T2;
        
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        
        return y;
    }
    
    AVLNode* insert(AVLNode* node, const Book& book) {
        if (node == nullptr) {
            return new AVLNode(book);
        }
        
        if (book.bookID < node->book.bookID) {
            node->left = insert(node->left, book);
        } else if (book.bookID > node->book.bookID) {
            node->right = insert(node->right, book);
        } else {
            return node; 
        }
        
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        
        int balance = getBalance(node);
        
        //LL
        if (balance > 1 && book.bookID < node->left->book.bookID) {
            return rightRotate(node);
        }
        
        //RR
        if (balance < -1 && book.bookID > node->right->book.bookID) {
            return leftRotate(node);
        }
        
        //LR
        if (balance > 1 && book.bookID > node->left->book.bookID) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        
        //RL
        if (balance < -1 && book.bookID < node->right->book.bookID) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        
        return node;
    }
    
    AVLNode* minValueNode(AVLNode* node) {
        AVLNode* current = node;
        while (current != nullptr && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }
    
    AVLNode* deleteNode(AVLNode* root, const std::string& bookID) {
        if (root == nullptr) {
            return root;
        }
        
        if (bookID < root->book.bookID) {
            root->left = deleteNode(root->left, bookID);
        } else if (bookID > root->book.bookID) {
            root->right = deleteNode(root->right, bookID);
        } else {
            if (root->left == nullptr || root->right == nullptr) {
                AVLNode* temp = root->left;
                if (temp == nullptr) {
                    temp = root->right;
                }
                
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }
                delete temp;
            } else {
                AVLNode* temp = minValueNode(root->right);
                root->book = temp->book;
                root->waitingQueue = temp->waitingQueue;
                root->right = deleteNode(root->right, temp->book.bookID);
            }
        }
        
        if (root == nullptr) {
            return root;
        }
        
        root->height = 1 + max(getHeight(root->left), getHeight(root->right));
        
        int balance = getBalance(root);
        
        //LL
        if (balance > 1 && getBalance(root->left) >= 0) {
            return rightRotate(root);
        }
        
        //LR
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        
        //RR
        if (balance < -1 && getBalance(root->right) <= 0) {
            return leftRotate(root);
        }
        
        //RL
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        
        return root;
    }
    
    AVLNode* searchByID(AVLNode* node, const std::string& bookID) {
        if (node == nullptr || node->book.bookID == bookID) {
            return node;
        }
        
        if (bookID < node->book.bookID) {
            return searchByID(node->left, bookID);
        } else {
            return searchByID(node->right, bookID);
        }
    }
    
    void searchByTitle(AVLNode* node, const std::string& title, std::vector<Book>& results) {
        if (node == nullptr) {
            return;
        }
        
        searchByTitle(node->left, title, results);
        
        if (node->book.title.find(title) != std::string::npos) {
            results.push_back(node->book);
        }
        
        searchByTitle(node->right, title, results);
    }
    
    void searchByAuthor(AVLNode* node, const std::string& author, std::vector<Book>& results) {
        if (node == nullptr) {
            return;
        }
        
        searchByAuthor(node->left, author, results);
        
        if (node->book.author.find(author) != std::string::npos) {
            results.push_back(node->book);
        }
        
        searchByAuthor(node->right, author, results);
    }
    
    void inOrderTraversal(AVLNode* node, std::vector<Book>& books) {
        if (node == nullptr) {
            return;
        }
        
        inOrderTraversal(node->left, books);
        books.push_back(node->book);
        inOrderTraversal(node->right, books);
    }
    
    void clearTree(AVLNode* node) {
        if (node == nullptr) {
            return;
        }
        
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr) {}
    
    ~AVLTree() {
        clearTree(root);
    }
    
    void insert(const Book& book) {
        root = insert(root, book);
    }
    
    void remove(const std::string& bookID) {
        root = deleteNode(root, bookID);
    }
    
    Book* searchByID(const std::string& bookID) {
        AVLNode* result = searchByID(root, bookID);
        if (result == nullptr) {
            return nullptr;
        } else {
            return &(result->book);
        }
    }
    
    std::vector<Book> searchByTitle(const std::string& title) {
        std::vector<Book> results;
        searchByTitle(root, title, results);
        return results;
    }
    
    std::vector<Book> searchByAuthor(const std::string& author) {
        std::vector<Book> results;
        searchByAuthor(root, author, results);
        return results;
    }
    
    std::vector<Book> getAllBooks() {
        std::vector<Book> books;
        inOrderTraversal(root, books);
        return books;
    }
    
    Queue* getWaitingQueue(const std::string& bookID) {
        AVLNode* node = searchByID(root, bookID);
        if (node == nullptr) {
            return nullptr;
        } else {
            return &(node->waitingQueue);
        }
    }
    
    bool bookExists(const std::string& bookID) {
        return searchByID(root, bookID) != nullptr;
    }
    
    void clear() {
        clearTree(root);
        root = nullptr;
    }
};

#endif