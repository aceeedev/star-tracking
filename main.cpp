// compile with: make
// run with ./main.exe

#define _USE_MATH_DEFINES

#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include "util.cpp"  

using namespace std;

double toRad(double deg);
double toDeg(double rad);
double fracHours(double hour, double minute, double second);

tm getGMTTime();
double getJulianDate(tm gmt);
double getGAST(double julianDate);
double getLHA(double gast, double a, double longitudeDeg);
double getAltitude(double latitudeRad, double declinationRad, double LHA);
double getAzimuth(double latitudeRad, double declinationRad, double LHA, double altitudeRad);

int main(int argc, char *argv[]) {
    // Inputs:
    double latitude = 32;
    double longitude = -117;
    double rightAscension = fracHours(11, 59, 25);
    double declination = fracDegrees(1, 54, 30);

    tm GMT = getGMTTime();

    if (argc == 10) {
        longitude = stod(argv[1]);
        latitude = stod(argv[2]);
        rightAscension = stod(argv[3]);
        declination = stod(argv[4]);
    }

    // Calculations:
    double julianDate = getJulianDate(GMT);
    double GAST = getGAST(julianDate);

    double LHA = getLHA(GAST, rightAscension, longitude);

    double altitude = toDeg(getAltitude(toRad(latitude), toRad(declination), toRad(LHA)));
    double azimuth = toDeg(getAzimuth(toRad(latitude), toRad(declination), toRad(LHA), toRad(altitude)));

    // Results:
    cout << "latitude: " <<  latitude << endl;
    cout << "longitude: " << longitude << endl;
    cout << "declination: " << declination << endl;
    cout << "rightAscension: " << rightAscension << endl;
    cout << "\n" << endl;
    cout << "GMT: "  << GMT.tm_mon + 1 << "/" << GMT.tm_mday << "/" << GMT.tm_year + 1900 << " " << GMT.tm_hour << ":" << GMT.tm_min << ":" << GMT.tm_sec << endl;
    cout << "julianDate: " << setprecision(15) << julianDate << endl;
    cout << "GAST: " << setprecision(15) << GAST << endl;
    cout << "LHA: " << setprecision(15) << LHA << endl;
    cout << "\n" << endl;
    cout << "altitude: " << setprecision(15) << altitude << endl;
    cout << "azimuth: " << setprecision(15) << azimuth << endl;
}


/**
 * Returns the julian date based off of GMT.
 *
 * @param gmt The Greenwich Mean Time to convert into a julian date.
 * @return the julian date in number of days.
 */
double getJulianDate(tm gmt) {
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
    
    double fractionOfDay = (hour-12) / 24.0 + minute / 1440.0 + second / 86400.0;
    
    double julianDate = jdn + fractionOfDay;
    return julianDate;
}

/**
 * Returns the Greenwich Apparent Sidereal Time based off of a julian date.
 *
 * @param julianDate The UT1 julian date.
 * @return gast in hours.
 */
double getGAST(double julianDate) {
    double JD_UT = julianDate;
    double JD_TT = JD_UT; // approximation, can be off by ~0.001
    double JD_0 = floor(JD_UT);
    if (JD_UT - JD_0 > 0.5) {
        JD_0 += 0.5;
    } else {
        JD_0 -= 0.5;
    }
    double H = (JD_UT - JD_0) * 24.0;

    double julianDateJ2000 = 2451545.0;
    double D_TT = JD_TT - julianDateJ2000;
    double D_UT = JD_UT - julianDateJ2000;
    double T = D_TT / 36525.0;

    double GMST = fmod(6.697375 + (0.065707485828 * D_UT) + (1.0027379 * H) + (0.0854103 * T) + (0.0000258 * T * T), 24);

    double omega = 125.04 - (0.052954 * D_TT);
    double L = 280.47 + (0.98565 * D_TT);
    double obliquity = 23.4393 - (0.0000004 * D_TT);

    double nutation = (-0.000319 * sin(omega)) - (0.000024 * sin(2 * L));
    double eqeq = nutation * cos(obliquity);

    return GMST + eqeq;
}

/**
 * Returns the current Greenwich Mean Time (GMT).
 *
 * @return current GMT time.
 */
tm getGMTTime(){    
    time_t now = time(nullptr);
    tm *gmtTime = gmtime(&now);
    
    return *gmtTime;
}

/**
 * Returns the local hour angle (LHA) in degrees.
 *
 * @param gast The Greenwich Apparent Sidereal Time in hours.
 * @param a The right ascension of the star in hours.
 * @param longitudeDeg The longitude of the observer in degrees.
 * @return LHA in degrees.
 */
double getLHA(double gast, double a, double longitudeDeg) {
    /*if(getGMTTime().tm_hour > 12) { 
        // you are west of greenwich
        return (gast - a) * 15 - longitudeDeg;
    } else {
        // you are east of greenwich
        return (gast - a) * 15 + longitudeDeg;
    }*/
    return ((gast - a) * 15) + longitudeDeg;
}

/**
 * Returns the altitude value of a star in radians.
 *
 * @param latitudeRad The latitude of the observer in radians.
 * @param declinationRad The declination of the star in radians.
 * @param LHA The local hour angle in radians.
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
 * @param LHA The local hour angle in radians.
 * @return azimuth in radians of the star within the range 0 to 2 pi.
 */
double getAzimuth(double latitudeRad, double declinationRad, double LHA, double altitudeRad) {
    double azimuthRad = atan2(-1 * sin(LHA),  tan(declinationRad) * cos(latitudeRad) - sin(latitudeRad) * cos(LHA));
    
    if (azimuthRad < 0) {
        azimuthRad += (2 * M_PI);
    }

    return azimuthRad;
}
