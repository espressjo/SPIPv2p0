#ifndef TELEMETRY_H
#define TELEMETRY_H
#include "register.h"
#include <string>
#include <unistd.h>
#include <vector>

#define TELOK 0
#define TELNOK -1

int telemetry_mux(unsigned long handle, unsigned char slctASIC, uint16_t mux,
                  double *tension, double *current);

int telemetry_asic(unsigned long handle, unsigned char slctASIC,
                   std::vector<double> *tension, std::vector<double> *current);

int telemetry_hxrg(unsigned long handle, unsigned char slctASIC,
                   std::vector<double> *tension, std::vector<double> *current);
/*
class telemetry {
public:
  telemetry(unsigned long asicHandle, unsigned char asic_slct, uint mux_t,
            bool reconf_after_set_mux = true);
  telemetry(unsigned long asicHandle, unsigned char asic_slct,
            bool reconf_after_set_mux = true);
  int status;
  int read_all_sca(std::vector<double> *voltage, std::vector<double> *current,
                   std::vector<std::string> *sca_name,
                   std::vector<std::string> *sca_def);
  int setMux(uint mux);
  int _telemetry(double *v, double *c);

private:
  int r_setMux();
  unsigned long handle;
  unsigned char asicslct;

  int reconf();
  void r_delay(int ms);
  uint mux;
  bool rasm;
  int get_reference_resistance();
  int get_value(uint16_t addr, xreg *reg);
  double ref_res;
  int read_all_analogue();
  std::vector<double> v;
  std::vector<double> c;

  const std::vector<std::string> sca = {"Vreset",     "Dsub",      "VbiasGate",
                                        "VbiasPower", "CellDrain", "Drain",
                                        "VDDA",       "VDD"};
  const std::vector<std::string> sca_def_t = {
      "Detector reset voltage",
      "Detector substrate voltage",
      "Gate voltage of SCA internal current sources",
      "Source voltage for SCA internal current sources",
      "Celldrain (drain node for pixel source follower)",
      "Drain (drain node for output buffer)",
      "Analog SCA supply voltage",
      "Digital SCA power supply"};
};
*/
#endif // TELEMETRY_H
