#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->customPlot->installEventFilter(this);
    setMouseTracking(true);

    QList<int> randState;
    for (int var = 0; var < 250; ++var) {
        if( (50 < var && var < 130) || (170 < var && var < 230) )
        {
            randState.append(1);
        }
        else
        {
            randState.append(0);
        }
    }

    plotGraph(randState);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::plotGraph(QList<int> dataList)
{

    // set locale to english, so we get english month names:
    ui->customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));


    // seconds of current time, we'll use it as starting point in time for data:
    double now = QDateTime::currentDateTime().toTime_t();


    srand(8); // set the random seed, so we always get the same random data
    // create multiple graphs:
    for (int gi=0; gi<1; ++gi)
    {
        ui->customPlot->addGraph();
        QColor color(20+200/4.0*gi,70*(1.6-gi/4.0), 150, 150);
        ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
        ui->customPlot->graph()->setPen(QPen(color.lighter(200)));
        ui->customPlot->graph()->setBrush(QBrush(color));


        // generate random walk data:
        QVector<QCPGraphData> timeData(dataList.count());
        for (int i=0; i<dataList.count(); ++i)
        {
            timeData[i].key = i;
            if (i == 0)
                timeData[i].value = 0;
            else
            {
                timeData[i].value = dataList.at(i);
            }
        }
        ui->customPlot->graph()->data()->set(timeData);
    }


    // configure bottom axis to show date instead of number:
    //    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    //    dateTicker->setDateTimeFormat("hh:ss:mm\n dd/MM/yyyy");
    //    ui->customPlot->xAxis->setTicker(dateTicker);


    // configure left axis text labels:
    QSharedPointer<QCPAxisTicker> stateTicker(new QCPAxisTicker);
    ui->customPlot->xAxis->setTicker(stateTicker);
    ui->customPlot->yAxis->setTicker(stateTicker);

    //    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    //    textTicker->addTick(10, "a bit\nlow");
    //    textTicker->addTick(25, "my\nticker");
    //    textTicker->addTick(50, "quite\nhigh");
    //    ui->customPlot->yAxis->setTicker(textTicker);


    // set a more compact font size for bottom and left axis tick labels:
    ui->customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));


    // set axis labels:
    ui->customPlot->xAxis->setLabel("Date");
    ui->customPlot->yAxis->setLabel("State");


    // make top and right axes visible but without ticks and labels:
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTicks(false);
    ui->customPlot->yAxis2->setTicks(false);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setTickLabels(false);


    // set axis ranges to show all data:
    ui->customPlot->xAxis->setRange(min_xAxis, max_xAxis);
    ui->customPlot->yAxis->setRange(min_yAxis, max_yAxis);


    // show legend with slightly transparent background brush:
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 150));
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
