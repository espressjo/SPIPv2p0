#ifndef ASIC_PREAMP_H
#define ASIC_PREAMP_H

#include "asicio.h"
#include "preamp_t.h"
#include "register.h"
#include <string>

class preamp : public asicio {

public:
  preamp(unsigned long asicHandle, unsigned char asic_slct);
  int set_opTempCold();
  int set_opTempWarm();
  int setResetRow();
  int setResetFrame();
  int setKTC(bool enable);
  int setCompCap(uint16_t setting);
  int setPreAmpLowPass(uint16_t setting);
  int setGain(uint16_t g);
  int buff_det_output();
  int ubuff_det_output();
  int preamp_cur_source_up();
  int preamp_cur_source_down();
  int H4RG_specific_buffmode(bool enable);
  int setSingleEnded();
  int setDifferential();
  int setGnd();
  std::string to_string();
  asic_preamp_t get_info();

private:
  int update();
  xreg h401a, h401b, h4015, h4016, h4054, h4055, h5100, h5101, h5102, h4019,
      h6100;

  std::string cCap(uint16_t c);
  const std::string Tcode[16] = {"Padp",     "Padn",  "InPCommon", "InNCommon",
                                 "VrefMain", "Vref1", "Vref2",     "VrefMid",
                                 "VC",       "VDAC",  "Gndpin",    "Padn8",
                                 "Padp8",    "",      "",          ""};
};
#endif
