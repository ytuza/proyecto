#include "graphicEdge.hpp"
#include <QPen>

GraphicEdge::GraphicEdge(CoordGraph::EdgeType *data, QGraphicsItem *parent)
    : QGraphicsLineItem(
          QLineF(data->conNodes.first->value, data->conNodes.second->value),
          parent) {
  this->data = data;
}

void GraphicEdge::setColor(const QColor &color) {
  QPen tmpPen = this->pen();
  tmpPen.setColor(color);
  this->setPen(tmpPen);
}
