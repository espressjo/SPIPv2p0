#ifndef ASIC_REGISTER_H
#define ASIC_REGISTER_H

#include <stdint.h>

typedef struct {
  uint16_t addrs_even;
  uint16_t addrs_odd;
  uint8_t Vrange;
  uint8_t Irange;
  bool Vsource_power;
  bool Isource_power;
  double voltage; // in volt
  uint16_t c_compensator;
  uint8_t Isink_En;
  double current;    // in ampere
  double dac_buffer; // in mA
  bool valid;
} asic_register_t;

#endif
