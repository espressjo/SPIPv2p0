#ifndef PREAMP_T_H
#define PREAMP_T_H

#include <string>
using namespace std;

typedef struct {
  string opTemp_str;
  string reset_str;
  string KTC_str;
  string compCap_str;
  string lowPass_str;
  string gain_str;
  string tension;
  string sampling;
  string v1;
  string v2;
  string v3;
  string v4;
  uint16_t compCap;
  uint16_t lowPass;
  uint16_t gain;
  bool KTC;
  bool resetRow;
  bool resetFrame;
  bool opCold;
  bool opWarm;
  bool buffered;
  bool NMOS;
  bool sfollower_c;
  bool valid;
} asic_preamp_t;
#endif
