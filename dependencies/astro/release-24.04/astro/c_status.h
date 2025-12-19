#ifndef C_STATUS_H
#define C_STATUS_H

#include "insthandle.h"
#include <string>
#include <uics.h>

using namespace std;

void cmd_status(instHandle *handle);
string p_handle_info(instHandle *handle);
string p_handle_preamp(instHandle *handle);
string p_handle_macie(instHandle *handle);
string p_handle_acq(instHandle *handle);
void cmd_status_b64(instHandle *handle);
#endif
