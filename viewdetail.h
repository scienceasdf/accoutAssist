#ifndef VIEWDETAIL_H
#define VIEWDETAIL_H

#include <QWidget>
#include <QCloseEvent>

enum dataType{expenses,incomes};

namespace Ui {
class viewDetail;
}

class viewDetail : public QWidget
{
    Q_OBJECT

public:
    explicit viewDetail(int index, dataType type, QWidget *parent = 0);
    ~viewDetail();

   void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_clicked();

private:
    Ui::viewDetail *ui;

    dataType m_type;
    int m_id;
signals:
    void deleteItem(viewDetail*);
};

#endif // VIEWDETAIL_H
