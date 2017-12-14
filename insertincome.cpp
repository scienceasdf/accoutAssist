#include "insertincome.h"
#include "ui_insertincome.h"

insertIncome::insertIncome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::insertIncome)
{
    ui->setupUi(this);

    m_type = to_insert;
}

insertIncome::insertIncome(int index) : ui(new Ui::insertIncome)
{
    ui->setupUi(this);

    QSqlDatabase db1 = QSqlDatabase::database("connection1");
    QSqlQuery query1(db1);
    m_type = to_update;
    query1.exec("select amount from incomes where id = " + QString::number(index));
    query1.next();
    ui->countInput->setText(query1.value(0).toString());
    m_id = index;
}

insertIncome::~insertIncome()
{
    delete ui;
}

void insertIncome::on_OkButton_clicked()
{
    QString count = ui->countInput->text();
    QString date = ui->calendarWidget->selectedDate().toString("yyyy-MM-dd");
    QString remarks = ui->remarksInput->toPlainText();

    QSqlDatabase db1 = QSqlDatabase::database("connection1");
    QSqlQuery query1(db1);

    if(m_type == to_insert){
        query1.exec("insert into incomes values(NULL," + count + ",'" + date + "','" +
                      remarks +"')");
    }
    else{
        query1.exec("update incomes set amount = " + count + ",date = '" + date + "',remarks = '" + remarks +
                    "'where id = " + QString::number(m_id));
    }

    db1.close();

    emit insert(this);
}

void insertIncome::on_cancelButton_clicked()
{
    emit cancel(this);
}
