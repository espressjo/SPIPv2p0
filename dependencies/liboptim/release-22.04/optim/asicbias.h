#ifndef ASICBIAS_H
#define ASICBIAS_H
#include "common_register.h"
#include "macie.h"
#include "register.h"
#include "telemetry.h"
#include <array>
#include <string>
#include <unistd.h>
#include <vector>

class asic {
public:
  asic(unsigned long asicHandle, unsigned char asic_slct);
  ~asic();
  // internal register
  std::string ls_registers();
  c_register *Vref1;
  c_register *Vref2;
  c_register *VrefMain;
  c_register *Vpcf;
  c_register *Vrp;
  c_register *Vrn;
  c_register *VmidRef;
  c_register *Vcm;
  // external register
  c_register *Vreset;
  c_register *Dsub;
  c_register *VbiasGate;
  c_register *VbiasPower;
  c_register *CellDrain;
  c_register *Drain;
  c_register *Vdda;
  c_register *Vdd;
  // utility functions
  void read_all();
  void write_all();
  bool isValid(std::string bias);
  c_register *operator[](std::string r_name);

private:
  std::array<c_register *, 16> registers;
};

#endif // ASICBIAS_H
