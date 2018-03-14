#ifndef NCRUNFANUCMILL_H
#define NCRUNFANUCMILL_H

#include "common.h"
#include "ncRunBase.h"
#include "ncDocISOGcode.h"


/** Nc Status Struct ,record machine running status
 * infomation .
 */
struct st_ncMillStatus{
    double CG00;//非模态代码组
    double CG01;//运动方式G00/G01/G02/G03
    double CG02;//平面选择G17/G18/G19
    double CG03;//绝对相对指令 G90/G91
    double CG04;//存储行程检测 G22/G23
    double CG05;//进给值方式 G93/G94/G95
    double CG06;//单位 G20/G21
    double CG07;//半径补偿 G40/G41/G42
    double CG08;//长度补偿 G43/G44/G49
    double CG09;//固定循环 G73/G74/G76/G80/G81/G82/G83/G84/G84.2/G84.3/G85/G86/G87/G88/G89
    double CG10;//固定循环返回 G98/G99
    double CG11;//比例缩放 G50/G51
    double CG12;//宏模态调用 G66/G67
    double CG13;//恒转速 G96/G97
    double CG14;//工件坐标系 G54/G54.1/G55/G56/G57/G58/G59
    double CG15;//切削停止方式 G61/G62/G63/G64
    double CG16;//坐标系旋转G68/G69
    double CG17;//极坐标 G15/G16
    double CG18;//法线方向控制 G40.1/G41.1/G42.1
    double CG20;//切磨控制 G160/G161
    double CG22;//镜像控制 G50.1/G51.1
    double CG34;//电子齿轮箱 G80.4/G81.4
    double A;//A轴坐标
    double B;//B轴坐标
    double C;//C轴坐标
    double D;//半径补偿号
    double E;
    double F;//进给  毫米/分钟
    double H;//长度补偿号
    double I;//圆心矢量
    double J;//圆心矢量
    double K;//圆心矢量
    double L;
    double M;//M代码
    double PG;//程序号
    double P;//暂停时间（毫秒）
    double PX;//暂停时间（秒）
    double Q;
    double aR;//圆弧半径
    double pR;//固定循环
    double S;//转速
    double T;//刀号
    double X;//X轴坐标
    double Y;//Y轴坐标
    double Z;//Z轴坐标

    st_ncMillStatus(){
        CG00=0;
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
        CG13=0;
        CG14=0;
        CG15=0;
        CG16=0;
        CG17=0;
        CG18=0;
        CG20=0;
        CG22=0;
        CG34=0;
        A=0;
        B=0;
        C=0;
        D=0;
        E=0;
        F=0;
        H=0;
        I=0;
        J=0;
        K=0;
        L=0;
        M=0;
        PG=0;
        P=0;
        PX=0;
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
        CG00=0;
        CG01=0;
        CG02=17;
        CG03=90;
        CG04=22;
        CG05=94;
        CG06=21;
        CG07=40;
        CG08=49;
        CG09=80;
        CG10=98;
        CG11=50;
        CG12=67;
        CG13=97;
        CG14=54;
        CG15=64;
        CG16=69;
        CG17=15;
        CG18=40.1;
        CG20=160;
        CG22=50.1;
        CG34=80.4;
        A=0;
        B=0;
        C=0;
        D=0;
        E=0;
        F=0;
        H=0;
        I=0;
        J=0;
        K=0;
        L=0;
        M=-1;
        PG=0;
        P=0;
        PX=0;
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

class ncRunFanucMill : public ncRunBase
{
    Q_OBJECT

    ncDocISOGcode * doc;

    //G00 快速移动速度
    Q_PROPERTY(double RapidSpeed READ RapidSpeed)
    //总的运行时间
    Q_PROPERTY(double RunnedTime READ RunnedTime NOTIFY RunnedTimeChanged)
    //总的运行距离
    Q_PROPERTY(double MoveDistance READ RunnedDistance  NOTIFY RunnedDistanceChanged)
    //进给距离
    Q_PROPERTY(double FeedDistance READ FeedDistance  NOTIFY FeedDistanceChanged)
    // 退给差量
    Q_PROPERTY(double BackDispersion READ BackDispersion)

    //声明错误列表
    QList<st_Error> ErrList;
    //声明当前模态
    st_ncMillStatus fanucmill;
    //检查是否进入子程序
    int IsLoop=0;

    //快速移动速度（毫米/分钟）
    double m_RapidSpeed=5000;
    //运行时间（分钟）
    double m_RunnedTime=0;
    //换刀时间
    double m_exchangeTool=20;
    //移动距离
    double m_MoveDistance=0;
    //进给距离
    double m_FeedDistance=0;
    //退给距离
    double m_BackDispersion=0;

public:
    ncRunFanucMill(ncDocISOGcode *document,QObject *parent=0);
    //获取运行时间（实例化）
    double getRunningTime(int startLine=0,int endLine=-1);
    //获取刀具运行时间（实例化）
    double getToolRunTime(int ToolNo);
    //获取子程序运行时间（实例化）
    double getSubRunTime(int ProNo);
    //获取行运行状态（实例化）
    QStringList getLineModalGcode(int LineNo);
    //获取行运行状态（实例化）
    QMap<QChar,double> getLineModalMap(int LineNo);
    //指定行模态，返回模模态结构（实例化）
    st_ncMillStatus getLineModel(int LineNo,st_ncMillStatus CurrentModel);


    //返回快速移动速度
    double RapidSpeed() const
    {
        return m_RapidSpeed;
    }

    //返回运行时间
    double RunnedTime() const
    {
        return m_RunnedTime;
    }

    //返回运行距离
    double RunnedDistance() const
    {
        return m_MoveDistance;
    }

    //返回进给距离
    double FeedDistance() const
    {
        return m_FeedDistance;
    }

    //返回退给距离
    double BackDispersion() const
    {
        return m_BackDispersion;
    }

public slots:



signals:
    void RunnedTimeChanged(double RunnedTime);
    void RunnedDistanceChanged(double MoveDistance);
    void FeedDistanceChanged(double FeedDistance);
};

#endif // NCRUNFANUCMILL_H
