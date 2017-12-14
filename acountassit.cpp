#include "acountassit.h"
#include "connection.h"
#include "dailyreport.h"

#include "ui_acountassit.h"

#include <QtSql>
#include <QMessageBox>
#include <QInputDialog>

#include "insertwidget.h"
#include "rangereport.h"
#include "insertincome.h"
#include "viewdetail.h"
#include "customgesture.h"

acountAssit::acountAssit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::acountAssit)
{
    ui->setupUi(this);

    updateModel();
    updateIncome();

    ui->tabWidget->setCurrentIndex(0);

    //grabGesture(Qt::SwipeGesture);
    //grabGesture(Qt::PanGesture);
    //grabGesture(Qt::PinchGesture);
    //grabGesture(Qt::TapGesture);
    grabGesture(Qt::TapAndHoldGesture);

    //setAttribute(Qt::WA_WState_AcceptedTouchBeginEvent);
    setAttribute(Qt::WA_AcceptTouchEvents);

}

acountAssit::~acountAssit()
{
    delete ui;
}

void acountAssit::on_pushButton_clicked()
{
    insertWidget* insert1 = new insertWidget;
    insert1->show();

    connect(insert1,SIGNAL(insert(insertWidget*)),this,SLOT(insertWidgetClose(insertWidget*)));
    connect(insert1,SIGNAL(cancel(insertWidget*)),this,SLOT(insertWidgetClose(insertWidget*)));

}

void acountAssit::updateModel()
{
    QSqlDatabase db1 = QSqlDatabase::database("connection1");


    static QSqlQueryModel *model = new QSqlQueryModel(ui->tableView);
    model->setQuery("SELECT * FROM accounts",db1);
    model->setHeaderData(0, Qt::Horizontal, tr("索引号"));
    model->setHeaderData(1, Qt::Horizontal, tr("金额"));
    model->setHeaderData(2, Qt::Horizontal, tr("日期"));
    model->setHeaderData(3, Qt::Horizontal, tr("用途"));
    model->setHeaderData(4, Qt::Horizontal, tr("支付方式"));
    model->setHeaderData(5, Qt::Horizontal, tr("备注"));
    ui->tableView->setModel(model);
    db1.close();


}

void acountAssit::insertWidgetClose(insertWidget* ptr)
{
    delete ptr;
    updateModel();
}

void acountAssit::on_dailyReport_clicked()
{
    dailyReport* daily = new dailyReport;
    daily->show();
}

void acountAssit::on_updateButton_clicked()
{
    int r = ui->tableView->currentIndex().row();
    int c = 0;
    //QAbstractItemModel *model = ui->Fr_tableView->model ();
    int index = ui->tableView->model()->index(r,c).data().toInt();
    insertWidget*  updateWidget = new insertWidget(index);
    connect(updateWidget,SIGNAL(insert(insertWidget*)),this,SLOT(insertWidgetClose(insertWidget*)));
    connect(updateWidget,SIGNAL(cancel(insertWidget*)),this,SLOT(insertWidgetClose(insertWidget*)));
    updateWidget->show();
}

bool acountAssit::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture){
        //qDebug()<<"grab!";
        return gestureEvent(static_cast<QGestureEvent*>(event));
    }
        return QWidget::event(event);
}

void acountAssit::swipeTriggered(QSwipeGesture *gesture)
{
    if (gesture->state() == Qt::GestureFinished) {
           if (gesture->horizontalDirection() == QSwipeGesture::Left
               || gesture->verticalDirection() == QSwipeGesture::Up) {
               ui->tabWidget->setCurrentIndex((ui->tabWidget->currentIndex() - 1 < 0)?0:ui->tabWidget->currentIndex() - 1 );
           } else {
               //qCDebug(lcExample) << "swipeTriggered(): swipe to next";
               ui->tabWidget->setCurrentIndex((ui->tabWidget->currentIndex() + 1 > ui->tabWidget->count())
                                              ?ui->tabWidget->count():ui->tabWidget->currentIndex() + 1 );
           }
           update();
       }
}

void acountAssit::tapAndHoldTriggered(QTapAndHoldGesture *gesture)
{

    if (gesture->state() == Qt::GestureFinished){
        //qDebug()<<ui->tabWidget->currentIndex();

        if (ui->tabWidget->currentIndex() == 0){
            int r = ui->tableView->currentIndex().row();
            int c = 0;
            //QAbstractItemModel *model = ui->Fr_tableView->model ();
            int index = ui->tableView->model()->index(r,c).data().toInt();
            viewDetail* detail = new viewDetail(index,expenses);
            detail->show();
            connect(detail,SIGNAL(deleteItem(viewDetail*)),this,SLOT(updateAll(viewDetail*)));
        }
        if (ui->tabWidget->currentIndex() == 1){
            int r = ui->incomeTable->currentIndex().row();
            int c = 0;
            //QAbstractItemModel *model = ui->Fr_tableView->model ();
            int index = ui->incomeTable->model()->index(r,c).data().toInt();
            viewDetail* detail = new viewDetail(index,incomes);
            detail->show();
            connect(detail,SIGNAL(deleteItem(viewDetail*)),this,SLOT(updateAll(viewDetail*)));
        }
    }

}


bool acountAssit::gestureEvent(QGestureEvent *event)
{
    //qDebug() << "gestureEvent():" << event;
    if (QGesture *tapAndHold = event->gesture(Qt::TapAndHoldGesture))
        tapAndHoldTriggered(static_cast<QTapAndHoldGesture *>(tapAndHold));
    else if (QGesture *pan = event->gesture(Qt::PanGesture))
        //panTriggered(static_cast<QPanGesture *>(pan));
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
        //pinchTriggered(static_cast<QPinchGesture *>(pinch));
    return true;
}

void acountAssit::on_rangeButton_clicked()
{
    if (ui->rangeTypeBox->currentIndex() == 4){
        QDate date1 = ui->beginCalendar->selectedDate();
        QDate date2 = ui->endCalendar->selectedDate();
        rangeReport* report = new rangeReport(date1,date2);
        report->show();
    }
    else{
        rangeReport* report = new rangeReport(static_cast<rangeType>(ui->rangeTypeBox->currentIndex()));
        report->show();
    }
}

void acountAssit::on_insertIncomeButton_clicked()
{
    insertIncome* insert1 = new insertIncome;
    insert1->show();

    connect(insert1,SIGNAL(insert(insertIncome*)),this,SLOT(insertIncomeClose(insertIncome*)));
    connect(insert1,SIGNAL(cancel(insertIncome*)),this,SLOT(insertIncomeClose(insertIncome*)));

}

void acountAssit::insertIncomeClose(insertIncome * ptr)
{
    delete ptr;
    updateIncome();
}

void acountAssit::updateIncome()
{
    QSqlDatabase db1 = QSqlDatabase::database("connection1");


    static QSqlQueryModel *model = new QSqlQueryModel(ui->tableView);
    model->setQuery("SELECT * FROM incomes",db1);
    model->setHeaderData(0, Qt::Horizontal, tr("索引号"));
    model->setHeaderData(1, Qt::Horizontal, tr("金额"));
    model->setHeaderData(2, Qt::Horizontal, tr("日期"));
    model->setHeaderData(3, Qt::Horizontal, tr("备注"));
    ui->incomeTable->setModel(model);
}


void acountAssit::updateAll(viewDetail *detail)
{
    delete detail;
    updateModel();
    updateIncome();
}
