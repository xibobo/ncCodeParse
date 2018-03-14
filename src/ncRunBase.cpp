#include "../inc/ncRunBase.h"



/**
 * @brief ncRunBase::getLineLength获取直线长度
 * @param startPoint起点
 * @param endPoint终点
 * @return 直线长度
 */
double ncRunBase::getLineLength(st_ncPoint startPoint, st_ncPoint endPoint)
{
    return sqrt(pow(fabs(endPoint.x-startPoint.x),2)+pow(fabs(endPoint.y-startPoint.y),2)+pow(fabs(endPoint.z-startPoint.z),2));
}

/**
 * @brief ncRunBase::getArcLength获取圆弧长度
 * @param startPoint起点
 * @param endPoint终点
 * @param centrePoint偏量圆心点
 * @param plane所在平面
 * @return 圆弧长度
 */
double ncRunBase::getArcLength(st_ncPoint startPoint, st_ncPoint endPoint, st_ncIncPoint centrePoint, em_Plane plane)
{
    st_ncPoint cen;
    double se=0,sc=0,ec=0,v=0,newse=0;
    double angle=0,arclength=0,length=0;
    if(plane==G17){
        //确定圆心点位置
        cen.x=startPoint.x-centrePoint.i;
        cen.y=startPoint.y-centrePoint.j;
        //计算三条边长度
        se=sqrt(pow(fabs(endPoint.x-startPoint.x),2)+pow(fabs(endPoint.y-startPoint.y),2));
        sc=sqrt(pow(fabs(cen.x-startPoint.x),2)+pow(fabs(cen.y-startPoint.y),2));
        ec=sqrt(pow(fabs(cen.x-endPoint.x),2)+pow(fabs(cen.y-endPoint.y),2));

        //判断是否为圆
        if(sc-ec>0.0000001){
            throw QString("this is not a circle!");
        }

        //判断是否为直径
        if(fabs(se-sc-ec)<0.0000001){
            return M_PI*sc;
        }

        //判断是否为整圆
        if( startPoint.x - endPoint.x == 0 && startPoint.y-endPoint.y == 0){
            return 2*M_PI*sc;
        }
        //平面圆弧计算
        if(startPoint.z==endPoint.z){
            //求圆心角（弧度制）
            angle=acos(((pow(sc,2)+pow(ec,2)-pow(se,2))/(2*sc*ec)));
            //圆心角的转换（角度制）
            angle=angle*(180/M_PI);
            //圆弧长度
            arclength=M_PI*sc*angle/180;

            //计算优弧和劣弧
            v=(startPoint.x-cen.x)*(endPoint.x-cen.x)+(startPoint.y-cen.y)*(endPoint.y-cen.y);
            //劣弧
            if(v>=0){
                return arclength;
            }else{
                return 2*M_PI*sc-arclength;
            }
        //空间圆弧计算
        }else{

            //计算起点和终点直线距离
            newse=sqrt(pow(fabs(endPoint.x-startPoint.x),2)+pow(fabs(endPoint.y-startPoint.y),2)+pow(fabs(endPoint.z-startPoint.z),2));
            //计算圆心角（弧度制）
            angle=acos((pow(sc,2)+pow(ec,2)-pow(se,2))/2*sc*ec);
            //圆心角（角度制）
            angle=angle*(180/M_PI);
            //平面圆弧长度
            length=M_PI*sc*angle/180;
            //空间圆弧长度
            arclength=length*(newse/se);

            //优弧 劣弧计算
            v=(startPoint.x-cen.x)*(endPoint.x-cen.x)+(startPoint.y-cen.y)*(endPoint.y-cen.y);
            //劣弧
            if(v>=0){
                return arclength;
            //优弧
            }else{
                return 2*M_PI*sc-arclength;
            }
        }
    }else if(plane==G18){
        cen.x=startPoint.x-centrePoint.i;
        cen.z=startPoint.z-centrePoint.k;
        se=sqrt(pow(fabs(endPoint.x-startPoint.x),2)+pow(fabs(endPoint.z-startPoint.z),2));
        sc=sqrt(pow(fabs(cen.x-startPoint.x),2)+pow(fabs(cen.z-startPoint.z),2));
        ec=sqrt(pow(fabs(cen.x-endPoint.x),2)+pow(fabs(cen.z-endPoint.z),2));
        if(sc-ec>0.0000001){
            throw QString("this is not a circle!");
        }
        if(fabs(se-sc-ec)<0.0000001){
            return M_PI*sc;
        }
        if(startPoint.x-endPoint.x==0&&startPoint.z-endPoint.z){
            return 2*M_PI*sc;
        }
        if(startPoint.y==endPoint.y){
            angle=acos(((pow(sc,2)+pow(ec,2)-pow(se,2))/(2*sc*ec)));//*(M_PI/180);
            angle=angle*(180/M_PI);
            arclength=M_PI*sc*angle/180;
            v=(startPoint.x-cen.x)*(endPoint.x-cen.x)+(startPoint.z-cen.z)*(endPoint.z-cen.z);
            if(v>=0){
                return arclength;
            }else{
                return 2*M_PI*sc-arclength;
            }
        }else{
            newse=sqrt(pow(fabs(endPoint.x-startPoint.x),2)+pow(fabs(endPoint.y-startPoint.y),2)+pow(fabs(endPoint.z-startPoint.z),2));
            angle=acos((pow(sc,2)+pow(ec,2)-pow(se,2))/2*sc*ec);
            angle=angle*(180/M_PI);
            length=M_PI*sc*angle/180;
            arclength=length*(newse/se);
            v=(startPoint.x-cen.x)*(endPoint.x-cen.x)+(startPoint.z-cen.z)*(endPoint.z-cen.z);
            if(v>=0){
                return arclength;
            }else{
                return 2*M_PI*sc-arclength;
            }
            return arclength;
        }
    }else if(plane==G19){
        cen.y=startPoint.y-centrePoint.j;
        cen.z=startPoint.z-centrePoint.k;
        se=sqrt(pow(fabs(endPoint.y-startPoint.y),2)+pow(fabs(endPoint.z-startPoint.z),2));
        sc=sqrt(pow(fabs(cen.y-startPoint.y),2)+pow(fabs(cen.z-startPoint.z),2));
        ec=sqrt(pow(fabs(cen.y-endPoint.y),2)+pow(fabs(cen.z-endPoint.z),2));
        if(sc-ec>0.0000001){
            throw QString("this is not a circle!");
        }
        if(fabs(se-sc-ec)<0.0000001){
            return M_PI*sc;
        }
        if(startPoint.y-endPoint.y==0&&startPoint.z-endPoint.z==0){
            return 2*M_PI*sc;
        }
        if(startPoint.x==endPoint.x){
            angle=acos(((pow(sc,2)+pow(ec,2)-pow(se,2))/(2*sc*ec)));//*(M_PI/180);
            angle=angle*(180/M_PI);
            arclength=M_PI*sc*angle/180;
            v=(startPoint.y-cen.y)*(endPoint.y-cen.y)+(startPoint.z-cen.z)*(endPoint.z-cen.z);
            if(v>=0){
                return arclength;
            }else{
                return 2*M_PI*sc-arclength;
            }
        }else{
            newse=sqrt(pow(fabs(endPoint.x-startPoint.x),2)+pow(fabs(endPoint.y-startPoint.y),2)+pow(fabs(endPoint.z-startPoint.z),2));
            angle=acos((pow(sc,2)+pow(ec,2)-pow(se,2))/2*sc*ec);
            angle=angle*(180/M_PI);
            length=M_PI*sc*angle/180;
            arclength=length*(newse/se);
            v=(startPoint.y-cen.y)*(endPoint.y-cen.y)+(startPoint.z-cen.z)*(endPoint.z-cen.z);
            if(v>=0){
                return arclength;
            }else{
                return 2*M_PI*sc-arclength;
            }
            return arclength;
        }
    }

}
/**
 * @brief ncRunBase::getArcLength获取圆弧长度
 * @param startPoint起点
 * @param endPoint终点
 * @param Radius半径
 * @param plane所在平面
 * @return 圆弧长度
 */
double ncRunBase::getArcLength(st_ncPoint startPoint, st_ncPoint endPoint, double Radius, em_Plane plane)
{
    double se=0,newse=0;
    double angle=0,arclength=0,length=0;
    double rad=fabs(Radius);
    if(plane==G17){
        se=sqrt(pow((fabs(endPoint.x-startPoint.x)),2)+pow((fabs(endPoint.y-startPoint.y)),2));
        if(fabs(se-2*rad)<0.001){
            return M_PI*rad;
        }
        if( startPoint.x - endPoint.x == 0 && startPoint.y-endPoint.y == 0){
            return 2*M_PI*rad;
        }
        if(startPoint.z==endPoint.z){
            angle=acos(((pow(rad,2)+pow(rad,2)-pow(se,2))/(2*rad*rad)));//*(M_PI/180);
            angle=angle*(180/M_PI);
            arclength=M_PI*rad*angle/180;

            if(Radius<0){
                return 2*M_PI*rad-arclength;
            }else{
                return arclength;
            }
        }else{
            newse=sqrt(pow(fabs(endPoint.x-startPoint.x),2)+pow(fabs(endPoint.y-startPoint.y),2)+pow(fabs(endPoint.z-startPoint.z),2));
            angle=acos((pow(rad,2)+pow(rad,2)-pow(se,2))/(2*rad*rad));
            angle=angle*(180/M_PI);
            length=M_PI*rad*angle/180;
            arclength=length*(newse/se);
            if(Radius<0){
                return 2*M_PI*rad-arclength;
            }else{
                return arclength;
            }
        }
    }else if(plane==G18){
        if(fabs(se-2*rad)<0.001){
            return M_PI*rad;
        }
        if( startPoint.x - endPoint.x == 0 && startPoint.z-endPoint.z == 0){
            return 2*M_PI*rad;
        }
        se=sqrt(pow(fabs(endPoint.x-startPoint.x),2)+pow(fabs(endPoint.z-startPoint.z),2));
        if(startPoint.y==endPoint.y){
            angle=acos(((pow(rad,2)+pow(rad,2)-pow(se,2))/(2*rad*rad)));//*(M_PI/180);
            angle=angle*(180/M_PI);
            arclength=M_PI*rad*angle/180;
            if(Radius<0){
                return 2*M_PI*rad-arclength;
            }else{
                return arclength;
            }
        }else{
            newse=sqrt(pow(fabs(endPoint.x-startPoint.x),2)+pow(fabs(endPoint.y-startPoint.y),2)+pow(fabs(endPoint.z-startPoint.z),2));
            angle=acos((pow(rad,2)+pow(rad,2)-pow(se,2))/2*rad*rad);
            angle=angle*(180/M_PI);
            length=M_PI*rad*angle/180;
            arclength=length*(newse/se);
            if(Radius<0){
                return 2*M_PI*rad-arclength;
            }else{
                return arclength;
            }
        }
    }else if(plane==G19){
        if(fabs(se-2*rad)<0.001){
            return M_PI*rad;
        }
        if( startPoint.y - endPoint.y == 0 && startPoint.z-endPoint.z == 0){
            return 2*M_PI*rad;
        }
        se=sqrt(pow(fabs(endPoint.y-startPoint.y),2)+pow(fabs(endPoint.z-startPoint.z),2));
        if(startPoint.x==endPoint.x){
            angle=acos(((pow(rad,2)+pow(rad,2)-pow(se,2))/(2*rad*rad)));//*(M_PI/180);
            angle=angle*(180/M_PI);
            arclength=M_PI*rad*angle/180;
            if(Radius<0){
                return 2*M_PI*rad-arclength;
            }else{
                return arclength;
            }
        }else{
            newse=sqrt(pow(fabs(endPoint.x-startPoint.x),2)+pow(fabs(endPoint.y-startPoint.y),2)+pow(fabs(endPoint.z-startPoint.z),2));
            angle=acos((pow(rad,2)+pow(rad,2)-pow(se,2))/2*rad*rad);
            angle=angle*(180/M_PI);
            length=M_PI*rad*angle/180;
            arclength=length*(newse/se);
            if(Radius<0){
                return 2*M_PI*rad-arclength;
            }else{
                return arclength;
            }
        }
    }

}

ncRunBase::ncRunBase(QObject *parent) : QObject(parent)
{
    ModelName="";
    MachineType="";
    Manufacturer="";
}

QString ncRunBase::getModelName()
{
    return ModelName;

}

QString ncRunBase::getMachineType()
{
    return MachineType;
}

QString ncRunBase::getManufacturer()
{
    return Manufacturer;
}
