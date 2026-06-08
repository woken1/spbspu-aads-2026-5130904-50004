#ifndef AUSHEV_GRAPH_MANAGER_HPP
#define AUSHEV_GRAPH_MANAGER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "graph.hpp"
#include "hash_table.hpp"

namespace aushev {

class GraphManager {
public:
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) {
                continue;
            }
            std::istringstream iss(line);
            std::string graphName;
            size_t edgeCount;
            if (!(iss >> graphName >> edgeCount)) {
                continue;
            }
            if (graphs_.has(graphName)) {
                continue;
            }
            Graph newGraph;
            graphs_.add(graphName, newGraph);
            for (size_t i = 0; i < edgeCount; ++i) {
                std::string v1, v2;
                unsigned int weight;
                if (iss >> v1 >> v2 >> weight) {
                    graphs_.drop(graphName).addEdge(v1, v2, weight);
                }
            }
        }
        return true;
    }

    bool createGraph(const std::string& name, const DynamicArray<std::string>& vertices) {
        if (graphs_.has(name)) {
            return false;
        }
        Graph newGraph;
        for (size_t i = 0; i < vertices.size(); ++i) {
            newGraph.addVertex(vertices[i]);
        }
        graphs_.add(name, newGraph);
        return true;
    }

    bool bindEdge(const std::string& graphName, const std::string& v1, const std::string& v2, unsigned int weight) {
        if (!graphs_.has(graphName)) {
            return false;
        }
        graphs_.drop(graphName).addEdge(v1, v2, weight);
        return true;
    }

    bool cutEdge(const std::string& graphName, const std::string& v1, const std::string& v2, unsigned int weight) {
        if (!graphs_.has(graphName)) {
            return false;
        }
        Graph& g = graphs_.drop(graphName);
        if (!g.hasVertex(v1) || !g.hasVertex(v2)) {
            graphs_.add(graphName, g);
            return false;
        }
        if (!g.removeEdge(v1, v2, weight)) {
            graphs_.add(graphName, g);
            return false;
        }
        graphs_.add(graphName, g);
        return true;
    }

private:
    HashTable<std::string, Graph, std::hash<std::string>, std::equal_to<std::string>> graphs_;
};

}

#endif
