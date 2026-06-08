#ifndef AUSHEV_GRAPH_MANAGER_HPP
#define AUSHEV_GRAPH_MANAGER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "graph.hpp"
#include "hash_table.hpp"
#include "dynamic_array.hpp"
#include "simple_list.hpp"

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
                    graphs_.getRef(graphName).addEdge(v1, v2, weight);
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
        graphs_.getRef(graphName).addEdge(v1, v2, weight);
        return true;
    }

    bool cutEdge(const std::string& graphName, const std::string& v1, const std::string& v2, unsigned int weight) {
        if (!graphs_.has(graphName)) {
            return false;
        }
        Graph& g = graphs_.getRef(graphName);
        if (!g.hasVertex(v1) || !g.hasVertex(v2)) {
            return false;
        }
        if (!g.removeEdge(v1, v2, weight)) {
            return false;
        }
        return true;
    }

    bool mergeGraphs(const std::string& newName, const std::string& old1, const std::string& old2) {
        if (graphs_.has(newName) || !graphs_.has(old1) || !graphs_.has(old2)) {
            return false;
        }
        const Graph& g1 = graphs_.get(old1);
        const Graph& g2 = graphs_.get(old2);
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
        graphs_.add(newName, newGraph);
        return true;
    }

    bool extractGraph(const std::string& newName, const std::string& oldName, const DynamicArray<std::string>& vertices) {
        if (graphs_.has(newName) || !graphs_.has(oldName)) {
            return false;
        }
        const Graph& oldGraph = graphs_.get(oldName);
        for (size_t i = 0; i < vertices.size(); ++i) {
            if (!oldGraph.hasVertex(vertices[i])) {
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
                    const Graph::EdgeValue& weights = oldGraph.getEdges().get(key);
                    for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
                        newGraph.addEdge(v1, v2, *wit);
                    }
                }
            }
        }
        graphs_.add(newName, newGraph);
        return true;
    }

    void printGraphs() {
        DynamicArray<std::string> names;
        for (auto it = graphs_.begin(); it != graphs_.end(); ++it) {
            names.pushBack((*it).first);
        }
        names.sort();
        for (size_t i = 0; i < names.size(); ++i) {
            std::cout << names[i] << std::endl;
        }
    }

    bool printVertexes(const std::string& graphName) {
        if (!graphs_.has(graphName)) {
            return false;
        }
        const Graph& g = graphs_.get(graphName);
        DynamicArray<std::string> verts;
        for (auto it = g.getVertices().begin(); it != g.getVertices().end(); ++it) {
            verts.pushBack((*it).first);
        }
        verts.sort();
        for (size_t i = 0; i < verts.size(); ++i) {
            std::cout << verts[i] << std::endl;
        }
        return true;
    }

    bool printOutbound(const std::string& graphName, const std::string& vertex) {
        if (!graphs_.has(graphName)) {
            return false;
        }
        const Graph& g = graphs_.get(graphName);
        if (!g.hasVertex(vertex)) {
            return false;
        }
        DynamicArray<std::string> targets;
        for (auto it = g.getEdges().begin(); it != g.getEdges().end(); ++it) {
            if ((*it).first.first == vertex) {
                bool found = false;
                for (size_t i = 0; i < targets.size(); ++i) {
                    if (targets[i] == (*it).first.second) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    targets.pushBack((*it).first.second);
                }
            }
        }
        targets.sort();
        for (size_t i = 0; i < targets.size(); ++i) {
            std::cout << targets[i];
            Graph::EdgeKey key = std::make_pair(vertex, targets[i]);
            const Graph::EdgeValue& weights = g.getEdges().get(key);
            DynamicArray<unsigned int> wArr;
            for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
                wArr.pushBack(*wit);
            }
            wArr.sort();
            for (size_t j = 0; j < wArr.size(); ++j) {
                std::cout << " " << wArr[j];
            }
            std::cout << std::endl;
        }
        return true;
    }

    bool printInbound(const std::string& graphName, const std::string& vertex) {
        if (!graphs_.has(graphName)) {
            return false;
        }
        const Graph& g = graphs_.get(graphName);
        if (!g.hasVertex(vertex)) {
            return false;
        }
        DynamicArray<std::string> sources;
        for (auto it = g.getEdges().begin(); it != g.getEdges().end(); ++it) {
            if ((*it).first.second == vertex) {
                bool found = false;
                for (size_t i = 0; i < sources.size(); ++i) {
                    if (sources[i] == (*it).first.first) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    sources.pushBack((*it).first.first);
                }
            }
        }
        sources.sort();
        for (size_t i = 0; i < sources.size(); ++i) {
            std::cout << sources[i];
            Graph::EdgeKey key = std::make_pair(sources[i], vertex);
            const Graph::EdgeValue& weights = g.getEdges().get(key);
            DynamicArray<unsigned int> wArr;
            for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
                wArr.pushBack(*wit);
            }
            wArr.sort();
            for (size_t j = 0; j < wArr.size(); ++j) {
                std::cout << " " << wArr[j];
            }
            std::cout << std::endl;
        }
        return true;
    }

private:
    HashTable<std::string, Graph, std::hash<std::string>, std::equal_to<std::string>> graphs_;
};

}

#endif
