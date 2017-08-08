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


    plotGraphData(randState);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::plotGraph()
{
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

    ui->customPlot->xAxis->setRange(0, 10);
    ui->customPlot->yAxis->setRange(0, 1);
}

void MainWindow::plotGraphData(QVector<QCPGraphData> dataList)
{
    ui->customPlot->setLocale(QLocale(QLocale::Turkish, QLocale::Turkey));

    ui->customPlot->addGraph();
    QColor color(150, 255, 150, 150);
    ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    ui->customPlot->graph()->setPen(QPen(color.lighter(200)));
    ui->customPlot->graph()->setBrush(QBrush(color));

    ui->customPlot->graph()->data()->set(dataList);

    min_xAxis = dataList.first().key;
    max_xAxis = dataList.last().key;

    lower_xAxis = min_xAxis;
    upper_xAxis = lower_xAxis + displayRange;

    lower_yAxis = 0.0;
    upper_yAxis = displayRangeY;

    ui->customPlot->xAxis->setRange(lower_xAxis , upper_xAxis);
    ui->customPlot->yAxis->setRange(lower_yAxis , upper_yAxis);

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

            return true;
        }
        else if (event->type() == QEvent::KeyRelease)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            if(keyEvent->key() == Qt::Key_Control)
                ctrlKeyDown = false;

            return true;
        }

        if ( event->type() == QEvent::Wheel )
        {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
            int zoom = wheelEvent->delta();

            if (zoom > 0)
            {
                if(ctrlKeyDown)
                {
                    displayRangeSetY(mouseWheelStep);
                    ui->customPlot->yAxis->setRange(lower_yAxis, upper_yAxis );
                    ui->customPlot->replot();
                }
                else
                {
                    displayRangeSet(mouseWheelStep);
                    ui->customPlot->xAxis->setRange(lower_xAxis, upper_xAxis );
                    ui->customPlot->replot();
                }
            }
            else
            {
                if(ctrlKeyDown)
                {
                    displayRangeSetY(-mouseWheelStep);
                    ui->customPlot->yAxis->setRange(lower_yAxis, upper_yAxis );
                    ui->customPlot->replot();
                }
                else
                {
                    displayRangeSet(-mouseWheelStep);
                    ui->customPlot->xAxis->setRange(lower_xAxis, upper_xAxis );
                    ui->customPlot->replot();
                }
            }
            return true;
        }

        //Capture mouse event
        if((event->type() == QEvent::MouseButtonPress) && Qt::LeftButton)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            lastPoint = QPoint(mouseEvent->globalX(),mouseEvent->globalY());
            mouseDown = true;

            return true;
        }
        else if ( event->type() == QEvent::MouseButtonRelease)
        {
            //QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            lower_xAxis = lower_xAxis + difx;
            upper_xAxis = upper_xAxis + difx;

            mouseDown = false;

            return true;
        }
        else if ( event->type() == QEvent::MouseMove )
        {
            if(mouseDown == true)
            {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

                difx = (lastPoint.x() - mouseEvent->globalX())/5;

                ui->customPlot->xAxis->setRange(lower_xAxis + difx, upper_xAxis + difx);
                ui->customPlot->replot();

                return true;
            }
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

void MainWindow::displayRangeSet(double range)
{
    displayRange += 2*range;
    lower_xAxis -= range;
    upper_xAxis = lower_xAxis + displayRange;
}

void MainWindow::displayRangeSetY(double range)
{
    displayRangeY += 2*range;
    lower_yAxis -= range;
    upper_yAxis = lower_xAxis + displayRange;
}
