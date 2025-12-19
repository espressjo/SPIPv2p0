#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include "uics_cmds.h"
#include <vector>
#include <string>
#include "uics_sndmsg.h"
#include "uics_join.h"
#include "uics_err_code.h"
#include <filesystem>
#include "uics_config.h"
#include "inst_log.h"
#include "insthandle.h"
#include "states.h"

#define TRUE 1
#define FALSE 0
std::string getStates(STATE S);
class msgHandler
{
public:
    msgHandler(std::string confPath,instHandle *handle);
    //msgHandler(std::string log,int level);
    void setlPath(std::string path);
    void setllevel(int level);
    void run();
    Log* getLog();
    
private:
    struct sockaddr_in address;
    uint16_t PORT4MSG;
    int opt,max_sd,master_socket,addrlen,new_socket,client_socket[30],max_clients,activity,i , valread , sd;
    bool compstrip(char *buffer,const char* str );
    char buffer[1025];  //data buffer of 1K
    cmdList *allcmds;
    int getLevel(std::string S);
    fd_set readfds;     //set of socket descriptors
    instHandle *handle;
};

#endif // MSGHANDLER_H
