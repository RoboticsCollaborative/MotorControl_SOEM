/**
 * \file
 * \Headerfile of config_map.c
 * 
 * Edited by Zikun Yu -- yu.zik@husky.neu.edu
 *
 * Whitney Robotics Lab  9/6/2018 
 */

#ifndef _CONFIG_MAP_H
#define _CONFIG_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "ethercat.h"


int motor_setup (uint16 slave);
double saturation (double encoder);
//int TargetPosition (uint16 slave, int32 value);
//int TargetTorque (uint16, int16);
//int GetActualPosition (uint16 slave);
//int GetActualVelocity (uint16 slave);


#endif //_CONFIG_MAP_h
