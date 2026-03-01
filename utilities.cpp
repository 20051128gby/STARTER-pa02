#include "utilities.h"

PrefixTrie::Node::Node() : terminalIndex(-1) {
    for (int i = 0; i < 128; i++) next[i] = -1;
}

PrefixTrie::PrefixTrie() {
    nodes.emplace_back();
}

void PrefixTrie::insert(const std::string& prefix, int prefixIndex) {
    int cur = 0;
    for (unsigned char ch : prefix) {
        if (ch >= 128) return;
        int& nxt = nodes[cur].next[ch];
        if (nxt == -1) {
            nxt = (int)nodes.size();
            nodes.emplace_back();
        }
        cur = nxt;
    }
    nodes[cur].terminalIndex = prefixIndex;
}

void PrefixTrie::dispatchMovieToBuckets(const Movie& mv,
                                        std::vector<std::vector<Movie>>& buckets) const {
    int cur = 0;
    for (unsigned char ch : mv.name) {
        if (ch >= 128) break;
        int nxt = nodes[cur].next[ch];
        if (nxt == -1) break;
        cur = nxt;
        int idx = nodes[cur].terminalIndex;
        if (idx != -1) buckets[idx].push_back(mv);
    }
}

std::string rtrim_copy(std::string s) {
    while (!s.empty()) {
        char c = s.back();
        if (c == '\r' || c == ' ' || c == '\t') s.pop_back();
        else break;
    }
    return s;
}