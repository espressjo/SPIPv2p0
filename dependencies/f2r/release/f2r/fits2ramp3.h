#include "medflt.h"
#include <vector>
#include <numeric>
#include <thread>
#include "fitslib2.h"
#include "refpxcorr2.h"
#include <unistd.h>
#include <cmath>
#include "readim.h"
#include "fitsheader.h"
#include "engfitsio.h"
#include <mutex>

enum f2r_status {f2r_processing=0,f2r_writing,f2r_waiting,f2r_waiting_imready};

class fits2ramp
{
public:
    fits2ramp(uint16_t xshape,uint16_t yshape,double tIntTime);
    ~fits2ramp();
    f2r_status status;
    std::string load_nl_coefficient(std::string fname);//load NL coefficient. The nl correction will automatically be performed
    std::string load_bias(std::string fname);
    uint16_t xlength, ylength;
    void run();//start main thread
    void get_im_ptr(void **ptr, size_t *length);
    bool *im_ready;
    //setup parameter
    void dataready(void);
    void ow_existingfile(bool ow);//use set flags to over write file if it existe
    bool refpxoddeven;//median odd and even col. separetly
    bool refpxtoponly;//only use top ref. px.?
    bool useRefPix;//use ref pix. to correct the data?
    uint16_t saturation;/*!< This member can be change and represent the saturation level in adu.*/
    void getIm_ptr(void** im_ptr, size_t *length);
    void copy(uint16_t *im);
    void execute(uint16_t *im);
    void writetofile(std::string f_name, header2 h_ead);
    void writetofilenow(std::string f_name, header2 h_ead);

    void setBlockSize(int bsize);
    void stopthread();
    void createCoefficient();
    void createCoefficientNow();

    void saveCoefficient();
    void get_new_image_status(bool *status);
bool refCorr;//correct the refpixels at the end

private:
    //method
    bool savefile;
    std::string fname; //must be set by writetofile()
    header2 head;
    bool threadclose;
    bool writeCoeff;
    int nbBlockSIze;
    std::mutex mtx;
    //void compute_coefficient();
    void get_coefficient(double *a, double *b);
    //void saveCoefficient(std::string fname);
    void saveCoefficient_t();
    //setup
//    bool refCorr;//correct the refpixels at the end
    bool selfbias;//self bias or user defined bias
    bool nlCorr;//correct for nl. Only if coeff are defined
    const uint32_t arraySize;//length of image array
    //data
    double *bias;//pointer that hold the bias frame
    double *c3,*c2;//c3 and c2 coeofficient of NL
    double *sy;
    double *a ;
    double *b ;
	double *errors;
    uint16_t *n;
    double cumulTint;
    double *sxy;
    uint16_t *databuffer;
    bool exec_ready;
    double t;
    bool delete_coeff;//whenever c3 and c2 is allocated we want to delete the memory
    bool owf;//overwrite flag

    bool threaded;
    std::vector <double> intTime;
    void execute_t();
    void refpixcorr(double *im);//reference pixels algorythm
    void cumulsum(std::vector<double> s,std::vector<double> *Sum);//cumul sum algorythm
};
