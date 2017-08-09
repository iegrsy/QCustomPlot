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

    bool mouseDown = false;
    bool ctrlKeyDown =  false;


    QCPItemText *textItem;
    QVector<QCPGraphData> randomDataCreate();

    void mouseClickData(QMouseEvent *event, QObject *obj);
public slots:
private:
    Ui::MainWindow *ui;
protected:
    bool eventFilter(QObject *obj, QEvent *ev);
protected slots:
private slots:
    void ShowContextMenu(const QPoint &pos);
    void myContextMenuSlot1();
    void myContextMenuSlot2();
    void onMouseMove(QMouseEvent *event);
};

#endif // MAINWINDOW_H
