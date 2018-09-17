#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsPathItem>

namespace Ui {
class MainWindow;
}

class SceneWidget;
class GraphNodeScene;
class NodeGraph;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setNodeGraph(NodeGraph* graph);
    void newNodeGraph();

    ~MainWindow();

private:
    void saveGraph();
    void loadGraph();
    void restoreGraphPositions(const QJsonObject& data);

    GraphNodeScene* createNewScene();

private:
    Ui::MainWindow *ui;
    GraphNodeScene* scene;
    SceneWidget* sceneWidget;
};
#endif // MAINWINDOW_H
