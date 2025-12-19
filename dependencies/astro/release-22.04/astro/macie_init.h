#ifndef MACIE_INIT_H
#define MACIE_INIT_H

#include "insthandle.h"
#include <liboptim.h>
#include <macie.h>
#include <uics.h>

using namespace std;

void IP2MACIEIPList(MACIE_IpAddr *mylist, std::string IP);

int openMacie(instHandle *handle);
void hrdw_initialization(instHandle *handle, cmd *cc);
int initASIC(instHandle *handle);
void i_handle(instHandle *handle);
int ASIC_user_setup(instHandle *handle);

double get_range(string key, double _default_value = -9999.99,
                 double min = -1000000.00, double max = 1000000.00);
int get_range(string key, int _default_value = -9999, int min = -1000000,
              int max = 1000000);
int get_range_uint(string key, uint _default_value = 0, uint min = 0,
                   int uint = 65535);
void eprint(string msg);

extern const char* get_configfile(void);
extern void set_configfile(const char* file);
extern const char* get_basepath(void);
extern void set_basepath(const char* path);







#endif
