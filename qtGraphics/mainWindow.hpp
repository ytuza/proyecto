#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QGraphicsView;
QT_END_NAMESPACE

class GraphScene;

class MainWindow : public QMainWindow {
  Q_OBJECT

private:
  QGraphicsView *view;
  GraphScene *scene;

public:
  MainWindow();
};
