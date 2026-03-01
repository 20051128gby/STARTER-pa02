#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include "movies.h"

class PrefixTrie {
public:
    PrefixTrie();
    void insert(const std::string& prefix, int prefixIndex);
    void dispatchMovieToBuckets(const Movie& mv,
                                std::vector<std::vector<Movie>>& buckets) const;

private:
    struct Node {
        int next[128];
        int terminalIndex;
        Node();
    };

    std::vector<Node> nodes;
};

std::string rtrim_copy(std::string s);

#endif