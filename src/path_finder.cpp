#include "path_finder.h"

void Graph::add_vertex(Position position) {
  vertices.push_back(position);
}

void Graph::add_edge(Position start, Position end) {
  edges.push_back(Edge(start, end));
}
