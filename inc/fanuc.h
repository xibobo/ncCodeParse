#ifndef FANUC_H
#define FANUC_H

/** Nc Status Struct ,record machine running status
 * infomation .
 */
struct st_ncStatus{
    double CG01;//运动方式G00/G01/G02/G03
    double CG02;//平面选择G17/G18/G19
    double CG03;//绝对相对指令 G90/G91
    double CG04;//
    double CG05;
    double CG06;
    double CG07;
    double CG08;
    double CG09;
    double CG10;
    double CG11;
    double CG12;
    double CG13;
    double CG14;
    double CG15;
    double CG16;
    double CG17;
    double CG18;
    double CG20;
    double CG22;
    double CG34;
    double A;
    double B;
    double C;
    double D;
    double E;
    double F;
    double H;
    double I;
    double J;
    double K;
    double L;
    double M;
    double P;
    double Q;
    double aR;
    double pR;
    double S;
    double T;
    double X;
    double Y;
    double Z;

    st_ncStatus(){
        CG01=0;
        CG02=17;
        CG03=90;
        CG04=;
        CG05=;
        CG06=;
        CG07=;
        CG08=;
        CG09=;
        CG10=;
        CG11=;
        CG12=;
        CG13=;
        CG14=;
        CG15=;
        CG16=;
        CG17=;
        CG18=;
        CG20=;
        CG22=;
        CG34=;
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

#endif // FANU7C_H
