#pragma once

#include "graph/graph.hpp"
#include <QPointF>
#include <deque>

class CoordGraph : public Graph<QPointF, double> {

private:


  std::deque<NodeType *> randomGenNodes(int nodesN, double limitA, double limitB);
  void sortNodes();
  double distance(NodeType *a, NodeType *b);
  void randomGenEdges(unsigned int maxEdges, double distance);

public:
  using EdgeType = Edge<Graph<QPointF, double>>;
  void randomGeneration(int nodesN, int edgesN, double distance, double limitA,
                        double limitB);
  void removeNodeRange(QPointF first, QPointF last);
  std::pair<double, std::deque<EdgeType *>> TreeA(NodeType *start,NodeType *end);

};
