#pragma once

#include <utility>

class GraphicEdge;

template <typename G> class Edge {
public:
  using ValueType = typename G::EdgeValueType;
  using NodeType = typename G::NodeType;
  using EdgeType = typename G::EdgeType;

private:
  void removeCon(Edge *e) {
    // This is the edgeList inside the first conected node:
    auto &eList1 = e->conNodes.first->edgeList;
    auto fnd = eList1.begin();
    while (*fnd != e)
      fnd++;
    eList1.erase(fnd);
    // And this is  the edge list of the second conected node:
    auto &eList2 = e->conNodes.second->edgeList;

    // If the edge is a loop then it was already deleted from the
    // edge list so the function returns;
    if (e->conNodes.first == e->conNodes.second)
      return;

    fnd = eList2.begin();

    while (*fnd != e)
      fnd++;
    eList2.erase(fnd);
    // TODO: convert the conected nodes to nullptr ??
  }

public:
  ValueType value;
  std::pair<NodeType *, NodeType *> conNodes;
  bool direction; // 0: [0] <-> [1] ; 1: [0] -> [1]

  // Connects this part with the graphics part:
  GraphicEdge *graphics;

  // TODO: copy constructor  ???
  // TODO: overlodad operator = ???

  Edge(ValueType val, NodeType *nodeA, NodeType *nodeB, bool dir = 0) {
    value = val;
    conNodes = {nodeA, nodeB};
    direction = dir;

    graphics = nullptr;
  }

  ~Edge() { removeCon(this); }

  NodeType *otherNode(const NodeType *n) {
    if (n != conNodes.second)
      return conNodes.second;
    else if (n != conNodes.first)
      return conNodes.first;
    else
      return nullptr;
  }
};
