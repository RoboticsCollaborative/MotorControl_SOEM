/**
 * \file
 * \Headerfile for controller.c.
 *
 * Edited by Zikun Yu -- yu.zik@husky.neu.edu
 *
 * Whitney Robotics Lab  10/1/2018
 */

#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <stdio.h>
#include <inttypes.h>

double PDcontroller (double ReferencePosition, double ActualPosition, double ActualVelocity);

#endif //_CONTROLLER_H
