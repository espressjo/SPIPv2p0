#ifndef PREAMP_TENSION_H
#define PREAMP_TENSION_H

#include <array>
#include <string>

#define PSIZE 16

namespace preamptension {
inline const std::array<std::string, PSIZE> tension = {
    "Padp",  "Padn",    "InPCommon", "InNCommon", "VrefMain", "Vref1",
    "Vref2", "VrefMid", "VC",        "VDAC",      "Gndpin",   "Padn8",
    "Padp8", "",        "",          ""};
inline const int length = PSIZE;
} // namespace preamptension

#endif
