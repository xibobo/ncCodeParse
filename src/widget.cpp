#include "../inc/widget.h"
#include "ui_widget.h"
#include "QMessageBox"
#include "QPlainTextDocumentLayout"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{   
    QString filename = QFileDialog::getOpenFileName(this,tr("选择NC文件打开:"),"",tr("NC文件(*.NC *.CNC)"));
    ncDocISOGcode* ncDoc = new ncDocISOGcode();
    try{
        ncDoc->ncOpenFile(filename);
        ncDoc->ncInfoRead();

    }catch( QString e){
        QMessageBox::about(NULL,"EXCEP",e);
        return ;
    }
    ncRunFanucMill * fanc= new ncRunFanucMill(ncDoc);
    fanc->getRunningTime();
    QPlainTextDocumentLayout* pl = new QPlainTextDocumentLayout(ncDoc);
    ncDoc->setDocumentLayout(pl);
    ui->plainTextEdit->setDocument(ncDoc);


}

void Widget::on_pushButton_2_clicked()
{

}
