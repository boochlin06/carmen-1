#include <carmen/carmen.h>
#include "sick.h"
#include "laser.h"
#include "laser_ipc.h"

sick_laser_t laser1, laser2, laser3, laser4;
int use_laser1 = 0, use_laser2 = 0;
int use_laser3 = 0, use_laser4 = 0;
int quit_signal = 0;

void set_default_parameters(sick_laser_p laser, int laser_num)
{
  laser->settings.type = LMS;
  laser->settings.range_res = CM;
  laser->settings.range_dist = RANGE80M;
  laser->settings.laser_num = laser_num;
  strcpy(laser->settings.device_name, "/dev/ttyS0");
  laser->settings.detect_baudrate = TRUE;
  laser->settings.use_highspeed = FALSE;
  laser->settings.start_baudrate = 9600;
  laser->settings.set_baudrate = 38400;
  laser->settings.databits = 8;
  laser->settings.parity = N;
  laser->settings.stopbits = 1;
  laser->settings.hwf = 0;
  laser->settings.swf = 0;
  laser->settings.angle_range = 180;
  laser->settings.angle_resolution = RES_1_00_DEGREE;
  laser->settings.laser_flipped = 0;
}

void check_parameter_settings(sick_laser_p laser)
{
  /*********************** TYPE CHECKING **************************/
  if(laser->settings.type == PLS) {
    strncpy(laser->settings.password, PLS_PASSWORD, 8);
    laser->settings.parity = E;
  } 
  if(laser->settings.type == LMS) {
    strncpy(laser->settings.password, LMS_PASSWORD, 8);
    laser->settings.parity = N;
  } 
  
  /*********************** START BAUDRATE **************************/
  if(laser->settings.detect_baudrate)
    laser->settings.start_baudrate = 9600;
  else if(laser->settings.start_baudrate != 9600 &&
	  laser->settings.start_baudrate != 19200 &&
	  laser->settings.start_baudrate != 38400 &&
	  laser->settings.start_baudrate != 500000) {
    fprintf(stderr, "ERROR: start baudrate = %d is not valid!\n",
	    laser->settings.start_baudrate);
    exit(1);
  }

  /*********************** SET BAUDRATE **************************/
  if(laser->settings.set_baudrate != 9600 &&
     laser->settings.set_baudrate != 19200 &&
     laser->settings.set_baudrate != 38400 &&
     laser->settings.set_baudrate != 500000) {
    fprintf(stderr, "ERROR: set baudrate = %d is not valid!\n",
	    laser->settings.set_baudrate);
    exit(1);
  } 
  else if(laser->settings.set_baudrate == 500000)
    laser->settings.use_highspeed = TRUE;
  
  /*********************** NUM VALUES **************************/
  if(laser->settings.angle_range != 180 && 
     laser->settings.angle_range != 100) {
    fprintf(stderr, "ERROR: angle range = %d is not valid!\n",
	    laser->settings.angle_range);
    exit(1);
  }
  
  /************************** ANGLE RANGE ************************/
  if(laser->settings.angle_range == 100) {
    if(laser->settings.angle_resolution == RES_1_00_DEGREE)
      laser->settings.num_values = 101;
    else if(laser->settings.angle_resolution == RES_0_50_DEGREE)
      laser->settings.num_values = 201;
    else if(laser->settings.type == LMS)
      laser->settings.num_values = 401;
    else
      fprintf(stderr, "ERROR: ang-res=0.25 is not valid for this laser!\n");
  }
  else {
    if(laser->settings.angle_resolution == RES_1_00_DEGREE)
      laser->settings.num_values = 181;
    else if(laser->settings.angle_resolution == RES_0_50_DEGREE)
      laser->settings.num_values = 361;
    else {
      fprintf(stderr, "ERROR: ang-res=0.25 and ang-range=180 is not valid!\n");
      exit(1);
    }
  }
  if(laser->settings.type == PLS) {
    if(laser->settings.angle_range == 100) {
      fprintf(stderr, "ERROR: type = PLS and ang-range=100 is not valid!\n");
      exit(1);
    } 
  }
}

void interpret_params(sick_laser_p laser, char *dev, char *type, double res)
{
  strcpy(laser->settings.device_name, dev);
  if(strcmp(type, "LMS") == 0)
    laser->settings.type = LMS;
  else if(strcmp(type, "PLS") == 0)
    laser->settings.type = PLS;
  if(res == 0.25) {
    laser->settings.angle_resolution = RES_0_25_DEGREE;
    laser->settings.angle_range = 100;
  }
  else if(res == 0.5)
    laser->settings.angle_resolution = RES_0_50_DEGREE;
  else
    laser->settings.angle_resolution = RES_1_00_DEGREE;
  check_parameter_settings(laser);
}

void read_parameters(int argc, char **argv)
{
  char *dev1, *dev2, *dev3, *dev4;
  char *str1, *str2, *str3, *str4;
  double res1, res2, res3, res4;

  carmen_param_t laser_devs[] = {
    {"laser", "front_laser_dev", CARMEN_PARAM_STRING, &dev1, 0, NULL},
    {"laser", "rear_laser_dev", CARMEN_PARAM_STRING, &dev2, 0, NULL},
    {"laser", "laser3_dev", CARMEN_PARAM_STRING, &dev3, 0, NULL},
    {"laser", "laser4_dev", CARMEN_PARAM_STRING, &dev4, 0, NULL}};
  carmen_param_t laser1_params[] = {
    {"laser", "front_laser_type", CARMEN_PARAM_STRING, &str1, 0, NULL},
    {"laser", "front_laser_resolution", CARMEN_PARAM_DOUBLE, &res1, 0, NULL},
    {"laser", "front_laser_baud", CARMEN_PARAM_INT, 
     &laser1.settings.set_baudrate, 0, NULL},
    {"laser", "front_laser_flipped", CARMEN_PARAM_INT, 
     &laser1.settings.laser_flipped, 0, NULL}};
  carmen_param_t laser2_params[] = {
    {"laser", "rear_laser_type", CARMEN_PARAM_STRING, &str2, 0, NULL},
    {"laser", "rear_laser_resolution", CARMEN_PARAM_DOUBLE, &res2, 0, NULL},
    {"laser", "rear_laser_baud", CARMEN_PARAM_INT, 
     &laser2.settings.set_baudrate, 0, NULL}};
  carmen_param_t laser3_params[] = {
    {"laser", "laser3_type", CARMEN_PARAM_STRING, &str3, 0, NULL},
    {"laser", "laser3_resolution", CARMEN_PARAM_DOUBLE, &res3, 0, NULL},
    {"laser", "laser3_baud", CARMEN_PARAM_INT, 
     &laser3.settings.set_baudrate, 0, NULL}};
  carmen_param_t laser4_params[] = {
    {"laser", "laser4_type", CARMEN_PARAM_STRING, &str4, 0, NULL},
    {"laser", "laser4_resolution", CARMEN_PARAM_DOUBLE, &res4, 0, NULL},
    {"laser", "laser4_baud", CARMEN_PARAM_INT, 
     &laser4.settings.set_baudrate, 0, NULL}};

  carmen_param_install_params(argc, argv, laser_devs, 
			      sizeof(laser_devs) / sizeof(laser_devs[0]));

  if(strncmp(dev1, "none", 4) != 0) {
    use_laser1 = 1;
    carmen_param_install_params(argc, argv, laser1_params,
				sizeof(laser1_params) / 
				sizeof(laser1_params[0]));
    interpret_params(&laser1, dev1, str1, res1);
  }
  if(strncmp(dev2, "none", 4) != 0) {
    use_laser2 = 1;
    carmen_param_install_params(argc, argv, laser2_params,
				sizeof(laser2_params) / 
				sizeof(laser2_params[0]));
    interpret_params(&laser2, dev2, str2, res2);
  }
  if(strncmp(dev3, "none", 4) != 0) {
    use_laser3 = 1;
    carmen_param_install_params(argc, argv, laser3_params,
				sizeof(laser3_params) / 
				sizeof(laser3_params[0]));
    interpret_params(&laser3, dev3, str3, res3);
  }
  if(strncmp(dev4, "none", 4) != 0) {
    use_laser4 = 1;
    carmen_param_install_params(argc, argv, laser4_params,
				sizeof(laser4_params) / 
				sizeof(laser4_params[0]));
    interpret_params(&laser4, dev4, str4, res4);
  }
}

int carmen_laser_start(int argc, char **argv)
{
  /* initialize laser messages */
  ipc_initialize_messages();

  /* get laser parameters */
  set_default_parameters(&laser1, FRONT_LASER_NUM);  
  set_default_parameters(&laser2, REAR_LASER_NUM);
  set_default_parameters(&laser3, LASER3_NUM);
  set_default_parameters(&laser4, LASER4_NUM);
  read_parameters(argc, argv);
  
  /* start lasers, and start publishing scans */
  if(use_laser1)
    sick_start_laser(&laser1);
  if(use_laser2)
    sick_start_laser(&laser2);
  if(use_laser3)
    sick_start_laser(&laser3);
  if(use_laser4)
    sick_start_laser(&laser4);
  return 0;
}

void carmen_laser_shutdown(int signo __attribute__ ((unused)))
{
  if(use_laser1)
    sick_stop_laser(&laser1);
  if(use_laser2)
    sick_stop_laser(&laser2);
  if(use_laser3)
    sick_stop_laser(&laser3);
  if(use_laser4)
    sick_stop_laser(&laser4);
}

int carmen_laser_run(void)
{
  static int first = 1;
  static double last_update;
  static double last_alive = 0;
  double current_time;
  int print_stats;
  static int laser1_stalled = 0, laser2_stalled = 0, laser3_stalled = 0,
    laser4_stalled = 0;

  if(first) {
    last_update = carmen_get_time_ms();
    first = 0;
  }
  current_time = carmen_get_time_ms();
  print_stats = (current_time - last_update > 1.0);
  if(use_laser1) {
    sick_handle_laser(&laser1);
    if(laser1.new_reading)
      publish_laser_message(&laser1);   
    laser1_stalled = (current_time - laser1.timestamp > 1.0);
    if(print_stats) 
      fprintf(stderr, "L1: %s(%.1f%% full) ", laser1_stalled ?
	      "STALLED " : " ", (laser1.buffer_position - 
				 laser1.processed_mark) / 
	      (float)LASER_BUFFER_SIZE * 100.0);
  }
  if(use_laser2) {
    sick_handle_laser(&laser2);
    if(laser2.new_reading)
      publish_laser_message(&laser2);
    laser2_stalled = (current_time - laser2.timestamp > 1.0);
    if(print_stats) 
      fprintf(stderr, "L2: %s(%.1f%% full) ", laser2_stalled ?
	      "STALLED " : " ", (laser2.buffer_position - 
				 laser2.processed_mark) / 
	      (float)LASER_BUFFER_SIZE * 100.0);
  }
  if(use_laser3) {
    sick_handle_laser(&laser3);
    if(laser3.new_reading)
      publish_laser_message(&laser3);   
    laser3_stalled = (current_time - laser3.timestamp > 1.0);
    if(print_stats) 
      fprintf(stderr, "L3: %s(%.1f%% full) ", laser3_stalled ?
	      "STALLED " : " ", laser3.buffer_position / 
	      (float)LASER_BUFFER_SIZE * 100.0);
  }
  if(use_laser4) {
    sick_handle_laser(&laser4);
    if(laser4.new_reading)
      publish_laser_message(&laser4);   
    laser4_stalled = (current_time - laser4.timestamp > 1.0);
    if(print_stats) 
      fprintf(stderr, "L4: %s(%.1f%% full) ", laser4_stalled ?
	      "STALLED " : " ", laser4.buffer_position / 
	      (float)LASER_BUFFER_SIZE * 100.0);
  }
  if(print_stats) {
    fprintf(stderr, "\n");
    last_update = current_time;
  }
  if(current_time - last_alive > 1.0) {
    publish_laser_alive(laser1_stalled, laser2_stalled, laser3_stalled,
			laser4_stalled);
    last_alive = current_time;
  }
  return 0;
}

void shutdown_laser(int x)
{
  carmen_laser_shutdown(x);
  close_ipc();
  exit(-1);
}

