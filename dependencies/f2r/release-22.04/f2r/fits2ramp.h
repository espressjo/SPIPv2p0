#ifndef FITS2RAMP_H
#define FITS2RAMP_H

#include "refpxcorr.h"
#include <cmath>
#include <cstring>
#include <mutex>
#include <numeric>
#include <thread>
#include <uics/uics_fits.h>
#include <vector>

class fits2ramp {
public:
  fits2ramp(uint16_t xshape, uint16_t yshape, double tIntTime);
  ~fits2ramp();

  // Calibration loading
  std::string load_nl_coefficient(std::string fname);
  std::string load_bias(std::string fname);

  // Main processing
  void execute(uint16_t *im);
  void clear(); // Clear arrays for next ramp, keep calibrations and headers

  // File writing
  void writetofile(std::string f_name);    // Threaded, returns immediately
  void writetofilenow(std::string f_name); // Synchronous, blocks until done

  // Configuration
  void ow_existingfile(bool ow); // Set overwrite flag
  bool refpxoddeven;             // Median odd and even col. separately
  bool refpxtoponly;             // Only use top ref. px.?
  bool useRefPix;                // Use ref pix. to correct the data?
  bool refCorr;                  // Correct the refpixels at the end
  uint16_t saturation;           // Saturation level in ADU

  // Image ready callback
  void get_new_image_status(bool *status);

  fitsio<double> *fits;

private:
  // Dimensions and timing
  const uint32_t arraySize;
  uint16_t xlength, ylength;
  double t;         // Integration time per read
  double cumulTint; // Cumulative integration time

  // Calibration data
  fitsio<double> nl_fits_c2;
  fitsio<double> nl_fits_c3;
  fitsio<double> bias_fits;
  double *bias;
  bool selfbias; // Use first read as bias
  bool nlCorr;   // Apply non-linearity correction

  // Accumulation arrays
  double *sy;                  // Sum of y values
  double *sxy;                 // Sum of x*y values
  uint16_t *n;                 // Number of valid reads per pixel
  std::vector<double> intTime; // Integration times for each read

  // Result arrays
  double *a;      // Slope (DN/s)
  double *b;      // Intercept (DN)
  double *errors; // Fit errors

  // Working buffer
  uint16_t *databuffer;

  bool owf; // Overwrite flag

  // Image ready status
  bool *im_ready;

  // Thread management
  std::mutex mtx;
  std::thread write_thread;
  bool thread_running;
  void writeCoefficient_threaded(std::string fname);

  // Helper methods
  void saveCoefficient(std::string fname);
  void refpixcorr(double *im);
  void cumulsum(std::vector<double> s, std::vector<double> *Sum);
};

#endif // FITS2RAMP_H
