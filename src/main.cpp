// #include <nlohmann/json.hpp>
#include <iostream>
#include "../include/Tree.hpp"
#include "../include/books.hpp"
#include "../include/MenuUtilities.hpp"
using namespace std;
// using json = nlohmann::json;

int main() {
    string username;
    Tree<Login> userTree;
    Tree<Books> bookTree;
    
    cout << "=== Welcome to Library Management System ===" << endl;
    cout << "Enter your username: ";
    getline(cin, username);
    
    if (username.empty()) {
        cout << "Username cannot be empty!" << endl;
        return 1;
    }
    
    userTree = Login::load_users();
    bookTree = Books::load_book();
    
    Login currentUser = getUser(username, userTree);
    
    int choice;
    bool running = true;
    
    while (running) {
        displayMainMenu();
        cin >> choice;
        
        if (cin.fail()) {
            cout << "Invalid input! Please enter a number." << endl;
            clear_input_buffer();
            continue;
        }
        
        switch (choice) {
            case 1:
                borrowBook(userTree, bookTree, currentUser);
                break;
            case 2:
                returnBook(userTree, bookTree, currentUser);
                break;
            case 3:
                displayBooks(bookTree);
                break;
            case 4:
                displayBorrowedBooks(currentUser);
                break;
            case 5:
                running = false;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
                break;
        }
    }
    
    return 0;
}

