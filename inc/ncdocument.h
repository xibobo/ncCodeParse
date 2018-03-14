/***************************************************************************************************
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  ***************************************************************************************************/


#ifndef NCDOCUMENT_H
#define NCDOCUMENT_H

#include "QTextDocument"
#include "QFile"
#include <QWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextBlock>
#include <QString>
#include "common.h"


///程序类型
typedef enum{
    pt_unknown,
    pt_Main,            //主程序
    pt_Sub              //子程序
} em_ProgType;

///程序信息结构
typedef struct{
    int ProgNo=-1;         //程序号
    int StartRow=-1;       //开始行
    int EndRow=-1;         //结束行
    int ProgType=-1;       //程序类型
    QList<int> CallPos;    //呼叫子程序的行
}st_ProgInfo;


///刀具信息结构
typedef struct{
    int ToolNo=-1;         //刀号
    int InRow = -1;      //所在行
    int StartRow = -1;
    int EndRow = -1;
}st_ToolInfo;



class ncDocument : public QTextDocument
{

private:


protected:
    double Xmax=0;
    double Xmin=0;
    double Ymax=0;
    double Ymin=0;
    double Zmax=0;
    double Zmin=0;
    double Amax=0;
    double Amin=0;
    double Bmax=0;
    double Bmin=0;
    double Cmax=0;
    double Cmin=0;

    QList<st_ToolInfo> ToolList;//刀具信息表
    QList<st_ProgInfo> ProgList;//文件信息表
    QList<st_Error> errList;//错误信息表

    int lineno=-1;//行号
    virtual void ncInfoRead()=0;                //nc文件读取


public:

    ncDocument();
    //打开文件并处理，
    void ncOpenFile(QString filename);//nc文件打开
    //获取指定行的所有代码，

    //virtual QMap<QChar,double> getLineCode(int LineNo)=0;//获取行代码
    virtual void dealNcInfo(QMap<QChar,double> list)=0;


    //返回文件中所有的程序信息
    QList<st_ProgInfo> getProList();
    //返回文件中的所有刀具信息
    QList<st_ToolInfo> getTooList();
    //获取所有错误信息
    QList<st_Error> getErrList();



    //返回程序中X坐标最大值
    double getXmaxValue();
    //返回程序中X坐标最小值
    double getXminValue();
    //返回程序中Y坐标最大值
    double getYmaxValue();
    //返回程序中Y坐标最小值
    double getYminValue();
    //返回程序中Z坐标最大值
    double getZmaxValue();
    //返回程序中Z坐标最小值
    double getZminValue();
    //返回程序中A坐标最大值
    double getAmaxValue();
    //返回程序中A坐标最小值
    double getAminValue();
    //返回程序中B坐标最大值
    double getBmaxValue();
    //返回程序中B坐标最小值
    double getBminValue();
    //返回程序中C坐标最大值
    double getCmaxValue();
    //返回程序中C坐标最小值
    double getCminValue();


};




#endif // NCDOCUMENT_H
