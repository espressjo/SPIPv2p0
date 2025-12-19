#ifndef COLUMN_DESELECT_H
#define COLUMN_DESELECT_H
#include <array>
#include <insthandle.h>
#include <liboptim.h>
#include <string>
#include <uics.h>
#include <vector>
using namespace std;

void col_deselect(instHandle *handle, cmd *cc);
int disable_col_deselect(instHandle *handle);
int open_col_deselect_file(string fname, vector<int> *columns);
array<xreg, 256> generate_registers(vector<int> columns);
int disable_col_deselect(instHandle *handle);
int write_coldeselect_asic(instHandle *handle, array<xreg, 256> values);
int read_coldeselect_asic(instHandle *handle, array<xreg, 256> &values);
#endif
