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
    delete ncDoc;
    delete ui;
}

void Widget::on_pushButton_clicked()
{   
    if(ncDoc != nullptr){
        delete ncDoc;
        ncDoc =nullptr;
    }
    QString filename = QFileDialog::getOpenFileName(this,tr("选择NC文件打开:"),"",tr("NC文件(*.NC *.CNC)"));
    ncDoc = new ncDocISOGcode();
    try{
        ncDoc->ncOpenFile(filename);
        ncDoc->ncInfoRead();

    }catch( QString e){
        QMessageBox::about(NULL,"EXCEP",e);
        return ;
    }
    QPlainTextDocumentLayout* pl = new QPlainTextDocumentLayout(ncDoc);
    ncDoc->setDocumentLayout(pl);
    ui->plainTextEdit->setDocument(ncDoc);
}

void Widget::on_pushButton_2_clicked()
{
    ncRunFanucMill * fanc= new ncRunFanucMill(ncDoc);
    QMessageBox::information(this,"code running time",QString("this code file running time is: %1 minute").arg( fanc->getRunningTime()),QMessageBox::Ok);
    delete fanc;
    fanc = nullptr;

}
