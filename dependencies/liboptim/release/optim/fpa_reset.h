#ifndef FPA_RESET_H
#define FPA_RESET_H

#include "asicio.h"
#include "register.h"
#include <string>

class reset : public asicio {
public:
  reset(unsigned long asicHandle, unsigned char asic_slct);
  int setNormal_clck();
  int setPixelbyPixel();
  int setLinebyLine();
  int setEnhanced_clck();
  int globalReset_up(bool enable);
  std::string to_string();

private:
  xreg h4017;
};

#endif
