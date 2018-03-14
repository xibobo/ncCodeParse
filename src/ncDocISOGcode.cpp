#include "../inc/ncDocISOGcode.h"

ncDocISOGcode::ncDocISOGcode()
{

}
/**
 * @brief ncDocISOGcode::getLineCode获取行代码
 * @param LineNo 行号
 * @return 返回map类型的List
 */
QMap<QChar, double> ncDocISOGcode::getLineCode(int LineNo)
{
     return parseCodeLine(findBlockByLineNumber(LineNo).text());

}

void ncDocISOGcode::ncInfoRead()
{
    QMap<QChar,double> CodeList;
    for(QTextBlock it =this->begin();it!= this->end();it=it.next()){
        //获取行号
        lineno=it.blockNumber();
        try{

            //检查是否是空文本
            if(it.text().isEmpty())
                continue;
            //nc代码行处理
            CodeList=parseCodeLine(it.text());
            //处理NC文件信息
            dealNcInfo(CodeList);

        }catch(QString e){

            QMessageBox::about(NULL,"EXCEP",e);

        }
    }
    //处理M98代码
    dealM();
}
/**
 * @brief ncDocument::AnsysCodeLine,分解line中的Nc代码并按照QStringList 返回。
 * @param line :NC Code Line ;
 * @return  QStringList : All NC Code int the Line;
 */
QMap<QChar, double> ncDocISOGcode::parseCodeLine(const QString line)
{
    QString str =line;
    str.remove(" ");//去除空格
    str.toUpper();//转换为大写
    QChar key;
    double value;
    QMap<QChar,double>  list;
    QString word="";
    for(int i =0;i<str.length();i++){
        QChar c =str.at(i);
        //处理含（和%的行
        if(c=='(' || c=='%'){
           return list;
        }
        if(c<='Z' && c>='A'){
            if(!key.isNull()){
                value= word.toDouble();
                //将键值对压入容器中
                list.insertMulti(list.constEnd(),key,value);
            }
            word.clear();
            key=c;
        }else  if(c<'9',c>='0'||c=='-'||c=='.'){
            if(key.isNull())
                throw QString("NC line format Error");
            word.append(c);
        }
     }
    value= word.toDouble();
    list.insertMulti(list.constEnd(),key,value);
    //list.append(word);
    return list ;
}
/**
 * @brief ncDocument::dealM处理呼叫子程序的所在行
 */
void ncDocISOGcode::dealM()
{
    for(QMap<int, int>::iterator i=subCall.begin();i!=subCall.end();i++){

        for(int j=0;j<ProgList.count();j++){
            //取出结构体变量进行赋值
            st_ProgInfo t= ProgList.at(j);
            if(t.ProgNo==i.key()){

                t.CallPos.append(i.value());
                ProgList.removeAt(j);
                ProgList.append(t);

            }
        }

    }
}
/**
 * @brief ncDocISOGcode::dealNcInfo获取NC文件信息
 * @param list map类型的表
 * @return 返回map list
 */
void ncDocISOGcode::dealNcInfo(QMap<QChar, double> list)
{
    st_ProgInfo proInfo;
    st_ToolInfo toolInfo;
    st_Error ncError;
    for(QMap<QChar,double>::iterator k =list.begin();k != list.end();k++){

        switch (k.key().toLatin1()){
            case 'O':
            {

            for(int i=0;i<ProgList.size();i++){
                if(ProgList.at(i).EndRow==-1){
                    //取出结构体变量进行赋值
                    st_ProgInfo t = ProgList.at(i);
                    t.EndRow=lineno;
                    t.ProgType=pt_unknown;
                    ProgList.removeAt(i);
                    ProgList.append(t);

                    ncError.ErrorID=lineno;
                    ncError.ErrotLine=lineno;
                    ncError.ErrorInfo=tr("the program is error");
                    errList.append(ncError);
                    break;
                }
            }
            proInfo.ProgNo=k.value();
            proInfo.StartRow=lineno;
            ProgList.append(proInfo);
            break;
            }
            case 'M':
            {
                if(k.value()==98){
                   k++;
                   int key=k.value();
                   int value=lineno;
                   subCall.insertMulti(key,value);
                }else if(k.value()==99){
                    for(int i=0;i<ProgList.size();i++){
                        if(ProgList.at(i).EndRow==-1){
                            st_ProgInfo t = ProgList.at(i);//取出结构体变量进行赋值
                            t.EndRow=lineno;
                            t.ProgType=pt_Sub;
                            ProgList.removeAt(i);
                            ProgList.append(t);
                        }
                    }
                }else if(k.value()==30||k.value()==2){
                    for(int i=0;i<ProgList.size();i++){
                        if(ProgList.at(i).EndRow==-1){
                            st_ProgInfo t = ProgList.at(i);//取出结构体变量进行赋值
                            t.EndRow=lineno;
                            t.ProgType=pt_Main;
                            ProgList.removeAt(i);
                            ProgList.append(t);
                        }
                    }
                }
                break;
            }
            case 'T':
            {
                if(ToolList.size() != NULL){
                   for(int i = 0; i < ToolList.size(); i++){
                       if(i == ToolList.size()-1){
                           st_ToolInfo t1 = ToolList.at(i);
                           t1.EndRow = lineno;
                           ToolList.removeAt(i);
                           ToolList.append(t1);
                       }
                    }
                }
                toolInfo.ToolNo=k.value();
                toolInfo.InRow = lineno;
                toolInfo.StartRow = lineno;
                ToolList.append(toolInfo);
               break;
            }
            case 'X':
            {
                if(k.value()>Xmax)
                    Xmax=k.value();
                if(k.value()<Xmin)
                    Xmin=k.value();
                break;
            }
            case 'Y':
            {
                if(k.value()>Ymax)
                    Ymax=k.value();
                if(k.value()<Ymin)
                    Ymin=k.value();
                break;
            }
            case 'Z':
            {
                if(k.value()>Zmax)
                    Zmax=k.value();
                if(k.value()<Zmin)
                    Zmin=k.value();
                break;
            }
            case 'A':
            {
                if(k.value()>Amax)
                    Amax=k.value();
                if(k.value()<Amin)
                    Amin=k.value();
                break;
            }
            case 'B':
            {
                if(k.value()>Bmax)
                    Bmax=k.value();
                if(k.value()<Bmin)
                    Bmin=k.value();
                break;
            }
            case 'C':
            {
                if(k.value()>Cmax)
                    Cmax=k.value();
                if(k.value()<Cmin)
                    Cmin=k.value();
                break;
           }
        }
    }
}
