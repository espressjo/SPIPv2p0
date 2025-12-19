#ifndef TELEMETRY_SCA_DEF_H
#define TELEMETRY_SCA_DEF_H
#include <array>
#include <string>
namespace telemetry_def {
const int size_HxRG = 8;
const int size_ASIC = 8;
const std::array<std::string, size_HxRG> HxRG = {
    "Vreset",    "Dsub",  "VbiasGate", "VbiasPower",
    "CellDrain", "Drain", "VDDA",      "VDD"};
const std::array<std::string, size_HxRG> HxRG_def = {
    "Detector reset voltage",
    "Detector substrate voltage",
    "Gate voltage of SCA internal current sources",
    "Source voltage for SCA internal current sources",
    "Celldrain (drain node for pixel source follower)",
    "Drain (drain node for output buffer)",
    "Analog SCA supply voltage",
    "Digital SCA power supply"};
const std::array<std::string, size_ASIC> ASIC = {

    "VpreAmpRef1", "VpreAmpRef2", "VrefMain", "VPCFbias", "VRP",
    "VRN",         "VpreMidRef",  "Vcm"}; // namespace telemetry_def

} // namespace telemetry_def
#endif
