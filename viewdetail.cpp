#include "viewdetail.h"
#include "ui_viewdetail.h"

#include <QtSql>
#include <QMessageBox>

viewDetail::viewDetail(int index, dataType type, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::viewDetail)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    QSqlDatabase db1 = QSqlDatabase::database("connection1");
    QSqlQuery query1(db1);

    m_type = type;
    m_id = index;

    if(type == expenses){
        query1.exec("select * from accounts where id = " + QString::number(index));
        query1.next();
        ui->label->setText("金额：" + query1.value(1).toString() + "\n日期：" + query1.value(2).toString() +
                           "\n用途：" + query1.value(3).toString() + "\n支付方式：" + query1.value(4).toString()
                           + "\n备注：\n" + query1.value(5).toString());
    }
    if(type == incomes){
        query1.exec("select * from incomes where id = " + QString::number(index));
        query1.next();
        ui->label->setText("金额：" + query1.value(1).toString() + "\n日期：" + query1.value(2).toString() +
                           "\n备注：\n" + query1.value(3).toString());
    }
}

viewDetail::~viewDetail()
{
    delete ui;
}

void viewDetail::on_pushButton_clicked()
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("确认删除"),
            QString(tr("确定要删除此条记录吗？")),
            QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::Yes) {

        QSqlDatabase db1 = QSqlDatabase::database("connection1");
        QSqlQuery query1(db1);
        if(m_type == expenses){
            query1.exec("delete from accounts where id = " + QString::number(m_id));
        }
        if(m_type == incomes){
            query1.exec("delete from incomes where id = " + QString::number(m_id));
        }

        emit deleteItem(this);
    }

}

void viewDetail::closeEvent(QCloseEvent *event)
{
    event->accept();
}
