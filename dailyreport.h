#ifndef DAILYREPORT_H
#define DAILYREPORT_H

#include <QWidget>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class dailyReport;
}

class dailyReport : public QWidget
{
    Q_OBJECT

public:
    explicit dailyReport(QWidget *parent = 0);
    ~dailyReport();

private slots:
    void on_calendarWidget_clicked(const QDate &date);

private:
    Ui::dailyReport *ui;
    void updateGraph();

    QChart* chart;
    QChart* chart2;
};

#endif // DAILYREPORT_H
