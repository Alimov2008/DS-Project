#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
using json=nlohmann::json;
using namespace std;

class Login
{
private:
    string user_name;
    vector<string> borrowed_books;
    vector<string> queued_books;
public:
    Login(){};
    
    void get_user_name(){
        cout<<"UserName: ";
        cin>>user_name;
        if (user_name=="admin")
        {
            
        }
        
    }

    void new_user(){
        json users;
        ifstream input("../database/Books.json");
        if(input.is_open()){
            input>>users;
        }
        else{
            users=json::object();
        }

        int newID=0;
        for (auto& [key,value]:users.items())
        {
            newID=max(newID,stoi(key)+1);
        }

        new_user[(to_string(newID))]={
            {"UserName",this->user_name},
            {"Borrowed books : ",{}}
        };

    }
};




#endif