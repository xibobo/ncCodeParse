#ifndef NCDOCISOGCODE_H
#define NCDOCISOGCODE_H

#include "common.h"
#include "ncdocument.h"
#include "QMessageBox"
#include "QTextBlock"

class ncDocISOGcode :public ncDocument
{
private:
    //标记主程序中子程序调用的位置
    QMap<int,int> subCall;//<程序号，所在行>

    //解析line字符串为nc代码并返回
    QMap<QChar,double> parseCodeLine(const QString line);
    //处理NC文件（重写）
    void dealNcInfo(QMap<QChar,double> list) override;
    //处理M98代码
    void dealM();



public:
    ncDocISOGcode();
    //按行号获取代码键值对
    QMap<QChar,double> getLineCode(int LineNo);
    //NC文件读取
    void ncInfoRead();
};

#endif // NCDOCISOGCODE_H
