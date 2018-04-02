#pragma once

#include <deque>

class GraphicNode;

template <typename G> class Node {
private:
public:
  using NodeType = typename G::NodeType;
  using EdgeType = typename G::EdgeType;
  using ValueType = typename G::NodeValueType;

  ValueType value;
  std::deque<EdgeType *> edgeList;

  // Connects this part with the graphics part:
  GraphicNode *graphics;

  // This is used to find the shortest path:
  typename G::PathFindData *pathData;

  // TODO: copy constructor
  // TODO: overlodad operator =

  Node() = default;

  Node(ValueType v) {
    value = v;

    graphics = nullptr;
    pathData = nullptr;
  }

  ~Node() {
    // The edge destructor is in charge of removing the edge ptr
    // contained in the edgeList...
    while (!edgeList.empty())
      delete edgeList.front();
  }
};
