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
#include <QVector>
#include <QMouseEvent>
#include <QPointF>


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

    //pan veriable
    double displayRange = 100;
    double displayRangeY = 10;
    double mouseWheelStep = 3.0;
    bool mouseDown = false;
    bool ctrlKeyDown =  false;
    QPoint lastPoint;
    double difx;
    double dify;
    double lower_xAxis,upper_xAxis,lower_yAxis,upper_yAxis;
    double min_xAxis, max_xAxis;
    void displayRangeSet(double range);
    void displayRangeSetY(double range);

public slots:
    void ShowContextMenu(const QPoint &pos);
private:
    Ui::MainWindow *ui;
protected:
    bool eventFilter(QObject *obj, QEvent *ev);

protected slots:
private slots:
    void myContextMenuSlot1();
    void myContextMenuSlot2();
};

#endif // MAINWINDOW_H
