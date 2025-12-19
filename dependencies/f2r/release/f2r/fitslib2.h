#ifndef FITSLIB2_H
#define FITSLIB2_H

#include <iostream>
#include <string>
#include <string.h>
#include <fitsio.h>
//#include "fl2_header.h"
#include "engfitsio.h"


template <class dtype>
class fitslib2
{
public:
    fitslib2(int shapeX,int shapeY,std::string fname,bool owf=false);//used to create file
    fitslib2(int shapeX,int shapeY,std::string fname,header2 head,bool owf=false);//used to create file
    fitslib2();//use to open file
    void addExt(dtype *ext, const char *extname);
    void addExt(const char *extname,uint16_t *ext);
    void write();
    int blockSize(int bSize);
    int readIm( std::string fname,dtype **im,size_t *s);
    int readIm( std::string fname,dtype **im,size_t *s,int ext);
    void create_flux_table(header2 head);
private:
    void printerror(int status);
    fitsfile *fptr;
    int bitpix   ;
    bool donothing;
    long naxis;
    long naxes[2];
    int status;
    long  fpixel, nelements;
    int datatype;
    uint32_t size;
};
//template <class dtype>
//int fitslib2<dtype>::write_table(std::vector<std::pair<std::string,double>> flux)
//{   int stat = 0;
//    char *ttype[] = { "datetime", "flux"};
//    char *tform[] = { "1A", "1D"}; /* special CFITSIO codes */
//    char *tunit[] = { " ", "adu"};
//    fits_create_tbl(fptr, BINARY_TBL,
//                    0,2,ttype,
//                    tform, tunit,
//                   ( char*)"posemeter", &stat);
//    size_t s = flux.size();
//    char *datetime = new char[s];
//    double *ff = new double[s];
//    for (int i=0;i<s;++i) {
//        sprintf()
//    }

//    colnum = 1;
//    firstrow = 1;
//    firstelem = 1;
//    nelements = 100;
//    fits_write_col(fptr, TUSHORT, colnum, firstrow, firstelem,
//    nelements, uarray, &status);

//    delete [] ff;
//    delete [] datetime;

//}
template <class dtype>
int fitslib2<dtype>::readIm(std::string fname, dtype **im, size_t *s, int ext)
/*
 * Read a FITS file with multiple image extension. The image extension
 * starts at 1.
 */
{
    dtype nullval=0;
    int anynull=0;
    int error=0;
    naxes[0] = 1;
    naxes[1] = 1;
    long shape[2] = {1,1};
    fpixel =1;//first pixel to start reading
    
    if ( fits_open_file(&fptr, fname.c_str(), READONLY, &status) ){
    
             printerror( status );
             error = -1;
    }
    if (fits_movabs_hdu(fptr,ext,IMAGE_HDU,&status) ){
    
        printerror( status );
        error = -1;
    }
    if (fits_get_img_type (fptr, &bitpix,&status))
    {
        printerror( status );
        error = -1;
    }
    int ll;
    if (fits_get_img_dim(fptr, &ll, &status)){
    
        printerror(status);
        error = -1;
    }
    int maxdim=2;
    if (fits_get_img_size(fptr, maxdim, shape, &status))
    {
            printerror(status);
            error = -1;
    }

    naxis    =   ll;
    size = shape[0]*shape[1];
    
    *s = size;
	
    if (fits_read_img(fptr,datatype,fpixel,size,&nullval,*im,&anynull,&status))
    {
         printerror(status);
         error = -1;
    }

    /* close the file */
    if ( fits_close_file(fptr, &status) ){printerror( status );}
    return error;
}
template <class dtype>
int fitslib2<dtype>::readIm( std::string fname,dtype **im,size_t *s)

    /************************************************************************/
    /* Read a FITS image and determine the minimum and maximum pixel values */
    /************************************************************************/
{
    //fitsfile *fptr;
    int anynull;
    int nullval=0;
    naxes[0] = 1;
    naxes[1] = 1;
        long shape[2] = {1,1};
    fpixel =1;
    if ( fits_open_file(&fptr, fname.c_str(), READONLY, &status) ){
             printerror( status );
                return -1;}
    if (fits_get_img_type (fptr, &bitpix,&status))
    {
        printerror( status );
    }
    int ll;
    if (fits_get_img_dim(fptr, &ll, &status)){
        printerror(status);
    }
    int maxdim=2;
    if (fits_get_img_size(fptr, maxdim, shape, &status))
    {
            printerror(status);
    }

    naxis    =   ll;  /* 2-dimensional image
                  */


   //naxes[0] = shapeX;
   //naxes[1] = shapeY;   /* image is 300 pixels wide by 200 rows */
        size = shape[0]*shape[1];
        *s = size;
        dtype *imc = new dtype[size]{2};
    if (fits_read_pix(fptr, datatype, naxes, size,&nullval, imc, &anynull, &status))
    {
         printerror( status );
         return -1;
    }
    /*
     * int fits_read_pix / ffgpxv
      (fitsfile *fptr, int  datatype, long *fpixel, LONGLONG nelements,
       DTYPE *nulval, > DTYPE *array, int *anynul, int *status)
     * */
    if ( fits_close_file(fptr, &status) )                /* close the file */
         printerror( status );
    *im = imc;


    return 0;
}
template <class dtype>
void fitslib2<dtype>::printerror( int status)
{
    /*****************************************************/
    /* Print out cfitsio error messages and exit program */
    /*****************************************************/


    if (status)
    {
       fits_report_error(stderr, status); /* print error report */

       //exit( status );    /* terminate the program, returning error status */
    }
    return;
}

template <class dtype>
fitslib2<dtype>::fitslib2(int shapeX,int shapeY,std::string fname,bool owf)
/*!
  *Supported types are uint8_t, uint16_t, uint32_t, int, long int, float and double.
  * owf allows you to overwrite existing file when creating a file.
  */
{
    status = 0;
    donothing = false;
    if (owf)
    {
        if (fname[0]!='!')
        {
            fname.insert(0,1,'!');
        }

    }
    if (std::is_same<dtype,uint16_t>::value)
    {
        bitpix   =  USHORT_IMG; /* 16-bit unsigned short pixel values       */
        datatype = TUSHORT;
    }
    else if (std::is_same<dtype,uint8_t>::value)
    {
        bitpix   =  BYTE_IMG;
        datatype = TBYTE;
    }
    else if (std::is_same<dtype,int>::value)
    {
        bitpix   =  LONG_IMG;
        datatype = TINT;
    }
    else if (std::is_same<dtype,uint32_t>::value)
    {
        bitpix   =  ULONG_IMG;
        datatype = TUINT;
    }
    else if (std::is_same<dtype,long>::value)
    {
        bitpix   =  LONG_IMG;
        datatype = TLONG;
    }
    else if (std::is_same<dtype,float>::value)
    {
        bitpix   =  FLOAT_IMG;
        datatype = TFLOAT;
    }
    else if (std::is_same<dtype,double>::value)
    {
        bitpix   =  DOUBLE_IMG;
        datatype = TDOUBLE;
    }


    naxis    =   2;  /* 2-dimensional image                            */
   naxes[0] = shapeX;
   naxes[1] = shapeY;   /* image is 300 pixels wide by 200 rows */
    size = shapeX*shapeY;
    /* allocate memory for the whole image */


    /* initialize pointers to the start of each row of the image */



              /* initialize status before calling fitsio routines */

    if (fits_create_file(&fptr, fname.c_str(), &status))/* create new FITS file */
        {/* most probably the folder does not exsit or we dont have acces to it*/
            printerror( status );
            donothing = true;
            return;
    }


//create an empty first PrimaryHDU
    long ax[2];
    ax[0] = 0;
    ax[1] = 0;
    if (fits_create_img(fptr,USHORT_IMG,0,ax,&status))
    {
        printerror(status);
    }

    if (fits_write_date(fptr, &status))
    {
        printerror(status);
    }





}
template <class dtype>
fitslib2<dtype>::fitslib2(int shapeX,int shapeY,std::string fname,header2 head,bool owf)
/*!
  *Supported types are uint8_t, uint16_t, uint32_t, int, long int, float and double.
  * owf allows to overwrite the file when write is called.
  */
{
    status = 0;
    donothing = false;
    if (owf)
    {
        if (fname[0]!='!')
        {
            fname.insert(0,1,'!');
        }

    }

    if (std::is_same<dtype,uint16_t>::value)
    {
        bitpix   =  USHORT_IMG; /* 16-bit unsigned short pixel values       */
        datatype = TUSHORT;
    }
    else if (std::is_same<dtype,uint8_t>::value)
    {
        bitpix   =  BYTE_IMG;
        datatype = TBYTE;
    }
    else if (std::is_same<dtype,int>::value)
    {
        bitpix   =  LONG_IMG;
        datatype = TINT;
    }
    else if (std::is_same<dtype,uint32_t>::value)
    {
        bitpix   =  ULONG_IMG;
        datatype = TUINT;
    }
    else if (std::is_same<dtype,long>::value)
    {
        bitpix   =  LONG_IMG;
        datatype = TLONG;
    }
    else if (std::is_same<dtype,float>::value)
    {
        bitpix   =  FLOAT_IMG;
        datatype = TFLOAT;
    }
    else if (std::is_same<dtype,double>::value)
    {
        bitpix   =  DOUBLE_IMG;
        datatype = TDOUBLE;
    }


    naxis    =   2;  /* 2-dimensional image                            */
   naxes[0] = shapeX;
   naxes[1] = shapeY;   /* image is 300 pixels wide by 200 rows */
    size = shapeX*shapeY;
    /* allocate memory for the whole image */


    /* initialize pointers to the start of each row of the image */




              /* initialize status before calling fitsio routines */

    if (fits_create_file(&fptr, fname.c_str(), &status)) /* create new FITS file */
    {/* most probably the folder does not exsit or we dont have acces to it*/
        printerror( status );
        donothing = true;
        return ;
    }


//create an empty first PrimaryHDU
    long ax[2];
    ax[0] = 0;
    ax[1] = 0;
    if (fits_create_img(fptr,USHORT_IMG,0,ax,&status))
    {
        printerror(status);
    }
    int stat = head.create_header_in_chu(fptr);
    if (stat!=0)
    {
        printerror(stat);
    }
    if (fits_write_date(fptr, &status))
    {
        printerror(status);
    }



}

template <class dtype>
void fitslib2<dtype>::create_flux_table(header2 head)
{   int stat=0;
    if (donothing)
    {//we where unable to create the file
        return;
    }
    if (head.posemeter_size>0)
    {
        stat = 0;
         char *ttype[3] = { (char*)"TIME", (char*)"FIBRE1",(char*)"FIBRE2"};
         char *tform[3] = { (char*)"1D",(char*)"1D", (char*)"1D"}; /* special CFITSIO codes */
         char *tunit[3] = { (char*)"days", (char*)"ADU", (char*)"ADU"};
        if (fits_create_tbl(fptr, BINARY_TBL,
                            0,3,ttype,
                            tform, tunit,
                           ( char*)"posemeter", &stat))
        {
            printerror(stat);
        }
        head.create_table(fptr);

    }

}


template <class dtype>
int fitslib2<dtype>::blockSize(int bSize)
{
    if (donothing)
    {//we where unable to create the file
        return -1;;
    }
    if (bSize<=0)
    {
        return -1;
    }
    if(fits_set_hdrsize(fptr, bSize*36, &status))
    {
        printerror( status );
        return -1;
    }
    return 0;
}
template <class dtype>
fitslib2<dtype>::fitslib2()
/*!
  *Supported types are uint8_t, uint16_t, uint32_t, int, long int, float and double
  */
{
        status = 0;
    /* pointer to the FITS file, defined in fitsio.h */

    if (std::is_same<dtype,uint16_t>::value)
    {
        datatype = TUSHORT;
    }
    else if (std::is_same<dtype,uint8_t>::value)
    {
        datatype = TBYTE;
    }
    else if (std::is_same<dtype,int>::value)
    {
        datatype = TINT;
    }
    else if (std::is_same<dtype,uint32_t>::value)
    {
        datatype = TUINT;
    }
    else if (std::is_same<dtype,long>::value)
    {
        datatype = TLONG;
    }
    else if (std::is_same<dtype,float>::value)
    {
        datatype = TFLOAT;
    }
    else if (std::is_same<dtype,double>::value)
    {
        datatype = TDOUBLE;
    }



}
template <class dtype>
void fitslib2<dtype>::addExt(dtype *ext, const char *extname)
{
    if (donothing)
    {//we where unable to create the file
        return;
    }
    if ( fits_create_img(fptr,  bitpix, naxis, naxes, &status) )
         printerror( status );
    fpixel = 1;                               /* first pixel to write      */
    nelements = naxes[0] * naxes[1];          /* number of pixels to write */


    /* write the array of unsigned integers to the FITS file */

    if ( fits_write_img(fptr, datatype, fpixel, nelements, ext, &status) )
        printerror( status );
    if (fits_write_key(fptr, TSTRING, "EXTNAME", (char*)extname,"Extension name", &status))
        printerror(status);
    if (fits_flush_buffer(fptr,0,&status))
    {
        printerror(status);
    }
}

template <class dtype>
void fitslib2<dtype>::write()
{
    if (donothing)
    {//we where unable to create the file
        return;
    }
    //make sure everything is written on disk
    if (fits_flush_file(fptr,&status))
    {
        printerror( status );
    }
    if ( fits_close_file(fptr, &status) )                /* close the file */
         printerror( status );
}

template <class dtype>
void fitslib2<dtype>::addExt(const char *extname, uint16_t *ext)
{
    if (donothing)
    {//we where unable to create the file
        return;
    }
    if ( fits_create_img(fptr,  USHORT_IMG, naxis, naxes, &status) )
         printerror( status );
    fpixel = 1;                               /* first pixel to write      */
    nelements = naxes[0] * naxes[1];          /* number of pixels to write */

    /* write the array of unsigned integers to the FITS file */
    if ( fits_write_img(fptr, TUSHORT, fpixel, nelements, ext, &status) )
        printerror( status );

    if (fits_write_key(fptr, TSTRING, "EXTNAME", (char*)extname,"Extension name", &status))
        printerror(status);
    if (fits_flush_buffer(fptr,0,&status))
    {
        printerror(status);
    }
}
#endif // FITSLIB2_H
