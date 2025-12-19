#ifndef MACIE_REGISTER_UTILS_H
#define MACIE_REGISTER_UTILS_H

#include <time.h>
#include <cstdio> 
#include <string>
#include <uics.h>

#include "regdef.h"
#include "insthandle.h"

enum ASIC_STATE {ASIC_IDLE=0,ASIC_ACQ,ASIC_FAILED,ASIC_RECONF};

using namespace std;

void delay(int ms);//create delay in milliseconds
uint r_bit(uint value,uint bit);
string uint2address(unsigned int value);//convert uint to h0000
string uint2value(unsigned int value);//convert uint to 0x0000
int updateHandle(instHandle *handle);
int reconfigure(instHandle *handle);//trigger a reconfiguration of the ASIC
int readRegister(instHandle *handle,std::string reg,uint *value);
int readRegister(instHandle *handle,unsigned short reg,uint *value);//read register
//wrapper of MACIEWrite_register which log more information
int writeRegister(instHandle *handle,std::string regAddr,std::string hexVal,bool trigger_reconfiguration=false);
int writeRegister(instHandle *handle,std::string regAddr,unsigned int hexVal,bool trigger_reconfiguration=false);
int writeRegister(instHandle *handle,unsigned short regAddr,unsigned int hexVal,bool trigger_reconfiguration=false);

ASIC_STATE r_ASIC6900(instHandle *handle);//utility function to manage register 6900
int w_ASIC6900(instHandle *handle,ASIC_STATE state,int timeout=20);//utility function to manage register 6900
#endif

