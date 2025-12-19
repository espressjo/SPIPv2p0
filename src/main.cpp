// then we include libastro
#include "spip_acquisition.h"
#include "spip_getstatus.h"
#include "standard_spip_cmd.h"
#include <astro/telemetry_cmd.h>
#include <libastro.h>
#include <liboptim.h>
#include <uics.h>
using namespace std;

Log HxRGlog;

int main(int argc, char *argv[]) {
  // we nee to override BASEPATH and CONFIGFILE
  set_basepath("/opt/spipv2p0");
  set_configfile("/opt/spipv2p0/config/spip.conf");

  // set the log path
  HxRGlog.setPath(join(get_basepath(), "/log"));
  HxRGlog.writeto("Startup");
  // initialize the handle
  instHandle handle = {}; // init to zero
  i_handle(&handle);
  ArgParser parser(argc, argv);

  //::::::::::::::::::::::::::::::::
  //:::   Manage the arguments   :::
  //::::::::::::::::::::::::::::::::

  /*
  parser parse(argc, argv);

  if (parse.isarg("--help") || parse.isarg("-h")) { // print help menu
    parse.helper();
    return 0;
  }

  if (parse.isarg("--telemetry") ||
      parse.isarg("-t")) { // print telemetry right after initialization
    handle.acq.readtelemetry = true; // default false
    HxRGlog.writeto("Init Telemetry will be perform");
  }
  if (parse.isarg("--simulation") ||
      parse.isarg("-s")) { // set the hardware simulation flag.
    HxRGlog.writetoVerbose("Hardware Simulation Mode set");
    handle.m_status.simulator = 1; // default false
  }
*/
  //::::::::::::::::::::::::::::::::::::::::::::
  //:::   Start the UDP and TCP/IP handler   :::
  //::::::::::::::::::::::::::::::::::::::::::::
  uics UICS(&handle, join(get_basepath(), "/config"),
            join(get_basepath(), "/config"), false);

  //::::::::::::::::::::::::::::::::::::::
  //:::   Setup the forced transition  :::
  //::::::::::::::::::::::::::::::::::::::
  UICS.executeOnce(ACQ, IDLE);

  //::::::::::::::::::::::::::::
  //:::   Start the threads  :::
  //::::::::::::::::::::::::::::
  // UICS.add_thread(cmd_status, &handle, "status", false);
  // UICS.add_thread(cmd_status_b64, &handle, "b64status", false);
  UICS.add_thread(spip_status_t, &handle, "b64spipstatus", false);

  //::::::::::::::::::::::::::::::::::::::
  //:::   Define all function to the   :::
  //:::   to the proper state          :::
  //::::::::::::::::::::::::::::::::::::::

  // UICS.add_function("simulator", c_set_simulation, INIT);
  UICS.add_function("init", hrdw_initialization, INIT);
  UICS.add_function("set_acq_parameters", setParam, CONFIG);
  UICS.add_function("close", closeMacie, CONFIG);
  // UICS.add_function("start", acquisition_cpapir, ACQ);
  UICS.add_function("preamp", preamp_cmd, CONFIG);
  UICS.add_function("telemetry", r_telemetry, CONFIG);

  UICS.add_function("dataproduct", set_data_product, CONFIG);
  // UICS.add_function("telemetry", r_telemetry, CONFIG);
  UICS.add_function("upload", uploadMCD, CONFIG);
  UICS.add_function("start", spip_acquisition, ACQ);
  // UICS.add_function("testdelay", testDelay, CONFIG);
  UICS.run();
  return 0;
}
