// File header
#include <config_applier.h>

using sbg::ConfigApplier;

/*!
 * Class to apply configuration to a device.
 */
//---------------------------------------------------------------------//
//- Constructor                                                       -//
//---------------------------------------------------------------------//

ConfigApplier::ConfigApplier(void):
m_reboot_needed_(false)
{

}

//---------------------------------------------------------------------//
//- Private  methods                                                  -//
//---------------------------------------------------------------------//

void ConfigApplier::checkConfigurationGet(const SbgErrorCode& ref_sbg_error_code, const std::string& ref_conf_title) const
{
  if (ref_sbg_error_code != SBG_NO_ERROR)
  {
    ROS_WARN("SBG_DRIVER - [Config] Unable to get the %s configuration : %s", ref_conf_title.c_str(), sbgErrorCodeToString(ref_sbg_error_code));
  }
}

void ConfigApplier::checkConfigurationApplied(const SbgErrorCode& ref_sbg_error_code, const std::string& ref_conf_title)
{
  if (ref_sbg_error_code != SBG_NO_ERROR)
  {
    ROS_WARN("SBG_DRIVER - [Config] Unable to set the %s configuration : %s", ref_conf_title.c_str(), sbgErrorCodeToString(ref_sbg_error_code));
  }
  else
  {
    ROS_INFO("SBG_DRIVER - [Config] %s updated on the device.", ref_conf_title.c_str());
    m_reboot_needed_ = true;
  }
}

void ConfigApplier::configureInitCondition(SbgEComHandle& ref_sbg_com_handle, const SbgEComInitConditionConf& ref_init_condition)
{
  //
  // Get the initial condition of the device, compare with the loaded parameters.
  // If the conditions are different, update the device configuration with the loaded parameters.
  //
  SbgEComInitConditionConf  init_condition;
  SbgErrorCode              error_code;

  error_code = sbgEComCmdSensorGetInitCondition(&ref_sbg_com_handle, &init_condition);

  checkConfigurationGet(error_code, std::string("Init conditions"));

  if ((init_condition.year != ref_init_condition.year)
  ||  (init_condition.month != ref_init_condition.month)
  ||  (init_condition.day != ref_init_condition.day)
  ||  (init_condition.altitude != ref_init_condition.altitude)
  ||  (init_condition.latitude != ref_init_condition.latitude)
  ||  (init_condition.longitude != ref_init_condition.longitude))
  {
    error_code = sbgEComCmdSensorSetInitCondition(&ref_sbg_com_handle, &ref_init_condition);

    checkConfigurationApplied(error_code, std::string("Init conditions"));
  }
}

void ConfigApplier::configureMotionProfile(SbgEComHandle& ref_sbg_com_handle, const SbgEComModelInfo& ref_motion_profile)
{
  //
  // Get the motion profile ID, and compare with the loaded one parameter.
  // If the profiles are different, update the device with the loaded one.
  //
  SbgEComModelInfo  motion_profile;
  SbgErrorCode      error_code;

  error_code = sbgEComCmdSensorGetMotionProfileInfo(&ref_sbg_com_handle, &motion_profile);

  checkConfigurationGet(error_code, std::string("Motion profile"));

  if (motion_profile.id != ref_motion_profile.id)
  {
    error_code = sbgEComCmdSensorSetMotionProfileId(&ref_sbg_com_handle, ref_motion_profile.id);

    checkConfigurationApplied(error_code, std::string("Motion profile"));
  }
}

void ConfigApplier::configureImuAlignement(SbgEComHandle& ref_sbg_com_handle, const SbgEComSensorAlignmentInfo& ref_sensor_align, std::array<float, 3> level_arms)
{
  //
  // Get the IMU alignement and level arms, and compare with the parameters.
  // If the alignement are differents, update the device with the loaded parameters.
  //
  SbgErrorCode                error_code;
  SbgEComSensorAlignmentInfo  sensor_alignement;
  float                       level_arms_device[3];

  error_code = sbgEComCmdSensorGetAlignmentAndLeverArm(&ref_sbg_com_handle, &sensor_alignement, level_arms_device);

  checkConfigurationGet(error_code, std::string("IMU alignement"));
 
  if ((level_arms_device[0] != level_arms[0])
  ||  (level_arms_device[1] != level_arms[1])
  ||  (level_arms_device[2] != level_arms[2])
  ||  (sensor_alignement.axisDirectionX != ref_sensor_align.axisDirectionX)
  ||  (sensor_alignement.axisDirectionY != ref_sensor_align.axisDirectionY)
  ||  (sensor_alignement.misRoll != ref_sensor_align.misRoll)
  ||  (sensor_alignement.misPitch != ref_sensor_align.misPitch)
  ||  (sensor_alignement.misYaw != ref_sensor_align.misYaw))
  {
    error_code = sbgEComCmdSensorSetAlignmentAndLeverArm(&ref_sbg_com_handle, &ref_sensor_align, level_arms.data());

    checkConfigurationApplied(error_code, std::string("IMU alignement"));
  }
}

void ConfigApplier::configureAidingAssignement(SbgEComHandle& ref_sbg_com_handle, const SbgEComAidingAssignConf& ref_aiding_assign)
{
  //
  // Get the aiding assignement, and compare with the loaded parameters.
  // If the assignement are differents, udpdate the device with the loaded parameters.
  //
  SbgEComAidingAssignConf aiding_assign;
  SbgErrorCode            error_code;

  error_code = sbgEComCmdSensorGetAidingAssignment(&ref_sbg_com_handle, &aiding_assign);

  checkConfigurationGet(error_code, std::string("Aiding assignement"));

  if ((aiding_assign.gps1Port != ref_aiding_assign.gps1Port)
  ||  (aiding_assign.gps1Sync != ref_aiding_assign.gps1Sync)
  ||  (aiding_assign.odometerPinsConf != ref_aiding_assign.odometerPinsConf)
  ||  (aiding_assign.rtcmPort != ref_aiding_assign.rtcmPort))
  {
    error_code = sbgEComCmdSensorSetAidingAssignment(&ref_sbg_com_handle, &aiding_assign);

    checkConfigurationApplied(error_code, std::string("Aiding assignement"));
  }
}

void ConfigApplier::configureMagModel(SbgEComHandle& ref_sbg_com_handle, const SbgEComModelInfo& ref_mag_model)
{
  //
  // Get the magnetometer model, and compare with the loaded parameter.
  // If the model are different, update the device with the loaded parameter.
  //
  SbgEComModelInfo  model_info;
  SbgErrorCode      error_code;

  error_code = sbgEComCmdMagGetModelInfo(&ref_sbg_com_handle, &model_info);

  checkConfigurationGet(error_code, std::string("Magnetometer model"));

  if (model_info.id != ref_mag_model.id)
  {
    error_code = sbgEComCmdMagSetModelId(&ref_sbg_com_handle, ref_mag_model.id);

    checkConfigurationApplied(error_code, std::string("Magnetometer model"));
  }
}

void ConfigApplier::configureMagRejection(SbgEComHandle& ref_sbg_com_handle, const SbgEComMagRejectionConf& ref_mag_rejection)
{
  //
  // Get the magnetometer rejection model, and compare with the loaded parameter.
  // If the model are different, update the device with the loaded parameter.
  //
  SbgEComMagRejectionConf mag_rejection;
  SbgErrorCode            error_code;

  error_code = sbgEComCmdMagGetRejection(&ref_sbg_com_handle, &mag_rejection);

  checkConfigurationGet(error_code, std::string("Magnetometer rejection"));

  if (mag_rejection.magneticField != ref_mag_rejection.magneticField)
  {
    error_code = sbgEComCmdMagSetRejection(&ref_sbg_com_handle, &ref_mag_rejection);

    checkConfigurationApplied(error_code, std::string("Magnetometer rejection"));
  }
}

void ConfigApplier::configureGnssModel(SbgEComHandle& ref_sbg_com_handle, const SbgEComModelInfo& ref_gnss_model)
{
  //
  // Get the Gnss model, and compare with the loaded model.
  // If the models are different, update the device with the loaded model.
  //
  SbgEComModelInfo  model_info;
  SbgErrorCode      error_code;

  error_code = sbgEComCmdGnss1GetModelInfo(&ref_sbg_com_handle, &model_info);

  checkConfigurationGet(error_code, std::string("Gnss model"));

  if (model_info.id != ref_gnss_model.id)
  {
    error_code = sbgEComCmdGnss1SetModelId(&ref_sbg_com_handle, ref_gnss_model.id);

    checkConfigurationApplied(error_code, std::string("Gnss model"));
  }
}

void ConfigApplier::configureGnssAlignement(SbgEComHandle& ref_sbg_com_handle, const SbgEComGnssAlignmentInfo& ref_gnss_alignement)
{
  //
  // Get the Gnss level arm, and compare with the loaded parameters.
  // If the level arms are different, update the device with the loaded parameters.
  //
  SbgEComGnssAlignmentInfo  gnss_alignement;
  SbgErrorCode              error_code;

  error_code = sbgEComCmdGnss1GetLeverArmAlignment(&ref_sbg_com_handle, &gnss_alignement);

  checkConfigurationGet(error_code, std::string("Gnss level arms"));

  if ((gnss_alignement.antennaDistance != ref_gnss_alignement.antennaDistance)
  ||  (gnss_alignement.leverArmX != ref_gnss_alignement.leverArmX)
  ||  (gnss_alignement.leverArmY != ref_gnss_alignement.leverArmY)
  ||  (gnss_alignement.leverArmZ != ref_gnss_alignement.leverArmZ)
  ||  (gnss_alignement.pitchOffset != ref_gnss_alignement.pitchOffset)
  ||  (gnss_alignement.yawOffset != ref_gnss_alignement.yawOffset))
  {
    error_code = sbgEComCmdGnss1SetLeverArmAlignment(&ref_sbg_com_handle, &ref_gnss_alignement);

    checkConfigurationApplied(error_code, std::string("Gnss level arms"));
  }
}

void ConfigApplier::configureGnssRejection(SbgEComHandle& ref_sbg_com_handle, const SbgEComGnssRejectionConf& ref_gnss_rejection)
{
  //
  // Get the Gnss rejection, and compare with the loaded parameters.
  // If the rejection are different, update the device with the loaded parameters.
  //
  SbgEComGnssRejectionConf  rejection;
  SbgErrorCode              error_code;

  error_code = sbgEComCmdGnss1GetRejection(&ref_sbg_com_handle, &rejection);

  checkConfigurationGet(error_code, std::string("Gnss rejection"));

  if ((rejection.hdt != ref_gnss_rejection.hdt)
  ||  (rejection.position != ref_gnss_rejection.position)
  ||  (rejection.velocity != ref_gnss_rejection.velocity))
  {
    error_code = sbgEComCmdGnss1SetRejection(&ref_sbg_com_handle, &ref_gnss_rejection);

    checkConfigurationApplied(error_code, std::string("Gnss rejection"));
  }
}

void ConfigApplier::configureOdometer(SbgEComHandle& ref_sbg_com_handle, const SbgEComOdoConf& ref_odometer)
{
  //
  // Get the odometer configuration, and compare with the loaded parameters.
  // If the conf are different, update the device with the loaded parameters.
  //
  SbgEComOdoConf  odom_conf;
  SbgErrorCode    error_code;

  error_code = sbgEComCmdOdoGetConf(&ref_sbg_com_handle, &odom_conf);

  checkConfigurationGet(error_code, std::string("Odometer"));

  if ((odom_conf.gain != ref_odometer.gain)
  ||  (odom_conf.gainError != ref_odometer.gainError)
  ||  (odom_conf.reverseMode != ref_odometer.reverseMode))
  {
    error_code = sbgEComCmdOdoSetConf(&ref_sbg_com_handle, &ref_odometer);

    checkConfigurationApplied(error_code, std::string("Odometer"));
  }
}

void ConfigApplier::configureOdometerLevelArm(SbgEComHandle& ref_sbg_com_handle, std::array<float, 3> odometer_level_arms)
{
  //
  // Get the odometer level arm, and compare with the loaded parameters.
  // If the level arms are different, update the device with the loaded parameters.
  //
  float         leverArm[3];
  SbgErrorCode  error_code;

  error_code = sbgEComCmdOdoGetLeverArm(&ref_sbg_com_handle, leverArm);

  checkConfigurationGet(error_code, std::string("Odometer level arms"));

  if ((leverArm[0] != odometer_level_arms[0])
  || (leverArm[1] != odometer_level_arms[1])
  || (leverArm[2] != odometer_level_arms[2]))
  {
    error_code = sbgEComCmdOdoSetLeverArm(&ref_sbg_com_handle, odometer_level_arms.data());

    checkConfigurationApplied(error_code, std::string("Odometer level arms"));
  }
}

void ConfigApplier::configureOdometerRejection(SbgEComHandle& ref_sbg_com_handle, const SbgEComOdoRejectionConf& ref_odometer_rejection)
{
  //
  // Get the odometer rejection mode, and compare with the loaded parameter.
  // If the mode are different, update the device with the loaded parameter.
  //
  SbgEComOdoRejectionConf odom_rejection;
  SbgErrorCode            error_code;

  error_code = sbgEComCmdOdoGetRejection(&ref_sbg_com_handle, &odom_rejection);

  checkConfigurationGet(error_code, std::string("Odometer rejection"));
  
  if (odom_rejection.velocity != ref_odometer_rejection.velocity)
  {
    error_code = sbgEComCmdOdoSetRejection(&ref_sbg_com_handle, &ref_odometer_rejection);

    checkConfigurationApplied(error_code, std::string("Odometer rejection"));
  }
}

void ConfigApplier::configureOutput(SbgEComHandle& ref_sbg_com_handle, SbgEComOutputPort output_port, const ConfigStore::SbgLogOutput &ref_log_output)
{
  SbgErrorCode      error_code;
  SbgEComOutputMode current_output_mode;

  //
  // Get the current output mode for the device and the selected log ID.
  // If output modes are different, udpate the device mode with the one loaded from the parameters.
  //
  error_code = sbgEComCmdOutputGetConf(&ref_sbg_com_handle, output_port, ref_log_output.message_class, ref_log_output.message_id, &current_output_mode);

  if (error_code != SBG_NO_ERROR)
  {
    ROS_WARN("Unable to get the output configuration from the device %s", sbgErrorCodeToString(error_code));
  }

  if (current_output_mode != ref_log_output.output_mode)
  {
    error_code = sbgEComCmdOutputSetConf(&ref_sbg_com_handle, output_port, ref_log_output.message_class, ref_log_output.message_id, ref_log_output.output_mode);

    if (error_code != SBG_NO_ERROR)
    {
      ROS_WARN("Unable to set the output configuration log ID %d from the device %s", ref_log_output.message_id, sbgErrorCodeToString(error_code));
    }
    else
    {
      m_reboot_needed_ = true;
    }
  }
}

//---------------------------------------------------------------------//
//- Parameters                                                        -//
//---------------------------------------------------------------------//

//---------------------------------------------------------------------//
//- Operations                                                        -//
//---------------------------------------------------------------------//

void ConfigApplier::applyConfiguration(const ConfigStore& ref_config_store, SbgEComHandle& ref_sbg_com_handle)
{
  //
  // Configure the connected device.
  //
  configureInitCondition(ref_sbg_com_handle, ref_config_store.getInitialConditions());
  configureMotionProfile(ref_sbg_com_handle, ref_config_store.getMotionProfile());
  configureImuAlignement(ref_sbg_com_handle, ref_config_store.getSensorAlignement(), ref_config_store.getSensorLevelArms());
  configureAidingAssignement(ref_sbg_com_handle, ref_config_store.getAidingAssignement());
  configureMagModel(ref_sbg_com_handle, ref_config_store.getMagnetometerModel());
  configureMagRejection(ref_sbg_com_handle, ref_config_store.getMagnetometerRejection());
  configureGnssModel(ref_sbg_com_handle, ref_config_store.getGnssModel());
  configureGnssAlignement(ref_sbg_com_handle, ref_config_store.getGnssAlignement());
  configureGnssRejection(ref_sbg_com_handle, ref_config_store.getGnssRejection());
  configureOdometer(ref_sbg_com_handle, ref_config_store.getOdometerConf());
  configureOdometerLevelArm(ref_sbg_com_handle, ref_config_store.getOdometerLevelArms());
  configureOdometerRejection(ref_sbg_com_handle, ref_config_store.getOdometerRejection());

  //
  // Configure the output, with all output defined in the store.
  //
  const std::vector<ConfigStore::SbgLogOutput>& ref_output_modes = ref_config_store.getOutputModes();

  for (const ConfigStore::SbgLogOutput& ref_output : ref_output_modes)
  {
    configureOutput(ref_sbg_com_handle, ref_config_store.getOutputPort(), ref_output);
  }

  //
  // Save configuration if needed.
  //
  if (m_reboot_needed_)
  {
    saveConfiguration(ref_sbg_com_handle);
  }
}

void ConfigApplier::saveConfiguration(SbgEComHandle& ref_sbg_com_handle)
{
  SbgErrorCode error_code;

  error_code = sbgEComCmdSettingsAction(&ref_sbg_com_handle, SBG_ECOM_SAVE_SETTINGS);

  if (error_code != SBG_NO_ERROR)
  {
    ROS_ERROR("Unable to save the settings on the SBG device - %s", sbgErrorCodeToString(error_code));
  }
  else
  {
    ROS_INFO("SBG_DRIVER - Settings saved and device rebooted.");
  }
}