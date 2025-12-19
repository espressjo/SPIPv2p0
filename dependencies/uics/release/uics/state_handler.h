#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

#include "uics_sockets.h"
#include "states.h"
#include "insthandle.h"
#include <sys/types.h>
#include <sys/socket.h>
#include "uics_cmds.h"
#include "uics_state.h"
#include <thread>
#include <unistd.h>
#include "uics_config.h"
#include "uics_sndmsg.h"
#include "uics_join.h"
#include "uics_err_code.h"
#include "uics_config.h"
#include <filesystem>
#include "inst_log.h"
#include "msghandler.h"
#include "udp_msghandler.h"

#define SUCCESS "ready"
#define FAILED "failed"

extern Log instLog;

inline void log_stateH(std::string txt){instLog.writeto("[state H] "+txt,2);}

class state_handler
{
public:
    state_handler(instHandle *handle,std::string confPath,bool udp = true,std::string specific_cfg_file = "");
    void run();//run the main thread
    //:::::::::::::::::::::::::::
    //:::   add states here   :::
    //:::::::::::::::::::::::::::
        uics_state *s_config;
        uics_state *s_idle;
        uics_state *s_init;
        uics_state *s_acq;
    //:::::::::::::::::::::::::::
private:
    STATE defaultState;
    STATE nextState;
    uint16_t PORT4STATE;
    uint16_t PORT4MSG;
    instHandle *myHandle;
    int sockfd,commfd;
    std::string msg;
    
    //:::::::::::::::::::::::::::::
    //:::   Utility Functions   :::
    //:::::::::::::::::::::::::::::
    int checkState(cmd *cc,bool requestChange);
    void handler();
    void parseState(std::string path);//needed by parseStateLine
    std::vector<std::vector<STATE>> state_chart;//states chart for transition
    std::vector<STATE> parseStateLine(std::string s);//utility to read all the states from config file
};

#endif // STATE_HANDLER_H
