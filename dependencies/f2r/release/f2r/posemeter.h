#ifndef POSEMETER_H
#define POSEMETER_H
#include <cstdint>
#include <string>
#include <vector>

#define HxRGDIM 4096
#define MAX 35000  // a ajuster
#define MIN -35000 // a ajuster
#define FLUX 0
#define NFLUX -1

class posemeter {

public:
  posemeter(uint32_t size);
  ~posemeter();
  int update_mask(std::string f_mask);
  int update_mask(std::string f_mask1, std::string f_mask2);
  std::vector<uint32_t> mask;
  std::vector<uint32_t> mask_optional;
  double extract_value(double *im);
  double extract_value(uint16_t *im);
  void extract_value(double *im, double *m1, double *m2);
  // void extract_value(uint16_t *im,double* m1,double* m2);
  int execute(int *im, uint16_t readNb, double *flux_level1);
  int execute(uint16_t *im, uint16_t readNb, double *flux_level1);
  int execute(int *im, uint16_t readNb, double *flux_level1,
              double *flux_level2);
  int execute(uint16_t *im, uint16_t readNb, double *flux_level1,
              double *flux_level2);

private:
  bool is_file_exist(const char *fileName);
  bool mask_loaded;
  std::string current_mask, current_mask_optional;
  uint16_t *im1, *im2;
  double *cds;
  uint32_t S;
  double *cIm;
};

#endif
