# Star Position Calculator

This is a C++ program that calculates the position of a star in the sky for a given geographic location (latitude and longitude). It provides the altitude and azimuth of the star (horizontal coordinate system), which are essential for astronomical observations. The program uses Greenwich Mean Time (GMT) and various astronomical formulas to perform these calculations.

## Getting Started

Note: This project was initially designed to run on Windows with the location near San Diego, CA.

### Prerequisites

To compile and run this program, you need a C++ compiler. If you don't have one installed, you can download and install [GCC](https://gcc.gnu.org/).

### Compilation

You can compile the program using the provided Makefile. Simply run the following command in your terminal:

```bash
make
```

### Usage

After compilation, you can run the program with the following command:

```bash
./main.exe [latitude] [longitude] [rightAscension] [declination]
```

Where latitude and longitude are the coordinates of the observer and the rightAscension and declination are the equatorial coordinates of the star you want to locate.

If you run the program without any arguments, it will use the default values for latitude, longitude, right ascension, and declination that are in the source code. However, you can specify these values as command-line arguments to calculate the star's position for a specific location and star.

## Program Overview

The program performs the following steps:

1. Obtains Greenwich Mean Time (GMT) using the system time.
2. Optionally, accepts latitude, longitude, right ascension, and declination from the command line.
3. Calculates the Julian Date based on GMT.
4. Calculates the Greenwich Apparent Sidereal Time (GAST) based on the Julian Date.
5. Calculates the Local Hour Angle (LHA) of the star.
6. Determines the altitude and azimuth of the star using the calculated values.

## Results

The program provides the following results:

- Latitude of the observer.
- Longitude of the observer.
- Right Ascension of the star.
- Declination of the star.
- GMT (Greenwich Mean Time).
- Local time adjusted for the observer's longitude.
- Julian Date (UT1).
- Julian Date (TT) approximation.
- Greenwich Apparent Sidereal Time (GAST).
- Local Hour Angle (LHA) in degrees.
- Altitude of the star in degrees.
- Azimuth of the star in degrees.

## Unit Tests

The project also includes a [tester file](tester.py) which can run a unit test for either the moon or all the planets by web scraping the actual values. Make sure to edit the latitude and longitude values in the main function. 

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE.md) file for details.

## Contributions

Created in collaboration by Andrew Collins and [Kanvar Soin](https://www.linkedin.com/in/kanvar-soin-ba94101b1).


## Acknowledgments

This program is based on astronomical algorithms and formulas that can be found from the following sources:

- The Astronomical Almanac
- [Navy - Astronomical Applications Department
](https://aa.usno.navy.mil/faq/alt_az)
