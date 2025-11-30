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
    
    int get_book_id(){
        return this->book_id;
    }
    string get_title(){
        return this->title;
    }
    string get_author(){
        return this->author;
    }
    int get_total_copy(){
        return this->total_copy;
    }
    int get_available_copy(){
        return this->available_copy;
    }

    void set_book_id(int book_id){
        this->book_id=book_id;
    }
    void set_title(string title){
        this->title=title;
    }
    void set_author(string author){
        this->author=author;
    }
    void set_total_copy(int total_copy){
        this->total_copy=total_copy;
    }
    void set_available_copy(int available_copy){
        this->available_copy=available_copy;
    }

    friend ostream& operator<<(ostream& os, const Books& b) {
        os << "[ID: " << b.book_id
           << ", Title: " << b.title
           << ", Author: " << b.author
           << ", Total: " << b.total_copy
           << ", Available: " << b.available_copy
           << " ]"
           << endl;
        return os;
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

    static void save_all_books(Tree<Books>& book_tree) {
        json books = json::object();
        auto allBooks = book_tree.getAll();
        
        for (auto& book : allBooks) {
            books[to_string(book.get_book_id())] = {
                {"Title", book.get_title()},
                {"Author", book.get_author()},
                {"Total", book.get_total_copy()},
                {"Available", book.get_available_copy()}
            };
        }
        
        ofstream output("../database/Books.json");
        output << books.dump(4);
        output.close();
    }

    static Tree<Books> load_book(){
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
            if (value.contains("Author")) 
            {b.author = value["Author"].get<string>();
            }
            if (value.contains("Available")) 
            {b.available_copy = value["Available"].get<int>();
            }
            if (value.contains("Title")) 
            {b.title = value["Title"].get<string>();
            }
            if (value.contains("Total")) 
            {b.total_copy = value["Total"].get<int>();
            }

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

    
};

#endif