#ifndef HDWR_STATUS_H
#define HDWR_STATUS_H
#include "insthandle.h"
#include "optim/register.h"
#include <stdint.h>
#pragma pack(push, 1) // force padding
typedef struct {

  uint16_t
      status; //<0>halttrigger,<1>reconfiguring?<2>imready?<3>sim?<4>window?\
              //<5>onGoing?<6>
  uint16_t hxrgstatus;
  uint8_t UICS_state;
  uint8_t UICS_status;

} hdwr_status_t;
#pragma pack(pop)
hdwr_status_t getStatus(instHandle *handle);

#endif
