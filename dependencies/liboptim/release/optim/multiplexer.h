#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include "asicio.h"
#include "register.h"

#define ENGOK 0
#define ENGNOK -1

typedef struct {
  uint16_t multi_type;        // 1,2,4
  std::string multi_type_str; // HxRG
  uint16_t module;            // 10 or 15
  bool column_deselect;
  uint16_t output;
  uint16_t ch_average;
  bool valid;
} multi_t;

class multiplexer : public asicio {
public:
  multiplexer(unsigned long asicHandle, unsigned char asic_slct);
  int setH1RG();
  int setH2RG();
  int setH4RG();
  int setH4RG_10();
  int setH4RG_15();
  int enable_column_deselect();
  int disable_column_deselect();
  int setOutput(uint16_t output);
  int channel_averaged(uint16_t channels);
  std::string to_string();
  multi_t getMultiplexerValue();

private:
  void extract();
  // multi_t multi_val{0, "", 0, false, 0, 0};
  xreg h4010, h4011, h4050, h4012;
  int update();
};

#endif
