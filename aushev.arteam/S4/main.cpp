#include <iostream>
#include <string>
#include "dictionary_manager.hpp"

namespace aushev {

int run(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: filename is required" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    DictionaryManager manager;
    if (!manager.loadFromFile(filename)) {
        std::cerr << "Error: cannot open file" << std::endl;
        return 1;
    }

    std::string cmdLine;
    while (std::getline(std::cin, cmdLine)) {
        size_t pos = 0;
        pos = skipSpaces(cmdLine, pos);
        if (pos == cmdLine.size()) {
            continue;
        }

        std::string command = readToken(cmdLine, pos);

        if (command == "print") {
            std::string arg1 = readToken(cmdLine, pos);
            if (arg1.empty() || pos != cmdLine.size()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            if (!manager.printDataset(arg1)) {
                std::cout << "<INVALID COMMAND>\n";
            }
        } else if (command == "complement") {
            std::string arg1 = readToken(cmdLine, pos);
            std::string arg2 = readToken(cmdLine, pos);
            std::string arg3 = readToken(cmdLine, pos);
            if (arg1.empty() || arg2.empty() || arg3.empty() || pos != cmdLine.size()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            if (!manager.complement(arg1, arg2, arg3)) {
                std::cout << "<INVALID COMMAND>\n";
            }
        } else if (command == "intersect") {
            std::string arg1 = readToken(cmdLine, pos);
            std::string arg2 = readToken(cmdLine, pos);
            std::string arg3 = readToken(cmdLine, pos);
            if (arg1.empty() || arg2.empty() || arg3.empty() || pos != cmdLine.size()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            if (!manager.intersect(arg1, arg2, arg3)) {
                std::cout << "<INVALID COMMAND>\n";
            }
        } else if (command == "union") {
            std::string arg1 = readToken(cmdLine, pos);
            std::string arg2 = readToken(cmdLine, pos);
            std::string arg3 = readToken(cmdLine, pos);
            if (arg1.empty() || arg2.empty() || arg3.empty() || pos != cmdLine.size()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            if (!manager.unionDicts(arg1, arg2, arg3)) {
                std::cout << "<INVALID COMMAND>\n";
            }
        } else {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
    return 0;
}

}

int main(int argc, const char* argv[]) {
    return aushev::run(argc, argv);
}
