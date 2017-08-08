#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->customPlot->installEventFilter(this);
    setMouseTracking(true);

    QVector<QCPGraphData> randState;
    for (int var = 0; var < 250; ++var) {
        if( (50 < var && var < 130) || (170 < var && var < 230) )
        {
            randState.append(QCPGraphData(var,var*2));
        }
        else
        {
            randState.append(QCPGraphData(var,0));
        }
    }

    //    plotGraph();
    plotGraphData(randState);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::plotGraph()
{
    min_xAxis=0,max_xAxis=500,min_yAxis=0,max_yAxis=10;

    ui->customPlot->setLocale(QLocale(QLocale::Turkish, QLocale::Turkey));

    ui->customPlot->addGraph();
    QColor color(255,150, 150, 150);
    ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    ui->customPlot->graph()->setPen(QPen(color.lighter(200)));
    ui->customPlot->graph()->setBrush(QBrush(color));

    QSharedPointer<QCPAxisTicker> stateTicker(new QCPAxisTicker);
    ui->customPlot->xAxis->setTicker(stateTicker);
    ui->customPlot->yAxis->setTicker(stateTicker);

    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTicks(false);
    ui->customPlot->yAxis2->setTicks(false);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setTickLabels(false);

    //    ui->customPlot->xAxis->setRange(0, 10);
    //    ui->customPlot->yAxis->setRange(0, 1);
}

void MainWindow::plotGraphData(QVector<QCPGraphData> dataList)
{
    min_xAxis=0,max_xAxis=500,min_yAxis=0,max_yAxis=10;

    ui->customPlot->addGraph();
    QColor color(150, 255, 150, 150);
    ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    ui->customPlot->graph()->setPen(QPen(color.lighter(200)));
    ui->customPlot->graph()->setBrush(QBrush(color));

    ui->customPlot->graph()->data()->set(dataList);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->customPlot)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            qDebug() << "Ate key press" << keyEvent->key();
            return true;
        }
        else if((event->type() == QEvent::MouseButtonPress) && Qt::LeftButton)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            startPoint = QPoint(mouseEvent->x(),mouseEvent->y());
            qDebug()<< "mouse start point: " << startPoint;
            mouseDown = true;
            return true;
        }
        else if ( event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            stopPoint = QPoint(mouseEvent->x(),mouseEvent->y());
            qDebug()<< "mouse release: " << stopPoint;
            mouseDown = false;
            //            startPoint = Po;
            return true;
        }
        else if ( event->type() == QEvent::MouseMove )
        {
            if(mouseDown == true)
            {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
                stopPoint = QPoint(mouseEvent->x(),mouseEvent->y());
                difx = stopPoint.x()-startPoint.x();
                ui->customPlot->xAxis->setRange(min_xAxis - difx, max_xAxis - difx);
                ui->customPlot->replot();
            }
        }
        else if ( event->type() == QEvent::Wheel )
        {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
            int zoom = wheelEvent->delta();

            if (zoom > 0){
                if(max_yAxis/1.3 >1)
                    max_yAxis = max_yAxis/1.3;

                ui->customPlot->yAxis->setRange(min_yAxis, max_yAxis);
                ui->customPlot->replot();
            }
            else
            {
                if(max_yAxis>1)
                    max_yAxis = max_yAxis*1.3;

                ui->customPlot->yAxis->setRange(min_yAxis, max_yAxis);
                ui->customPlot->replot();
            }

            qDebug()<< "mouse wheel: " << zoom <<":"<<max_yAxis;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // pass the event on to the parent class
        return QMainWindow::eventFilter(obj, event);
    }
}


