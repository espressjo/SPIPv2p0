#ifndef LOW_PASS_H
#define LOW_PASS_H
#include <array>
#include <string>

#define LOWPASSSIZE 16

namespace low_pass_filter {
inline const std::array<std::string, LOWPASSSIZE> low_pass_filter = {
    "132,629 KHz", "7,579 KHz", "3,789 KHz", "2,526 KHz",
    "1,895 KHz",   "1,263 KHz", "947 KHz",   "632 KHz",
    "474 KHz",     "392 KHz",   "273 KHz",   "196 KHz",
    "136 KHz",     "110 KHz",   "86 KHz",    "66 KHz"};
inline const int length = LOWPASSSIZE;
} // namespace low_pass_filter
#endif
