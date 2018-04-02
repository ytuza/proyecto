#pragma once

#include <QGraphicsScene>

class CoordGraph;
class GraphicNode;

class GraphScene : public QGraphicsScene {
  Q_OBJECT

private:
  CoordGraph *graph;
  QGraphicsRectItem *selectRect;
  QPointF selectFirstPoint;
  GraphicNode *selecNodes[2];

  void removeSelected(QRectF rect);

public:
  explicit GraphScene(QObject *parent = 0);

  void showGraph();
  void randomizeGraph(int nodesN, int edgesN, double distance, double limitA,
                      double limitB);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
};
