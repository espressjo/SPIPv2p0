#include "spip_getstatus.h"
#include <cstring>

void spip_status_t(instHandle *handle) {

  int fd = create_socket(5023);
  cmd *cc = new cmd;

  while (1) {
    cc->recvCMD(fd);
    static char *encodedStructure = NULL;
    spip_telemetry_t tlm{};

    tlm.firmware_slot = handle->macie.firmware_slot;
    //:::::::::::::::::::::
    //:::   fits2ramp   :::
    //:::::::::::::::::::::
    handle->f2r.bias_reference.copy(tlm.f2r.bias_reference, 1024);
    handle->f2r.uid_bias.copy(tlm.f2r.uid_bias, 128);
    handle->f2r.uid_nonlin.copy(tlm.f2r.uid_nonlin, 128);
    handle->f2r.nonlin_reference.copy(tlm.f2r.nonlin_reference, 1024);
    tlm.f2r.flag_nonlin = handle->f2r.flag_nonlin;
    tlm.f2r.flag_bias = handle->f2r.flag_bias;
    tlm.f2r.flag_refpx = handle->f2r.flag_refpx;

    tlm.f2r.oddeven = handle->f2r.oddeven;
    tlm.f2r.toponly = handle->f2r.toponly;
    //::::::::::::::::::::::

    //:::::::::::::::::::::::::
    //:::   status struct   :::
    //:::::::::::::::::::::::::

    tlm.status.isWindow = handle->m_status.isWindow;
    tlm.status.isOngoing = handle->m_status.isOngoing;
    tlm.status.simulator =
        handle->m_status
            .simulator; // mode 0-> normal acquisition, mode 1-> simulator
    handle->m_status.type.copy(tlm.status.type, 128); // type of integration
    tlm.status.im_ready = handle->m_status.im_ready;
    tlm.status.asBeenInit =
        handle->m_status.asBeenInit; // start     //0-> never initialized, 1->
                                     // MACIE_INIT() has
    tlm.status.isReconfiguring =
        handle->m_status.isReconfiguring; // 0-> not reconfiguring, 1-> is
                                          // currently reconfiguring

    //:::::::::::::::::::::::::

    tlm.preamp = handle->preamp;
    memset(tlm.object, 0, 128);
    strcpy(tlm.object, handle->info.OBJET.c_str());

    memset(tlm.namesequence, 0, 128);
    strcpy(tlm.namesequence, handle->info.NAMESEQUENCE.c_str());

    memset(tlm.type, 0, 128);
    strcpy(tlm.type, handle->m_status.type.c_str());

    memset(tlm.user, 0, 128);
    strcpy(tlm.user, handle->info.user.c_str());

    tlm.asicserial = handle->info.ASICSERIAL;
    tlm.scaserial = handle->info.SCASERIAL;
    tlm.readouttime = static_cast<float>(handle->info.readoutTime);
    tlm.effExpTime = handle->info.effExpTime;
    tlm.ID = handle->info.ID;
    tlm.isWindow = handle->m_status.isWindow;
    tlm.isOngoing = handle->m_status.isOngoing;
    tlm.simulator = handle->m_status.simulator;
    tlm.asBeenInit = handle->m_status.asBeenInit;
    tlm.im_ready = handle->m_status.im_ready;
    tlm.read = handle->acq.read;
    tlm.isReconfiguring = handle->m_status.isReconfiguring;
    tlm.ramp = handle->acq.ramp;
    tlm.reset = handle->acq.reset;
    tlm.group = handle->acq.group;
    tlm.drop = handle->acq.drop;

    tlm.readtelemetry = handle->acq.readtelemetry;
    tlm.optimHeader = handle->acq.optimHeader;
    tlm.fn_increment = handle->acq.fn_increment;
    tlm.stream_cube = handle->acq.stream_cube;
    tlm.saveRampFile = handle->acq.saveRampFile;
    tlm.overwrite_file = handle->acq.overwrite_file;

    // b64
    size_t length =
        islb64EncodeAlloc((const char *)&tlm, sizeof(tlm), &encodedStructure);
    cc->respond(std::to_string(length) + " " + std::string(encodedStructure));
  }
}
/*
  char observator[128];
  char operator_name[128];




 */
