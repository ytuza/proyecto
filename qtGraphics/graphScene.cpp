#include "graphScene.hpp"
#include "coordGraph/coordGraph.hpp"
#include "graphicEdge.hpp"
#include "graphicNode.hpp"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <utility>

GraphScene::GraphScene(QObject *parent) : QGraphicsScene(parent) {
  graph = new CoordGraph();
  selectRect = nullptr;
  selecNodes[0] = selecNodes[1] = nullptr;
}

void GraphScene::showGraph() {
  const auto &v = graph->getNodeList();

  for (const auto &i : v) {
    GraphicNode *node = new GraphicNode(i, 3);
    i->graphics = node;
    this->addItem(node);
    for (const auto &j : i->edgeList) {
      if (!j->graphics) {
        GraphicEdge *edge = new GraphicEdge(j);
        j->graphics = edge;
        this->addItem(edge);
        edge->setZValue(-1.0);
      }
    }
  }
}

void GraphScene::randomizeGraph(int nodesN, int edgesN, double distance,
                                double limitA, double limitB) {
  if (this->items().count())
    this->clear();

  graph->randomGeneration(nodesN, edgesN, distance, limitA, limitB);
  this->showGraph();
}

void GraphScene::removeSelected(QRectF rect) {
  // TODO: IMPORTANT!!!! :
  // the function items in rectangle include all the items that intersect with
  // the rectangle. if the center of the node(circle) is not intersecting with
  // the rectangle, this will not be deleted from the base graph!!!!
  auto selectedItems = this->items(rect);
  if (selectedItems.empty())
    return;

  QList<GraphicNode *> nodes;
  for (const auto &item : selectedItems) {
    if (item->type() == GraphicNode::Type) {
      GraphicNode *gnode = qgraphicsitem_cast<GraphicNode *>(item);
      nodes.push_back(gnode);
    }
  }
  for (const auto &gnode : nodes) {
    gnode->removeEdges(); // aqui explota ?
    this->removeItem(gnode);
    delete gnode;
  }
  graph->removeNodeRange(rect.topLeft(), rect.bottomRight());
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  if (mouseEvent->button() == Qt::LeftButton) {
    // for (const auto& i : this->selectedItems()) {
    //     if (i->type() == GraphicNode::Type) {
    //         if (selecNodes[0] == nullptr)
    //             selecNodes[0] = qgraphicsitem_cast<GraphicNode*>(i);
    //         else
    //             selecNodes[1] = qgraphicsitem_cast<GraphicNode*>(i);
    //     }
    // }

    // TODO: Move all this into a function;
    // TODO: update the colors of edges and nodes autommatically when
    // other mouse button is pressed.
    // TODO: remove qDebug() and draw the path lenght in the scene;
    QList<QGraphicsItem *> itemsList = items(mouseEvent->scenePos());
    if (!itemsList.empty()) {
      QGraphicsItem *item = itemsList.front();
      if (item and item->type() == GraphicNode::Type) {
        GraphicNode *gnode = qgraphicsitem_cast<GraphicNode *>(item);
        if (selecNodes[0] == nullptr) {
          selecNodes[0] = gnode;
          gnode->setColor(Qt::green);
          this->update();
        } else {
          selecNodes[1] = gnode;
          gnode->setColor(Qt::green);
          this->update();
        }
      }
      if (selecNodes[0] and selecNodes[1]) {
        auto res = graph->TreeA(selecNodes[0]->getData(),
                                          selecNodes[1]->getData());
        qDebug() << res.first << endl;
        for (const auto &i : res.second) {
          i->graphics->setColor(Qt::green);
        }
        this->update();
        // selecNodes[0]->setColor(Qt::red);
        // selecNodes[1]->setColor(Qt::red);
        // for (const auto& i : res.second) {
        //     i->graphics->setColor(Qt::black);
        // }
        selecNodes[0] = selecNodes[1] = nullptr;
      }
    }

  } else if (mouseEvent->button() == Qt::RightButton) {
    selectFirstPoint = mouseEvent->scenePos();
    selectRect =
        new QGraphicsRectItem(QRectF(selectFirstPoint, selectFirstPoint));
    selectRect->setPen(QPen(Qt::black, 1, Qt::DashLine));
    this->addItem(selectRect);
  }

  QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  // TODO: When modes are implemented, comprobe the mode
  if (selectRect != nullptr) {
    QPointF a = selectFirstPoint, b = mouseEvent->scenePos();
    QRectF tmpRect;

    if (a.x() <= b.x()) {
      if (a.y() <= b.y())
        tmpRect = QRectF(a, b);
      else {
        a.setX(b.x());
        b.setX(selectFirstPoint.x());
        tmpRect = QRectF(b, a);
      }
    } else {
      if (a.y() > b.y())
        tmpRect = QRectF(b, a);
      else {
        a.setX(b.x());
        b.setX(selectFirstPoint.x());
        tmpRect = QRectF(a, b);
      }
    }
    selectRect->setRect(tmpRect);
  } else
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  if (selectRect != nullptr and mouseEvent->button() == Qt::RightButton) {
    this->removeSelected(selectRect->rect());

    this->removeItem(selectRect);
    delete selectRect;
    selectRect = nullptr;
  }
  QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
