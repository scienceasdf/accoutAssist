#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QInputDialog>
#include <QResource>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>

static bool createConnection()
{
    QSqlDatabase db1 = QSqlDatabase::addDatabase("SQLITECIPHER","connection1");

    QString password = QInputDialog::getText(NULL, "密码输入",
                                                       "请输入密码",
                                                       QLineEdit::Normal,
                                                       "");

    qDebug()<<password;
    db1.setPassword(password);
#ifdef FOR_DESKTOP
    db1.setDatabaseName(QCoreApplication::applicationDirPath() + "/myDB.db");
#endif
#ifdef FOR_MOBILE
    db1.setDatabaseName("/storage/emulated/0/data/myDB.db");
#endif
    if(!db1.open()){
        QMessageBox::critical(0, "数据库无法打开","无法打开数据库，密码错误？ ", QMessageBox::Cancel);
        //QMessageBox::critical()
        return false;
    }

    QSqlQuery query1(db1);
    query1.exec("create table IF NOT EXISTS accounts (id integer primary key, amount real, date datetime, purposeType varchar(8), payType varchar(8), remarks varchar(60))");
    query1.exec("CREATE table IF NOT EXISTS incomes (id INTEGER PRIMARY KEY, amount REAL, date datetime, remarks VARCHAR(40))");
    query1.exec("create view IF NOT EXISTS accountsMonthly AS "
                "select date(accounts.date,'start of month') as month1,"
                "sum(accounts.amount) as s1 "
                "from accounts "
                "group by month1");


    query1.exec("create view IF NOT EXISTS incomessMonthly AS "
                "select date(incomes.date,'start of month') as month1, "
                "sum(incomes.amount) as s1 "
                "from incomes "
                "group by month1");

    query1.exec("create view IF NOT EXISTS allMonths AS "
                "select accountsMonthly.month1 from accountsMonthly "
                "union "
                "select incomessMonthly.month1 from incomessMonthly");
    // For the initial version, I didn't set a primary key

    /*select *
from accounts
where julianday(date) > julianday("2017-12-01")*/

    return true;
}

/* some useful sql codes
 * ---------------------------------------------
 * select  date(DateColumn, 'start of month')
    ,  sum(TransactionValueColumn)
       from    YourTable
       group by
       date(DateColumn, 'start of month')
--------------------------------------------------
select SUM(amount) as Price,
strftime("%m-%Y", date) as 'month-year'
from accounts
group by strftime("%m-%Y", date);
----------------------------------------
THIS IS FOR GROUPING BY WEEKS, ASSUMING THAT THE FIRST DAY OF A WEEK IS MONDAY:

select  date(date,  'weekday 0', '-6 days')
,       sum(amount)
from    accounts
group by
date(date, 'weekday 0', '-6 days')

------------------------------------
GROUP BY QUARTER
select round(strftime ('%m',accounts.date) / 3.0 + 0.495)
from accounts
*/


#endif // CONNECTION_H
