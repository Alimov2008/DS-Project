#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include "../include/AVLTree.hpp"
#include "../include/JSONHandler.hpp"

class LibraryManager {
private:
    AVLTree library;
    JSONHandler jsonHandler;
    
    void displayMenu() {
        std::cout << "=== LIBRARY MANAGEMENT SYSTEM ===" << std::endl;
        std::cout << "1. Add New Book" << std::endl;
        std::cout << "2. Search Books" << std::endl;
        std::cout << "3. Issue Book" << std::endl;
        std::cout << "4. Return Book" << std::endl;
        std::cout << "5. Display All Books" << std::endl;
        std::cout << "6. Exit" << std::endl;
    }
    
    void addBook() {
        std::string bookID, title, author;
        int totalCopies;
        
        std::cout << "Enter Book ID: ";
        std::getline(std::cin, bookID);
        
        if (library.bookExists(bookID)) {
            std::cout << "Error: Book with ID " << bookID << " already exists!" << std::endl;
            return;
        }
        
        std::cout << "Enter Title: ";
        std::getline(std::cin, title);
        std::cout << "Enter Author: ";
        std::getline(std::cin, author);
        std::cout << "Enter Total Copies: ";
        std::cin >> totalCopies;
        
        if (totalCopies < 0) {
            std::cout << "Error: Total copies cannot be negative!" << std::endl;
            return;
        }
        
        Book newBook(bookID, title, author, totalCopies, totalCopies);
        library.insert(newBook);
        std::cout << "Book added successfully!" << std::endl;
    }
    
    void searchBooks() {
        int searchChoice;
        std::cout << "Search by:" << std::endl;
        std::cout << "1. Book ID" << std::endl;
        std::cout << "2. Title" << std::endl;
        std::cout << "3. Author" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> searchChoice;
        std::cin.ignore();
        
        std::string query;
        std::vector<Book> results;
        
        if (searchChoice == 1) {
            std::cout << "Enter Book ID: ";
            std::getline(std::cin, query);
            Book* book = library.searchByID(query);
            if (book != nullptr) {
                results.push_back(*book);
            }
        } else if (searchChoice == 2) {
            std::cout << "Enter Title: ";
            std::getline(std::cin, query);
            results = library.searchByTitle(query);
        } else if (searchChoice == 3) {
            std::cout << "Enter Author: ";
            std::getline(std::cin, query);
            results = library.searchByAuthor(query);
        } else {
            std::cout << "Invalid choice!" << std::endl;
            return;
        }
        
        if (results.empty()) {
            std::cout << "No books found!" << std::endl;
        } else {
            std::cout << "Found " << results.size() << " book(s):" << std::endl;
            for (size_t i = 0; i < results.size(); i++) {
                displayBook(results[i]);
            }
        }
    }
    
    void issueBook() {
        std::string bookID, studentID;
        
        std::cout << "Enter Book ID: ";
        std::getline(std::cin, bookID);
        std::cout << "Enter Student ID: ";
        std::getline(std::cin, studentID);
        
        Book* book = library.searchByID(bookID);
        if (book == nullptr) {
            std::cout << "Error: Book not found!" << std::endl;
            return;
        }
        
        Queue* queue = library.getWaitingQueue(bookID);
        
        if (book->availableCopies > 0) {
            book->availableCopies--;
            std::cout << "Book issued successfully to student " << studentID << std::endl;
            std::cout << "Available copies: " << book->availableCopies << std::endl;
        } else {
            if (queue != nullptr && queue->contains(studentID)) {
                std::cout << "Error: Student " << studentID << " is already in the waiting queue!" << std::endl;
                return;
            }
            
            if (queue != nullptr) {
                queue->enqueue(studentID);
                std::cout << "Book is currently unavailable. Student " << studentID 
                          << " added to waiting queue at position " << queue->getSize() << std::endl;
            }
        }
    }
    
    void returnBook() {
        std::string bookID;
        
        std::cout << "Enter Book ID: ";
        std::getline(std::cin, bookID);
        
        Book* book = library.searchByID(bookID);
        if (book == nullptr) {
            std::cout << "Error: Book not found!" << std::endl;
            return;
        }
        
        Queue* queue = library.getWaitingQueue(bookID);
        
        if (queue != nullptr && !queue->isEmpty()) {
            std::string nextStudent = queue->dequeue();
            std::cout << "Book returned and immediately issued to student " << nextStudent 
                      << " from waiting queue." << std::endl;
            std::cout << "Available copies remain: " << book->availableCopies << std::endl;
            
            if (!queue->isEmpty()) {
                std::cout << "Next student in queue: " << queue->peek() << std::endl;
            }
        } else {
            if (book->availableCopies < book->totalCopies) {
                book->availableCopies++;
                std::cout << "Book returned successfully!" << std::endl;
                std::cout << "Available copies: " << book->availableCopies << std::endl;
            } else {
                std::cout << "Error: All copies are already available!" << std::endl;
            }
        }
    }
    
    void displayAllBooks() {
        std::vector<Book> books = library.getAllBooks();
        
        if (books.empty()) {
            std::cout << "No books in the library!" << std::endl;
            return;
        }
        
        std::cout << "Total books in library: " << books.size() << std::endl;
        for (size_t i = 0; i < books.size(); i++) {
            displayBook(books[i]);
        }
    }
    
    void displayBook(const Book& book) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Book ID: " << book.bookID << std::endl;
        std::cout << "Title: " << book.title << std::endl;
        std::cout << "Author: " << book.author << std::endl;
        std::cout << "Total Copies: " << book.totalCopies << std::endl;
        std::cout << "Available Copies: " << book.availableCopies << std::endl;
        
        Queue* queue = library.getWaitingQueue(book.bookID);
        if (queue != nullptr && !queue->isEmpty()) {
            std::cout << "Waiting Queue: " << queue->getSize() << " student(s)" << std::endl;
            std::cout << "Next in queue: " << queue->peek() << std::endl;
        } else {
            std::cout << "Waiting Queue: Empty" << std::endl;
        }
    }

public:
    LibraryManager() : jsonHandler("database/library_data.json") {}
    
   void run() {
    jsonHandler.loadFromFile(library);
    
    int choice;
    do {
        displayMenu();
        std::cout << "Enter your choice: ";
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << "Invalid input! Please enter a number (1-6).\n" << std::endl;
            continue;
        }
        
        std::cin.ignore(); 
        
        if (choice == 1) {
            addBook();
        } else if (choice == 2) {
            searchBooks();
        } else if (choice == 3) {
            issueBook();
        } else if (choice == 4) {
            returnBook();
        } else if (choice == 5) {
            displayAllBooks();
        } else if (choice == 6) {
            jsonHandler.saveToFile(library);
            std::cout << "Thank you for using Library Management System!" << std::endl;
        } else {
            std::cout << "Invalid choice! Please try again.\n" << std::endl;
        }
        
    } while (choice != 6);
}
};

int main() {
    LibraryManager manager;
    manager.run();
    return 0;
}