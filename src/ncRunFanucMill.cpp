#include "../inc/ncRunFanucMill.h"
#include "../inc/ncRunBase.h"
#include "math.h"

ncRunFanucMill::ncRunFanucMill(ncDocISOGcode*document,QObject *parent):ncRunBase(parent)
{
     doc = document;
}

/**
 * @brief ncRunFanucMill::getRunningTime获取运行时间
 * @param startLine起始行
 * @param endLine终止行
 * @return ncRunFanucMill运行模态
 */
double ncRunFanucMill::getRunningTime(int startLine, int endLine)
{
    int endRow=0;
    //当前状态存放结构
    st_ncMillStatus tempModel;//上一个状态存放结构
    tempModel.setInit();
    fanucmill.setInit();
    st_Error st;
    //返回的总时间
    double ttime=0;

    //停止计算的行号
    if(endLine == -1){
        endRow =doc->blockCount();
    }else{
        endRow = endLine;
    }

    //时间计算
    for(int i = startLine; i < endRow; i++){
        double movedis=0,rundis=0,fastdis=0;// 总路程 运行距离 快速运动距离
        double ftime=0,rtime=0,time=0;//快速运动时间 运动时间 总时间
        //当前模态
        tempModel = fanucmill;
        //获取当前模态
        fanucmill = getLineModel(i,fanucmill);

        //判断是否进入子程序
        if(IsLoop > 0){
            fanucmill.M = -1;
            IsLoop--;
        }

        //子程序时间计算
        if(fanucmill.M == 98){
            QList<st_ProgInfo> ProList;
            ProList = doc->getProList();
            for(int j = 0; j < ProList.size(); j++){
                if(ProList.at(j).ProgNo == fanucmill.PG){
                    int startline=0,endline=0;
                    IsLoop++;
                    startline = ProList.at(j).StartRow;
                    endline = ProList.at(j).EndRow;
                    time = getRunningTime(startline,endline);
                    ttime+=time;
                }
            }
            continue;
        }

        //结束主程序时间计算
        if(fanucmill.M == 30|| fanucmill.M == 2){
            return ttime;
            break;
        }

        //结束子程序时间计算
        if(fanucmill.M == 99 ){
            return ttime;
        }

        //换刀时间计算
        if(fanucmill.T != tempModel.T){
            time = m_exchangeTool/60;
            ttime +=time;
            m_RunnedTime+=time;
            continue;
        }

// 00 g04 g60同g00不覆盖g00
// 09         g83 g73       g74 g84 g84.2 g84.3 85    81 82 86  87 88 89 76
// 01

        //声明起点和终点
        st_ncPoint start,end;

        //绝对相对选择
        if(fanucmill.CG03 == 90){

           start.x = tempModel.X;start.y =tempModel.Y;start.z =tempModel.Z;
           end.x =fanucmill.X;end.y = fanucmill.Y;end.z = fanucmill.Z;

        }else{

            start.x = tempModel.X;start.y = tempModel.Y;start.z = tempModel.Z;
            end.x = tempModel.X-fanucmill.X;end.y=tempModel.Y-fanucmill.Y;end.z=tempModel.Z-fanucmill.Z;

        }

        //00组代码 暂停时间计算
        if(fanucmill.CG00==4){
          if(fanucmill.P == 0 && fanucmill.PX == 0){
              st.ErrorID=0;
              st.ErrorInfo="Find repeated mistake";
              st.ErrotLine=i;
              ErrList.append(st);
          }
          //暂停毫秒
          if(fanucmill.P!=0){
              time=fanucmill.P/60000;
              m_RunnedTime+=time;
              ttime +=time;
          }
          //暂停分钟
          if(fanucmill.PX!=0){
              time=fanucmill.PX/60;
              m_RunnedTime+=time;
              ttime+= time;
          }
          continue;
        }

        //09组代码时间计算
        if(fanucmill.CG09!=80){
            //73 83代码计算
            if(fanucmill.CG09 == 73 || fanucmill.CG09 == 83){
                double disper=0;//余数
                int times=0;//次数
                disper = fmod(fabs(fanucmill.Z - fanucmill.pR),fabs(fanucmill.Q));
                times = fabs(fanucmill.Z - fanucmill.pR)/fabs(fanucmill.Q);
                if(fanucmill.CG10 == 98){
                    for(int j = 1 ; j <= times; j++){
                        rundis = rundis + fabs(fanucmill.Q) * j + m_BackDispersion;
                        fastdis = fastdis + fabs(fanucmill.Q) *j + fabs(fanucmill.pR);
                        if(j == times){
                            rundis = rundis + fabs(disper) + m_BackDispersion;
                            fastdis = fastdis + fabs(disper);
                        }
                    }
                    movedis = rundis + fastdis;
                    m_MoveDistance += movedis;
                    rtime = rundis / fanucmill.F;
                    ftime = fastdis / m_RapidSpeed;
                    time = rtime + ftime;
                    ttime +=time;
                    m_RunnedTime += time;
                }
                if(fanucmill.CG10 == 99){
                    for(int j = 1; j <= times; j++){
                        rundis = rundis + fabs(fanucmill.Q) * j + m_BackDispersion;
                        fastdis = fabs(fanucmill.pR);
                        if(j == times){
                           rundis = rundis + fabs(disper) + m_BackDispersion;
                        }
                    }
                    movedis = rundis + fastdis;
                    m_MoveDistance += movedis;
                    rtime = rundis / fanucmill.F;
                    ftime = fastdis / m_RapidSpeed;
                    time = rtime + ftime;
                    ttime +=time;
                    m_RunnedTime += time;
                }
                continue;
            }

            //74 84代码计算
            if(fanucmill.CG09 == 74 || fanucmill.CG09 == 84 || fanucmill.CG09 == 85){
                if(fanucmill.CG10 == 98){

                    rundis = fabs(fanucmill.Z - fanucmill.pR) * 2 + m_BackDispersion;
                    fastdis = fabs(fanucmill.pR) * 2;
                    movedis = rundis + fastdis;
                    m_MoveDistance += movedis;
                    rtime = rundis / fanucmill.F;
                    ftime = fastdis / m_RapidSpeed;
                    time = rtime + ftime;
                    m_RunnedTime += time;

                }
                if(fanucmill.CG10 == 99){

                    rundis = fabs(fanucmill.Z - fanucmill.pR) * 2 + m_BackDispersion;
                    fastdis = fabs(fanucmill.pR);
                    movedis = rundis + fastdis;
                    m_MoveDistance += movedis;
                    rtime = rundis / fanucmill.F;
                    ftime = fastdis / m_RapidSpeed;
                    time = rtime + ftime;
                    ttime +=time;
                    m_RunnedTime += time;

                }
                continue;
            }

            //其余代码计算
            if(fanucmill.CG09 == 76 || fanucmill.CG09 == 81 || fanucmill.CG09 == 82 || fanucmill.CG09 >= 86 && fanucmill.CG09 <= 89){
                if(fanucmill.CG10 == 98){

                    rundis = fabs(fanucmill.Z - fanucmill.pR) + m_BackDispersion;
                    fastdis = fabs(fanucmill.pR) + fabs(fanucmill.Z);
                    movedis = rundis + fastdis;
                    m_MoveDistance += movedis;
                    rtime = rundis / fanucmill.F;
                    ftime = fastdis / m_RapidSpeed;
                    time = rtime + ftime;
                    ttime +=time;
                    m_RunnedTime += time;

                }
                if(fanucmill.CG10 == 99){

                    rundis = fabs(fanucmill.Z) - fabs(fanucmill.pR) + m_BackDispersion;
                    fastdis = fanucmill.Z;
                    movedis = rundis +fastdis;
                    m_MoveDistance += movedis;
                    rtime = rundis / fanucmill.F;
                    ftime = fastdis / m_RapidSpeed;
                    time = rtime + ftime;
                    ttime +=time;
                    m_RunnedTime += time;

                }
                continue;
            }
        }

        //01组代码计算
        //快速插补计算
        if(fanucmill.CG01 == 0){

          movedis = getLineLength(start,end);
          m_MoveDistance += movedis;
          time = movedis / m_RapidSpeed;
          ttime +=time;
          m_RunnedTime += time;
          continue;

        }

        //直线插补计算
        if(fanucmill.CG01==1){

          movedis = getLineLength(start,end);
          m_MoveDistance += movedis;
          time = movedis / fanucmill.F;
          ttime += time;
          m_RunnedTime += time;
          continue;

        }

        //圆弧计算
        if(fanucmill.CG01==2||fanucmill.CG01==3){

          if(fanucmill.aR != 0){

              em_Plane plane = (em_Plane)fanucmill.CG02;
              movedis = getArcLength(start,end,fanucmill.aR,plane);
              m_MoveDistance += movedis;
              time = movedis / fanucmill.F;
              ttime+=time;
              m_RunnedTime += time;
              continue;

          }
          if(fanucmill.I != 0 || fanucmill.J != 0 || fanucmill.K != 0){

              st_ncIncPoint incpoint;
              em_Plane plane = (em_Plane)fanucmill.CG02;
              incpoint.i = fanucmill.I;incpoint.j = fanucmill.J;incpoint.k = fanucmill.K;
              movedis = getArcLength(start,end,incpoint,plane);
              m_MoveDistance += movedis;
              time = movedis / fanucmill.F;
              ttime+=time;
              m_RunnedTime += time;
              continue;

          }
        }
      //将当前行状态写入系统模态
    }

    return ttime;
}

double ncRunFanucMill::getToolRunTime(int ToolNo)
{
    QList<st_ProgInfo> ProList;
    ProList = doc->getProList();
    for(int k = 0; k < ProList.size(); k++ ){
        if(ProList.at(k).ProgType == pt_Main){
            //endRow = ProList.at(k).EndRow;
        }
    }
}

double ncRunFanucMill::getSubRunTime(int ProNo)
{
    QList<st_ProgInfo> ProList;
    ProList = doc->getProList();
    int start=0,end=0;
    double time;
    for(int k = 0; k < ProList.size(); k++ ){
        if(ProList.at(k).ProgType == pt_Main && ProList.at(k).ProgNo == ProNo){
           start =ProList.at(k).StartRow;
           end = ProList.at(k).EndRow;
        }
    }
    time = getRunningTime(start,end);
    return time;
}

QStringList ncRunFanucMill::getLineModalGcode(int LineNo)
{

}
/** 获取指定行的模态，返回模态结构
 * @brief ncRunFanucMill::getLineModel
 * @param LineNo  指定行号
 * @return  指定行的模态结构，这个模态是在全局模态的基础上增加了指定行的模态
 */
st_ncMillStatus ncRunFanucMill::getLineModel( int LineNo,st_ncMillStatus CurrentModel)
{
    //当前状态存放结构
    st_ncMillStatus tempModel=CurrentModel;
    //错误结构
    st_Error st;

    //处理line中的功能代码，保函G代码和M代码
    QMap <QChar,double> line=doc->getLineCode(LineNo);
    for(QMap <QChar,double>::iterator k = line.begin(); k != line.end(); k++){
       int Gbit=0;//G代码的状态；从0——32位；依次为：CG00(0),CG01(1),CG02(2),CG03(3),CG04(4),CG05(5),CG06(6),CG07(7),CG08(8),CGO9(9),CG10(10),CG11(11)
                                            //CG12(12),CG13(13),CG14(14),CG15(15),CG16(16),CG17(17),CG18(18),CG20(19),CG22(20),CG34(21);
                                            //A(22),B(23),C(23),D(24),E(25),F(26),G(27),H(28),I(29),J(30),K(31),L(32),P(33),PX(34),Q,AR,S,T,X,Y,Z
                                            // 取int型变量a的第k位 (k=0,1,2……sizeof(int))，即a>>k&1
                                            // 将int型变量a的第k位清0，即a=a&~(1 < <k)
                                            // 将int型变量a的第k位置1， 即a=a|(1 < <k)
       if(k.key() == 'G'){
          if(k.value() == 0 || k.value() == 1 || k.value() == 2 || k.value() == 3 || k.value() == 33 || k.value() == 75 || k.value() == 77 || k.value() ==78 || k.value() == 79){
              if(Gbit>>1&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG01 = k.value();
              Gbit=Gbit|(1<<1);
              cgbit=0;
          }
          if(k.value() == 4){
              if(Gbit>>0&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG00 = k.value();
              Gbit=Gbit|(1<<0);
          }
          if(k.value() == 15 || k.value() == 16){
              if(Gbit>>17&0x01 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG17=k.value();
              Gbit=Gbit|(1<<17);
          }
          if(k.value() >=17 && k.value() <= 19){
              if(Gbit>>2&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG02 = k.value();
              Gbit=Gbit|(1<<2);
          }
          if(k.value() == 20 || k.value() == 21 ){
              if(Gbit>>6&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG06 = k.value();
              Gbit=Gbit|(1<<6);
              cgbit=cgbit|(1<<6);
          }
          if(k.value() == 22 || k.value() == 23){
              if(Gbit>>4&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG04 = k.value();
              Gbit=Gbit|(1<<4);
          }
          if(k.value() == 40 || k.value() == 41 || k.value() == 42){
              if(Gbit>>7&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG07 = k.value();
              Gbit=Gbit|(1<<7);
          }
          if(k.value() == 40.1 || k.value() ==41.1|| k.value() == 42.1){
              if(Gbit>>18&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG18 = k.value();
              Gbit=Gbit|(1<<18);
          }
          if(k.value() == 43 || k.value() ==44 || k.value() == 49){
              if(Gbit>>8&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG08 = k.value();
              Gbit=Gbit|(1<<8) ;
          }
          if(k.value() == 50 || k.value() == 51){
              if(Gbit>>11&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG11 = k.value();
              Gbit=Gbit|(1<<11);
          }
          if(k.value() == 50.1 || k.value() == 51.1){
              if(Gbit>>22&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG22 = k.value();
              Gbit=Gbit|(1<<22) ;
          }
          if(k.value() >= 54 && k.value() <= 59){
              if(Gbit>>14&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG14 = k.value();
              Gbit=Gbit|(1<<14);
          }
          if(k.value() >=60 && k.value() <= 64){
              if(Gbit>>15&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG15 = k.value();
              Gbit=Gbit|(1<<15);
          }
          if(k.value() == 66 || k.value() == 67){
              if(Gbit>>12&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG12 = k.value();
              Gbit=Gbit|(1<<12);
          }
          if(k.value() == 68 || k.value() == 69){
              if(Gbit>>16&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG16 = k.value();
              Gbit=Gbit|(1<<16);
          }
          if(k.value() == 73 || k.value() == 74 || k.value() == 76 || k.value() <=89 && k.value() >= 80){
              if(Gbit>>9&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG09 = k.value();
              Gbit=Gbit|(1<<9);
          }
          if(k.value() == 80.4 || k.value() == 81.4){
              if(Gbit>>21&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG34 = k.value();
              Gbit=Gbit|(1<<21);
          }
          if(k.value() == 90 || k.value() == 91){
              if(Gbit>>3&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG03 = k.value() ;
              Gbit=Gbit|(1<<3);
          }
          if(k.value() == 93 || k.value() == 94 || k.value() == 95){
              if(Gbit>>5&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG05 = k.value();
              Gbit=Gbit|(1<<5);
          }
          if(k.value() == 96 || k.value() == 97){
              if(Gbit>>13&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG13 = k.value();
              Gbit=Gbit|(1<<13);
          }
          if(k.value() == 98 || k.value() == 99){
              if(Gbit>>10&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG10 = k.value();
              Gbit=Gbit|(1<<10);
          }
          if(k.value() == 160 || k.value() == 161){
              if(Gbit>>20&1 == 1){
                  st.ErrorID=0;
                  st.ErrorInfo="State is repeated";
                  st.ErrotLine=LineNo;
                  ErrList.append(st);
              }
              tempModel.CG20 = k.value();
              Gbit=Gbit|(1<<20);
          }
       }
       if(k.key() == 'M') {
           if(k.value() == 98){
               tempModel.M = k.value();
               k++;
               if(k.key() == 'P'){
                   tempModel.PG =k.value();
               }else{
                   st.ErrorID = 0;
                   st.ErrorInfo = "Code format error";
                   st.ErrotLine = LineNo;
                   ErrList.append(st);
               }
           }else{
               tempModel.M= k.value();
           }
       }
    }
 //Function Code Process

    //处理功能代码外的其他代码
    for(QMap <QChar,double>::iterator k = line.begin(); k != line.end(); k++){
       if(k.key() == 'X'){
           if(tempModel.CG00==4){
               tempModel.PX = k.value();
           }
           else{
               tempModel.X = k.value();
           }
       }
       if(k.key() == 'Y'){
           tempModel.Y = k.value();
       }
       if(k.key() == 'Z'){
           tempModel.Z = k.value();
       }
       if(k.key() == 'I'){
           tempModel.I = k.value();
       }
       if(k.key() == 'J'){
           tempModel.J = k.value();
       }
       if(k.key() == 'K'){
           tempModel.K = k.value();
       }
       if(k.key()=='H'){
           tempModel.H= k.value();
       }
       if(k.key()=='P'){
           tempModel.P=k.value();
       }
       if(k.key()== 'R'){
           if(tempModel.CG09 ==80){
               tempModel.aR = k.value();
           }
           else{
               tempModel.pR = k.value();
           }
       }
       if(k.key() =='S'){
           tempModel.S = k.value();
       }
       if(k.key()=='F'){
           tempModel.F=k.value();
       }
       if(k.key()=='T'){
           tempModel.T = k.value();
       }
       if(k.key()=='D'){
           tempModel.D = k.value();
       }
       if(k.key()=='E'){
           tempModel.E = k.value();
       }
       if(k.key()=='L'){
           tempModel.L = k.value();
       }
       if(k.key()=='Q'){
           tempModel.Q = k.value();
       }
    }
    return tempModel;
}

QMap<QChar, double> ncRunFanucMill::getLineModalMap(int LineNo)
{

}
