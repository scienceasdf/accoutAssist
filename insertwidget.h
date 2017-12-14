#ifndef INSERTWIDGET_H
#define INSERTWIDGET_H

#include <QWidget>
#include <QtSql>

// This widget is for both inserting and updating expenses data

enum op_type{to_insert, to_update};

namespace Ui {
class insertWidget;
}

class insertWidget : public QWidget
{
    Q_OBJECT

public:
    explicit insertWidget(QWidget *parent = 0);

    // For updating data
    insertWidget(int index);

    ~insertWidget();

private slots:
    void on_OkButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::insertWidget *ui;

    op_type m_type;

    int m_id;

signals:
    void insert(insertWidget*);
    void cancel(insertWidget*);
};

#endif // INSERTWIDGET_H
