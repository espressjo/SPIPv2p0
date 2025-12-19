#include "standard_spip_cmd.h"
#include "uics/uics_err_code.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include <uics/uics_number.h>
extern Log HxRGlog;

void testDelay(instHandle *handle, cmd *cc) {
  if (!uics_is_int((*cc)["ms"])) {
    cc->respond("Wrong argument, require integer", uicsCMD_ERR_PARAM_VALUE);
    return;
  }
  int _ms = std::stod((*cc)["ms"]);
  std::this_thread::sleep_for(std::chrono::milliseconds(_ms));
  cc->respond();

  return;
}
