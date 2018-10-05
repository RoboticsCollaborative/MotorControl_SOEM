/**
 * \file
 * \ Configure motor static parameters.
 * 
 * Edited by Zikun Yu -- yu.zik@husky.neu.edu
 *
 * Whitney Robotics Lab  10/2/2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "ethercat.h"
#include "config_map.h"
#include "ecattype.h"

int motor_init(uint16 motor)
{

    uint8 BUF8; uint32 BUF32;

    /* Switch to torque mode */
    /* OpMode: 8 => CSP mode */
    /* OpMode: 10 => Torque mode */
    WRITE_SDO(motor, 0x6060, 0, BUF8, 10, "OpMode");
    READ_SDO(motor, 0x6061, 0, BUF8, "OpMode display");
    /* rated torque (0.001 Nm) */
    /* 1 Nm = 5000 units (up to 4Nm) */
    /* 1 unit = 0.2 mNm */ 
    WRITE_SDO(motor, 0x6076, 0, BUF32, 200, "Motor rated torque");

    return 0;
}

