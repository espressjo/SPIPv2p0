#include "spip_acquisition.h"

#include "astro/registers_header.h"
#include <f2r/fits2ramp.h>
#include <f2r/posemeter.h>
#include <iostream>
#include <sstream>
#include <uics/uics_header.h>

void spip_acquisition(instHandle *handle, cmd *cc) {

  bool read_telemetry = false;
  if (!(*cc)["-telemetry"].empty()) {
    read_telemetry = true;
  }

  cc->respond(); // this function always return OK
  // set a few status
  handle->m_status.hdwr_status = hxrgEXP_INTEGRATING;
  handle->macie.haltTriggered = 0; // make sure we start fresh
  handle->m_status.isOngoing = 1;  // make sure no other command are sent

  double mjd_obs = 0, m_intTime = 0;
  int socket_size = 0, nbWords = 0, ramp = 1, read = 1, ret = 0;
  char file[1024];
  string nl_uid = "", bias_uid = "", filename = "", read_path = "";
  string m1 = join(join(handle->root_path, "/cal"), handle->acq.posemeter_mask);
  string m2 = join(join(handle->root_path, "/cal"),
                   handle->acq.posemeter_mask_optional);

  const double readout_t = c_integration_time(handle);
  double triggerTimeout = c_trigger_timeout(handle);
  const long buffer_size = frameSize(handle);
  double deltaT = 0.0;
  string GMTEND = "";
  std::chrono::time_point<std::chrono::high_resolution_clock> now_time;
  // create data array
  unsigned short *pData = nullptr;
  uint16_t *posemeter_im = nullptr;
  // fits2ramp *f2r = nullptr;
  // Create the fits2ramp and readout fits object
  long naxes[2] = {handle->preamp.frameY, handle->preamp.frameX};
  fitsio<uint16_t> fits(naxes, handle->acq.stream_cube);
  fits2ramp f2r(naxes[0], naxes[1], readout_t);
  //
  // Allocate image array
  try {
    pData = new unsigned short[buffer_size];
  } catch (const std::bad_alloc &e) {
    HxRGlog.writetoVerbose("[critical] Failed to allocate memory for pData");
    handle->m_status.hdwr_status = hxrgEXP_COMPL_FAILURE;
    handle->m_status.isOngoing = 0;
    return;
  }

  // this is used to create the folders for the "loose" individual reads.
  std::time_t t = std::time(nullptr);
  std::tm *now = std::localtime(&t);
  if (!now) {
    HxRGlog.writetoVerbose("[critical] localtime() failed");
    handle->m_status.hdwr_status = hxrgEXP_COMPL_FAILURE;
    delete[] pData;
    handle->m_status.isOngoing = 0;
    return;
  }
  std::ostringstream directory_t;
  directory_t << std::put_time(now, "%Y%m%d%H%M%S");
  std::string dir_C = directory_t.str();
  // Check for division by zero (Issue #12)
  if (handle->preamp.nbOutput == 0) {
    HxRGlog.writetoVerbose(
        "[critical] nbOutput is zero - cannot calculate sampling timeout");
    handle->m_status.hdwr_status = hxrgEXP_COMPL_FAILURE;
    delete[] pData;
    handle->m_status.isOngoing = 0;
    return;
  }

  unsigned short sampling_timeout = static_cast<unsigned short>(
      (static_cast<double>(handle->preamp.frameX) *
       static_cast<double>(handle->preamp.frameY) * handle->info.readoutTime /
       static_cast<double>(handle->preamp.nbOutput)) *
          1000.0 +
      10000.0);
  //:::::::::::::::::::::::::::::::
  //:::   SETUP the posemeter   :::
  //:::::::::::::::::::::::::::::::
  // f2r.fits->header.binTable.create(
  //     {"time", "fiber1", "fiber2"},
  //     {ColType::DOUBLE, ColType::DOUBLE, ColType::DOUBLE});
  // posemeter _posemeter(naxes[0],naxes[1])
  //::::::::::::::::::::::::::::::::::::::
  //:::   Setup the fits2ramp object   :::
  //::::::::::::::::::::::::::::::::::::::

  // Set some f2r parameters

  // f2r.setBlockSize(handle->head.numBlock);
  // Configure fits2ramp
  f2r.ow_existingfile(true);              // Overwrite existing files
  f2r.refCorr = handle->f2r.flag_refpx;   // Apply reference pixel correction
  f2r.refpxtoponly = handle->f2r.toponly; // Use only top reference pixels
  f2r.refpxoddeven = handle->f2r.oddeven; // Don't separate odd/even columns
  f2r.saturation = 45000;                 // Saturation level

  // handle->f2r.nl_status = false;
  handle->f2r.uid_nonlin = "";
  handle->f2r.uid_bias = "";
  // handle->f2r.bias_status = false;
  // if (handle->f2r..nl) {
  // upload non-lin coefficient if they exist
  nl_uid =
      f2r.load_nl_coefficient(join(handle->acq.calpath, "spip_cal_nl.fits"));
  if (nl_uid.compare("") != 0) {
    // handle->f2rConfig.nl_status = true;
    handle->f2r.uid_nonlin = nl_uid;
  }
  //}
  // if (handle->f2rC.bias) {
  bias_uid = f2r.load_bias(join(handle->acq.calpath, "spip_cal_bias.fits"));
  if (bias_uid.compare("") != 0) {
    // handle->f2rConfig.bias_status = true;
    handle->f2r.uid_bias = bias_uid;
  }
  //:::::::::::::::::::::::::::::::::::
  //:::    END of fits2ramp SETUP   :::
  //:::::::::::::::::::::::::::::::::::
  //::::::::::::::::::: set parameter and reconfigure :::::::::::::::::
  delay(500);

  if (set_asic_acq_parameters(handle) != 0) {
    handle->m_status.hdwr_status = hxrgEXP_COMPL_FAILURE;
    goto endacq;
  }

  //:::::::::::::::::::::::::::::::
  //:::   Make default header   :::
  //:::::::::::::::::::::::::::::::
  if (u_header(&fits.header, handle, read_telemetry) != 0 ||
      u_header(&f2r.fits->header, handle, read_telemetry) !=
          0) // header for data cube or single file
  {
    HxRGlog.writetoVerbose("[critical] Failed to setup the header.");
    handle->m_status.hdwr_status = hxrgEXP_COMPL_FAILURE;
    goto endacq;
  }
  //:::::::::::::::::::::::::::::::
  //:::   Trigger integration   :::
  //:::::::::::::::::::::::::::::::

  HxRGlog.writetoVerbose("Configuring science interface for acquisition.");
  if (MACIE_ConfigureGigeScienceInterface(handle->macie.handle,
                                          handle->macie.slctMACIEs, 0, 0, 42037,
                                          &socket_size) != MACIE_OK) {
    HxRGlog.writetoVerbose(
        "[critical] Gige science interface re-configuration failed.");
    handle->m_status.hdwr_status = hxrgEXP_COMPL_FAILURE;
    goto endacq;
  }

  if (r_ASIC6900(handle) != ASIC_IDLE) {
    HxRGlog.writetoVerbose(
        " [critical] Asic h6900 is not in idle when we expect it to be.");
    handle->m_status.hdwr_status = hxrgEXP_COMPL_FAILURE;
    goto endacq;
  }

  if (w_ASIC6900(handle, ASIC_ACQ) != 0) {
    HxRGlog.writetoVerbose("{ciritical} Unable to trigger an exposure.");
    handle->m_status.hdwr_status = hxrgEXP_COMPL_FAILURE;
    goto endacq;
  }

  HxRGlog.writeto("Trigger succeeded.");
  HxRGlog.writeto("Expected framesize: " + std::to_string(buffer_size));
  HxRGlog.writeto("Theoritical integration time: " + std::to_string(readout_t) +
                  " s.");
  HxRGlog.writeto("Socket size: " + std::to_string(socket_size) + " KB.");
  HxRGlog.writetoVerbose("Waiting for Science Data");

  delay(1000);

  fits.header.edit_entry("SEQID", std::to_string(uniqueID()));

  if (wait_for_data(handle) != 0) {
    handle->m_status.hdwr_status = hxrgEXP_COMPL_FAILURE;
    goto endacq;
  }
  if (isHaltTriggered(handle)) {
    goto endacq;
  }

  fits.header.edit_entry("RAMP", 1);
  // getCurrentTimestamp,calculateTimeDifference
  now_time = getCurrentTimestamp();
  deltaT = c_integration_time(handle);
  GMTEND = "";

  for (ramp = 1; ramp <= static_cast<int>(handle->acq.ramp); ramp++) {
    //:::::::::::::::::::::
    //:::   Ramp loop   :::
    //:::::::::::::::::::::
    mjd_obs = 0;
    // We may want to override the UID.
    if (handle->optim.override_uid != 0) {
      fits.header.edit_entry("OUID", true);
      f2r.fits->header.edit_entry("OUID", true);
      fits.header.edit_entry("UID", std::to_string(handle->optim.override_uid));
      f2r.fits->header.edit_entry("UID",
                                  std::to_string(handle->optim.override_uid));
    } else {
      fits.header.edit_entry("OUID", false);
      f2r.fits->header.edit_entry("OUID", false);
      fits.header.edit_entry("UID", std::to_string(uniqueID()));
      f2r.fits->header.edit_entry("UID", std::to_string(uniqueID()));
    }
    fits.header.edit_entry("RAMP", ramp);
    f2r.fits->header.edit_entry("RAMP", ramp);
    //:::::::::::::::::::::
    //:::   Read loop   :::
    //:::::::::::::::::::::
    for (read = 1; read <= static_cast<int>(handle->acq.read); read++) {
      fits.header.edit_entry("READ", read);
      f2r.fits->header.edit_entry("READ", read);

      if (isHaltTriggered(handle)) {
        goto endacq;
      }

      HxRGlog.writetoVerbose("Reading (ramp/read): " + to_string(ramp) + "/" +
                             to_string(read));
      nbWords = 0;
      nbWords = MACIE_ReadGigeScienceData(handle->macie.handle,
                                          sampling_timeout, buffer_size, pData);

      //:::::::::::::::::::::::::::::::::::::::::::::::::::
      //:::   For engineering purpose.                  :::
      //:::   Measure the sampling (integration) time   :::
      //:::   Only reads > 3 will be computed.          :::
      //:::::::::::::::::::::::::::::::::::::::::::::::::::
      if (read == 1) {
        // begining of integration
        fits.header.edit_entry("RSTART", gmt_HH_MM_SSmm(-1 * readout_t));
      }
      if (read == 2) {
        now_time = getCurrentTimestamp();
      }
      if (read > 2) {
        deltaT = calculateTimeDifference(now_time, getCurrentTimestamp());
        deltaT = deltaT / (read - 2);
        fits.header.edit_entry("MSAMP", deltaT);
      }
      // Calculate the begining of the integration.
      // We have to subtract the time to read a frame.
      mjd_obs = mjd_time_now() - secondsToMJD(static_cast<double>(readout_t));

      // Time related information
      fits.header.edit_entry("GMTSTART", gmt_HH_MM_SSmm(-1 * readout_t));
      fits.header.edit_entry("LOCSTART", local_HH_MM_SSmm(-1 * readout_t));
      fits.header.edit_entry("GMTSTOP", gmt_HH_MM_SSmm());
      fits.header.edit_entry("LOCSTOP", local_HH_MM_SSmm());
      fits.header.edit_entry("MJD", mjd_obs); // MJD at begining of integration
      fits.header.edit_entry("READ", read);
      GMTEND = gmt_HH_MM_SSmm(); // running end of integration timestamp

      // check the number of words received.
      if (static_cast<int>(buffer_size) != nbWords) {
        HxRGlog.writetoVerbose(
            "Size of data read does not match the size of the array");
        HxRGlog.writetoVerbose("Received " + to_string(nbWords) +
                               ", but expected " + to_string(buffer_size) +
                               " words");
      }
      if (!pData) {
        HxRGlog.writetoVerbose("[critical] no image received.");
        handle->m_status.hdwr_status = hxrgEXP_COMPL_FAILURE;
        goto endacq;
      }
      HxRGlog.writetoVerbose("Image read successfully.");
      //::::::::::::::::::::::::::::::::::::::::::::::::
      //:::   copy the image inside the f2r object   :::
      //::::::::::::::::::::::::::::::::::::::::::::::::
      f2r.execute(pData);
      // copy_arr(&pData, &posemeter_im, buffer_size);
      //::::::::::::::::::::::::::::::::::::::::::::::::

      //::::::::::::::::::::::::::::::
      //:::   exectute posemeter   :::
      //::::::::::::::::::::::::::::::

      // pm.execute(posemeter_im, static_cast<uint16_t>(read),
      // &handle->info.flux,
      //           &handle->info.flux_optional);
      // add posemeter line in header
      // fits.header.add_posemeter(jd_time_now(), handle->info.flux,
      // handle->info.flux_optional);

      // Check if we want to override the path
      read_path = (handle->optim.override_path.compare("") != 0)
                      ? handle->optim.override_path
                      : handle->acq.savePath;

      //:::::::::::::::::::::::::::::::::::::::::::::::::::
      //:::   Manage which type of read file to write
      //::::
      //:::::::::::::::::::::::::::::::::::::::::::::::::::
      if (handle->acq.stream_cube) {
        filename = c_cube_name(handle, ramp);
        if (!fits.write(pData, filename)) {
          HxRGlog.writetoVerbose("[critical] failed to save file: " + filename);
          HxRGlog.writetoVerbose(MACIE_Error());
          handle->m_status.hdwr_status = hxrgEXP_COMPL_FAILURE;
          goto endacq;
        }
      } else {
        filename = c_read_name(handle, read, ramp, dir_C);
        if (!fits.write(pData, filename)) {
          HxRGlog.writetoVerbose("[critical] failed to save file: " + filename);
          HxRGlog.writetoVerbose(MACIE_Error());
          handle->m_status.hdwr_status = hxrgEXP_COMPL_FAILURE;
          goto endacq;
        }
      }
      HxRGlog.writetoVerbose("Fits written successfully.");
    } // read

    //::::::::::::::::::::::::::::::
    //:::	save the fits2ramp   :::
    //::::::::::::::::::::::::::::::

    fits.header.edit_entry("TINT", deltaT * (read - 1));
    f2r.fits->header.edit_entry("TINT", deltaT * (read - 1));
    fits.header.edit_entry("RSTOP", GMTEND);
    f2r.fits->header.edit_entry("RSTOP", GMTEND);

    if (handle->acq.saveRampFile) {
      filename = c_ramp_name(handle, ramp);
      f2r.writetofilenow(filename);
    }

    HxRGlog.writetoVerbose("Total integration time: " +
                           std::to_string(deltaT * (read - 1)) + " s.");
    handle->m_status.hdwr_status =
        hxrgEXP_COMPL_SUCCESS; // important to trigger temporary image with VLT
                               // engineering panel
  } // ramp

  handle->m_status.hdwr_status = hxrgEXP_COMPL_SUCCESS;

endacq:
  // wait 20 seconds to be sure we're back to idle
  if (wait4integration(handle, 20) != 0) {
    HxRGlog.writetoVerbose("[critical] Wait end of exposure timeout.");
  }
  // if (f2r) {
  //   f2r.stopthread();
  //   delete f2r;
  // }
  log_macie_error(handle);
  if (MACIE_CloseGigeScienceInterface(handle->macie.handle,
                                      handle->macie.slctMACIEs) != MACIE_OK) {
    HxRGlog.writetoVerbose("[critical] unable to close MACIE communication");
    HxRGlog.writetoVerbose(MACIE_Error());
  }
  handle->optim.override_uid = 0;
  handle->macie.haltTriggered = 0;
  handle->m_status.isOngoing = 0;
  handle->macie.imSize = 0;
  handle->macie.im_ptr = nullptr;
  if (pData) {
    delete[] pData;
  }
  if (posemeter_im) {
    delete[] posemeter_im;
  }
  return;
}
