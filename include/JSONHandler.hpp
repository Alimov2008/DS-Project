#ifndef JSONHANDLER_HPP
#define JSONHANDLER_HPP

#include "AVLTree.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h> 

class JSONHandler {
private:
    std::string filename;

    void createDatabaseFolder() {
        CreateDirectory("database", NULL);
    }

    std::string escapeString(const std::string& str) {
        std::string result;
        for (char c : str) {
            if (c == '"' || c == '\\') {
                result += '\\';
            }
            result += c;
        }
        return result;
    }

    void writeQueue(std::ostream& file, Queue* queue) {
        file << "[";
        if (queue && !queue->isEmpty()) {
            std::vector<std::string> students = queue->getAllStudents();
            for (size_t i = 0; i < students.size(); i++) {
                file << "\"" << escapeString(students[i]) << "\"";
                if (i < students.size() - 1) {
                    file << ", ";
                }
            }
        }
        file << "]";
    }

    void readQueue(Queue& queue, const std::string& queueStr) {
        std::string temp = queueStr;
        size_t start = temp.find('[');
        size_t end = temp.find(']');
        
        if (start != std::string::npos && end != std::string::npos) {
            temp = temp.substr(start + 1, end - start - 1);
            
            size_t pos = 0;
            while (pos < temp.length()) {
                size_t quoteStart = temp.find('"', pos);
                if (quoteStart == std::string::npos) break;
                
                size_t quoteEnd = temp.find('"', quoteStart + 1);
                if (quoteEnd == std::string::npos) break;
                
                std::string studentID = temp.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
                queue.enqueue(studentID);
                
                pos = quoteEnd + 1;
            }
        }
    }

public:
    JSONHandler() {
        createDatabaseFolder();
        filename = "database/library_data.json";
    }
    
    void saveToFile(AVLTree& tree) {
        createDatabaseFolder(); 
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
            std::cerr << "Current directory: ";
            system("cd"); 
            return;
        }
        
        std::vector<Book> books = tree.getAllBooks();
        
        file << "{\n";
        file << "  \"books\": [\n";
        
        for (size_t i = 0; i < books.size(); i++) {
            const Book& book = books[i];
            Queue* queue = tree.getWaitingQueue(book.bookID);
            
            file << "    {\n";
            file << "      \"bookID\": \"" << escapeString(book.bookID) << "\",\n";
            file << "      \"title\": \"" << escapeString(book.title) << "\",\n";
            file << "      \"author\": \"" << escapeString(book.author) << "\",\n";
            file << "      \"totalCopies\": " << book.totalCopies << ",\n";
            file << "      \"availableCopies\": " << book.availableCopies;
            
            if (queue && !queue->isEmpty()) {
                file << ",\n";
                file << "      \"waitingQueue\": ";
                writeQueue(file, queue);
            } else {
                file << "\n";
            }
            
            file << "    }";
            if (i < books.size() - 1) {
                file << ",";
            }
            file << "\n";
        }
        
        file << "  ]\n";
        file << "}\n";
        
        file.close();
        std::cout << "Data saved successfully to " << filename << std::endl;
    }
    
    void loadFromFile(AVLTree& tree) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "No existing data file found. Starting with empty library." << std::endl;
            return;
        }
        
        std::string line;
        std::string jsonContent;
        while (std::getline(file, line)) {
            jsonContent += line + "\n";
        }
        file.close();
        
        size_t booksStart = jsonContent.find("\"books\"");
        if (booksStart == std::string::npos) {
            std::cout << "Invalid JSON format." << std::endl;
            return;
        }
        
        booksStart = jsonContent.find('[', booksStart);
        if (booksStart == std::string::npos) {
            std::cout << "Invalid JSON format." << std::endl;
            return;
        }
        
        size_t pos = booksStart + 1;
        while (pos < jsonContent.length()) {
            size_t bookStart = jsonContent.find('{', pos);
            if (bookStart == std::string::npos) break;
            
            size_t bookEnd = jsonContent.find('}', bookStart);
            if (bookEnd == std::string::npos) break;
            
            std::string bookStr = jsonContent.substr(bookStart, bookEnd - bookStart + 1);
            
            std::string bookID, title, author;
            int totalCopies = 0, availableCopies = 0;
            std::string waitingQueueStr;
            
            size_t fieldPos = 0;
            while (fieldPos < bookStr.length()) {
                size_t quoteStart = bookStr.find('"', fieldPos);
                if (quoteStart == std::string::npos) break;
                
                size_t quoteEnd = bookStr.find('"', quoteStart + 1);
                if (quoteEnd == std::string::npos) break;
                
                std::string fieldName = bookStr.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
                
                size_t valueStart = bookStr.find(':', quoteEnd);
                if (valueStart == std::string::npos) break;
                
                if (fieldName == "bookID" || fieldName == "title" || fieldName == "author") {
                    size_t valueQuoteStart = bookStr.find('"', valueStart);
                    size_t valueQuoteEnd = bookStr.find('"', valueQuoteStart + 1);
                    if (valueQuoteEnd == std::string::npos) break;
                    
                    std::string value = bookStr.substr(valueQuoteStart + 1, valueQuoteEnd - valueQuoteStart - 1);
                    
                    if (fieldName == "bookID") bookID = value;
                    else if (fieldName == "title") title = value;
                    else if (fieldName == "author") author = value;
                    
                    fieldPos = valueQuoteEnd + 1;
                } else if (fieldName == "totalCopies" || fieldName == "availableCopies") {
                    size_t valueEnd = bookStr.find_first_of(",}", valueStart);
                    if (valueEnd == std::string::npos) break;
                    
                    std::string value = bookStr.substr(valueStart + 1, valueEnd - valueStart - 1);
                    
                    if (fieldName == "totalCopies") totalCopies = std::stoi(value);
                    else if (fieldName == "availableCopies") availableCopies = std::stoi(value);
                    
                    fieldPos = valueEnd + 1;
                } else if (fieldName == "waitingQueue") {
                    size_t arrayStart = bookStr.find('[', valueStart);
                    size_t arrayEnd = bookStr.find(']', arrayStart);
                    if (arrayEnd == std::string::npos) break;
                    
                    waitingQueueStr = bookStr.substr(arrayStart, arrayEnd - arrayStart + 1);
                    fieldPos = arrayEnd + 1;
                } else {
                    fieldPos = quoteEnd + 1;
                }
            }
            
            if (!bookID.empty()) {
                Book book(bookID, title, author, totalCopies, availableCopies);
                tree.insert(book);
                
                if (!waitingQueueStr.empty()) {
                    Queue* queue = tree.getWaitingQueue(bookID);
                    if (queue) {
                        readQueue(*queue, waitingQueueStr);
                    }
                }
            }
            
            pos = bookEnd + 1;
        }
        
        std::cout << "Data loaded successfully from " << filename << std::endl;
    }
};

#endif