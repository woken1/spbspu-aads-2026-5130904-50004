#include <iostream>
#include <string>
#include <sstream>
#include "graph_manager.hpp"
#include "dynamic_array.hpp"

namespace aushev {

int run(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: filename is required" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    GraphManager manager;
    if (!manager.loadFromFile(filename)) {
        std::cerr << "Error: cannot open file" << std::endl;
        return 1;
    }
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }
        std::istringstream iss(line);
        std::string command;
        iss >> command;
        if (command == "graphs") {
            manager.printGraphs();
        } else if (command == "vertexes") {
            std::string graphName;
            if (!(iss >> graphName)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                continue;
            }
            if (!manager.printVertexes(graphName)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        } else if (command == "outbound") {
            std::string graphName, vertex;
            if (!(iss >> graphName >> vertex)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                continue;
            }
            if (!manager.printOutbound(graphName, vertex)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        } else if (command == "inbound") {
            std::string graphName, vertex;
            if (!(iss >> graphName >> vertex)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                continue;
            }
            if (!manager.printInbound(graphName, vertex)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        } else if (command == "bind") {
            std::string graphName, v1, v2;
            unsigned int weight;
            if (!(iss >> graphName >> v1 >> v2 >> weight)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                continue;
            }
            if (!manager.bindEdge(graphName, v1, v2, weight)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        } else if (command == "cut") {
            std::string graphName, v1, v2;
            unsigned int weight;
            if (!(iss >> graphName >> v1 >> v2 >> weight)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                continue;
            }
            if (!manager.cutEdge(graphName, v1, v2, weight)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        } else if (command == "create") {
            std::string graphName;
            size_t count;
            if (!(iss >> graphName >> count)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                continue;
            }
            DynamicArray<std::string> vertices;
            for (size_t i = 0; i < count; ++i) {
                std::string v;
                if (!(iss >> v)) {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                    continue;
                }
                vertices.pushBack(v);
            }
            if (!manager.createGraph(graphName, vertices)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        } else if (command == "merge") {
            std::string newName, old1, old2;
            if (!(iss >> newName >> old1 >> old2)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                continue;
            }
            if (!manager.mergeGraphs(newName, old1, old2)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        } else if (command == "extract") {
            std::string newName, oldName;
            size_t count;
            if (!(iss >> newName >> oldName >> count)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                continue;
            }
            DynamicArray<std::string> vertices;
            for (size_t i = 0; i < count; ++i) {
                std::string v;
                if (!(iss >> v)) {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                    continue;
                }
                vertices.pushBack(v);
            }
            if (!manager.extractGraph(newName, oldName, vertices)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        } else {
            std::cout << "<INVALID COMMAND>" << std::endl;
        }
    }
    return 0;
}

}

int main(int argc, char* argv[]) {
    return aushev::run(argc, argv);
}
