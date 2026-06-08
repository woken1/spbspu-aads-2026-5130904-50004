#ifndef AUSHEV_GRAPH_MANAGER_HPP
#define AUSHEV_GRAPH_MANAGER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "graph.hpp"
#include "hash_table.hpp"
#include "dynamic_array.hpp"

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

    bool mergeGraphs(const std::string& newName, const std::string& old1, const std::string& old2) {
        if (graphs_.has(newName) || !graphs_.has(old1) || !graphs_.has(old2)) {
            return false;
        }
        const Graph& g1 = graphs_.drop(old1);
        const Graph& g2 = graphs_.drop(old2);
        Graph newGraph;
        for (auto it = g1.getVertices().begin(); it != g1.getVertices().end(); ++it) {
            newGraph.addVertex((*it).first);
        }
        for (auto it = g2.getVertices().begin(); it != g2.getVertices().end(); ++it) {
            newGraph.addVertex((*it).first);
        }
        for (auto it = g1.getEdges().begin(); it != g1.getEdges().end(); ++it) {
            const auto& key = (*it).first;
            const auto& weights = (*it).second;
            for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
                newGraph.addEdge(key.first, key.second, *wit);
            }
        }
        for (auto it = g2.getEdges().begin(); it != g2.getEdges().end(); ++it) {
            const auto& key = (*it).first;
            const auto& weights = (*it).second;
            for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
                newGraph.addEdge(key.first, key.second, *wit);
            }
        }
        graphs_.add(old1, g1);
        graphs_.add(old2, g2);
        graphs_.add(newName, newGraph);
        return true;
    }

    bool extractGraph(const std::string& newName, const std::string& oldName, const DynamicArray<std::string>& vertices) {
        if (graphs_.has(newName) || !graphs_.has(oldName)) {
            return false;
        }
        const Graph& oldGraph = graphs_.drop(oldName);
        for (size_t i = 0; i < vertices.size(); ++i) {
            if (!oldGraph.hasVertex(vertices[i])) {
                graphs_.add(oldName, oldGraph);
                return false;
            }
        }
        Graph newGraph;
        for (size_t i = 0; i < vertices.size(); ++i) {
            newGraph.addVertex(vertices[i]);
        }
        for (size_t i = 0; i < vertices.size(); ++i) {
            for (size_t j = 0; j < vertices.size(); ++j) {
                const std::string& v1 = vertices[i];
                const std::string& v2 = vertices[j];
                Graph::EdgeKey key = std::make_pair(v1, v2);
                if (oldGraph.getEdges().has(key)) {
                    const Graph::EdgeValue& weights = oldGraph.getEdges().drop(key);
                    for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
                        newGraph.addEdge(v1, v2, *wit);
                    }
                    const_cast<Graph&>(oldGraph).getEdges().add(key, weights);
                }
            }
        }
        graphs_.add(oldName, oldGraph);
        graphs_.add(newName, newGraph);
        return true;
    }

private:
    HashTable<std::string, Graph, std::hash<std::string>, std::equal_to<std::string>> graphs_;
};

}

#endif
