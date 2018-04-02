#pragma once

#include "coordGraph/coordGraph.hpp"
#include <QGraphicsLineItem>

class GraphicEdge : public QGraphicsLineItem {
private:
  CoordGraph::EdgeType *data;

public:
  enum { Type = UserType + 5 };
  int type() const override { return Type; }

  GraphicEdge(CoordGraph::EdgeType *data, QGraphicsItem *parent = 0);

  void setColor(const QColor &color);

  CoordGraph::EdgeType *getData() { return data; }
};
