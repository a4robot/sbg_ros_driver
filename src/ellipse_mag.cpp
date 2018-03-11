#include "ellipse.h"
#include <ros/ros.h>
#include <std_srvs/SetBool.h>
#include <std_srvs/Trigger.h>

Ellipse *e_ref;
bool start_calibration = false;
bool one_calibration_done = false;

bool calibration_process(std_srvs::Trigger::Request  &req,
                       std_srvs::Trigger::Response &res){
  if(start_calibration){
    e_ref->start_mag_calibration();
    res.success = true;
    res.message = "Start Calibration process";
    start_calibration = false;
  }
  else{
    e_ref->end_mag_calibration();
    res.success = true;
    res.message = "End Calibration process";
    one_calibration_done = true;
  }
  return true;
}

bool calibration_save(std_srvs::Trigger::Request  &req,
                      std_srvs::Trigger::Response &res){
  if(one_calibration_done){
    e_ref->save_mag_calibration();
    res.success = true;
    res.message = "Calibration saved";
  }
  else{
    res.success = false;
    res.message = "No Calibration found";
  }
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "sbg_ellipse_mag");
  ros::NodeHandle n;

  ROS_INFO("SBG DRIVER - Init node & load params");
  Ellipse ellipse(&n);
  e_ref = &ellipse;

  ellipse.init_publishers();

  ROS_INFO("SBG DRIVER - Ellipse connect");
  ellipse.connect();

  ros::ServiceServer calibration_srv = n.advertiseService("mag_calibration", calibration_process);
  ros::ServiceServer calibration_save_srv = n.advertiseService("mag_calibration_save", calibration_save);
  ros::spin();

  return 0;
}
