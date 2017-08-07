#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QImage image("/home/ieg/Resimler/images1.jpg");
    item= new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene= new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->addItem(item);


    plotGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::plotGraph()
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
        QVector<QCPGraphData> timeData(250);
        for (int i=0; i<250; ++i)
        {
            timeData[i].key = now + 24*3600*i;
            if (i == 0)
                timeData[i].value = (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);
            else
            {
                qDebug()<<rand()%2;
                //            timeData[i].value = qFabs(timeData[i-1].value)*(1+0.02/4.0*(4-gi)) + (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);
                timeData[i].value = rand()%2;
            }
        }
        ui->customPlot->graph()->data()->set(timeData);
    }
    // configure bottom axis to show date instead of number:
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("d. MMMM\nyyyy");
    ui->customPlot->xAxis->setTicker(dateTicker);
    // configure left axis text labels:
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTick(10, "a bit\nlow");
    textTicker->addTick(25, "my\nticker");
    textTicker->addTick(50, "quite\nhigh");
    ui->customPlot->yAxis->setTicker(textTicker);
    // set a more compact font size for bottom and left axis tick labels:
    ui->customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    // set axis labels:
    ui->customPlot->xAxis->setLabel("Date");
    ui->customPlot->yAxis->setLabel("Random wobbly lines value");
    // make top and right axes visible but without ticks and labels:
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTicks(false);
    ui->customPlot->yAxis2->setTicks(false);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setTickLabels(false);
    // set axis ranges to show all data:
    ui->customPlot->xAxis->setRange(now+24*3600*200, now+24*3600*249);
    ui->customPlot->yAxis->setRange(-3, 2);
    // show legend with slightly transparent background brush:
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 150));
}
