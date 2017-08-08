#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->customPlot->installEventFilter(this);
    ui->frame_3->installEventFilter(this);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ShowContextMenu(const QPoint &)));

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


    setMouseTracking(true);
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

    min_xAxis = dataList.first().key;
    max_xAxis = dataList.last().key;

    lower_xAxis = min_xAxis;
    upper_xAxis = lower_xAxis + displayRange;

    lower_yAxis = 0.0;
    upper_yAxis = displayRangeY;

    ui->customPlot->xAxis->setRange(lower_xAxis , upper_xAxis);
    ui->customPlot->yAxis->setRange(lower_yAxis , upper_yAxis);

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 150));
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignBottom);
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

        //Capture wheel event
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

            lower_yAxis = lower_yAxis + dify;
            upper_yAxis = upper_yAxis + dify;

            mouseDown = false;

            return true;
        }
        else if ( event->type() == QEvent::MouseMove )
        {
            if(mouseDown == true)
            {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

                difx = (lastPoint.x() - mouseEvent->globalX())/5;
                dify = (lastPoint.y() - mouseEvent->globalY());

                ui->customPlot->xAxis->setRange(lower_xAxis + difx, upper_xAxis + difx);
                ui->customPlot->yAxis->setRange(lower_yAxis + dify, upper_yAxis + dify);
                ui->customPlot->replot();

                return true;
            }
        }
        else if ( event->type() == QEvent::MouseButtonDblClick)
        {
            qDebug()<<"laaaaan";
            return true;
        }
        else if (event->type() == QEvent::MouseButtonPress && Qt::RightButton)
        {
            qDebug()<<"hadi ama";
            return true;
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

            qDebug()<<"burası frame0";
            return true;
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

void MainWindow::ShowContextMenu(const QPoint &pos)
{
   QMenu contextMenu(tr("Context menu"), this);

   QAction action1("item1", this);
   QAction action2("item2", this);

   connect(&action1, SIGNAL(triggered()), this, SLOT(myContextMenuSlot1()));
   connect(&action2, SIGNAL(triggered()), this, SLOT(myContextMenuSlot2()));

   contextMenu.addAction(&action1);
   contextMenu.addAction(&action2);

   contextMenu.exec(mapToGlobal(pos));
}

void MainWindow::myContextMenuSlot1()
{
    qDebug()<<"slot1 trig";
}

void MainWindow::myContextMenuSlot2()
{
    qDebug()<<"slot2 trig";
}







