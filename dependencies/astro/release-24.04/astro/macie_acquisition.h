#ifndef MACIE_ACQUISITION_H
#define MACIE_ACQUISITION_H

#include "insthandle.h"
#include <chrono>
#include <libf2r.h>
#include <liboptim.h>
#include <macie.h>
#include <string>
#include <uics.h>
using namespace std;

extern Log HxRGlog;
void acquisition(instHandle *handle, cmd *cc);
void copy_arr(unsigned short **pData, uint16_t **dest, long size);
void copy_arr(unsigned short **pData, double **dest, long size);
long frameSize(instHandle *handle); // calculate the size of the memory buffer
                                    // for the image
int u_header(uicsHeader *pHead, instHandle *handle,
             bool read_telemetry = false); // initialize header object.
double c_trigger_timeout(instHandle *handle);
double c_integration_time(instHandle *handle);
void fits2ramp_ptr(instHandle *handle, fits2ramp *&f2r);
int set_asic_acq_parameters(instHandle *handle);
long uniqueID();
bool isHaltTriggered(
    instHandle *handle); // check if halt is triggered, if yes update status

// utility function to generate and fool proof name for different files
string c_cube_name(instHandle *handle, int ramp);
string c_ramp_name(instHandle *handle, int ramp);
string c_read_name(instHandle *handle, int read, int ramp,
                   std::string directory);

int wait4integration(instHandle *handle, int timeout = 20);
double calculateTimeDifference(
    const std::chrono::high_resolution_clock::time_point &start,
    const std::chrono::high_resolution_clock::time_point &end);
std::chrono::time_point<std::chrono::high_resolution_clock>
getCurrentTimestamp();
int wait_for_data(
    instHandle *handle); // wait to the data to flow in the interface

#endif
