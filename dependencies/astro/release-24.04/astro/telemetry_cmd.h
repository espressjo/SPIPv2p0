#ifndef TELEMETRY_CMD_H
#define TELEMETRY_CMD_H
#include "insthandle.h"
#include <liboptim.h>
#include <uics.h>

std::string d2string(double a, int precision);
void r_telemetry(instHandle *handle, cmd *cc);
#endif
