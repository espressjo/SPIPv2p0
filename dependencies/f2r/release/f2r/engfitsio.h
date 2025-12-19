#ifndef ENGFITSIO_H
#define ENGFITSIO_H

#include <fitsio.h>
#include <string>
#include <iostream>
#include <vector>

class header2{
    public:
        header2();
        void add_entry(std::string Kw,std::string value,std::string description);
        void add_entry(std::string Kw,int value,std::string description);
        void add_entry(std::string Kw,double value,std::string description);
        void add_bool_entry(std::string Kw,bool value,std::string description);

        void edit_entry(std::string entry,std::string value);
        void edit_bool_entry(std::string entry,bool value);
        void edit_entry(std::string entry,double value);
        void edit_entry(std::string entry,int value);

        int create_header_in_chu(fitsfile *fptr);
        void add_posemeter(std::string ttime, double fflux);
        void add_posemeter(std::string ttime, double fflux,double fflux_optional);
        void add_posemeter(double ttime, double fflux,double fflux_optional);//to be implemented
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

//fitsio i/o functions
int write_fits(unsigned short *pData,long naxes[2],std::string filename,header2 head,bool overwrite = false);
int write_fits_stream(unsigned short *pData, long naxes[2], std::string filename);
int write_fits_cube(unsigned short *pData, long naxes[2], std::string filename,header2 head, bool overwrite = false);
//header update functions
int update_kw(std::string filename,std::string kw,int value);//update keyword after the file is written
int update_kw(std::string filename,std::string kw,std::string value);//update keyword after the file is written
int update_kw(std::string filename,std::string kw,double value);//update keyword after the file is written

void printerrors( int status);//print cfitsio errors

#endif // ENGFITSIO_H
