#ifndef ASTRO_EXTRA_H
#define ASTRO_EXTRA_H


#include <uics.h>
#include <iostream>
#include "insthandle.h"


void exit_acq(instHandle *handle,cmd *cc);
void f2rampParam(instHandle *handle,cmd *cc);
void acq_status(instHandle *handle);
void init_status(instHandle *handle);
void getUniqueId(instHandle *handle);


#endif
