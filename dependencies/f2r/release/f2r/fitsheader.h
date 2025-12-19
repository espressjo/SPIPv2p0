#ifndef FITSHEADER_H
#define FITSHEADER_H

#include <iostream>
#include <string>
#include <string.h>
#include <fitsio.h>

class fitsheader
{
    public:
        fitsheader(std::string fname);
        ~fitsheader();
        std::string operator [] (std::string kw);
    private:
        fitsfile *fptr;
        long naxis;
        long naxes[2];
        int status;
        void printerror(int status);
};
#endif // FITSHEADER_H
