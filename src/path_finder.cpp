#include "path_finder.h"

std::vector<Position> Graph::neighbours_of(Position node) {
  std::vector<Position> neighbours;
  for (size_t index = 0; index < edges.size(); index++) {
    Edge edge = edges[index];
    if (edge.start == node) {
      neighbours.push_back(edge.end);
    } else if (edge.end == node) {
      neighbours.push_back(edge.start);
    }
  }
  return neighbours;
}

void Graph::add_vertex(Position position) {
  vertices.push_back(position);
}

void Graph::add_edge(Position start, Position end) {
  edges.push_back(Edge(start, end));
}

bool Edge::contains(Position position) {
  return start == position || end == position;
}
