#include "mainWindow.hpp"
#include "graphScene.hpp"
#include <QGraphicsView>
#include <QHBoxLayout>

MainWindow::MainWindow() {
  scene = new GraphScene(this);
  scene->setSceneRect(
      QRectF(0, 0, 1000,
             1000)); // TODO: Adjust this values acording to the graph limits.
  scene->randomizeGraph(200, 3, 50, 1, 500);

  QHBoxLayout *layout = new QHBoxLayout;
  view = new QGraphicsView(scene);
  view->setRenderHint(QPainter::Antialiasing);
  layout->addWidget(view);
  QWidget *widget = new QWidget;
  widget->setLayout(layout);
  this->setCentralWidget(widget);
  this->setWindowTitle("Simple Graph");
}
