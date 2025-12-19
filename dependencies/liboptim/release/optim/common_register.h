#ifndef COMMON_REGISTER_H
#define COMMON_REGISTER_H

#include "asic_register.h"
#include "asicio.h"
#include <cstdint>
#include "register.h"
#include <string>

class c_register : public asicio {

public:
  c_register(uint16_t regaddr, std::string regname, unsigned long asicHandle,
             unsigned char asic_slct, std::string correspondingCname = "",
             int Tmux = -1);
  std::string registername;
  std::string registerCname;
  uint16_t mux;
  double DACVoltage;
  uint16_t DACRange;
  uint16_t DACpower;
  uint16_t CompSelect;
  uint16_t Isink_En;
  uint16_t Currentpower;
  std::string to_string();
  void setDACpowerDown();
  void setDACpowerUp();

  void setVoltage(double voltage, bool autorange = true);
  void setRangeHi();
  void setCurrentPowerUp();
  void setCurrentPowerDown();
  void setRangeLow();
  void setCompSelect(uint16_t c);
  void Isink_enable();
  void Isink_disable();
  void extract();
  std::string telemetry_t();
  int apply(bool reconf = true);
  // set current on register addr+1
  // by default currant is expressed in microamp
  double CurSelect_Fine;
  uint16_t CurSelect_Coarse;
  double DACBufferCurrent;
  void setCurrentRange(uint16_t range);
  void setCurrent(double uA);
  void setCurrent(
      double uA,
      uint16_t range); // will return true if range and current are compatible
  void setCurSelect(uint16_t select);
  // void setDACbuffer(uint16_t select);
  void setDACbuffer(double mA);
  int update();
  asic_register_t get_asic_register_t();

  xreg addr_even, addr_odd;

private:
  const double DacCurrentCoeff = 0.0000001;
  const double DacBufferCurrent_a = 0.9523492063492063;
  const double DacBufferCurrent_b = 0.002;
  const double DacVoltageCoeff = 0.0019550342130987292;
  unsigned long handle;
  unsigned char slctASIC;
  bool current_is_inside_range(double uA);
};

#endif
