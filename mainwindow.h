#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
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

    void plotGraph(QList<int> dataList);
private:
    Ui::MainWindow *ui;
protected:
    bool eventFilter(QObject *obj, QEvent *ev);

};

#endif // MAINWINDOW_H
