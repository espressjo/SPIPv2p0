#ifndef MACIE_FIRMWARE_H
#define MACIE_FIRMWARE_H

#include "hxrgerrcode.h"
#include "insthandle.h"
#include <iostream>
#include <macie.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <uics.h>

using namespace std;

// firmware structure
typedef struct {
  double version;
  int month;
  int day;
  int year;
} firmware_version;

int load_macie_firmware(instHandle *handle, int slot);
int get_firmware_version(instHandle *handle, firmware_version *fVersion);

// Command function
void get_firmware_info(instHandle *handle, cmd *cc);
void load_firmware(instHandle *handle, cmd *cc);

// Utility functions
std::string to_hex(unsigned int);

namespace macie_firmware {
bool isInt(std::string d);
}

#endif
