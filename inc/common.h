#ifndef COMMON_H
#define COMMON_H

#include <QString>
struct st_ncPoint{
    double x;
    double y;
    double z;

    st_ncPoint(){
        x=0;
        y=0;
        z=0;
    }
};

struct st_ncIncPoint{
    double i;
    double j;
    double k;
    st_ncIncPoint(){
        i=0;
        j=0;
        k=0;
    }

};

typedef enum{
    G17=17,
    G18=18,
    G19=19
}em_Plane;

typedef struct{
    int ErrorID;
    QString ErrotLine;
    QString ErrorInfo;
}st_Error;
#endif // COMMON_H
