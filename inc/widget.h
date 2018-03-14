#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "ncdocument.h"
#include "ncRunBase.h"
#include "ncRunFanucMill.h"
#include "ncDocISOGcode.h"
#include <QTextDocument>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    ncDocument *set;

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    //void on_pushButton_2_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
