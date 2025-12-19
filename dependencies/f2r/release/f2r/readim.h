#ifndef READIM_H
#define READIM_H

#include <iostream>
#include <string>
#include <string.h>
#include <fitsio.h>
int readIm( std::string fname,double **im,size_t *s);
int getarraysize(std::string fname,size_t *size);
void printerror( int status);
#endif
