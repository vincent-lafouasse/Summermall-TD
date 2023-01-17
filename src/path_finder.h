#pragma once
#include <queue>
#include <vector>
#include "geometry.h"

#define DISTANCE_PRECISION 1000

struct Edge {
  Position start;
  Position end;
  int distance;

  Edge(Position start_, Position end_) {
    start = start_;
    end = end_;
    distance = (int)(DISTANCE_PRECISION * euclidian_distance(start, end));
  }
};

struct Graph {
  std::vector<Position> vertices;
  std::vector<Edge> edges;

  void add_vertex(Position position);
  void add_edge(Position end, Position start);
};

std::vector<Position> compute_path_between(Graph graph,
                                           Position start,
                                           Position end);
