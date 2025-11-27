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

    static void save_all_users(Tree<Login>& user_tree) {
        json users = json::object();
        
        json all_users;
        ifstream input("../database/Users.json");
        if (input.is_open()) {
            input >> all_users;
        } else {
            all_users = json::object();
        }
        
        Tree<Login> current_users = load_users();
        
        ofstream output("../database/Users.json");
        output << current_users.dump(4);  
    }

    static void borrow_book(Tree<Login>& user_tree,Tree<Books>& book_tree, int user_id, int book_id, int amount) {
    Login target_user;
    target_user.set_user_id(user_id);

    Node<Login>* user_node = user_tree.search(target_user);
    if (user_node == nullptr) {
        cout << "User not found!" << endl;
        return;
    }
    
    Books target_book;
    target_book.set_book_id(book_id);

    Node<Books>* book_node=book_tree.search(target_book);
    if (!book_node) {
        cout << "Book not found!" << endl;
        return;
    }

    if (book_node->data.get_available_copy() < amount) 
    {
        cout << "Not enough copies available! Only " << book_node->data.get_available_copy() << " available." << endl;
        return;
    }

    string book_title=book_node->data.get_title();
    int current=0;
    if (user_node->data.borrowed.contains(book_title)) 
    {
        current=user_node->data.borrowed[book_title].get<int>();
    }
    user_node->data.borrowed[book_title] = current + amount;
    book_node->data.set_available_copy(book_node->data.get_available_copy()-amount);
    cout << "Successfully borrowed " << amount << " copy of '" << book_title << "'" << endl;
    }


    static void return_book(
        Tree<Login>& user_tree,
        Tree<Books>& book_tree, 
        int user_id, 
        int book_id, 
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

        string book_title = book_node->data.get_title();

        if (!user_node->data.borrowed.contains(book_title)) {
            cout << "You haven't borrowed this book!" << endl;
            return;
        }
        
        int current = user_node->data.borrowed[book_title].get<int>();
        
        if (amount > current) {
            cout << "You can't return more than you borrowed! You have " << current << " copy(ies)." << endl;
            return;
        }
        
        if (amount == current) {
            user_node->data.borrowed.erase(book_title);
        } else {
            user_node->data.borrowed[book_title] = current - amount;
        }
        
        book_node->data.set_available_copy(book_node->data.get_available_copy() + amount);
        
        cout << "Successfully returned " << amount << " copy(ies) of '" << book_title << "'" << endl;
    }

};




#endif