#ifndef BOOK_HPP
#define BOOK_HPP

#include <string>

struct Book {
    std::string bookID;
    std::string title;
    std::string author;
    int totalCopies;
    int availableCopies;
    
    Book(const std::string& id = "", const std::string& t = "", 
         const std::string& a = "", int total = 0, int available = 0)
        : bookID(id), title(t), author(a), totalCopies(total), availableCopies(available) {}
    
    bool operator<(const Book& other) const { return bookID < other.bookID; }
    bool operator>(const Book& other) const { return bookID > other.bookID; }
    bool operator==(const Book& other) const { return bookID == other.bookID; }
};

#endif