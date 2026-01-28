#ifndef SPIP_TELEMETRY_H
#define SPIP_TELEMETRY_H

#include "insthandle.h"
#include <astro/hxrgstatus.h>

#pragma pack(push, 1)
typedef struct {
  int isWindow;
  int isOngoing;
  bool simulator; // mode 0-> normal acquisition, mode 1-> simulator
  char type[128]; // type of integration
  bool im_ready;
  int asBeenInit; // start     //0-> never initialized, 1-> MACIE_INIT() has
                  // been called, this is true after the 1st few seconds of
                  // init, 2-> the whole init sequence is done and successfull.
  int isReconfiguring; // 0-> not reconfiguring, 1-> is currently reconfiguring

} status_info_t;
#pragma pack(pop)
#pragma pack(push, 1)
typedef struct {
  uint gain; // initHandle
  uint HxRG;
  uint frameX;            // initHandle
  uint frameY;            // initHandle
  uint nbOutput;          // initHandle
  uint preampInputScheme; //(1) Use InPCommon as V1,V2,V4 against InP on V3
                          //(h4502 and h45c2 for averaged channels, Use input
                          // configuration based on bit <0> of this register
  uint preampInputVal;
  uint windowMode;   // 0->fullframe, 1->window
  uint columnMode;   // initHandle
  uint coldWarmMode; // initHandle
  uint preAmpCap;
  uint preAmpFilter;
  uint idle;
} preamplifier_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
  bool toponly;
  bool oddeven;
  char nonlin_reference[1024]; // full path for nonlin 3 ext file
  char bias_reference[1024];   // full path for bias reference.
  bool flag_bias;              // flag for non-linearity correction
  bool flag_nonlin;            // flag for non-linearity correction
  bool flag_refpx;             // apply ref. pixel correction.
  char uid_bias[128];
  char uid_nonlin[128];

} fits2ramp_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {

  int firmware_slot;
  fits2ramp_t f2r;
  preamplifier_t preamp;
  status_info_t status;
  char user[128];
  char type[128]; // type of integration
  char object[128];
  char observator[128];
  char operator_name[128];
  char namesequence[128];
  int asicserial;
  int scaserial;
  float readouttime;
  float effExpTime;
  long ID;

  int isWindow;
  int isOngoing;
  bool simulator; // mode 0-> normal acquisition, mode 1-> simulator
  bool im_ready;
  int asBeenInit; // start     //0-> never initialized, 1-> MACIE_INIT() has
                  // been called, this is true after the 1st few seconds of
                  // init, 2-> the whole init sequence is done and successfull.
  int isReconfiguring; // 0-> not reconfiguring, 1-> is currently reconfiguring
  // preamp
  uint ramp;  // initHandle
  uint read;  // initHandle
  uint reset; // initHandle
  uint drop;  // initHandle
  uint group; // initHandle

  bool readtelemetry;
  bool optimHeader;
  bool fn_increment;
  bool stream_cube;
  bool saveRampFile;
  bool overwrite_file;

} spip_telemetry_t;
#pragma pack(pop)
#endif
