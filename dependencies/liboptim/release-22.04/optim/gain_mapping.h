#ifndef GAIN_MAPPING_H
#define GAIN_MAPPING_H

#include <array>
#include <string>

namespace gain {
inline const std::array<std::string, 16> gainCode = {
    "-3 dB small Cin", "0 dB small Cin",  "3 dB small Cin",  "6 dB small Cin",
    "6 dB large Cin",  "9 dB small Cin",  "9 dB large Cin",  "12 dB small Cin",
    "12 dB large Cin", "15 dB small Cin", "15 dB large Cin", "18 dB small Cin",
    "18 dB large Cin", "21 dB large Cin", "24 dB large Cin", "27 dB large Cin"};
inline const int lenght = 16;
} // namespace gain

#endif // MYSTRINGS_H
