#ifndef TELEMETRY_T_H
#define TELEMETRY_T_H

typedef struct {

  float VbiasGate;
  float IbiasGate;
  float VbiasPower;
  float IbiasPower;
  float Vdsub;
  float Idsub;
  float Vreset;
  float Ireset;
  float Vdda;
  float Idda;
  float Idd;
  float Vdd;
  float Vcelldrain;
  float Icelldrain;
  float Vdrain;
  float Idrain;

} telemetry_t;
#endif
