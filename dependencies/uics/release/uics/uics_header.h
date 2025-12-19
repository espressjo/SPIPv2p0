#ifndef UICS_HEADER_H
#define UICS_HEADER_H
#include <fitsio.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

class uicsHeader{
    public:
        uicsHeader();
        void add_entry(std::string Kw,std::string value,std::string description);
        void add_entry(std::string Kw,int value,std::string description);
        void add_entry(std::string Kw,double value,std::string description);
        void add_bool_entry(std::string Kw,bool value,std::string description);

        void edit_entry(std::string entry,std::string value);
        void edit_bool_entry(std::string entry,bool value);
        void edit_entry(std::string entry,double value);
        void edit_entry(std::string entry,int value);

        // Get methods to retrieve keyword values
        std::string get_string(std::string kw) const;
        int get_int(std::string kw) const;
        double get_double(std::string kw) const;
        bool get_bool(std::string kw) const;
        
        // Generic get that returns string representation
        std::string get(std::string kw) const;

        int create_header_in_chu(fitsfile *fptr);
        int read_header_from_chu(fitsfile *fptr);
        
        void add_posemeter(std::string ttime, double fflux);
        void add_posemeter(std::string ttime, double fflux,double fflux_optional);
        void add_posemeter(double ttime, double fflux,double fflux_optional);
        int create_table(fitsfile *fptr);
        int posemeter_size;

    private:
        std::vector<int> intVal;
        std::vector<std::string> intDescription;
        std::vector<std::string> intKW;

        std::vector<int> boolVal;
        std::vector<std::string> boolDescription;
        std::vector<std::string> boolKW;

        std::vector<double> doubleVal;
        std::vector<std::string> doubleDescription;
        std::vector<std::string> doubleKW;

        std::vector<std::string> stringVal;
        std::vector<std::string> stringDescription;
        std::vector<std::string> stringKW;
        std::vector<std::string> time;
        std::vector<double> time_db;
        std::vector<double> flux;
        std::vector<double> flux_optional;
        char** to_array(std::vector<std::string> V);
        double *to_array(std::vector<double> V);
};
#endif
