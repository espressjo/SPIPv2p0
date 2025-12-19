/*!
 * \brief HxRG reference pixels correction
 * function that corrects with reference pixels on the sides of the H2RG and
 * H4RG.
 *
 * On the periphery of the arrays, there are 4 pixels that are not
 * light-sensitive and that track drifts in the amplifiers. These are reference
 * pixels and they can reduce the effective readout noise by a factor of at
 * least 2 if properly used.
 *
 * The top and bottom pixels of each output (one of 32 vertical "ribbons") see
 * the start and end of each readout. To filter noise on a readout timescale, we
 * measure the median of the top and bottom reference pixels. We then define a
 * "slope" (matrix y_frac) that forces interpolates the gradient through the
 * light-sensitive pixels.
 *
 * For some arrays (e.g., the H2RG used for the AT4), the odd and even pixels
 * within each amplifier differ in behaviour. We therefore measure and correct
 * this "slope" independently for odd and even pixels. This is done by setting
 * oddeven=True in the function call. The default is oddeven=False
 *
 * The side (x=0-3 and x=N-3:N) of the HxRG arrays see the "faster" 1/f noise
 * that affects all amplifier. We therefore need to subtract the mean of the
 * side reference pixels to remove (most of) the 1/f noise. As the reference
 * pixels are themselves noisy, we apply a median filter to these pixels before
 * subtracting. The size of this running median filter is set with the
 * "medfilterwidth" variable.
 */
#ifndef REFPXCORR_LEGACY_H
#define REFPXCORR_LEGACY_H

#include <iostream>
// #include "fitslib.h"
#include "medianfilter.h"

class refpxcorr_legacy {

public:
  refpxcorr_legacy(double *Im, unsigned short imDim, bool oddeven = false,
                   bool toponly = true);
  ~refpxcorr_legacy();
  void refpixcorr(double *newIm);

private:
  bool refpxtoponly; // only use top ref. pixel
  bool refpxoddeven; // apply correction to odd and even column seperatly
  unsigned short dim;
  size_t size;
  double *im;
  double upEven[32];
  double downEven[32];
  double upOdd[32];
  double downOdd[32];
  double up[32];       // in case oddeven if false
  double down[32];     // in case oddeven if false
  double up_tmp[32];   // in case oddeven if false
  double down_tmp[32]; // in case oddeven if false
  // test coefficient
  double upEven_tmp[32];
  double downEven_tmp[32];
  double upOdd_tmp[32];
  double downOdd_tmp[32];

  double *frac_top;
  void addRef(double *add2, double *add);
  void extractCol(double *col, unsigned short c);
  double *frac_botom;
  void extractRefArray(double *newIm, unsigned short fsize);
  void extractOddEven(double *newIm);
  void removeTopBottom(double *newIm);
  void removeTop(double *newIm);
  void removeTopsimple(double *newIm);
  void removeTopBottomSimple(double *newIm);
  void extract(double *newIm);
};

#endif
