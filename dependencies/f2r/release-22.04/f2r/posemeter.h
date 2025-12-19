#ifndef POSEMETER_H
#define POSEMETER_H

#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include "uics/uics_fits.h"
#include "refpxcorr.h"

class posemeter {

public:
  posemeter(int shapeX, int shapeY, bool oddeven = false, bool toponly = true);
  ~posemeter();
  
  void add_data(uint16_t *data);       // Add new data
  void add_mask(std::string filename); // Load mask FITS file
  void get_flux(double fluxTable[]);   // Get flux for each mask
  
  int get_num_masks() const { return masks.size(); }

private:
  void compute();
  
  // Image dimensions
  int shapeX;
  int shapeY;
  size_t length;
  
  // Storage for images and processed data
  uint16_t *current_image;   // Current image
  uint16_t *previous_image;  // Previous image for subtraction
  double *diff_image;        // Difference image (current - previous)
  double *processed_image;   // After reference pixel correction
  
  // Masks storage - each mask is a vector of uint16_t
  std::vector<std::vector<uint16_t>> masks;
  
  // Reference pixel correction
  refpxcorr *refpx;
  
  // Flags
  bool has_previous;         // Whether we have a previous image for subtraction
  bool data_ready;           // Whether processed data is available
  
  // FITS reader
  fitsio<uint16_t> fits_reader;
};

#endif // POSEMETER_H
