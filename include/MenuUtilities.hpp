#ifndef MENUUTILITIES_HPP
#define MENUUTILITIES_HPP

#include <iostream>
#include <string>
#include <limits>
#include "../include/Login.hpp"
#include "../include/books.hpp"
using namespace std;

void clear_input_buffer(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
}

void displayMainMenu() {
    cout<<"\n======== Library Management System ========" << endl;
    cout<<"1. Borrow Books" << endl;
    cout<<"2. Return Books" << endl;
    cout<<"3. Display Available Books" << endl;
    cout<<"4. Display My Borrowed Books" << endl;
    cout<<"5. Exit" << endl;
    cout<<"Enter your choice: ";
}

void displayBooks(Tree<Books>& bookTree) {
    cout<<"\n======== Available Books ========" << endl;
    bookTree.inorder();
}

void displayBorrowedBooks(Login& currentUser) {
    cout << "\n=== My Borrowed Books ===" << endl;
    if (currentUser.get_borrowed().empty()) {
        cout << "No books currently borrowed." << endl;
    } else {
        for (auto& [book_title, amount] : currentUser.get_borrowed().items()) {
            cout << "Title: " << book_title << " - Quantity: " << amount.get<int>() << endl;
        }
    }
}

void borrowBook(Tree<Login>& userTree, Tree<Books>& bookTree, Login& currentUser) {
    int book_id, amount;
    
    displayBooks(bookTree);
    
    cout << "\nEnter Book ID to borrow: ";
    cin >> book_id;
    
    cout << "Enter quantity: ";
    cin >> amount;
    
    if (cin.fail() || amount <= 0) {
        cout << "Invalid quantity!" << endl;
        clear_input_buffer();
        return;
    }
    Login::borrow_book(userTree, bookTree, currentUser.get_user_id(), book_id, amount);
}

void returnBook(Tree<Login>& userTree, Tree<Books>& bookTree, Login& currentUser) {
    if (currentUser.get_borrowed().empty()) {
        cout << "You have no books to return" << endl;
        return;
    }
    
    displayBorrowedBooks(currentUser);
    
    string book_title;
    int amount;
    
    clear_input_buffer();
    cout << "\nEnter book title to return: ";
    getline(cin, book_title);
    
    cout << "Enter quantity to return: ";
    cin >> amount;
    
    if (cin.fail() || amount <= 0) {
        cout << "Invalid quantity!" << endl;
        clear_input_buffer();
        return;
    }

    int book_id = -1;
    
    Tree<Books> allBooks = Books::load_book();
    bool found = false;
    if (!found) {
        cout << "Book title not found in available books!" << endl;
        return;
    }
    Login::return_book(userTree, bookTree, currentUser.get_user_id(), book_id, amount);
}

Login getUser(const string& username, Tree<Login>& userTree) {
    Tree<Login> allUsers = Login::load_users();
    vector<Login> users;
    Login newUser;
    newUser.set_user_name(username);
    newUser.save_user();
    
    cout << "New user created with ID: " << newUser.get_user_id() << endl;
    return newUser;
}
#endif