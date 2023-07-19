#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>

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
