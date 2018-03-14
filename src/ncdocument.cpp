#include "../inc/ncdocument.h"
#include <QMessageBox>
#include <QDebug>


/**
 * @brief ncDocument::getProList
 * @return ProList程序信息表
 */
QList<st_ProgInfo> ncDocument::getProList()
{
    return ProgList;
}

/**
 * @brief ncDocument::getTooList
 * @return ToolList刀具信息表
 */
QList<st_ToolInfo> ncDocument::getTooList()
{
    return ToolList;
}

/**
 * @brief ncDocument::getErrList
 * @return errList 错误信息表
 */
QList<st_Error> ncDocument::getErrList()
{
    return errList;
}
/**
 * @brief ncDocument::getXmaxValue
 * @return X最大值
 */
double ncDocument::getXmaxValue()
{
    return Xmax;
}
/**
 * @brief ncDocument::getXminValue
 * @return X最小值
 */
double ncDocument::getXminValue()
{
    return Xmin;
}
/**
 * @brief ncDocument::getYmaxValue
 * @return Y最大值
 */
double ncDocument::getYmaxValue()
{
    return Ymax;
}
/**
 * @brief ncDocument::getYminValue
 * @return Y最小值
 */
double ncDocument::getYminValue()
{
    return Ymin;
}
/**
 * @brief ncDocument::getZmaxValue
 * @return Z最大值
 */
double ncDocument::getZmaxValue()
{
    return Zmax;
}
/**
 * @brief ncDocument::getZminValue
 * @return Z最小值
 */
double ncDocument::getZminValue()
{
    return Zmin;
}
/**
 * @brief ncDocument::getAmaxValue
 * @return A最大值
 */
double ncDocument::getAmaxValue()
{
    return Amax;
}
/**
 * @brief ncDocument::getAminValue
 * @return A最小值
 */
double ncDocument::getAminValue()
{
    return Amin;
}
/**
 * @brief ncDocument::getBmaxValue
 * @return B最大值
 */
double ncDocument::getBmaxValue()
{
    return Bmax;
}
/**
 * @brief ncDocument::getBminValue
 * @return B最小值
 */
double ncDocument::getBminValue()
{
    return Bmin;
}
/**
 * @brief ncDocument::getCmaxValue
 * @return C最大值
 */
double ncDocument::getCmaxValue()
{
    return Cmax;
}
/**
 * @brief ncDocument::getCminValue
 * @return C最小值
 */
double ncDocument::getCminValue()
{
    return Cmin;
}


ncDocument::ncDocument()
{

}
/**
 * @brief ncDocument::ncOpenFile，打开nc文件
 * @param filename，文件名
 */
void ncDocument::ncOpenFile(QString filename)
{
    QFile fp(filename);
        //检查文件是否存在
        if(!fp.exists()){
          throw QString("the file not exist!");
        }
        if(!fp.open(QIODevice::Text | QIODevice::ReadWrite))
        {
            throw QString(tr("Open file Error!"));
        }


     setPlainText(fp.readAll());

}



