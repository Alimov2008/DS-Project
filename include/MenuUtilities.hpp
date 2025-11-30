#ifndef MENUUTILITIES_HPP
#define MENUUTILITIES_HPP

#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include "Login.hpp"
#include "books.hpp"
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
            int quantity = 0;
            if (!amount.is_null()) {
                try {
                    quantity = amount.get<int>();
                } catch (const json::type_error& e) {
                    quantity = 0; 
                }
            }
            cout << "Title: " << book_title << " - Quantity: " << quantity << endl;
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
    
    Tree<Login> updatedUsers = Login::load_users();
    Login updatedTarget;
    updatedTarget.set_user_id(currentUser.get_user_id());
    Node<Login>* updatedUser = updatedUsers.search(updatedTarget);
    if (updatedUser) {
        currentUser = updatedUser->data;
    }
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

    Tree<Books> allBooks = Books::load_book();
    auto allBooksList = allBooks.getAll();
    int book_id = -1;
    bool found = false;
    for (auto& book : allBooksList) {
        if (book.get_title() == book_title) {
            book_id = book.get_book_id();
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Book title not found in available books" << endl;
        return;
    }
    Login::return_book_by_title(userTree, bookTree, currentUser.get_user_id(), book_title, amount);
}

Login getUser(const string& username, Tree<Login>& userTree) {
    Tree<Login> allUsers = Login::load_users();
    auto allUsersVector = allUsers.getAll(); 
    for (const auto& user : allUsersVector) {
        if (user.get_user_name() == username) {
            cout << "Welcome back, " << username << "!" << endl;
            return user;
        }
    }
    Login newUser;
    newUser.set_user_name(username);
    newUser.save_user();
    
    userTree = Login::load_users();
    
    auto updatedUsersVector = userTree.getAll();
    for (auto& u : updatedUsersVector)
    {
        if (u.get_user_name() == username)
        {
            cout << "New user created with ID: " << u.get_user_id() << endl;
            return u;
        }
    }
    cout << "New user created with ID: " << newUser.get_user_id() << endl;
    return newUser;
}

#endif
