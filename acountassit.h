#ifndef ACOUNTASSIT_H
#define ACOUNTASSIT_H

#include <QWidget>
#include <QSwipeGesture>

#include "insertwidget.h"
#include "insertincome.h"
#include "customgesture.h"
#include "viewdetail.h"



namespace Ui {
class acountAssit;
}

class acountAssit : public QWidget
{
    Q_OBJECT

public:
    explicit acountAssit(QWidget *parent = 0);
    ~acountAssit();

public slots:
    void insertWidgetClose(insertWidget*);

    void insertIncomeClose(insertIncome*);

    void updateAll(viewDetail*detail);

private slots:
    void on_pushButton_clicked();

    void on_dailyReport_clicked();

    void on_updateButton_clicked();

    void on_rangeButton_clicked();

    void on_insertIncomeButton_clicked();

private:
    Ui::acountAssit *ui;

    void updateModel();
    void updateIncome();


    void swipeTriggered(QSwipeGesture *gesture);
    void tapAndHoldTriggered(QTapAndHoldGesture * gesture);

    bool event(QEvent *event);

    bool gestureEvent(QGestureEvent *event);



    //op_type m_type;
};

#endif // ACOUNTASSIT_H
