#ifndef Graph_hpp
#define Graph_hpp

#include <vector>
#include <utility>

class Graph {
public:
    explicit Graph(int V);
    void AddEdge(int src, int dest, int weight = 0);

    std::vector<int> PrimAlgorithm();

private:
    static constexpr int MAX_V {1000};

    int V_;
    std::vector<std::pair<int, int>> ls_[MAX_V];
};

#endif  /* Graph_hpp */

