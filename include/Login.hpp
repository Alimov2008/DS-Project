#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <iostream>
#include <fstream>
#include <string>
#include "Tree.hpp"
#include "books.hpp"
#include <nlohmann/json.hpp>
using json=nlohmann::json;
using namespace std;

class Login
{
private:
    string user_name;
    int user_id;
    json borrowed;
public:
    Login() : user_id(0), user_name(""), borrowed(json::object()) {}

    string get_user_name() const {
        return this->user_name; }
    int get_user_id() const {
        return this->user_id; }
    json get_borrowed() const {
        return this->borrowed; }

    void set_user_name(const string& name) {
        this->user_name = name; }
    void set_user_id(int id) { 
        this->user_id = id; }
    void set_borrowed(const json& borrowed_data) { 
        this->borrowed = borrowed_data; }

    
    bool operator<(const Login& other) const {
        return this->user_id < other.user_id;
    }
    bool operator>(const Login& other) const {
        return this->user_id > other.user_id;
    }
    bool operator==(const Login& other) const {
        return this->user_id == other.user_id;
    }

    friend ostream& operator<<(ostream& os, const Login& user) {
        os << "[ID: " << user.user_id
           << ", User Name: " << user.user_name 
           << "]";
        return os;
    }

    void save_user() {
        json users;
        ifstream input("../database/Users.json");

        if (input.is_open()) {
            input >> users;
        } else {
            users = json::object(); 
        }

        int newID = 1;
        for (auto& [key, value] : users.items()) {
            newID = max(newID, std::stoi(key) + 1);
        }

        this->user_id = newID;

        users[to_string(newID)] = {
            {"Name", this->user_name},
            {"Borrowed", this->borrowed}
        };

        ofstream output("../database/Users.json");
        output << users.dump(4);
    }

    static void save_all_users(Tree<Login>& user_tree) {
        json users = json::object();
        auto allUsers = user_tree.getAll();
        
        for (const auto& user : allUsers) {
            users[to_string(user.get_user_id())] = {
                {"Name", user.get_user_name()},
                {"Borrowed", user.get_borrowed()}
            };
        }
        
        ofstream output("../database/Users.json");
        output << users.dump(4);
        output.close();
    }

    static Tree<Login> load_users() {
        Tree<Login> tree;
        json users;

        ifstream file("../database/Users.json");
        if (!file.is_open()) {
            return tree;
        }

        file >> users;
        
        for (auto& [key, value] : users.items()) {
            Login user;
            user.user_id = stoi(key);
            
            if (value.contains("Name"))
            user.user_name = value["Name"].get<string>();
            
            if (value.contains("Borrowed"))
            user.borrowed = value["Borrowed"];
            
            tree.insert(user);
        }
        
        return tree;
    }

    static void borrow_book(
    Tree<Login>& user_tree,
    Tree<Books>& book_tree, 
    int user_id, int book_id, 
    int amount) {
        Login target_user;
        target_user.set_user_id(user_id);

        Node<Login>* user_node = user_tree.search(target_user);
        if (user_node == nullptr) {
            cout << "User not found!" << endl;
            return;
        }
        
        Books target_book;
        target_book.set_book_id(book_id);

        Node<Books>* book_node = book_tree.search(target_book);
        if (!book_node) {
            cout << "Book not found!" << endl;
            return;
        }

        if (book_node->data.get_available_copy() < amount) {
            cout << "Not enough copies available! Only " << book_node->data.get_available_copy() << " available." << endl;
            return;
        }

        string book_title = book_node->data.get_title();
        int current = 0;
        if (user_node->data.borrowed.contains(book_title)) {
            current = user_node->data.borrowed[book_title].get<int>();
        }
        user_node->data.borrowed[book_title] = current + amount;
        book_node->data.set_available_copy(book_node->data.get_available_copy() - amount);
        
        save_all_users(user_tree);
        save_all_books(book_tree);
        
        cout << "Successfully borrowed " << amount << " copy of '" << book_title << "'" << endl;
    }


    static void return_book_by_title(
    Tree<Login>& user_tree,
    Tree<Books>& book_tree, 
    int user_id, 
    const string& book_title, 
    int amount) {
        Login target_user;
        target_user.set_user_id(user_id);

        Node<Login>* user_node = user_tree.search(target_user);
        if (user_node == nullptr) {
            cout << "User not found" << endl;
            return;
        }
        
        if (!user_node->data.borrowed.contains(book_title)) {
            cout << "You haven't borrowed this book" << endl;
            return;
        }
        
        Tree<Books> allBooks = Books::load_book();
        auto allBooksList = allBooks.getAll(); 
        
        int book_id = -1;
        for (auto& book : allBooksList) {
            if (book.get_title() == book_title) {
                book_id = book.get_book_id();
                break;
            }
        }
        
        if (book_id == -1) {
            cout << "Book not found" << endl;
            return;
        }
        
    }
};




#endif