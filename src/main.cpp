#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <windows.h>
#include "AvlDictionary.h"

using std::size_t;

#define NDEBUG

#ifndef NDEBUG

#define TEST(f, a, b) \
   do { \
       std::cerr << "TEST " << #f << " STARTED" << std::endl; \
       if(f()) { \
           std::cerr << "TEST " << #f << " PASSED" << std::endl; \
           a++; \
       } else { \
           std::cerr << "TEST " << #f << " FAILED" << std::endl; \
           b++; \
       } \
       std::cout << std::endl; \
   } while(false)

bool empty();
bool insert();
bool search();
bool searchError();
bool deleteTest();
bool deleteError();
void test();

#endif

void printDict(const AvlDictionary<std::string, std::string>& dict, int indent);
void printDict(const AvlDictionary<std::string, std::vector<std::string>>& dict, int indent);

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#ifndef NDEBUG
    test();

#else
    AvlDictionary<std::string, std::vector<std::string>> dict;

   std::ifstream file("C://dictionary.txt");

   while(!file.eof()) {
       std::string line;
       std::getline(file, line);
       if(line.empty()) break;
       if(line[line.length() - 1] == '\r') line.pop_back();

       size_t index = line.find(':');
       std::string word = line.substr(0, index);

       std::stringstream rest(line.substr(index + 1));
       std::vector<std::string> translations;
       while(!rest.eof()) {
           std::string translation;
           std::getline(rest, translation, ',');
           if(translation.empty()) break;
           translations.push_back(translation);
       }

       dict.insert(word, translations);
   }

   std::cout << "DICTIONARY TREE:" << std::endl;
   printDict(dict, 1);

   std::string word;
   std::cout<<"Enter word for search: ";
   std::cin >> word;
   try {
       for(auto translation: dict.search(word)) {
           std::cout << translation << std::endl;
       }
   } catch(std::runtime_error& e) {
       std::cerr << "No such word registered" << std::endl;
   }
#endif
    return 0;
}

void printDict(const AvlDictionary<std::string, std::string> &dict, int indent) {
    for(int i = 0; i < indent; i++) std::cout << ' ';

    if(dict.isNil()) {
        std::cout << "NIL" << std::endl;
        return;
    }

    std::cout << dict.getKey() << ": " << dict.getValue() << std::endl;

    printDict(dict.getLeft(), indent + 1);
    printDict(dict.getRight(), indent + 1);
}

void printDict(const AvlDictionary<std::string, std::vector<std::string>> &dict, int indent) {
    for(int i = 0; i < indent; i++) std::cout << ' ';

    if(dict.isNil()) {
        std::cout << "NIL" << std::endl;
        return;
    }

    std::cout << dict.getKey() << ": ";
    auto value = dict.getValue();
    std::cout << value[0];
    for(size_t i = 1; i < value.size(); i++) {
        std::cout << ", " << value[i];
    }
    std::cout << std::endl;

    printDict(dict.getLeft(), indent + 1);
    printDict(dict.getRight(), indent + 1);
}

#ifndef NDEBUG

bool empty() {
    bool result = true;

    try {
        AvlDictionary<std::string, std::string> dict;
        std::cout << "DICTIONARY STATE:" << std::endl;
        printDict(dict, 1);
    } catch(std::exception& e) {
        result = false;
    }

    return result;
}

bool insert() {
    bool result = true;

    try {
        AvlDictionary<std::string, std::string> dict;
        dict.insert("apple", "яблоко");
        dict.insert("orange", "апельсин");
        dict.insert("lemon", "лемон");
        dict.insert("pineapple", "ананас");
        dict.insert("strawberry", "клубника");
        dict.insert("melon", "дыня");
        dict.insert("peach", "персик");
        std::cout << "DICTIONARY STATE:" << std::endl;
        printDict(dict, 1);
    } catch(std::exception& e) {
        result = false;
    }

    return result;
}

bool search() {
    bool result = true;

    try {
        AvlDictionary<std::string, std::string> dict;
        dict.insert("apple", "яблоко");
        dict.insert("orange", "апельсин");
        dict.insert("lemon", "лемон");
        dict.insert("pineapple", "ананас");
        dict.insert("strawberry", "клубника");
        dict.insert("melon", "дыня");
        dict.insert("peach", "персик");
        std::cout << "DICTIONARY STATE:" << std::endl;
        printDict(dict, 1);
        result = result && dict.search("apple") == "яблоко";
        result = result && dict.search("peach") == "персик";
    } catch(std::exception& e) {
        result = false;
    }

    return result;
}

bool searchError() {
    bool result = false;

    try {
        AvlDictionary<std::string, std::string> dict;
        dict.insert("apple", "яблоко");
        dict.insert("orange", "апельсин");
        dict.insert("lemon", "лемон");
        dict.insert("pineapple", "ананас");
        dict.insert("strawberry", "клубника");
        dict.insert("peach", "персик");
        std::cout << "DICTIONARY STATE:" << std::endl;
        printDict(dict, 1);
        try {
            dict.search("melon");
        } catch(std::runtime_error& e) {
            result = true;
        }
    } catch(std::runtime_error& e) {
        result = false;
    }

    return result;
}

bool deleteTest() {
    bool result = false;

    try {
        AvlDictionary<std::string, std::string> dict;
        dict.insert("apple", "яблоко");
        dict.insert("orange", "апельсин");
        dict.insert("lemon", "лемон");
        dict.insert("pineapple", "ананас");
        dict.insert("strawberry", "клубника");
        dict.insert("melon", "дыня");
        dict.insert("peach", "персик");
        std::cout << "DICTIONARY STATE BEFORE DELETION:" << std::endl;
        printDict(dict, 1);
        dict.deleteKey("peach");
        std::cout << "DICTIONARY STATE AFTER DELETION:" << std::endl;
        printDict(dict, 1);
        try {
            dict.search("peach");
        } catch(std::runtime_error& e) {
            result = true;
        }
    } catch(std::runtime_error& e) {
        result = false;
    }

    return result;
}

bool deleteError() {
    bool result = false;

    try {
        AvlDictionary<std::string, std::string> dict;
        dict.insert("apple", "яблоко");
        dict.insert("lemon", "лемон");
        dict.insert("pineapple", "ананас");
        dict.insert("strawberry", "клубника");
        dict.insert("melon", "дыня");
        dict.insert("peach", "персик");
        std::cout << "DICTIONARY STATE:" << std::endl;
        printDict(dict, 1);
        try {
            dict.deleteKey("orange");
        } catch(std::runtime_error& e) {
            result = true;
        }
    } catch(std::runtime_error& e) {
        result = false;
    }

    return result;
}

void test() {
    int passed = 0;
    int failed  = 0;

    std::cerr << "==========" << std::endl;
    std::cerr << "START OF TESTING" << std::endl << std::endl;

    TEST(empty, passed, failed);
    TEST(insert, passed, failed);
    TEST(search, passed, failed);
    TEST(searchError, passed, failed);
    TEST(deleteTest, passed, failed);
    TEST(deleteError, passed, failed);

    std::cerr << "END OF TESTING" << std::endl;
    std::cerr << "TESTS PASSED: " << passed << std::endl;
    std::cerr << "TESTS FAILED: " << failed << std::endl;
    std::cerr << "==========" << std::endl;
    std::cerr << std::endl;
}
#endif
