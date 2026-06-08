#ifndef AUSHEV_GRAPH_HPP
#define AUSHEV_GRAPH_HPP

#include <string>
#include <utility>
#include "hash_table.hpp"
#include "simple_list.hpp"

namespace aushev {

struct PairHash {
    size_t operator()(const std::pair<std::string, std::string>& p) const {
        return std::hash<std::string>()(p.first) ^ (std::hash<std::string>()(p.second) << 1);
    }
};

class Graph {
public:
    using EdgeKey = std::pair<std::string, std::string>;
    using EdgeValue = SimpleList<unsigned int>;

    Graph() {
        vertices_.enableAutoRehash(true);
        edges_.enableAutoRehash(true);
    }

    void addVertex(const std::string& vertex) {
        if (!vertices_.has(vertex)) {
            vertices_.add(vertex, true);
        }
    }

    bool hasVertex(const std::string& vertex) const {
        return vertices_.has(vertex);
    }

    void addEdge(const std::string& from, const std::string& to, unsigned int weight) {
        addVertex(from);
        addVertex(to);
        EdgeKey key = std::make_pair(from, to);
        EdgeValue weights;
        if (edges_.has(key)) {
            weights = edges_.drop(key);
        }
        weights.pushBack(weight);
        edges_.add(key, weights);
    }

    bool removeEdge(const std::string& from, const std::string& to, unsigned int weight) {
        EdgeKey key = std::make_pair(from, to);
        if (!edges_.has(key)) {
            return false;
        }
        EdgeValue weights = edges_.drop(key);
        if (!weights.removeOne(weight)) {
            edges_.add(key, weights);
            return false;
        }
        if (!weights.empty()) {
            edges_.add(key, weights);
        }
        return true;
    }

    bool hasEdge(const std::string& from, const std::string& to, unsigned int weight) const {
        EdgeKey key = std::make_pair(from, to);
        if (!edges_.has(key)) {
            return false;
        }
        try {
            const EdgeValue& weights = edges_.drop(key);
            bool found = false;
            for (auto it = weights.begin(); it != weights.end(); ++it) {
                if (*it == weight) {
                    found = true;
                    break;
                }
            }
            const_cast<Graph*>(this)->edges_.add(key, weights);
            return found;
        } catch (...) {
            return false;
        }
    }

    const HashTable<std::string, bool, std::hash<std::string>, std::equal_to<std::string>>& getVertices() const {
        return vertices_;
    }

    const HashTable<EdgeKey, EdgeValue, PairHash, std::equal_to<EdgeKey>>& getEdges() const {
        return edges_;
    }

private:
    HashTable<std::string, bool, std::hash<std::string>, std::equal_to<std::string>> vertices_;
    HashTable<EdgeKey, EdgeValue, PairHash, std::equal_to<EdgeKey>> edges_;
};

}

#endif
