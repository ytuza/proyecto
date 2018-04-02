#include "coordGraph.hpp"
#include <algorithm>
#include <cmath>
#include <random>
#include <utility>


using EdgeType = Edge<Graph<QPointF, double>>;

std::deque<CoordGraph::NodeType *>
CoordGraph::randomGenNodes(int nodesN, double limitA, double limitB) {
  std::random_device seed;
  std::mt19937 gen(seed());
  std::uniform_real_distribution<double> disA(limitA, limitB),
      disB(limitA, limitB);

  std::deque<NodeType *> nodes(nodesN);
  for (auto &i : nodes)
    i = new NodeType({disA(gen), disB(gen)});

  return nodes;
}

// TODO: Check if is better to make this with the QpointF converted to Qpoint
void CoordGraph::sortNodes() {
  std::sort(nodeList.begin(), nodeList.end(), [](NodeType *a, NodeType *b) {
    if (a->value.y() < b->value.y())
      return true;
    else if (a->value.y() > b->value.y())
      return false;
    else
      return a->value.x() < b->value.x();
  });
}

// d = sqrt((x1 - x2)² +(y1 - y2)²)
double CoordGraph::distance(NodeType *a, NodeType *b) {
  double x = a->value.x() - b->value.x();
  double y = a->value.y() - b->value.y();
  return sqrt(x * x + y * y);
}

void CoordGraph::randomGenEdges(unsigned int maxEdges, double distance) {
  for (auto i = nodeList.begin(); i < nodeList.end(); i++) {
    for (auto j = i + 1; j < nodeList.end(); j++) {
      if ((*i)->edgeList.size() >= maxEdges)
        break;
      double dist = this->distance(*i, *j);
      if (dist <= distance)
        insertEdge(dist, *i, *j); // TODO: randomize the direction
    }
  }
}

void CoordGraph::randomGeneration(int nodesN, int edgesN, double distance,
                                  double limitA, double limitB) {
  if (!nodeList.empty()) {
    this->clear();
  }
  nodeList = randomGenNodes(nodesN, limitA, limitB);
  sortNodes();
  randomGenEdges(edgesN, distance); // TODO: find a good distance
}

void CoordGraph::removeNodeRange(QPointF first, QPointF last) {
  for (auto i = nodeList.begin(); i < nodeList.end();) {
    if ((*i)->value.x() >= first.x() and (*i)->value.x() <= last.x() and
        (*i)->value.y() >= first.y() and (*i)->value.y() <= last.y())
      i = this->removeNode(i);
    else
      i++;
  }
}

std::pair<double, std::deque<EdgeType *>> CoordGraph::TreeA(NodeType *start,NodeType *end){
    std::deque<NodeType *> queue;
    start->pathData = new PathFindData(0.0, nullptr);
    EdgeType *tem;
    (start->pathData)->done = true;
    queue.push_back(start);
    bool roads = true;
    double edgeD = 0.0;
    while(roads && queue.back() != end){
        NodeType *crtNode = queue.back(),*next_n;

        double max_d = 0.0;
        roads= false;
        for (const auto &iedge : crtNode->edgeList) {
            NodeType *otherNode = iedge->otherNode(crtNode);
            if((otherNode->pathData == nullptr || (otherNode->pathData)->done == false) && (distance(otherNode,end)+ iedge->value)>max_d){
                roads=true;
                edgeD =  iedge->value;
                next_n = otherNode;
                tem = iedge;
                max_d = distance(otherNode,end)+ iedge->value;
            }
        }
        if(roads){
            double disP  =((queue.back())->pathData)->accDist;
             next_n->pathData = new PathFindData(disP+edgeD, tem);
             (next_n->pathData)->done = true;
             queue.push_back(next_n);
        }else{
            queue.pop_back();
        }

    }

    std::deque<EdgeType *> edgePath;
    double pathLenght = 0.0;
    if (end->pathData) {
      pathLenght = end->pathData->accDist;
      NodeType *actNode = end;
      while (actNode != start) {
        edgePath.push_back(actNode->pathData->prev);
        actNode = actNode->pathData->prev->otherNode(actNode);
      }
      for (const auto &i : nodeList) {
        if (i->pathData) {
          delete i->pathData;
          i->pathData = nullptr;
        }
      }
    }
    return {pathLenght, edgePath};

}
