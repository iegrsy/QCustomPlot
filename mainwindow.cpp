#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTracking(true);

    ui->customPlot->installEventFilter(this);

    textItem = new QCPItemText(ui->customPlot);
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->customPlot, &QCustomPlot::mouseMove, this, &MainWindow::onMouseMove);

    plotGraphData(randomDataCreate());
    //    plotGraph();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::plotGraph()
{
    // add two new graphs and set their look:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph

    // generate some points of data (y0 for first, y1 for second graph):
    QVector<double> x(251), y0(251), y1(251);
    for (int i=0; i<251; ++i)
    {
        x[i] = i;
        y0[i] = qExp(-i/150.0)*qCos(i/10.0); // exponentially decaying cosine
        y1[i] = qExp(-i/150.0);              // exponential envelope
    }

    // configure right and top axis to show ticks but no labels:
    // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->yAxis2->setTickLabels(false);

    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    //    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // pass data points to graphs:
    ui->customPlot->graph(0)->setData(x, y0);
    ui->customPlot->graph(1)->setData(x, y1);

    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    ui->customPlot->graph(0)->rescaleAxes();

    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    ui->customPlot->graph(1)->rescaleAxes(true);

    // Note: we could have also just called ui->customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 150));

}

void MainWindow::plotGraphData(QVector<QCPGraphData> dataList)
{
    ui->customPlot->setLocale(QLocale(QLocale::Turkish, QLocale::Turkey));

    ui->customPlot->addGraph();
    QColor color(150, 255, 150, 150);
    ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    ui->customPlot->graph()->setPen(QPen(color.lighter(100)));
    ui->customPlot->graph()->setBrush(QBrush(color));
    ui->customPlot->graph()->data()->set(dataList);
    ui->customPlot->graph()->setName("Data");

    QSharedPointer<QCPAxisTicker> stateTicker(new QCPAxisTicker);
    ui->customPlot->xAxis->setTicker(stateTicker);
    ui->customPlot->yAxis->setTicker(stateTicker);

    ui->customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot->xAxis->setLabel("Value");
    ui->customPlot->yAxis->setLabel("Count");

    ui->customPlot->graph(0)->rescaleAxes();

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 150));
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iSelectItems);

}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->customPlot)
    {
        //Capture key event
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if(keyEvent->key() == Qt::Key_Control)
                ctrlKeyDown = true;
            return false;
        }
        else if (event->type() == QEvent::KeyRelease)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if(keyEvent->key() == Qt::Key_Control)
                ctrlKeyDown = false;
            return false;
        }

        //Capture wheel event
        if ( event->type() == QEvent::Wheel )
        {
            if(ctrlKeyDown)
            {
                ui->customPlot->axisRect()->setRangeZoomAxes(ui->customPlot->xAxis,ui->customPlot->yAxis);
                ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
            }
            else
            {
                ui->customPlot->axisRect()->setRangeZoomAxes(ui->customPlot->xAxis,ui->customPlot->yAxis);
                ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis->orientation());
            }
            return false;
        }

        //Capture mouse event
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            if(mouseEvent->button() == Qt::LeftButton)
            {
                mouseDown = true;
                mouseClickData(mouseEvent, obj);
            }
            else if (mouseEvent->button() == Qt::RightButton)
            {
                ShowContextMenu(mouseEvent->globalPos());
            }


            return false;
        }
        else if ( event->type() == QEvent::MouseButtonDblClick)
        {
            qDebug()<<"laaaaan";
            return false;
        }
        else
        {
            return false;
        }
    }
    else if (obj == ui->frame_3)
    {
        //Capture mouse event
        if((event->type() == QEvent::MouseButtonPress) && Qt::LeftButton)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            qDebug()<<mouseEvent;
            return true;
        }
    }
    else
    {
        // pass the event on to the parent class
        return QMainWindow::eventFilter(obj, event);
    }
}

void MainWindow::ShowContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("item1", this);
    QAction action2("item2", this);

    connect(&action1, SIGNAL(triggered()), this, SLOT(myContextMenuSlot1()));
    connect(&action2, SIGNAL(triggered()), this, SLOT(myContextMenuSlot2()));

    contextMenu.addAction(&action1);
    contextMenu.addAction(&action2);

    contextMenu.exec(pos);
}

void MainWindow::myContextMenuSlot1()
{
    qDebug()<<"slot1 trig";
}

void MainWindow::myContextMenuSlot2()
{
    qDebug()<<"slot2 trig";
}

void MainWindow::onMouseMove(QMouseEvent *event)
{
    QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(sender());
    double x = customPlot->xAxis->pixelToCoord(event->pos().x());
    double y = customPlot->yAxis->pixelToCoord(event->pos().y());
    textItem->setText(QString("(%1, %2)\n").arg((int)x).arg((int)y));
    textItem->position->setCoords(QPointF(x, (y)));
    textItem->setFont(QFont(font().family(), 10));
    customPlot->replot();
}

QVector<QCPGraphData> MainWindow::randomDataCreate(){
    QVector<QCPGraphData> randState;
    for (int var = 0; var < 10000; ++var) {
        if( (50 < var && var < 130) || (170 < var && var < 230) )
        {
            randState.append(QCPGraphData(var,50 + rand()%5));
        }
        else
        {
            randState.append(QCPGraphData(var,5 + rand()%3));
        }
    }
    return randState;
}



void MainWindow::mouseClickData(QMouseEvent *event, QObject *obj)
{
    QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(obj);
    double x = customPlot->xAxis->pixelToCoord(event->pos().x());
    double dataValue = customPlot->plottable()->interface1D()->dataMainValue(x);

    ui->lblData->setText(QString::number(dataValue));
    ui->customPlot->setMouseTracking(true);
}












