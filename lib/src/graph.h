#pragma once

#include <cmath>
#include <stdexcept>
#include <vector>
#include <list>

using std::vector;

template<typename T>
class Graph {
 private:
  int vertices_number;
  std::list<int>* adjacency_list;

 public:
  Graph(int number) {
    if (number <= 0) throw std::logic_error("number must be positive!");
    vertices_number = number;
    adjacency_list = new std::list<int>[vertices_number + 1];
  }
  void AddEdge(int first_verticle, int second_verticle) {
    if (first_verticle < 0 || first_verticle > vertices_number ||
        second_verticle < 0 || second_verticle > vertices_number)
      throw std::logic_error("such node does not exist");
    if (first_verticle == second_verticle)
      throw std::logic_error("graph must be acyclic!");
    adjacency_list[first_verticle].push_back(second_verticle);
  }
  void TopologySortStep(int current_vertice, bool visited_vertices[],
                        std::list<int>& list);
  std::list<int> TopologySort();
};

class Weighted_Graph {
 private:
  vector<vector<int>> table;
  int vertices_number;

 public:
  Weighted_Graph(vector<vector<int>> table_) {
    if (table_.empty())
      throw std::logic_error(
          "the graph was not created because the input table is empty");
    for (int i = 0; i < table_.size(); i++)
      if (table_[i].size() != table_.size())
        throw std::logic_error(
            "the graph was not created because the input table is incorrect");
    table = table_;
    vertices_number = table.size();
  }

  int MinDistance(vector<int> distances, bool is_shortest[]) {
    int min_distance = 2 ^ 31 - 1, min_index;
    for (int i = 0; i < vertices_number; i++)
      if (!is_shortest[i] && distances[i] < min_distance)
        min_distance = distances[i], min_index = i;
    return min_index;
  };
  vector<int> Dijkstra_algo(int source) {
    source -= 1;
    vector<int> distances;
    bool is_shortest[vertices_number];
    for (int i = 0; i < vertices_number; i++) {
      distances.push_back(std::pow(2, 31) - 1);
      is_shortest[i] = false;
    }
    distances[source] = 0;
    for (int i = 0; i < vertices_number - 1; i++) {
      int min_distance = MinDistance(distances, is_shortest);
      is_shortest[min_distance] = true;
      for (int j = 0; j < vertices_number; j++)
        if (!is_shortest[j] && table[min_distance][j] &&
            distances[j] != (2 ^ 31 - 1) &&
            distances[min_distance] + table[min_distance][j] < distances[j])
          distances[j] = distances[min_distance] + table[min_distance][j];
    }
    return distances;
  }
  vector<int> BellmanFord_Algorithm(int source) {
    vector<int> dist(source + 1, pow(2,31));
    dist[source] = 0;

    vector<vector<int>> edges_with_extra(table);
    for (int i = 0; i < source; ++i) {
        edges_with_extra.push_back({source, i, 0});
    }

    for (int i = 0; i < source; ++i) {
        for (const auto& edge : edges_with_extra) {
            if (dist[edge[0]] != pow(2,31) && dist[edge[0]] + edge[2] < dist[edge[1]]) {
                dist[edge[1]] = dist[edge[0]] + edge[2];
            }
        }
    }
    return vector<int>(dist.begin(), dist.begin() + V);
}
vector<vector<int>> JohnsonAlgorithm(const vector<vector<int>>& graph) {
    int V = graph.size();
    vector<vector<int>> edges;
    
    for (int i = 0; i < V; ++i) 
        for (int j = 0; j < V; ++j) 
            if (graph[i][j] != 0) 
                edges.push_back({i, j, graph[i][j]});
            
    vector<int> altered_weights = BellmanFord_Algorithm(edges, V);
    vector<vector<int>> altered_graph(V, vector<int>(V, 0));

    for (int i = 0; i < V; ++i) 
        for (int j = 0; j < V; ++j) 
            if (graph[i][j] != 0) 
                altered_graph[i][j] = graph[i][j] + altered_weights[i] - altered_weights[j];

    return altered_graph;

    // for (int source = 0; source < V; ++source) {
    //     Dijkstra_Algorithm(graph, altered_graph, source);
    // }
}
};