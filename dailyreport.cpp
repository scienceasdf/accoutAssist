#include "dailyreport.h"
#include "ui_dailyreport.h"

#include <QDate>
#include <QtSql>

QT_CHARTS_USE_NAMESPACE

dailyReport::dailyReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dailyReport)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);


    chart = new QChart();
    chart2 = new QChart();

    updateGraph();
}

dailyReport::~dailyReport()
{
    delete ui;
    delete chart;
    delete chart2;
}

void dailyReport::on_calendarWidget_clicked(const QDate &date)
{
    updateGraph();
}

void dailyReport::updateGraph()
{
    QString date = ui->calendarWidget->selectedDate().toString("yyyy-MM-dd");
    QSqlDatabase db1 = QSqlDatabase::database("connection1");
    QSqlQuery query1(db1);

    query1.exec("select purposeType,sum(amount) as mon "
                "from accounts "
                "where date = DATE('" + date + "')"
                "group by accounts.purposeType "
                );

    QPieSeries *pieSeries = new QPieSeries();

    while(query1.next()){
        //qDebug() << query1.value(0).toString() << query1.value(1).toDouble();
        pieSeries->append(query1.value(0).toString(),query1.value(1).toDouble());
    }

    query1.exec("select sum(amount) as mon "
                "from accounts "
                "where date = DATE('" + date + "')"
                );
    query1.next();
    ui->totalAmount->setText("总消费金额为:"+ QString::number(query1.value(0).toDouble())+"元");


    chart->removeAllSeries();
    chart->addSeries(pieSeries);  // 将 series 添加至图表中
    chart->legend()->setAlignment(Qt::AlignRight);  // 设置图例靠右显示
    chart->setTitle("用途统计");

    ui->purposeGraph->setChart(chart);

    QPieSeries *pieSeries2 = new QPieSeries();

    query1.exec("select payType,sum(amount) as mon "
                "from accounts "
                "where date = DATE('" + date + "')"
                "group by accounts.payType "
                );


    while(query1.next()){
        pieSeries2->append(query1.value(0).toString(),query1.value(1).toDouble());
    }


    chart2->removeAllSeries();
    chart2->addSeries(pieSeries2);  // 将 series 添加至图表中
    chart2->legend()->setAlignment(Qt::AlignRight);  // 设置图例靠右显示
    chart2->setTitle("支付方式统计");

    ui->payGraph->setChart(chart2);

}
