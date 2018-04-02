#pragma once

#include "edge.hpp"
#include "node.hpp"
#include <algorithm>
#include <deque>
#include <utility>

template <typename N, typename E> class Graph {
public:
  using SelfType = Graph<N, E>;
  using NodeValueType = N;
  using EdgeValueType = E;
  using NodeType = Node<SelfType>;
  using EdgeType = Edge<SelfType>;

  struct PathFindData {
    double accDist;
    bool done;
    EdgeType *prev;
    PathFindData(double dist, EdgeType *prev) {
      this->accDist = dist;
      this->prev = prev;
    }
  };

protected:
  std::deque<NodeType *> nodeList;

public:
  void insertNode(NodeValueType val) { nodeList.push_back(new NodeType(val)); }

  void insertEdge(EdgeValueType val, NodeType *nodeA, NodeType *nodeB, bool dir = 0) {
    EdgeType *tmp = new EdgeType(val, nodeA, nodeB, dir);
    nodeA->edgeList.push_back(tmp);
    if (nodeA != nodeB) // If the edge is a loop we don't have to push it again
      nodeB->edgeList.push_back(tmp);
  }

  void removeNode(NodeType *remv) {
    auto fnd = nodeList.begin();
    while (*fnd != remv)
      fnd++;
    delete *fnd;
    nodeList.erase(fnd);
  }

  typename std::deque<NodeType *>::iterator
  removeNode(typename std::deque<NodeType *>::iterator iterator) {
    delete *iterator;
    return nodeList.erase(iterator);
  }

  // Remove all the edges between two nodes:
  void removeEdgesBtwn(NodeType *nodeA, NodeType *nodeB) {
    std::deque<EdgeType *> toremove;
    for (EdgeType *i : nodeA->edgeList) {
      auto &first = i->conNodes.first;
      auto &second = i->conNodes.second;

      if ((first == nodeA and second == nodeB) or
          (first == nodeB and second == nodeA))
        toremove.push_back(i);
    }
    for (EdgeType *i : toremove)
      delete i;
  }

  void removeEdge(EdgeType *remv) { delete remv; }

  const std::deque<NodeType *> &getNodeList() { return nodeList; }

  void clear() {
    while (!nodeList.empty()) {
      delete nodeList.back();
      nodeList.pop_back();
    }
  }


  std::pair<double, std::deque<EdgeType *>> minPathDijkstra(NodeType *start,
                                                        NodeType *end) {

    std::deque<NodeType *> queue;
    start->pathData = new PathFindData(0.0, nullptr);
    queue.push_back(start);

    while (!queue.empty()) {
      NodeType *crtNode = queue.front();
      for (const auto &iedge : crtNode->edgeList) {
        NodeType *otherNode = iedge->otherNode(crtNode);
        double dist = crtNode->pathData->accDist + iedge->value;
        if (otherNode->pathData == nullptr) {
          otherNode->pathData = new PathFindData(dist, iedge);
          auto pos = std::upper_bound(
              queue.begin(), queue.end(), otherNode, [](auto a, auto b) {
                return a->pathData->accDist < b->pathData->accDist;
              });
          queue.insert(pos, otherNode);

        } else {
          if (dist < otherNode->pathData->accDist) {
            NodeType *tmp = new NodeType();
            tmp->pathData = new PathFindData(dist, nullptr);

            auto pos = std::upper_bound(
                queue.begin(), queue.end(), tmp, [](auto a, auto b) {
                  return a->pathData->accDist < b->pathData->accDist;
                });
            auto actPos =
                std::find(queue.begin(), queue.end(),
                          otherNode); // TODO: replace queue.begin() with pos
            std::rotate(pos, actPos, actPos + 1); // TODO: use reverse iterator

            otherNode->pathData->accDist = dist;
            otherNode->pathData->prev = iedge;

            delete tmp->pathData;
            delete tmp;
          }
        }
      }
      queue.pop_front();
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

  ~Graph() { this->clear(); }


};
