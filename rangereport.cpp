#include "rangereport.h"
#include "ui_rangereport.h"

#include <QtSql>
#include <QMessageBox>

#include <algorithm>

rangeReport::rangeReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rangeReport)
{
    ui->setupUi(this);

    ui->graph1->setMinimumWidth(ui->scrollAreaWidgetContents->width());
    qDebug()<<ui->scrollAreaWidgetContents->width();
}

rangeReport::rangeReport(rangeType type):
    ui(new Ui::rangeReport)
{
    ui->setupUi(this);
    registerGesture();

    if (type == week) plotWeek();
    if (type == month) plotMonth();
    if (type == quarter) plotQuarter();
    if (type == year) plotYear();
}


rangeReport::rangeReport(QDate begin, QDate end) :
    ui(new Ui::rangeReport)
{
    registerGesture();

    chart.resize(3);

    ui->setupUi(this);

    QString date1 = begin.toString("yyyy-MM-dd");
    QString date2 = end.toString("yyyy-MM-dd");
    int dist = begin.daysTo(end);
    QSqlDatabase db1 = QSqlDatabase::database("connection1");
    QSqlQuery query1(db1);


    if(dist < 150){
        query1.exec("SELECT purposeType,sum(amount) as mon "
                    "FROM accounts "
                    "WHERE date BETWEEN DATE('" + date1 + "')"
                    "AND DATE('" + date2 + "')"
                    "GROUP BY accounts.purposeType "
                    );

        QPieSeries *pieSeries = new QPieSeries();

        while(query1.next()){
            pieSeries->append(query1.value(0).toString(),query1.value(1).toDouble());
        }

        query1.exec("select sum(amount) as mon "
                    "from accounts "
                    "WHERE date BETWEEN DATE('" + date1 + "')"
                    "AND DATE('" + date2 + "')"
                    );
        query1.next();
        ui->totalAmount->setText("区间总支出:"+ QString::number(query1.value(0).toDouble())+"元");
        query1.exec("select sum(amount) as mon "
                    "from incomes "
                    "WHERE date BETWEEN DATE('" + date1 + "')"
                    "AND DATE('" + date2 + "')"
                    );
        query1.next();
        ui->totalIncome->setText("区间总收入:"+ QString::number(query1.value(0).toDouble())+"元");

        chart[0] = new QChart();
        chart[1] = new QChart();
        chart[2] = new QChart();

        chart[0]->addSeries(pieSeries);
        chart[0]->legend()->setAlignment(Qt::AlignRight);
        chart[0]->setTitle("用途统计");

        ui->graph1->setChart(chart[0]);
        currentGraph = 0;
        //--------------------------------------------------------------------------------
        QPieSeries *pieSeries2 = new QPieSeries();

        query1.exec("select payType,sum(amount) as mon "
                    "from accounts "
                    "WHERE date BETWEEN DATE('" + date1 + "')"
                    "AND DATE('" + date2 + "')"
                    "group by accounts.payType "
                    );


        while(query1.next()){
            pieSeries2->append(query1.value(0).toString(),query1.value(1).toDouble());
        }


        chart[1]->addSeries(pieSeries2);
        chart[1]->legend()->setAlignment(Qt::AlignRight);
        chart[1]->setTitle("支付方式统计");

        //-------------------------------------------------------
        query1.exec("select date,sum(amount) as mon "
                    "from accounts "
                    "WHERE date BETWEEN DATE('" + date1 + "')"
                    "AND DATE('" + date2 + "')"
                    "group by date"
                    );
        QLineSeries* lineSeries1 = new QLineSeries();
        QDateTime momentInTime;
        while(query1.next()){
            momentInTime.setDate(QDate::fromString(query1.value(0).toString(),"yyyy-MM-dd"));
            lineSeries1->append(momentInTime.toMSecsSinceEpoch(),query1.value(1).toDouble());
        }
        chart[2]->addSeries(lineSeries1);
        QDateTimeAxis *axisX = new QDateTimeAxis;
        //axisX->setTickCount(10);
        axisX->setFormat("MM-dd");
        axisX->setTitleText("Date");
        chart[2]->setTitle("每日支出状况");
        chart[2]->addAxis(axisX, Qt::AlignBottom);
        lineSeries1->attachAxis(axisX);
        QValueAxis *axisY = new QValueAxis;
        axisY->setLabelFormat("%i");
        axisY->setTitleText("金额");
        chart[2]->addAxis(axisY, Qt::AlignLeft);
        chart[2]->legend()->hide();
        lineSeries1->attachAxis(axisY);
    }
    else{
        QMessageBox::information(NULL, "提示", QString("区间长度大于150天，将直接区间日期所在的整个月份都进行统计，即区间精确到月份"), QMessageBox::Yes, QMessageBox::Yes);
        query1.exec("SELECT purposeType,sum(amount) as mon "
                    "FROM accounts "
                    "WHERE date(date,'start of month') BETWEEN DATE('" + date1 + "','start of month')"
                    "AND DATE('" + date2 + "','start of month')"
                    "GROUP BY accounts.purposeType "
                    );

        QPieSeries *pieSeries = new QPieSeries();

        while(query1.next()){
            pieSeries->append(query1.value(0).toString(),query1.value(1).toDouble());
        }

        query1.exec("select sum(amount) as mon "
                    "from accounts "
                    "WHERE date(date,'start of month') BETWEEN DATE('" + date1 + "','start of month')"
                    "AND DATE('" + date2 + "','start of month')"
                    );
        query1.next();
        ui->totalAmount->setText("区间总支出:"+ QString::number(query1.value(0).toDouble())+"元");
        query1.exec("select sum(amount) as mon "
                    "from incomes "
                    "WHERE date(date,'start of month') BETWEEN DATE('" + date1 + "','start of month')"
                    "AND DATE('" + date2 + "','start of month')"
                    );
        query1.next();
        ui->totalIncome->setText("区间总收入:"+ QString::number(query1.value(0).toDouble())+"元");

        chart[0] = new QChart();
        chart[1] = new QChart();
        chart[2] = new QChart();

        chart[0]->addSeries(pieSeries);
        chart[0]->legend()->setAlignment(Qt::AlignRight);
        chart[0]->setTitle("用途统计");

        ui->graph1->setChart(chart[0]);
        currentGraph = 0;
        //--------------------------------------------------------------------------------
        QPieSeries *pieSeries2 = new QPieSeries();

        query1.exec("select payType,sum(amount) as mon "
                    "from accounts "
                    "WHERE date(date,'start of month') BETWEEN DATE('" + date1 + "','start of month')"
                    "AND DATE('" + date2 + "','start of month')"
                    "group by accounts.payType "
                    );


        while(query1.next()){
            pieSeries2->append(query1.value(0).toString(),query1.value(1).toDouble());
        }


        chart[1]->addSeries(pieSeries2);
        chart[1]->legend()->setAlignment(Qt::AlignRight);
        chart[1]->setTitle("支付方式统计");

        query1.exec("select month1,CASE WHEN s1 IS NULL THEN 0 ELSE s1 END as ss from "
                    "(select * "
                    "from allMonths "
                    "left join accountsMonthly "
                    "on allMonths.month1 = accountsMonthly.month1) "
                    "WHERE month1 BETWEEN DATE('" + date1 + "','start of month')"
                    "AND DATE('" + date2 + "','start of month')"
                    "order by month1"
                    );
        QSqlQuery query2(db1);
        query2.exec("select month1,CASE WHEN s1 IS NULL THEN 0 ELSE s1 END as ss from "
                    "(select *"
                    "from allMonths "
                    "left join incomessMonthly "
                    "on allMonths.month1 = incomessMonthly.month1) "
                    "WHERE month1 BETWEEN DATE('" + date1 + "','start of month')"
                    "AND DATE('" + date2 + "','start of month')"
                    "order by month1"
                    );


        //axisX3->setLabelFormat("%d");

        QLineSeries* expensesLine = new QLineSeries();
        QLineSeries* incomeLine = new QLineSeries();
        //expensesLine->se
        while(query1.next()){
            query2.next();
            expensesLine->append(query1.value(0).toDate().month(),query1.value(1).toDouble());
            incomeLine->append(query2.value(0).toDate().month(),query2.value(1).toDouble());
        }
        expensesLine->setName("支出");
        incomeLine->setName("收入");
        chart[2]->addSeries(expensesLine);
        chart[2]->addSeries(incomeLine);
        chart[2]->createDefaultAxes();
        chart[2]->setTitle("每月收支情况");
    }

}

void rangeReport::plotWeek()
{
    chart.resize(3);

    QString date1 = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlDatabase db1 = QSqlDatabase::database("connection1");
    QSqlQuery query1(db1);

    query1.exec("SELECT purposeType,sum(amount) as mon "
                "FROM accounts "
                "WHERE date(date,'weekday 0', '-6 days') = date('"
                + date1 + "','weekday 0', '-6 days')"
                "GROUP BY accounts.purposeType "
                );

    QPieSeries *pieSeries = new QPieSeries();

    while(query1.next()){
        //qDebug() << query1.value(0).toString() << query1.value(1).toDouble();
        pieSeries->append(query1.value(0).toString(),query1.value(1).toDouble());
    }

    query1.exec("select sum(amount) as mon "
                "from accounts "
                "WHERE date(date,'weekday 0', '-6 days') = date('"
                + date1 + "','weekday 0', '-6 days')"
                );
    query1.next();
    ui->totalAmount->setText("本周总支出:"+ QString::number(query1.value(0).toDouble())+"元");

    query1.exec("select sum(amount) as mon "
                "from incomes "
                "WHERE date(date,'weekday 0', '-6 days') = date('"
                + date1 + "','weekday 0', '-6 days')"
                );
    query1.next();
    ui->totalIncome->setText("本周总收入:"+ QString::number(query1.value(0).toDouble())+"元");


    chart[0] = new QChart();
    chart[1] = new QChart();
    chart[2] = new QChart();


    chart[0]->addSeries(pieSeries);
    chart[0]->legend()->setAlignment(Qt::AlignRight);
    chart[0]->setTitle("用途统计");

    ui->graph1->setChart(chart[0]);
    currentGraph = 0;
    //--------------------------------------------------------------------------------
    QPieSeries *pieSeries2 = new QPieSeries();

    query1.exec("select payType,sum(amount) as mon "
                "from accounts "
                "WHERE date(date,'weekday 0', '-6 days') = date('"
                + date1 + "','weekday 0', '-6 days')"
                "group by accounts.payType "
                );


    while(query1.next()){
        pieSeries2->append(query1.value(0).toString(),query1.value(1).toDouble());
    }


    chart[1]->addSeries(pieSeries2);
    chart[1]->legend()->setAlignment(Qt::AlignRight);
    chart[1]->setTitle("支付方式统计");

    //-------------------------------------------------------
    query1.exec("select date,sum(amount) as mon "
                "from accounts "
                "WHERE date(date,'weekday 0', '-6 days') = date('"
                + date1 + "','weekday 0', '-6 days')"
                "group by date"
                );
    QLineSeries* lineSeries1 = new QLineSeries();
    QDateTime momentInTime;
    while(query1.next()){
        momentInTime.setDate(QDate::fromString(query1.value(0).toString(),"yyyy-MM-dd"));
        lineSeries1->append(momentInTime.toMSecsSinceEpoch(),query1.value(1).toDouble());
    }
    chart[2]->setTitle("每日支出状况");
    chart[2]->addSeries(lineSeries1);
    QDateTimeAxis *axisX = new QDateTimeAxis;
    //axisX->setTickCount(10);
    axisX->setFormat("MM-dd");
    //axisX->setTitleText("Date");
    chart[2]->addAxis(axisX, Qt::AlignBottom);
    lineSeries1->attachAxis(axisX);
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    //axisY->setTitleText("金额");
    chart[2]->addAxis(axisY, Qt::AlignLeft);
    chart[2]->legend()->hide();
    lineSeries1->attachAxis(axisY);

}

void rangeReport::plotMonth()
{
    chart.resize(3);
    QString date1 = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlDatabase db1 = QSqlDatabase::database("connection1");
    QSqlQuery query1(db1);

    query1.exec("SELECT purposeType,sum(amount) as mon "
                "FROM accounts "
                "WHERE date(date,'start of month') = date('"
                + date1 + "','start of month')"
                "GROUP BY accounts.purposeType "
                );

    QPieSeries *pieSeries = new QPieSeries();

    while(query1.next()){
        pieSeries->append(query1.value(0).toString(),query1.value(1).toDouble());
    }

    query1.exec("select sum(amount) as mon "
                "from accounts "
                "WHERE date(date,'start of month') = date('"
                + date1 + "','start of month')"
                );
    query1.next();
    ui->totalAmount->setText("本月总支出:"+ QString::number(query1.value(0).toDouble())+"元");
    query1.exec("select sum(amount) as mon "
                "from incomes "
                "WHERE date(date,'start of month') = date('"
                + date1 + "','start of month')"
                );
    query1.next();
    ui->totalIncome->setText("本月总收入:"+ QString::number(query1.value(0).toDouble())+"元");

    //chart1->removeAllSeries();
    chart[0] = new QChart();
    chart[1] = new QChart();
    chart[2] = new QChart();


    chart[0]->addSeries(pieSeries);
    chart[0]->legend()->setAlignment(Qt::AlignRight);
    chart[0]->setTitle("用途统计");

    ui->graph1->setChart(chart[0]);
    currentGraph = 0;
    //--------------------------------------------------------------------------------
    QPieSeries *pieSeries2 = new QPieSeries();

    query1.exec("select payType,sum(amount) as mon "
                "from accounts "
                "WHERE date(date,'start of month') = date('"
                + date1 + "','start of month')"
                "group by accounts.payType "
                );


    while(query1.next()){
        pieSeries2->append(query1.value(0).toString(),query1.value(1).toDouble());
    }


    //chart2->removeAllSeries();
    chart[1]->addSeries(pieSeries2);
    chart[1]->legend()->setAlignment(Qt::AlignRight);
    chart[1]->setTitle("支付方式统计");

    //ui->graph2->setChart(chart2);
    //-------------------------------------------------------
    query1.exec("select date,sum(amount) as mon "
                "from accounts "
                "WHERE date(date,'start of month') = date('"
                + date1 + "','start of month')"
                "group by date"
                );
    QLineSeries* lineSeries1 = new QLineSeries();
    QDateTime momentInTime;
    while(query1.next()){
        momentInTime.setDate(QDate::fromString(query1.value(0).toString(),"yyyy-MM-dd"));
        lineSeries1->append(momentInTime.toMSecsSinceEpoch(),query1.value(1).toDouble());
    }
    chart[2]->setTitle("每日支出状况");
    chart[2]->addSeries(lineSeries1);
    QDateTimeAxis *axisX = new QDateTimeAxis;
    //axisX->setTickCount(10);
    axisX->setFormat("MM-dd");
    //axisX->setTitleText("Date");
    chart[2]->addAxis(axisX, Qt::AlignBottom);
    lineSeries1->attachAxis(axisX);
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    //axisY->setTitleText("金额");
    chart[2]->addAxis(axisY, Qt::AlignLeft);
    chart[2]->legend()->hide();
    lineSeries1->attachAxis(axisY);
}

void rangeReport::plotQuarter()
{
    chart.resize(4);
    chart[0] = new QChart();
    chart[1] = new QChart();
    chart[2] = new QChart();
    chart[3] = new QChart();

    QString date1 = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlDatabase db1 = QSqlDatabase::database("connection1");
    QSqlQuery query1(db1);

    query1.exec("SELECT purposeType,sum(amount) as mon "
                "FROM accounts "
                "WHERE  round(strftime ('%m',date) / 3.0 + 0.495) = round(strftime ('%m','"
                + date1 + "') / 3.0 + 0.495) and date(date, 'start of year') = date('" + date1 +"','start of year')"
                "GROUP BY accounts.purposeType "
                );

    QPieSeries *pieSeries = new QPieSeries();

    while(query1.next()){
        pieSeries->append(query1.value(0).toString(),query1.value(1).toDouble());
    }

    query1.exec("select sum(amount) as mon "
                "from accounts "
                "WHERE  round(strftime ('%m',date) / 3.0 + 0.495) = round(strftime ('%m','"
                + date1 + "') / 3.0 + 0.495) and date(date, 'start of year') = date('" + date1 +"','start of year')"
                );
    query1.next();
    ui->totalAmount->setText("本季度总支出:"+ QString::number(query1.value(0).toDouble())+"元");
    query1.exec("select sum(amount) as mon "
                "from incomes "
                "WHERE  round(strftime ('%m',date) / 3.0 + 0.495) = round(strftime ('%m','"
                + date1 + "') / 3.0 + 0.495) and date(date, 'start of year') = date('" + date1 +"','start of year')"
                );
    query1.next();
    ui->totalIncome->setText("本季度总收入:"+ QString::number(query1.value(0).toDouble())+"元");

    //chart1->removeAllSeries();

    chart[0]->addSeries(pieSeries);
    chart[0]->legend()->setAlignment(Qt::AlignRight);
    chart[0]->setTitle("用途统计");

    ui->graph1->setChart(chart[0]);
    currentGraph = 0;
    //--------------------------------------------------------------------------------
    QPieSeries *pieSeries2 = new QPieSeries();

    query1.exec("select payType,sum(amount) as mon "
                "from accounts "
                "WHERE  round(strftime ('%m',date) / 3.0 + 0.495) = round(strftime ('%m','"
                + date1 + "') / 3.0 + 0.495) and date(date, 'start of year') = date('" + date1 +"','start of year')"
                "group by accounts.payType "
                );


    while(query1.next()){
        pieSeries2->append(query1.value(0).toString(),query1.value(1).toDouble());
    }


    //chart2->removeAllSeries();
    chart[1]->addSeries(pieSeries2);
    chart[1]->legend()->setAlignment(Qt::AlignRight);
    chart[1]->setTitle("支付方式统计");

    //-------------------------------------------------------
    query1.exec("select date,sum(amount) as mon "
                "from accounts "
                "WHERE  round(strftime ('%m',date) / 3.0 + 0.495) = round(strftime ('%m','"
                + date1 + "') / 3.0 + 0.495) and date(date, 'start of year') = date('" + date1 +"','start of year')"
                "group by date"
                );
    QLineSeries* lineSeries1 = new QLineSeries();
    QDateTime momentInTime;
    while(query1.next()){
        momentInTime.setDate(QDate::fromString(query1.value(0).toString(),"yyyy-MM-dd"));
        lineSeries1->append(momentInTime.toMSecsSinceEpoch(),query1.value(1).toDouble());
    }
    chart[2]->setTitle("每日支出状况");
    chart[2]->addSeries(lineSeries1);
    QDateTimeAxis *axisX = new QDateTimeAxis;
    //axisX->setTickCount(10);
    axisX->setFormat("MM-dd");
    //axisX->setTitleText("Date");
    chart[2]->addAxis(axisX, Qt::AlignBottom);
    lineSeries1->attachAxis(axisX);
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    //axisY->setTitleText("金额");
    chart[2]->addAxis(axisY, Qt::AlignLeft);
    chart[2]->legend()->hide();
    lineSeries1->attachAxis(axisY);

    //-----------------------------------------------
    query1.exec("select month1,CASE WHEN s1 IS NULL THEN 0 ELSE s1 END as ss from "
                "(select * "
                "from allMonths "
                "left join accountsMonthly "
                "on allMonths.month1 = accountsMonthly.month1) "
                "WHERE  round(strftime ('%m',month1) / 3.0 + 0.495) = round(strftime ('%m','"
                + date1 + "') / 3.0 + 0.495) and date(month1, 'start of year') = date('" + date1 +"','start of year') "
                "order by month1"
                );
    QSqlQuery query2(db1);
    query2.exec("select month1,CASE WHEN s1 IS NULL THEN 0 ELSE s1 END as ss from "
                "(select *"
                "from allMonths "
                "left join incomessMonthly "
                "on allMonths.month1 = incomessMonthly.month1) "
                "WHERE  round(strftime ('%m',month1) / 3.0 + 0.495) = round(strftime ('%m','"
                + date1 + "') / 3.0 + 0.495) and date(month1, 'start of year') = date('" + date1 +"','start of year') "
                "order by month1"
                );


    QBarSeries* barSeries1 = new QBarSeries();
    QList<QBarSet*> barSet;
    //barSet.
    int counter = 0;
    //QDateTime momentInTime;
     //ptrBarSet;


    QBarSet* ptrBarSet = new QBarSet("支出");

    QStringList categories;
    while(query1.next()){
        momentInTime.setDate(QDate::fromString(query1.value(0).toString(),"yyyy-MM-dd"));
        categories << QString::number(query1.value(0).toDate().month());
        *ptrBarSet << query1.value(1).toDouble();//<<query2.value(1).toDouble();


        ++counter;

    }
    barSet << ptrBarSet;
    ptrBarSet = new QBarSet("收入");
    while(query2.next()){
        momentInTime.setDate(QDate::fromString(query1.value(0).toString(),"yyyy-MM-dd"));

        *ptrBarSet << query2.value(1).toDouble();//<<query2.value(1).toDouble();


        ++counter;

        //barSeries1->append(barSet[counter]);
    }
    barSet << ptrBarSet;

    counter = 0;

    QBarCategoryAxis *axisX2 = new QBarCategoryAxis();
    axisX2->append(categories);
    qDebug()<<counter;
    barSeries1->append(barSet);
    chart[3]->setTitle("每月收支状况");
    chart[3]->addSeries(barSeries1);
    chart[3]->legend()->setAlignment(Qt::AlignRight);
    chart[3]->createDefaultAxes();
    chart[3]->setAxisX(axisX2,barSeries1);
}

void rangeReport::plotYear()
{
    chart.resize(4);
    chart[0] = new QChart();
    chart[1] = new QChart();
    chart[2] = new QChart();
    chart[3] = new QChart();

    QString date1 = QDate::currentDate().toString("yyyy-MM-dd");
    QSqlDatabase db1 = QSqlDatabase::database("connection1");
    QSqlQuery query1(db1);

    query1.exec("SELECT purposeType,sum(amount) as mon "
                "FROM accounts "
                "WHERE date(date, 'start of year') = date('" + date1 +"','start of year')"
                "GROUP BY accounts.purposeType "
                );

    QPieSeries *pieSeries = new QPieSeries();

    while(query1.next()){
        pieSeries->append(query1.value(0).toString(),query1.value(1).toDouble());
    }

    query1.exec("select sum(amount) as mon "
                "from accounts "
                "WHERE date(date, 'start of year') = date('" + date1 +"','start of year')"
                );
    query1.next();
    ui->totalAmount->setText("本年度总支出:"+ QString::number(query1.value(0).toDouble())+"元");
    query1.exec("select sum(amount) as mon "
                "from incomes "
                "WHERE date(date, 'start of year') = date('" + date1 +"','start of year')"
                );
    query1.next();
    ui->totalIncome->setText("本年度总收入:"+ QString::number(query1.value(0).toDouble())+"元");

    //chart1->removeAllSeries();

    chart[0]->addSeries(pieSeries);
    chart[0]->legend()->setAlignment(Qt::AlignRight);
    chart[0]->setTitle("用途统计");

    ui->graph1->setChart(chart[0]);
    currentGraph = 0;
    //--------------------------------------------------------------------------------
    QPieSeries *pieSeries2 = new QPieSeries();

    query1.exec("select payType,sum(amount) as mon "
                "from accounts "
                "WHERE date(date, 'start of year') = date('" + date1 +"','start of year')"
                "group by accounts.payType "
                );


    while(query1.next()){
        pieSeries2->append(query1.value(0).toString(),query1.value(1).toDouble());
    }


    //chart2->removeAllSeries();
    chart[1]->addSeries(pieSeries2);
    chart[1]->legend()->setAlignment(Qt::AlignRight);
    chart[1]->setTitle("支付方式统计");

    //-------------------------------------------------------



    //-----------------------------------------------
    query1.exec("select month1,CASE WHEN s1 IS NULL THEN 0 ELSE s1 END as ss from "
                "(select * "
                "from allMonths "
                "left join accountsMonthly "
                "on allMonths.month1 = accountsMonthly.month1) "
                "WHERE date(month1, 'start of year') = date('" + date1 +"','start of year')"
                "order by month1"
                );
    QSqlQuery query2(db1);
    query2.exec("select month1,CASE WHEN s1 IS NULL THEN 0 ELSE s1 END as ss from "
                "(select *"
                "from allMonths "
                "left join incomessMonthly "
                "on allMonths.month1 = incomessMonthly.month1) "
                "WHERE date(month1, 'start of year') = date('" + date1 +"','start of year')"
                "order by month1"
                );



    QValueAxis *axisX3 = new QValueAxis;
    axisX3->setRange(1, 12);
    axisX3->setTickCount(6);
    axisX3->setLabelFormat("%d");

    QLineSeries* expensesLine = new QLineSeries();
    QLineSeries* incomeLine = new QLineSeries();
    //expensesLine->se
    while(query1.next()){
        query2.next();
        expensesLine->append(query1.value(0).toDate().month(),query1.value(1).toDouble());
        incomeLine->append(query2.value(0).toDate().month(),query2.value(1).toDouble());
    }
    expensesLine->setName("支出");
    incomeLine->setName("收入");
    chart[2]->addSeries(expensesLine);
    chart[2]->addSeries(incomeLine);
    chart[2]->createDefaultAxes();
    chart[2]->setAxisX(axisX3);
    chart[2]->setTitle("每月收支情况");
    //---------------------------------------------------------------------------
    query1.exec("select round(strftime ('%m',month1) / 3.0 + 0.495) as quat ,sum(CASE WHEN s1 IS NULL THEN 0 ELSE s1 END) as ss from "
                "(select * "
                "from allMonths "
                "left join accountsMonthly "
                "on allMonths.month1 = accountsMonthly.month1) "
                "WHERE date(month1, 'start of year') = date('" + date1 +"','start of year')"
                "group by quat order by quat"
                );
    query2.exec("select round(strftime ('%m',month1) / 3.0 + 0.495) as quat ,sum(CASE WHEN s1 IS NULL THEN 0 ELSE s1 END) as ss from "
                "(select *"
                "from allMonths "
                "left join incomessMonthly "
                "on allMonths.month1 = incomessMonthly.month1) "
                "WHERE date(month1, 'start of year') = date('" + date1 +"','start of year')"
                "group by quat order by quat"
                );

    QList<QBarSet*> barSet;
    //barSet.
    int counter = 0;
    //QDateTime momentInTime;
     //ptrBarSet;


    QBarSet* ptrBarSet = new QBarSet("支出");
    QStringList categories;


    while(query1.next()){
        categories<<QString::number(query1.value(0).toInt());
        *ptrBarSet << query1.value(1).toDouble();//<<query2.value(1).toDouble();
        ++counter;
        //barSeries1->append(barSet[counter]);
    }
    barSet << ptrBarSet;
    ptrBarSet = new QBarSet("收入");
    while(query2.next()){
        *ptrBarSet << query2.value(1).toDouble();//<<query2.value(1).toDouble();

        ++counter;

        //barSeries1->append(barSet[counter]);
    }
    barSet << ptrBarSet;
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);

    counter = 0;

    QBarSeries* barSeries1 = new QBarSeries();
    barSeries1->append(barSet);
    chart[3]->setTitle("各季度收支状况");
    chart[3]->addSeries(barSeries1);
    chart[3]->legend()->setAlignment(Qt::AlignRight);
    chart[3]->createDefaultAxes();
    chart[3]->setAxisX(axisX,barSeries1);
}

rangeReport::~rangeReport()
{
    delete ui;

    std::for_each(chart.begin(),chart.end(),[](QChart* a) {delete a;});
}

bool rangeReport::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture){
        //qDebug()<<"grab!";
        return gestureEvent(static_cast<QGestureEvent*>(event));
    }
        return QWidget::event(event);
}

void rangeReport::swipeTriggered(QSwipeGesture *gesture)
{
    if (gesture->state() == Qt::GestureFinished) {
           if (gesture->horizontalDirection() == QSwipeGesture::Left
               || gesture->verticalDirection() == QSwipeGesture::Up) {
               if(currentGraph != chart.size() - 1){
                   currentGraph ++;
                   ui->graph1->setChart(chart[currentGraph]);
               }
           } else {
               if(currentGraph != 0){
                   currentGraph --;
                   ui->graph1->setChart(chart[currentGraph]);
               }                          //?ui->tabWidget->count():ui->tabWidget->currentIndex() + 1 );
           }
           update();
       }
}


bool rangeReport::gestureEvent(QGestureEvent *event)
{
    //qDebug() << "gestureEvent():" << event;
    if (QGesture *swipe = event->gesture(Qt::SwipeGesture))
        swipeTriggered(static_cast<QSwipeGesture *>(swipe));
    else if (QGesture *pan = event->gesture(Qt::PanGesture))
        //panTriggered(static_cast<QPanGesture *>(pan));
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
        //pinchTriggered(static_cast<QPinchGesture *>(pinch));
    return true;
}

void rangeReport::registerGesture()
{
    fftRecognizer = new SwipeGestureRecognizer();
    fftType = QGestureRecognizer::registerRecognizer(fftRecognizer);
    grabGesture(fftType);
}
