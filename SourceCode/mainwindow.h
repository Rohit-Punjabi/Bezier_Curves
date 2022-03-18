#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void PascalsTriangleSetter();

    void ShowControlPoints(int index);

    void UpdatePoints();

    double DistanceCalculator(double x1, double x2, double y1, double y2);

    void PlottingValues();

    float BernsteinY(float t, QVector<QCPCurveData>);

    float BernsteinX(float t, QVector<QCPCurveData>);

    //float NLI(float t, QVector<double> Ypoints);

    QCPCurveData NLI(float t, QVector<QCPCurveData>);

    void PLottingShells(int value);

    void MidPointSubdivision();

private slots:
    //void on_DegreeValue_currentIndexChanged(int index);

    //void mousePressEvent(QMouseEvent *e);

    void MouseMovementDetection(QMouseEvent *event);
    void ShowControlPointsOnClick(QMouseEvent* event);

    void on_TSlider_valueChanged(int value);

    void on_ResetButton_clicked();

    void on_CalculationMethod_currentIndexChanged(int index);

    void on_showShellOption_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;

    QCPCurve *curvePlotting;
    QCPCurve *pointsPlotting;
    QCPCurve *shellPlotting;

    QVector<QCPCurve*> shellCurves;
    QVector<QCPCurveData> curvePoints;
    QVector<QCPCurveData> dragPoints;
    QVector<QVector<QCPCurveData>> shellPoints;

    QVector<QVector<int>> PascalsTiangle;

    float Deg;
    int PointCounter = 0;
};
#endif // MAINWINDOW_H
