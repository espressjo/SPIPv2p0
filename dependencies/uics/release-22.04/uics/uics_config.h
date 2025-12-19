#ifndef UICS_CONFIG_H
#define UICS_CONFIG_H


#include <string>
#include <fstream>
#include <iostream>
#include "uics_number.h"

#define STRDEFAULT "NULL"
#define INTDEFAULT -999
#define DOUBLEDEFAULT -999.99

using namespace std;

string cfg_get_string(string c_file,string entry,string deflt=STRDEFAULT);
double cfg_get_double(string c_file,string entry,double deflt=DOUBLEDEFAULT);
int cfg_get_int(string c_file,string entry,int deflt=INTDEFAULT);
int cfg_get_bool(string c_file,string entry,bool *value);

string get_var(string line);//return the 1st word of a config line. 



#endif // UICS_CONFIG_H

