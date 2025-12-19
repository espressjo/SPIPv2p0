#ifndef UICS_SPECIAL_H
#define UICS_SPECIAL_H


#include <variant>
#include <string>

class special_commands{
    public:
        void add_function(void (*f)()),std::string fnc_name);
    private:
        std::vector<std::string> cmd_name;
        std::vector<void (*f)()> cmd_function;    
        std::vector<std::string> cmd_args;
        bool check_arg_type(std::vector<std::string> arguments,std::string types);
    
};

#endif