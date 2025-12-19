#ifndef MACIE_ERRORS_H
#define MACIE_ERRORS_H

#include "insthandle.h"
#include <macie.h>
#include <uics.h>
#include <string>
#include <iostream>
#include <array>


using namespace std;

inline string ERRORSCODE[33]={
                "UART Port: Parity Errors",
                "UART Port: Stopbit Errors",
                "UART Port: Timeout Errors",
                "USB Port: Timeout Errors",
                "GigE Port: Timeout Errors",
                "ASIC 1 Configuration: Acknowledge Errors Type 1",
                "ASIC 1 Configuration: Acknowledge Errors Type 2",
                "ASIC 1 Configuration: Start Bit Timeout Errors",
                "ASIC 1 Configuration: Stop Bit Timeout Errors",
                "ASIC 1 Configuration: Stop Bit Errors",
                "ASIC 1 Configuration: Parity Errors",
                "ASIC 1 Configuration: Data Errors",
                "ASIC 1 Configuration: CRC errors",
                "ASIC 2 Configuration: Acknowledge Errors Type 1",
                "ASIC 2 Configuration: Acknowledge Errors Type 2",
                "ASIC 2 Configuration: Start Bit Timeout Errors",
                "ASIC 2 Configuration: Stop Bit Timeout Errors",
                "ASIC 2 Configuration: Stop Bit Errors",
                "ASIC 2 Configuration: Parity Errors",
                "ASIC 2 Configuration: Data Errors",
                "ASIC 2 Configuration: CRC errors",
                "Main Science FIFO: Science FIFO Overflow Errors",
                "Main Science FIFO: spare",
                "Main Science FIFO: spare",
                "Main Science FIFO: spare",
                "ASIC 1 Science Data: Stop Errors",
                "ASIC 1 Science Data: Parity Errors",
                "ASIC 1 Science Data: Data Errors",
                "ASIC 1 Science Data: CRC Errors",
                "ASIC 2 Science Data: Stop Errors",
                "ASIC 2 Science Data: Parity Errors",
                "ASIC 2 Science Data: Data Errors",
                "ASIC 2 Science Data: CRC Errors"};

void p_errors(instHandle *handle, cmd *cc);
int log_macie_error(instHandle *handle);
#endif

