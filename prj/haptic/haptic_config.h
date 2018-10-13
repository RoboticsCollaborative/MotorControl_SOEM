/**
 * \file
 * \Headerfile for haptic_config.c.
 *
 * Edited by Zikun Yu -- yu.zik@husky.neu.edu
 *
 * Whitney Robotics Lab  9/13/2018
 */

#ifndef _PDO_MAP_H
#define _PDO_MAP_H

#include <stdio.h>

#include "ethercat.h"

void haptic_config(char *ifname);
OSAL_THREAD_FUNC ecatcheck( void *ptr );
OSAL_THREAD_FUNC switch_off( void *ptr );

#endif //_HAPTIC_CONFIG_H
