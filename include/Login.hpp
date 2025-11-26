#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <iostream>
#include <fstream>
#include <string>
#include "Tree.hpp"
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

    static void borrow_book(Tree<Login>& tree, int user_id, const string& book_title, int amount) {
        Login target;
        target.user_id = user_id;

        Node<Login>* found = tree.search(target);
        if (found == nullptr) {
            return;
        }
        found->data.borrowed[book_title] = amount;
    }
};




#endif