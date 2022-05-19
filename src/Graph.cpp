#include <vector>
#include <queue>
#include <numeric>
#include "Graph.hpp"

// Debugging Purposes
#include <string>
#include <iostream>

Graph::Graph(int V) : V_{V} {}

void Graph::AddEdge(int src, int dest, int weight)
{
    ls_[src].push_back({dest, weight});
    ls_[dest].push_back({src, weight});
}

std::vector<int> Graph::PrimAlgorithm()
{
    const int INF = 1000000007;
    const int ROOT = 0;  // picking arbitarily suffices

    std::vector<int> key (V_, INF);
    auto qcomp = [&key](int x, int y) {
        return key[x] > key[y];
    };

    std::vector<int> elems (V_);
    std::iota(elems.begin(), elems.end(), 0);
    std::vector<int> parent (V_, -1);
    std::vector<bool> in_q (V_, true);

    key[ROOT] = 0, parent[ROOT] = ROOT;
    std::priority_queue<int, std::vector<int>, decltype(qcomp)> q (qcomp, elems);
    
    while (!q.empty()) {
        int src = q.top(); q.pop();
        // check if the node is already processed
        if (!in_q[src]) continue;
        in_q[src] = false;
        for (std::pair<int, int> edge : ls_[src]) {
            int dest = edge.first, w = edge.second;
            if (in_q[dest] && w < key[dest]) {
                key[dest] = w, parent[dest] = src;
                // necessary, as there is no decrease-key operation
                q.push(dest);  
            }
        }
    }
    return parent;
}

