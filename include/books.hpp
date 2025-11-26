#ifndef BOOKS_HPP
#define BOOKS_HPP

#include <iostream>
#include <fstream>
#include <string>
#include "Tree.hpp"
#include <nlohmann/json.hpp>
using json=nlohmann::json;
using namespace std;

class Books
{
private:
    int book_id;
    string title;
    string author;
    int total_copy;
    int available_copy;
public:
    Books(string title,string author,int total_copy,int available_copy){
        this->title=title;
        this->author=author;
        this->total_copy=total_copy;
        this->available_copy=available_copy;
    };

    Books() : title(""), author(""), total_copy(0), available_copy(0) {};

    bool operator<(const Books& b) const {
        return this->book_id < b.book_id;
    }
    
    bool operator>(const Books& b) const {
        return this->book_id > b.book_id;
    }

    bool operator==(const Books& b) const {
        return this->book_id == b.book_id;
    }


    void save_book(){
        json books;
        ifstream input("../database/Books.json");
        if(input.is_open()){
            input>>books;
        }
        else{
            books=json::object();
        }

        int newID=1;
        for (auto& [key,value]:books.items())
        {
            newID=max(newID,stoi(key)+1);
        }

        books[to_string(newID)]={
            {"Title",this->title},
            {"Author",this->author},
            {"Total",this->total_copy},
            {"Available",this->available_copy}
        };

        ofstream output("../database/Books.json");
        output << books.dump(4);
    }

    Tree<Books> load_book(){
        Tree<Books> loaded_tree;
        json book;
        ifstream file("../database/Books.json");
        if (!file.is_open()){
            return loaded_tree;
        }
        file>>book;
        for(auto& [key,value]:book.items()){
            Books b;
            b.book_id=stoi(key);
            b.author=value["Author"];
            b.available_copy=value["Available"];
            b.title=value["Title"];
            b.total_copy=value["Total"];

            loaded_tree.insert(b);
        }
        return loaded_tree;
    }

    void update_book(Tree<Books>& library,int book_id,int amount){
        Books target_book;
        target_book.book_id=book_id;

        Node<Books>* found=library.search(target_book);
        if (found == nullptr) 
        {
            cout << "Book not found"<<endl;
            return;
        }
        found->data.available_copy-=amount;
    }

    friend ostream& operator<<(ostream& os, const Books& b) {
        os << "[ID: " << b.book_id
           << ", Title: " << b.title
           << ", Author: " << b.author
           << ", Total: " << b.total_copy
           << ", Available: " << b.available_copy
           << " ]";
        return os;
    }
    
};

#endif