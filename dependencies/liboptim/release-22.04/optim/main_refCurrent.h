#ifndef MAIN_REFCURRENT_H
#define MAIN_REFCURRENT_H
#include "asicio.h"
#include "register.h"
#include <string>

class Main_RefCurrent : public asicio

{
public:
  Main_RefCurrent(unsigned long asicHandle, unsigned char asic_slct);
  int setRefCurrent(double V);
  double getRefCurrent();
  std::string to_string();

private:
  unsigned long handle;
  unsigned char slctASIC;
  xreg addr;
  int update();
  bool safeblock;
};

#endif
