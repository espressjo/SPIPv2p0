#ifndef ASIC_REGISTER_NAME_H
#define ASIC_REGISTER_NAME_H

#include <array>
#include <string>
namespace REGNAME {
inline const std::array<std::string, 16> name = {
    "Vreset", "Dsub", "VbiasGate", "VbiasPower", "CellDrain", "Drain",
    "Vdda",   "Vdd",  "Vref1",     "Vref2",      "VrefMain",  "Vpcf",
    "Vrp",    "Vrn",  "VmidRef",   "Vcm"};
inline const int REGNAMELENGTH = 16;
} // namespace REGNAME
#endif
