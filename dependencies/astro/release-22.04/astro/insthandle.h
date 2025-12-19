#ifndef INSTHANDLE_H
#define INSTHANDLE_H

#include "hxrgstatus.h"
#include <macie.h>
#include <stdint.h>
#include <string>
#include <uics/states.h>

//:::::::::::::::::::::::::::::::::::::::::::::::
//:::   All stuff related to Macie hardware   :::
//:::::::::::::::::::::::::::::::::::::::::::::::

typedef struct {
  unsigned short numCards; // start
  MACIE_CardInfo *pCard;   // start //getHandle
  unsigned long
      handle; // InitializeASIC //initASIC //getHandle  //getAvailableMacie
  MACIE_Connection connection; // getHandle
  unsigned char avaiMACIEs;    // getAvailableMacie
  unsigned char slctMACIEs;    // InitializeASIC //getAvailableMacie
  unsigned char avaiASICs;     // initASIC
  unsigned char slctASIC;      // initASIC
  unsigned int val;            // initASIC //getAvailableMacie
  MACIE_STATUS bRet;           // start
  int haltTriggered;
  int firmware_slot; // InitializeASIC
  std::string firmware_version;
  float macie_libv; // start
  // image pointer
  void *im_ptr; // image pointer ()
  size_t imSize;
} Macie;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:::   All stuff related to on-the-fly correction   :::
//::::::::::::::::::::::::::::::::::::::::::::::::::::::

typedef struct {
  bool toponly;
  bool oddeven;
  std::string nonlin_reference; // full path for nonlin 3 ext file
  std::string bias_reference;   // full path for bias reference.
  bool flag_bias;               // flag for non-linearity correction
  bool flag_nonlin;             // flag for non-linearity correction
  bool flag_refpx;              // apply ref. pixel correction.
  std::string uid_bias;
  std::string uid_nonlin;
} f2r_c;

//:::::::::::::::::::::::::::::::::::::::::::::
//:::   All stuff related to preamp setup   :::
//:::::::::::::::::::::::::::::::::::::::::::::

typedef struct {
  unsigned short *pData;
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
  uint idle; // initHandle
} preamp_t;

typedef struct {
  // time
  std::string OBJET;
  std::string ACQDATE;
  std::string JD;
  std::string timeLocal;
  std::string timeGMT;
  // observation
  std::string OBSERVATEUR;
  std::string OBSERVATORY; // initHandle
  std::string OBSLOCATION; // initHandle
  std::string NAMESEQUENCE;
  // General information
  float SOFTVERSION; // initHandle
  std::string UNITS;
  int ASICSERIAL; // initHandle
  int SCASERIAL;  // initHandle
  // integration
  float readoutTime; // initHandle       //in ms
  float effExpTime;
  long ID;
  double flux;
  double flux_optional;
  std::string user; // initHandle

} information_t;

//:::::::::::::::::::::::::::::::::::::::::
//:::   All stuff related with status   :::
//:::::::::::::::::::::::::::::::::::::::::
typedef struct {
  int isWindow;
  int isOngoing;
  bool simulator;   // mode 0-> normal acquisition, mode 1-> simulator
  std::string type; // type of integration
  bool im_ready;
  int asBeenInit; // start     //0-> never initialized, 1-> MACIE_INIT() has
                  // been called, this is true after the 1st few seconds of
                  // init, 2-> the whole init sequence is done and successfull.
  int isReconfiguring; // 0-> not reconfiguring, 1-> is currently reconfiguring
  hxrgStatus hdwr_status;
} status_t;

//::::::::::::::::::::::::::::::::::::::::::::::
//:::   All stuff related with acquisition   :::
//::::::::::::::::::::::::::::::::::::::::::::::
typedef struct {
  // preamp
  uint ramp;  // initHandle
  uint read;  // initHandle
  uint reset; // initHandle
  uint drop;  // initHandle
  uint group; // initHandle

  // window mode
  uint winStartX;
  uint winStartY;
  uint winStopY;
  uint winStopX;

  // posemeter
  std::string posemeter_mask;
  std::string posemeter_mask_optional;

  // header
  int numBlock; // for the header
  bool readtelemetry;
  bool optimHeader;
  // path
  std::string savePath;      // initHandle
  std::string calpath;       // initHandle
  std::string n_ramp;        // basename for ramp files
  std::string n_single_read; // basename for single reads files
  std::string n_cube;        // basename for cube of reads
  bool fn_increment;
  // data acquisition
  bool stream_cube;
  bool saveRampFile;
  bool overwrite_file;

} acq_t;

//:::::::::::::::::::::::::::::::::::::::::
//:::  All stuff related to microcode   :::
//:::::::::::::::::::::::::::::::::::::::::

typedef struct {
  std::string mcdASIC; // InitializeASIC //initHandle
  std::string mcdOpt;  // initHandle
  std::string mrf;     // InitializeASIC //initHandle
} mcd_t;

//::::::::::::::::::::::::::::::::::::::::::::
//:::  All stuff related to optimization   :::
//::::::::::::::::::::::::::::::::::::::::::::
typedef struct {
  std::string addr;
  std::string val;
  // overrides for engineering
  long int override_uid;
  std::string override_serie_path;
  std::string override_path;
  bool add_telemetry_info; // only valid for 1 integration
} optimization_t;

typedef struct {

  // UICS stuff
  STATE nextState;
  STATE state;
  STATUS status;

  Macie macie;
  preamp_t preamp;
  f2r_c f2r;
  information_t info;
  status_t m_status;
  acq_t acq;
  mcd_t mcd;
  optimization_t optim;
  std::string log;       // initHandle
  std::string root_path; // initHandle     //chaged to only 1 path /opt/astro

} instHandle;

#endif // INSTHANDLE_H
