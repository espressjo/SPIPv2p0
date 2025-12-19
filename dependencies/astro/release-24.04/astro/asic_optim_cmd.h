#ifndef ASIC_OPTIM_CMD_H
#define ASIC_OPTIM_CMD_H

#include "insthandle.h"
#include <liboptim.h>
#include <string>
#include <uics.h>

using namespace std;
void asic_register_optimisation(instHandle *handle, cmd *cc);
void asic_register_io(instHandle *handle, cmd *cc);
void asic_mainRefCurrent(instHandle *handle, cmd *cc);

int decodeStatus(char **decode, int length);
namespace asic_optim {
bool isRegister(std::string &str);
bool isInt(std::string &str);
uint16_t register2uint(std::string str);
} // namespace asic_optim

#endif
