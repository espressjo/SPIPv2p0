#ifndef ASICIO_H
#define ASICIO_H

#include "register.h"
#include <macie.h>
#include <stdio.h>
#include <unistd.h>
#define IOK 0
#define IONOK -1

class asicio {
public:
  asicio(unsigned long asicHandle, unsigned char asic_slct);
  int read(uint16_t addr, uint16_t *value);
  int read(xreg *addr);
  int write(xreg *addr, bool reconf = true);
  int write(uint16_t addr, uint16_t value, uint16_t *readback,
            bool reconf = true);
  int write(uint16_t addr, uint16_t value, bool reconf = true);

private:
  unsigned long handle;
  unsigned char slctASIC;
  int reconfigure(int timeout = 12);
};

#endif // ASICIO_H
