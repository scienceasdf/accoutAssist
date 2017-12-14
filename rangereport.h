#ifndef RANGEREPORT_H
#define RANGEREPORT_H

#include <QWidget>
#include <QDate>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts>

#include <vector>

#include "customgesture.h"

QT_CHARTS_USE_NAMESPACE

enum rangeType{week,month,quarter,year,customize};

namespace Ui {
class rangeReport;
}

class rangeReport : public QWidget
{
    Q_OBJECT

public:
    explicit rangeReport(QWidget *parent = 0);

    rangeReport(rangeType type);

    rangeReport(QDate begin, QDate end);

    ~rangeReport();

private:
    Ui::rangeReport *ui;

    rangeType m_rangeType;

    void plotWeek();
    void plotMonth();
    void plotQuarter();
    void plotYear();

    std::vector<QChart*> chart;

    SwipeGestureRecognizer *fftRecognizer;
    Qt::GestureType fftType;

    void swipeTriggered(QSwipeGesture *gesture);

    bool event(QEvent *event);

    bool gestureEvent(QGestureEvent *event);

    int currentGraph;
    void registerGesture();
};

#endif // RANGEREPORT_H
