#ifndef ASTR_STD_CMD_H
#define ASTR_STD_CMD_H

#include "astro_error_code.h"
#include "insthandle.h"
#include "macie_acquisition.h"
#include <uics.h>
void closeMacie(instHandle *handle, cmd *cc);
void c_set_simulation(instHandle *handle, cmd *cc);
void setParam(instHandle *handle, cmd *cc);
void set_data_product(instHandle *handle, cmd *cc);
void uploadMCD(instHandle *handle, cmd *cc);
#endif
