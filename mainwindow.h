#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtGui>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void plotGraph();
private:
    Ui::MainWindow *ui;

    QGraphicsItem *item;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
