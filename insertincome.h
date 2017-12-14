#ifndef INSERTINCOME_H
#define INSERTINCOME_H

#include <QWidget>

#include "insertwidget.h"

namespace Ui {
class insertIncome;
}

class insertIncome : public QWidget
{
    Q_OBJECT

public:
    explicit insertIncome(QWidget *parent = 0);
    insertIncome(int index);
    ~insertIncome();

private:
    Ui::insertIncome *ui;

    op_type m_type;

    int m_id;

signals:
    void insert(insertIncome*);
    void cancel(insertIncome*);
private slots:
    void on_OkButton_clicked();
    void on_cancelButton_clicked();
};

#endif // INSERTINCOME_H
