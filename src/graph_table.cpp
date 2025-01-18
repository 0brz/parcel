#include "./graph_table.h"

using namespace std;


template<typename GraphElement>
bool graph_table<GraphElement>::head(vector<GraphElement>& out) {
    auto fn = _entries.find(_min_level);
    if (fn != end(_entries)) {
        out = (*fn).second;
            return true;
        }
            
    return false;
};

template<typename GraphElement>
bool graph_table<GraphElement>::tail(vector<GraphElement>& out) {
            auto fn = _entries.find(_last_level);
            if (fn != end(_entries)) {
                out = (*fn).second;
                return true;
            }
            
            return false;
};

template<typename GraphElement>
bool graph_table<GraphElement>::last(GraphElement& out) {
            auto fn = _entries.find(_last_level);
            if (fn != end(_entries)) {
                auto vec = (*fn).second;
                if (vec.size() > 0) {
                    out = vec.back();
                    return true;
                }

                return false;
            }
            
            return false;
};

template<typename GraphElement>
bool graph_table<GraphElement>::parent(int offset, GraphElement& out) {
            auto fn = _entries.find(_last_level);
            if (fn != end(_entries)) {
                auto vec = (*fn).second;
                if (vec.size() > 0) {
                    out = vec.back();
                    return true;
                }
                
                return false;
            }
            
            return false;
};

template<typename GraphElement>
void graph_table<GraphElement>::add(GraphElement& el, int offset) {
            auto fn = _entries.find(offset);
            if (fn != end(_entries)) {
                (*fn).second.push_back(el);
            }
            else {
                vector<lex_graph_path> v;
                v.push_back(el);
                _entries.insert({offset, v});
            }

            _last_level= offset;
            _min_level = min(_min_level, offset);
};

template<typename GraphElement>
graph_table<GraphElement>::~graph_table()  {
            printf("[graph_table].~()\n");
            _entries.clear();
        };