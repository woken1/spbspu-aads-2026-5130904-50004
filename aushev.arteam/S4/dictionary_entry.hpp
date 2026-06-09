#ifndef AUSHEV_DICTIONARY_ENTRY_HPP
#define AUSHEV_DICTIONARY_ENTRY_HPP

#include <string>
#include "bstree.hpp"

namespace aushev {

class DictionaryEntry {
    std::string name_;
    BSTree<int, std::string> tree_;

public:
    explicit DictionaryEntry(const std::string& name) :
        name_(name) {}

    const std::string& getName() const {
        return name_;
    }

    BSTree<int, std::string>& getTree() {
        return tree_;
    }

    const BSTree<int, std::string>& getTree() const {
        return tree_;
    }
};

}

#endif
