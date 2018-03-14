#ifndef NCRUNBASE_H
#define NCRUNBASE_H

#include "common.h"
#include "ncdocument.h"
#include "common.h"
#include "ncDocISOGcode.h"
#include <QObject>
#include <math.h>

class ncRunBase : public QObject
{
    Q_OBJECT
private:


protected:
   QString ModelName;//模块名称，
   QString MachineType;//适用机器类型 mill/lathe/grinder
   QString Manufacturer;//代码厂家 fanuc /siemens/
   ncDocument* doc;


public:
    ncRunBase();
    //获取运行时间（抽象函数）
    virtual double getRunningTime(int startLine=0,int endLine=-1)=0;
    //获取刀具运行时间（抽象函数）
    virtual double getToolRunTime(int ToolNo)=0;
    //获取子程序运行时间（抽象函数）
    virtual double getSubRunTime(int ProNo)=0;
    //获取行运行状态（抽象函数）
    virtual QStringList getLineModalGcode(int LineNo)=0;
    //获取行运行状态-键值对（抽象函数）
    virtual QMap<QChar,double> getLineModalMap(int LineNo)=0;


    //获取直线长度
    double getLineLength(st_ncPoint startPoint,st_ncPoint endPoint);
    //三点获取弧长
    double getArcLength(st_ncPoint startPoint,st_ncPoint endPoint,st_ncIncPoint centrePoint,em_Plane plane);
    //两点半径获取弧长
    double getArcLength(st_ncPoint startPoint,st_ncPoint endPoint,double Radius,em_Plane plane);
    ncRunBase(QObject *parent = 0);

    //获取模块名称
    QString getModelName();
    //获取适用类型
    QString getMachineType();
    //获取代码厂家
    QString getManufacturer();

signals:

public slots:
};

#endif // NCRUNBASE_H
