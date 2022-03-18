#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    ui->GraphContainer->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

    ui->GraphContainer->background().fill(Qt::GlobalColor::red);
    ui->GraphContainer->addGraph();
    ui->GraphContainer->addGraph();

    curvePlotting = new QCPCurve(ui->GraphContainer->xAxis, ui->GraphContainer->yAxis);
    pointsPlotting = new QCPCurve(ui->GraphContainer->xAxis, ui->GraphContainer->yAxis);
    shellPlotting = new QCPCurve(ui->GraphContainer->xAxis, ui->GraphContainer->yAxis);

    pointsPlotting->setScatterStyle(QCPScatterStyle::ssDisc);
    pointsPlotting->setPen(QPen(Qt::black));

    curvePlotting->setPen(QPen(Qt::blue));

    shellPlotting->setScatterStyle(QCPScatterStyle::ssDisc);
    shellPlotting->setPen(QPen(Qt::red));


    ui->GraphContainer->setMouseTracking(false);

    connect(ui->GraphContainer, SIGNAL(mouseMove(QMouseEvent*)), SLOT(MouseMovementDetection(QMouseEvent*)));
    connect(ui->GraphContainer, SIGNAL(mousePress(QMouseEvent*)), SLOT(ShowControlPointsOnClick(QMouseEvent*)));

    shellPoints.resize(51);

    PascalsTriangleSetter();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::PascalsTriangleSetter()
{
    PascalsTiangle.resize(51);

    for (int i = 0; i < 51; i++){PascalsTiangle[i].resize(51);}

    for(int i = 0; i < 51; i++){for(int j = 0; j < 51; j++){PascalsTiangle[i][j] = 0;}}

    PascalsTiangle[0][0] = 1;

    for(int i = 1; i < 51; i++)
    {
        PascalsTiangle[i][0] = 1;
        for(int j = 1; j < i + 1; j++)
        {
            PascalsTiangle[i][j] = PascalsTiangle[i-1][j-1] + PascalsTiangle[i-1][j];
        }
    }
}

void MainWindow::ShowControlPointsOnClick(QMouseEvent* event)
{
    ++PointCounter;
    if (event->button() == Qt::LeftButton)
    {
        float dragPointX = ui->GraphContainer->xAxis->pixelToCoord(event->pos().x());
        float dragPointY = ui->GraphContainer->yAxis->pixelToCoord(event->pos().y());

        dragPoints.push_back(QCPCurveData(PointCounter, dragPointX, dragPointY));

        if (dragPoints.count() > 2)
        {
            shellCurves.push_back(new QCPCurve(ui->GraphContainer->xAxis, ui->GraphContainer->yAxis));
        }

        UpdatePoints();
    }
}

double MainWindow::DistanceCalculator(double x1, double x2, double y1, double y2)
{
    double d = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
    return d;
}

void MainWindow::MouseMovementDetection(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton)
    {
        for (int i = 0; i < dragPoints.count(); i++)
        {
            if ((abs(dragPoints[i].key - ui->GraphContainer->xAxis->pixelToCoord(event->pos().x()))  < 0.2)
                && (abs(dragPoints[i].value - ui->GraphContainer->yAxis->pixelToCoord(event->pos().y()))  < 0.2))
            {
                dragPoints[i].value = ui->GraphContainer->yAxis->pixelToCoord(event->pos().y());
                dragPoints[i].key = ui->GraphContainer->xAxis->pixelToCoord(event->pos().x());
                UpdatePoints();

            }
        }
    }
}

void MainWindow::PlottingValues()
{
    if (dragPoints.count() == 0)
    {
        return;
    }
    int calcValue = ui->CalculationMethod->currentIndex();

    double count = 0;

    if (calcValue == 2)
    {
        MidPointSubdivision();
    }
    else
    {
        for (int i = 0; i < 101; i++)
        {
            float t = (float)i / 100;

            if (calcValue == 0)
            {
                //float BernValueY = BernsteinY(t, YcirclePoint);
                float BernValueY = BernsteinY(t, dragPoints);
                //YgraphPoints[i] = BernValueY;

                float BernValueX = BernsteinX(t, dragPoints);
                //XgraphPoints[i] = BernValueX;

                curvePoints.push_back(QCPCurveData(count, BernValueX, BernValueY));
            }
            else if (calcValue == 1)
            {
                float NLIValueX = NLI(t, dragPoints).key;
                float NLIValueY = NLI(t, dragPoints).value;

                curvePoints.push_back(QCPCurveData(count, NLIValueX, NLIValueY));
            }
            ++count;
        }
    }
}

float MainWindow::BernsteinY(float t, QVector<QCPCurveData> dragdragPoints)
{
    float sumY  = 0;
    for (int i = 0; i < dragdragPoints.count(); i++)
    {
       sumY += dragdragPoints[i].value * PascalsTiangle[dragdragPoints.count() - 1][i] * pow(1 - t, dragdragPoints.count() - 1 - i) * pow(t, i);
    }
    return sumY;
}

float MainWindow::BernsteinX(float t, QVector<QCPCurveData> dragdragPoints)
{
    float sumX  = 0;
    for (int i = 0; i < dragdragPoints.count(); i++)
    {
        sumX += dragdragPoints[i].key * PascalsTiangle[dragdragPoints.count() - 1][i] * pow(1 - t, dragdragPoints.count() - 1 - i) * pow(t, i);
    }
    return sumX;
}

QCPCurveData MainWindow::NLI(float t, QVector<QCPCurveData> dragdragPoints)
{
    QVector<QCPCurveData> temp(dragdragPoints);
    int count = temp.count();
    while(temp.count() > 1)
    {
        for(int i = 0; i < count - 1; i++)
        {
            float valueX = temp[i].key * (1 - t) + temp[i + 1].key * t;
            float valueY = temp[i].value * (1 - t) + temp[i + 1].value * t;
            temp.append(QCPCurveData(PointCounter, valueX, valueY));
        }

        for (int i = count - 1; i >= 0; i--)
        {
            temp.remove(i);
        }

        count = temp.count();
    }
    return temp[0];
}

void MainWindow::PLottingShells(int value)
{
    if (ui->CalculationMethod->currentIndex() == 1 && ui->showShellOption->isChecked())
    {
        float T = (float)value/100;
        int coonter = 0;
        int groupCounter = 0;
        QVector<QCPCurveData> temp(dragPoints);
        int count = temp.count();
        while(temp.count() > 1)
        {
            for(int i = 0; i < count - 1; i++)
            {
                float valueX = temp[i].key * (1 - T) + temp[i + 1].key * T;
                float valueY = temp[i].value * (1 - T) + temp[i + 1].value * T;
                temp.append(QCPCurveData(coonter, valueX, valueY));
                coonter++;
            }

            for (int i = count - 1; i >= 0; i--)
            {
                temp.remove(i);
            }


            shellPoints[groupCounter].append(temp);

            groupCounter++;
            count = temp.count();

        }
    }
    else
    {
        return;
    }

}

bool sortFunction(QCPCurveData x, QCPCurveData y )
{
    return x.t < y.t;
}

void MainWindow::MidPointSubdivision()
{
    if (dragPoints.count() == 1 || dragPoints.count() == 2)
    {
        return;
    }
    QVector<QCPCurveData> temp(dragPoints);
    int count = temp.count();

    while (curvePoints.count() < 100)
    {
        int orderFirst = 0;
        int orderLast = 301;

        curvePoints.clear();

        while (temp.count() > 0 )
        {
            curvePoints.push_back(QCPCurveData(orderFirst++, temp[0].key, temp[0].value));
            curvePoints.push_back(QCPCurveData(orderLast--, temp[temp.count() - 1].key, temp[temp.count() - 1].value));

            for (int i = 0; i < count - 1; i++)
            {
                float valueX = (temp[i].key/2.0) + (temp[i+1].key/2.0);
                float valueY = (temp[i].value/2.0) + (temp[i+1].value/2.0);
                temp.append(QCPCurveData(PointCounter, valueX, valueY));
            }

            for (int i = count - 1; i >= 0; i--)
            {
                temp.remove(i);
            }

            count = temp.count();
        }

        std::sort(curvePoints.begin(), curvePoints.end(), sortFunction);

       /*while (sortCounter > 0)
       {
           for (int i = 0; i < curvePoints.count() - 1; i++)
           {
               QCPCurveData buffer;

               if (curvePoints[i].t > curvePoints[i+1].t)
               {
                    buffer = curvePoints[i];
                    curvePoints[i] = curvePoints[i+1];
                    curvePoints[i+1] = buffer;
               }
           }
           sortCounter--;
       }*/

       for (int i = 0; i < curvePoints.count(); i++)
       {
           qDebug() << "In loop";
           qDebug() << curvePoints[i].t;
       }

       for (int i = 0; i < curvePoints.count(); i++)
       {
            temp.push_back(curvePoints[i]);
       }

       count = temp.count();
    }
}

void MainWindow::UpdatePoints()
{
    curvePoints.clear();
    for (int i = 0; i < 51; i++){shellPoints[i].clear();}
    PlottingValues();
    PLottingShells(ui->TSlider->value());
    curvePlotting->data()->set(curvePoints, true);
    pointsPlotting->data()->set(dragPoints, true);
    if (shellPoints.count() > 0)
    {
        for (int i = 0; i < dragPoints.count() - 2; i++)
        {
           shellCurves[i]->data()->set(shellPoints[i], true);
           shellCurves[i]->setPen(QPen(Qt::red));
           shellCurves[i]->setScatterStyle(QCPScatterStyle::ssDisc);
        }
    }

    ui->GraphContainer->replot();
    ui->GraphContainer->update();
}

void MainWindow::on_TSlider_valueChanged(int value)
{
    PLottingShells(value);
    UpdatePoints();
}

void MainWindow::on_ResetButton_clicked()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void MainWindow::on_CalculationMethod_currentIndexChanged(int index)
{
    PlottingValues();
    UpdatePoints();
}

void MainWindow::on_showShellOption_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        for (int i = 0; i < 51; i++){shellPoints[i].clear();}
        for (int i = 0; i < dragPoints.count() - 2; i++)
        {
           shellCurves[i]->data()->set(shellPoints[i], true);
           shellCurves[i]->setPen(QPen(Qt::red));
        }

        ui->GraphContainer->replot();
        ui->GraphContainer->update();
    }
    else if (arg1 == 2)
    {
        PLottingShells(ui->TSlider->value());
        UpdatePoints();
    }
}

