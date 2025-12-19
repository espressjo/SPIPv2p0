#ifndef UICS_FITS_H
#define UICS_FITS_H

#include "uics_header.h"
#include <fitsio.h>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <vector>

// Enum for extension data types
enum class ExtType { UINT16, DOUBLE };

// Structure to hold extension information
template <typename _type> struct ExtensionInfo {
  std::string name;
  long shape[2];
  ExtType type;
  _type *data;
  size_t length;

  ExtensionInfo() : data(nullptr), length(0) {
    shape[0] = 0;
    shape[1] = 0;
  }

  ~ExtensionInfo() {
    if (data != nullptr) {
      delete[] data;
      data = nullptr;
    }
  }
};

template <typename _type> class fitsio {

public:
  // Constructor for writing (requires image size)
  fitsio(long imsize[2], bool stream = false);

  // Constructor for reading (no size needed initially)
  fitsio();

  ~fitsio();

  uicsHeader header;

  void setInfo(std::string path);
  void setInfo(std::string path, std::string filename);

  // Thread-safe write: copies pData immediately to protect against external
  // modifications
  bool write(_type *pData, std::string filename = "");
  bool write_with_bintable(_type *pData, std::string filename = "");
  bool overwrite;

  // Read methods
  bool read(std::string filename, int extension = 1);
  _type *get_data() const { return im; }

  // Get single pixel value from specific extension
  _type extension_value(int extension_number, long x, long y);

  int update_keyword(std::string kw, int value);
  int update_keyword(std::string kw, double value);
  int update_keyword(std::string kw, std::string value);
  void get_shape(long shape[]);
  // Extension management methods
  int add_ext(std::string name, long shape[2], ExtType type);
  bool set_ext_data(_type *data, int extension);

  // Overload for setting uint16_t data to extensions when template is double
  bool set_ext_data_uint16(uint16_t *data, int extension);

  bool write_ext_file_on_dsk(std::string filename = "");

private:
  std::string path;
  std::string filename;
  std::string current_filename; // Track current file for streaming
  bool stream;

  // Internal buffer: pData is copied here immediately in write()
  // This protects against race conditions if pData is modified by another
  // thread
  _type *im;

  long imsize[2]; // x,y
  size_t length;  // length of array
  uint16_t size_x;
  uint16_t size_y;

  // Extension storage
  std::vector<ExtensionInfo<_type>> extensions;

  // Mutex to protect write operations and internal buffer
  std::mutex write_mutex;

  // Helper functions for write operations
  bool write_new_file(_type *pData, std::string full_path,
                      bool prepare_for_stream);
  bool write_stream_append(_type *pData, std::string full_path);
};

void printerrors(int status);

#endif
