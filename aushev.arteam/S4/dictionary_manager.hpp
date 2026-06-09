#ifndef AUSHEV_DICTIONARY_MANAGER_HPP
#define AUSHEV_DICTIONARY_MANAGER_HPP

#include <string>
#include <fstream>
#include <iostream>
#include "dictionary_entry.hpp"

namespace aushev {

class DictList {
    struct Node {
        DictionaryEntry data_;
        Node* next_;
        Node(const std::string& name) : data_(name), next_(nullptr) {}
    };
    Node* head_;

public:
    DictList() : head_(nullptr) {}

    ~DictList() {
        clear();
    }

    void clear() {
        while (head_) {
            Node* tmp = head_;
            head_ = head_->next_;
            delete tmp;
        }
    }

    DictionaryEntry* find(const std::string& name) {
        Node* curr = head_;
        while (curr) {
            if (curr->data_.getName() == name) {
                return &curr->data_;
            }
            curr = curr->next_;
        }
        return nullptr;
    }

    bool add(const std::string& name) {
        if (find(name)) {
            return false;
        }
        Node* newNode = new Node(name);
        newNode->next_ = head_;
        head_ = newNode;
        return true;
    }
};

int stringToInt(const std::string& str) {
    int result = 0;
    int sign = 1;
    size_t i = 0;
    if (i < str.size() && str[i] == '-') {
        sign = -1;
        ++i;
    }
    for (; i < str.size(); ++i) {
        result = result * 10 + (str[i] - '0');
    }
    return result * sign;
}

size_t skipSpaces(const std::string& str, size_t pos) {
    while (pos < str.size() && str[pos] == ' ') {
        ++pos;
    }
    return pos;
}

std::string readToken(const std::string& str, size_t& pos) {
    pos = skipSpaces(str, pos);
    std::string token;
    while (pos < str.size() && str[pos] != ' ') {
        token += str[pos];
        ++pos;
    }
    return token;
}

class DictionaryManager {
    DictList dicts_;

public:
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            size_t pos = 0;
            pos = skipSpaces(line, pos);
            if (pos == line.size()) {
                continue;
            }

            std::string name = readToken(line, pos);
            if (name.empty()) {
                continue;
            }

            dicts_.add(name);
            DictionaryEntry* entry = dicts_.find(name);

            while (pos < line.size()) {
                std::string keyStr = readToken(line, pos);
                if (keyStr.empty()) {
                    break;
                }
                std::string valStr = readToken(line, pos);
                if (valStr.empty()) {
                    break;
                }

                int key = stringToInt(keyStr);
                entry->getTree().push(key, valStr);
            }
        }
        return true;
    }

    DictionaryEntry* findDict(const std::string& name) {
        return dicts_.find(name);
    }

    bool createDict(const std::string& name) {
        return dicts_.add(name);
    }

    bool printDataset(const std::string& name) {
        DictionaryEntry* entry = dicts_.find(name);
        if (!entry) {
            return false;
        }

        const BSTree<int, std::string>& tree = entry->getTree();
        if (tree.cbegin() == tree.cend()) {
            std::cout << "<EMPTY>\n";
            return true;
        }

        std::cout << name;
        for (auto it = tree.cbegin(); it != tree.cend(); ++it) {
            std::cout << ' ' << (*it).first << ' ' << (*it).second;
        }
        std::cout << '\n';
        return true;
    }

    bool complement(const std::string& newName, const std::string& name1, const std::string& name2) {
        DictionaryEntry* entry1 = dicts_.find(name1);
        DictionaryEntry* entry2 = dicts_.find(name2);
        if (!entry1 || !entry2 || dicts_.find(newName)) {
            return false;
        }

        dicts_.add(newName);
        DictionaryEntry* newEntry = dicts_.find(newName);
        const BSTree<int, std::string>& tree1 = entry1->getTree();
        const BSTree<int, std::string>& tree2 = entry2->getTree();

        for (auto it = tree1.cbegin(); it != tree1.cend(); ++it) {
            if (!tree2.contains((*it).first)) {
                newEntry->getTree().push((*it).first, (*it).second);
            }
        }
        return true;
    }

    bool intersect(const std::string& newName, const std::string& name1, const std::string& name2) {
        DictionaryEntry* entry1 = dicts_.find(name1);
        DictionaryEntry* entry2 = dicts_.find(name2);
        if (!entry1 || !entry2 || dicts_.find(newName)) {
            return false;
        }

        dicts_.add(newName);
        DictionaryEntry* newEntry = dicts_.find(newName);
        const BSTree<int, std::string>& tree1 = entry1->getTree();
        const BSTree<int, std::string>& tree2 = entry2->getTree();

        for (auto it = tree1.cbegin(); it != tree1.cend(); ++it) {
            if (tree2.contains((*it).first)) {
                newEntry->getTree().push((*it).first, (*it).second);
            }
        }
        return true;
    }

    bool unionDicts(const std::string& newName, const std::string& name1, const std::string& name2) {
        DictionaryEntry* entry1 = dicts_.find(name1);
        DictionaryEntry* entry2 = dicts_.find(name2);
        if (!entry1 || !entry2 || dicts_.find(newName)) {
            return false;
        }

        dicts_.add(newName);
        DictionaryEntry* newEntry = dicts_.find(newName);
        const BSTree<int, std::string>& tree1 = entry1->getTree();
        const BSTree<int, std::string>& tree2 = entry2->getTree();

        for (auto it = tree1.cbegin(); it != tree1.cend(); ++it) {
            newEntry->getTree().push((*it).first, (*it).second);
        }
        for (auto it = tree2.cbegin(); it != tree2.cend(); ++it) {
            if (!newEntry->getTree().contains((*it).first)) {
                newEntry->getTree().push((*it).first, (*it).second);
            }
        }
        return true;
    }
};

}

#endif
