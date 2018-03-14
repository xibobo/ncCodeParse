#ifndef NCRUNFANUCLATHE_H
#define NCRUNFANUCLATHE_H

#include "ncRunBase.h"
#include "ncdocument.h"


/** Nc Status Struct ,record machine running status
 * infomation .  Fanuc Lathe B serial Code
 */
struct st_ncLatheStatus{
    double CG01;//运动方式G00/G01/G02/G03/G71/G72/G73/G74
    double CG02;//恒转速 G96/G96
    double CG03;//绝对相对指令 G90/G91
    double CG04;//刀台镜像 G68/G69
    double CG05;//进给值方式 G94/G95
    double CG06;//单位 G20/G21
    double CG07;//半径补偿 G40/G41/G42
    double CG08;//主轴速度变得检测 G25/G26
    double CG09;//存储行程检测 G22/G23
    double CG10;//固定循环 G80/G81/G82/G83/G83.1/G84/G84.2/G85/G86/G87/G88/G89
    double CG11;//固定循环返回 G98/G99
    double CG12;//宏模态调用 G66/G67
    double CG14;//工件坐标系 G54/G54.1/G55/G56/G57/G58/G59
    double CG15;//切削停止方式 G61/G63/G64
    double CG16;//平面选择G17/G18/G19
    double CG20;//多边形加工 G50.2/G51.2/G250/G251 G160/G161
    double CG21;//极坐标 G12.1/G13.1/G112/G113
    double A;//A轴坐标
    double B;//B轴坐标
    double C;//C轴坐标
    double D;//半径补偿号
    double E;
    double F;//进给
    double I;//圆心矢量
    double J;//圆心矢量
    double K;//圆心矢量
    double L;
    double P;
    double Q;
    double aR;//圆弧半径
    double pR;//固定循环
    double S;//转速
    double T;//刀号
    double X;//X轴坐标
    double Y;//Y轴坐标
    double Z;//Z轴坐标

    st_ncLatheStatus(){
        CG01=0;
        CG02=0;
        CG03=0;
        CG04=0;
        CG05=0;
        CG06=0;
        CG07=0;
        CG08=0;
        CG09=0;
        CG10=0;
        CG11=0;
        CG12=0;
        CG14=0;
        CG15=0;
        CG16=0;
        CG20=0;
        CG21=0;
        A=0;
        B=0;
        C=0;
        D=0;
        E=0;
        F=0;
        I=0;
        J=0;
        K=0;
        L=0;
        P=0;
        Q=0;
        aR=0;
        pR=0;
        S=0;
        T=0;
        X=0;
        Y=0;
        Z=0;

    }

    /** 设置结构初始值
     * @brief setInit
     */
    setInit(){
        CG01=0;
        CG02=97;
        CG03=90;
        CG04=69;
        CG05=95;
        CG06=21;
        CG07=40;
        CG08=25;
        CG09=22;
        CG10=80;
        CG11=98;
        CG12=67;
        CG14=54;
        CG15=64;
        CG16=18;
        CG20=50.2;
        CG21=13.1;
        A=0;
        B=0;
        C=0;
        D=0;
        E=0;
        F=0;
        I=0;
        J=0;
        K=0;
        L=0;
        P=0;
        Q=0;
        aR=0;
        pR=0;
        S=0;
        T=0;
        X=0;
        Y=0;
        Z=0;

    }
};

class ncRunFanucLathe : public ncRunBase{

private:
    st_ncLatheStatus ncModal,ncCurr;

public:
    ncRunFanucLathe(QObject* parent=0);
    double getRunningTime(int startLine=0,int endLine=-1);//获取运行时间
    double getToolRunTime(int ToolNo);//获取刀具运行时间
    double getSubRunTime(int ProNo);//获取子程序运行时间
    QStringList getLineModalGcode(int LineNo);//获取行运行状态
    QMap<QChar,double> getLineModalMap(int LineNo);//获取行运行状态

};

#endif // NCRUNFANUCLATHE_H
