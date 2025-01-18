#ifndef _GRAPH_TABLE_
#define _GRAPH_TABLE_

#include <vector>
#include <map>

using namespace std;

template<typename GraphElement>
class graph_table {
    private:
    map<int, vector<GraphElement>> _entries;
    int _last_level;
    int _min_level;
    
    public:
        // return first paths in graph (entrypoints)
        bool head(vector<GraphElement>& out);

        // return last paths in graph (most deepest)
        bool tail(vector<GraphElement>& out);

        // return last path of the last added level
        bool last(GraphElement& out);

        // return last parent path in graph
        bool parent(int offset, GraphElement& out);

        void add(GraphElement& el, int offset);

        graph_table(): _last_level(0), _min_level(6) {};
        ~graph_table();
};

#endif