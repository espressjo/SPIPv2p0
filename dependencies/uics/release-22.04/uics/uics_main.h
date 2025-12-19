#ifndef UICS_MAIN_H
#define UICS_MAIN_H


#include "msghandler.h"
#include "state_handler.h"
#include "udp_msghandler.h"
#include <string>
#include <thread>
#include <unistd.h>
#include "uics_cmds.h"
#include <vector>
#include "uics_number.h"
#include "insthandle.h"

class uics {
    
public:
    uics(instHandle *handle,std::string cfg_file,std::string cfg_path,bool udp = true);
    void run();
    void add_thread(void (*f)(),std::string label,bool hrdw);//not yet implemented
    void add_thread(void (*f)(instHandle*),instHandle *handle,std::string label,bool hrdw);
    void add_thread(void (*f)(instHandle*,cmd*),instHandle *handle,cmd *cc,std::string label,bool hrdw);
    void add_function(std::string f_name,void (*f)(instHandle*,cmd*),STATE state);
    void manage_special_commands();
    void executeOnce(STATE state,STATE next_state);
    
private:
    state_handler *sHandler;   
    std::string cfg_file;
    std::string cfg_path;
    instHandle *handle;
    std::vector<std::thread*> t_func; 
    bool udp;
};



#endif
