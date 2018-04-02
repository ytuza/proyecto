#include "graphicNode.hpp"
#include "graphicEdge.hpp"
#include <QBrush>
#include <QGraphicsScene>
#include <QStringBuilder>

GraphicNode::GraphicNode(CoordGraph::NodeType *data, qreal radius,
                         QGraphicsItem *parent)
    : QGraphicsEllipseItem(data->value.x() - radius, data->value.y() - radius,
                           radius * 2, radius * 2, parent) {
  this->data = data;
  this->setPen(QPen(Qt::black, 1));
  this->setBrush(Qt::red); // TODO: find a good color;
  this->setFlag(QGraphicsItem::ItemIsSelectable, true);
  this->setToolTip(QString::number(data->value.x()) % ", " %
                   QString::number(data->value.y()));
}

void GraphicNode::removeEdges() {
  for (auto &i : data->edgeList) {
    if (i->graphics != nullptr) {
      scene()->removeItem(i->graphics);
      delete i->graphics;
      i->graphics = nullptr;
    }
  }
}

void GraphicNode::setColor(const QColor &color) { this->setBrush(color); }
