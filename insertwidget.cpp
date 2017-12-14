#include "insertwidget.h"
#include "ui_insertwidget.h"

#include <QtSql>
#include <QMessageBox>
#include <QInputDialog>

insertWidget::insertWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::insertWidget)
{
    ui->setupUi(this);

    m_type = to_insert;
}

insertWidget::insertWidget(int index):
    ui(new Ui::insertWidget)
{
    ui->setupUi(this);

    QSqlDatabase db1 = QSqlDatabase::database("connection1");
    QSqlQuery query1(db1);

    m_type = to_update;
    query1.exec("select amount,remarks from accounts where id = " + QString::number(index));
    query1.next();
    ui->countInput->setText(query1.value(0).toString());
    ui->remarksInput->setPlainText(query1.value(1).toString());
    m_id = index;
}

insertWidget::~insertWidget()
{
    delete ui;
}

void insertWidget::on_OkButton_clicked()
{
    QString count = ui->countInput->text();
    QString useType = ui->purposeInput->currentText();
    QString payType = ui->payTypeInput->currentText();
    QString date = ui->calendarWidget->selectedDate().toString("yyyy-MM-dd");
    QString remarks = ui->remarksInput->toPlainText();

    QSqlDatabase db1 = QSqlDatabase::database("connection1");
    QSqlQuery query1(db1);

    if(m_type == to_insert){
        query1.exec("insert into accounts values(NULL," + count + ",'" + date + "','" +
                    useType + "','" + payType + "','" + remarks +"')");
    }
    else{
        query1.exec("update accounts set amount = " + count + ",date = '" + date +
                    "',purposeType = '" + useType + "',payType = '" + payType + "',remarks = '" + remarks +
                    "'where id = " + QString::number(m_id));
    }

    db1.close();

    emit insert(this);
}

void insertWidget::on_cancelButton_clicked()
{
    emit cancel(this);
}
