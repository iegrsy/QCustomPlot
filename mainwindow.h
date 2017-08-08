#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* source link
 * http://www.qcustomplot.com/index.php/demos/datedemo
*/

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QtGui>
#include <qcustomplot.h>


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
    void plotGraphData(QVector<QCPGraphData> dataList);

    //pan zoom veriables
    double min_xAxis,max_xAxis,min_yAxis,max_yAxis;

    //mouse point vector
    QPoint startPoint,stopPoint;
    bool mouseDown = false;
    int difx;

private:
    Ui::MainWindow *ui;
protected:
    bool eventFilter(QObject *obj, QEvent *ev);

};

#endif // MAINWINDOW_H
