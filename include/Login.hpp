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

    static void borrow_book(Tree<Login>& user_tree,Tree<Books>& book_tree, int user_id, int book_id, int amount) {
        Login target_user;
        target_user.user_id = user_id;

        Node<Login>* user_node = user_tree.search(target_user);
        if (user_node == nullptr) {
            return;
        }
        
        Books target_book;
        target_book.set_book_id(book_id);

        Node<Books>* book_node=book_tree.search(target_book);
        if (!book_node)
        {
            return;
        }

        if (book_node->data.get_available_copy()<amount)
        {
            return;
        }

        string book_title=book_node->data.get_title();
        int current;
        if (user_node->data.borrowed.contains(book_title))
        {
            current=user_node->data.borrowed[book_title].get<int>();
        }
        user_node->data.borrowed[book_title] = current + amount;
        book_node->data.set_available_copy(book_node->data.get_available_copy()-amount);
        cout<<"book borrowed"<<endl;
    }
};




#endif