/**
 * \file
 * \User defined controller.
 *
 * Edited by Zikun Yu -- yu.zik@husky.neu.edu
 *
 * Whitney Robotics Lab  10/1/2018
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "ethercat.h"
#include "config_map.h"

/*
typedef struct PACKED
{
    int32 ac_pos; // Actual position (6064)
    int32 ac_vel; // Acutal velocity (606C)
} in_motor_t;
PACKED_END

typedef struct PACKED
{
    int32 tg_pos; // Target position (607A)
    int16 tg_tau; // Target torque (6071)
} out_motor_t;
PACKED_END
*/

/** PD controller
 * @param[in] slave	=  Slave index.
 * @param[in] ref_pos	=  Reference position.
 * @param[out] u	=  Target position.
 */
double PDcontroller (double ReferencePosition, double ActualPosition, double ActualVelocity)
{
    double Kp = 50, Kd = 0.2, u;
    u = Kp*(ReferencePosition - ActualPosition) + Kd*(0 - ActualVelocity);
    if (u > 10) u = 10;
    else if(u<-10) u = -10;
    return u;
}

