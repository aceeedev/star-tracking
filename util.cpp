#define _USE_MATH_DEFINES
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include <cmath>
#include <ctime>
#include <iostream>

using namespace std;

/**
 * Returns an angle converted from degrees to radians.
 *
 * @param deg An angle in degrees.
 * @return the angle inputted in radians.
 */
double toRad(double deg) {
    return deg * (M_PI / 180.0);
}

/**
 * Returns an angle converted from radians to degrees.
 *
 * @param rad An angle in radians.
 * @return the angle inputted in degrees.
 */
double toDeg(double rad) {
    return rad * (180.0 / M_PI);
}

/**
 * Returns the number of hours with a fractional part from a time in military time.
 *
 * @param hour The hour from 0 to 23.
 * @param minute The minute from 0 to 59.
 * @param second The second from 0 to 59.
 * @return number of hours as a fractional number.
 */
double fracHours(double hour, double minute, double second) {
    return hour + (minute / 60.0) + (second / 3600.0);
}

/**
 * Returns the fractional degree by converting arcminutes and arcseconds into degrees.
 *
 * @param degree The hour from 0 to 23.
 * @param arcminute The minute from 0 to 59.
 * @param arcsecond The second from 0 to 59.
 * @return number of hours as a fractional number.
 */
double fracDegrees(double hour, double arcminute, double arcsecond) {
    return hour + (arcminute / 60.0) + (arcsecond / 3600.0);
}

/**
 * Returns a tm based off of the parameters.
 *
 * @param year The year.
 * @param month The month from 1 to 12.
 * @param day The day from 1 to 31.
 * @param hour The hour from 0 to 23.
 * @param minute The minute from 0 to 59.
 * @param second The second from 0 to 60.
 * @return tm created from the parameters.
 */
tm createTM(int year, int month, int day, int hour, int minute, int second) {
    tm tm;

    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;

    return tm;
}
