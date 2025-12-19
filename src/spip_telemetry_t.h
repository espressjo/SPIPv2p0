#ifndef SPIP_TELEMETRY_H
#define SPIP_TELEMETRY_H

#include "insthandle.h"
#include <astro/hxrgstatus.h>
#pragma pack(push, 1)
typedef struct {

  int firmware_slot;
  f2r_c f2r;
  preamp_t preamp;
  status_t status;
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
