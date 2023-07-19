// compile with: g++ -g -o main main.cpp -lstdc++
// run with ./main.exe

#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;

tm getGMTTime();
double GAST();
double calculateJulianDate();
//double LHA;

int main() {
    const double longitude = 32;
    const double latitude = -117;
    const double declination = 0.0;
    const double rightAscension = 0.0;

    tm gmt = getGMTTime();
    cout << "Current time at greenwich is:" << gmt.tm_year << " " << gmt.tm_mon << " " << gmt.tm_mday 
    << " " << gmt.tm_hour << " " << gmt.tm_min << " " << gmt.tm_sec << endl;

    cout << "Current julian date is: " << calculateJulianDate() <<endl;

    //LHA(GAST(), rightAscension, longitude);
}

double calculateJulianDate(){
    tm gmt = getGMTTime();
    int year = gmt.tm_year + 1900;
    int month = gmt.tm_mon + 1;
    int day = gmt.tm_mday;
    double hour = gmt.tm_hour;
    double minute = gmt.tm_min;
    double second = gmt.tm_sec;

    double a = (double)year/100;
    double b = a/4;
    double c = 2-a+b;
    double e = 365.25*((double)year+4716);
    double f = 30.6001*((double)month+1);

    double fractionalDay = (double)hour / 24.0 + (double)minute / (24.0 * 60.0) + (double)second / (24.0 * 60.0 * 60.0);

    return c+day+e+f-1524.5+fractionalDay;
}

double GAST(double julianDate) {
    double JD_UT = julianDate;
    double JD_TT = JD_UT; // approximation, can be off by ~0.001
    double JD_0 = floor(JD_UT);
    if (JD_UT - JD_0 > 0.5) {
        JD_0 += 0.5;
    } else {
        JD_0 -= 0.5;
    }
    double H = (JD_UT - JD_0) * 24;

    double julianDateJ2000 = 2451545.0;
    double D_TT = JD_TT - julianDateJ2000;
    double D_UT = JD_UT - julianDateJ2000;
    double T = D_TT / 36525.0;

    double GMST = fmod(6.697375 + 0.065707485828 * D_UT + 1.0027379 * H + 0.0854103 * T + 0.0000258 * (T * T), 24);

    double omega = 125.04 - 0.052954 * D_TT;
    double L = 280.47 + 0.98565 * D_TT;
    double obliquity = 23.4393 - 0.0000004 * D_TT;

    double nutation = -0.000319 * sin(omega) - 0.000024 * sin(2 * L);
    double eqeq = nutation * cos(obliquity);

    return GMST + eqeq;
}

tm getGMTTime(){    
    time_t now = time(nullptr);
    tm *gmtTime = gmtime(&now);
    
    return *gmtTime;
}

double LHA(double gast, double a, double longitudeDeg){
    if(getGMTTime().tm_hour > 12) { 
        // you are west of greenwich
        return (gast - a) * 15 - longitudeDeg;
    } else {
        // you are east of greenwich
        return (gast - a) * 15 + longitudeDeg;
    }
    
}
