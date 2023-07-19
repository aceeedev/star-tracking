// compile with: g++ -g -o main main.cpp -lstdc++
// run with ./main.exe

#define _USE_MATH_DEFINES

#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;

tm getGMTTime();
double GAST();
double calculateJulianDate();
//double LHA;
double getAltitude(double latitudeRad, double declinationRad, double LHA);
double getAzimuth(double latitudeRad, double declinationRad, double LHA);

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
    int hour = gmt.tm_hour;
    int minute = gmt.tm_min;
    int second = gmt.tm_sec;


    int a = (14-month)/12;
    int y = year + 4800 - a;
    int m = month +12 * a - 3;

    int jdn = day + (153* m +2) / 5 + 365 * y + y /4-y/100+y/400-32045;
    
    double fractionOfDay = (hour - 12) / 24.0 + minute / 1440.0 + second / 86400.0;
    
    double julianDate = jdn + fractionOfDay;
    return julianDate;
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

/**
 * Returns the altitude value of a star in radians.
 *
 * @param latitudeRad The latitude of the observer in radians.
 * @param declinationRad The declination of the star in radians.
 * @param LHA The local hour angle in radian.
 * @return altitude in radians of the star within the range -pi to pi.
 */
double getAltitude(double latitudeRad, double declinationRad, double LHA) {
    return asin(cos(LHA) * cos(declinationRad) * cos(latitudeRad) + sin(declinationRad) * sin(latitudeRad));
}

/**
 * Returns the azimuth value of a star in radians.
 *
 * @param latitudeRad The latitude of the observer in radians.
 * @param declinationRad The declination of the star in radians.
 * @param LHA The local hour angle in radian.
 * @return azimuth in radians of the star within the range 0 to 2 pi.
 */
double getAzimuth(double latitudeRad, double declinationRad, double LHA) {
    double azimuth = atan2(-1 * sin(LHA),  tan(declinationRad) * cos(latitudeRad) - sin(latitudeRad) * cos(LHA));

    if (azimuth < 0) {
        azimuth += 2 * M_PI;
    }

    return azimuth;
}
