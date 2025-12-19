#ifndef UICS_NUMBER_H
#define UICS_NUMBER_H
#include <cctype>
#include <cmath>
#include <limits>
#include <string>

using namespace std;
bool uics_is_double(string str);
bool uics_is_int(string str);
double
stringToDouble(const std::string &str,
               double defaultValue =
                   -999.999); // handle scientific number as well e.g., 12e-4
int stringToInt(const std::string &str, int defaultValue = -999);
#endif
