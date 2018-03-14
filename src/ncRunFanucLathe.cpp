#include "../inc/ncRunFanucLathe.h"

ncRunFanucLathe::ncRunFanucLathe(QObject *parent):ncRunBase(parent)
{

    ModelName="Fanuc Lather Serial B";
    Manufacturer="Fanuc";
    MachineType="Lathe";

}

double ncRunFanucLathe::getRunningTime(int startLine, int endLine)
{
    int endRow;
    if(endLine =-1)
        endRow = doc->blockCount();
    for(int i=startLine;i<endRow;i++){

       //QMap<QChar,double> line= doc->getLineCode(i);

    }

}

double ncRunFanucLathe::getToolRunTime(int ToolNo)
{
    QList<int> Rows;
    QList<st_ToolInfo>::iterator it = doc->getTooList().begin();

}

double ncRunFanucLathe::getSubRunTime(int ProNo)
{
    //int startRow,endRow;
    for(QList<st_ProgInfo>::iterator it = doc->getProList().begin();it!=doc->getProList().end();it++){
       st_ProgInfo pi = *it;
       if(pi.ProgNo=ProNo){
            return	getRunningTime(pi.StartRow,pi.EndRow);
       }

    }

    return 0;
}

QStringList ncRunFanucLathe::getLineModalGcode(int LineNo)
{

}

QMap<QChar, double> ncRunFanucLathe::getLineModalMap(int LineNo)
{

}
