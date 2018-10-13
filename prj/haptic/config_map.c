/**
 * \file
 * \ PDO mapping functions for haptic robot
 * 
 * Edited by Zikun Yu -- yu.zik@husky.neu.edu
 *
 * Whitney Robotics Lab  9/5/2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include "ethercat.h"

//uint8  buf8;
//uint16 buf16;
//uint32 buf32;

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

/** SDO transfer functions 
 *
 * @param[in] slave	=  Slave number.
 * @param[in] index	=  Index to write.
 * @param[in] subindex	=  Subindex to write, must be 0 or 1 if CA is used.	
 * @param[in] value	=  Value to write.
 * @return wkc after a single SDO is trasfered.
 */
int SDO_write8 (uint16 slave, uint16 index, uint8 subindex, uint8 value)
{
    int wkc;
    wkc = ec_SDOwrite (slave, index, subindex, FALSE, sizeof(value), &value, EC_TIMEOUTRXM);
    return wkc;
}

int SDO_write16 (uint16 slave, uint16 index, uint8 subindex, uint16 value)
{
    int wkc;
    wkc = ec_SDOwrite (slave, index, subindex, FALSE, sizeof(value), &value, EC_TIMEOUTRXM);
    return wkc;
}

int SDO_write32 (uint16 slave, uint16 index, uint8 subindex, uint32 value)
{
    int wkc;
    wkc = ec_SDOwrite (slave, index, subindex, FALSE, sizeof(value), &value, EC_TIMEOUTRXM);
    return wkc;
}


/** Selective PDO mapping
 *
 * @param[in] slave	=  Slave number.
 * @param[out] wkc 	=  Working counter.
 * */
int pdo_map (uint16 slave)
{
    int wkc = 0;
    
    printf ("Motor drive setup\n");

    wkc += SDO_write8  (slave, 0x1C12, 0, 0);
    wkc += SDO_write8  (slave, 0x1C13, 0, 0);

    /* CSP Inputs */
    wkc += SDO_write8  (slave, 0x1A00, 0, 0);
    wkc += SDO_write32 (slave, 0x1A00, 1, 0x60640020); //Actual position
    wkc += SDO_write32 (slave, 0x1A00, 2, 0x606C0020); //Actual velocity
    wkc += SDO_write8  (slave, 0x1A00, 0, 2);

    /* CSP Outputs */
    wkc += SDO_write8  (slave, 0x1600, 0, 0);
    wkc += SDO_write32 (slave, 0x1600, 1, 0x607A0020); //Target position
    wkc += SDO_write32 (slave, 0x1600, 2, 0x60710010); //Target torque
    wkc += SDO_write8  (slave, 0x1600, 0, 2);

    wkc += SDO_write16 (slave, 0x1C12, 1, 0X1600);
    wkc += SDO_write8  (slave, 0x1C12, 0, 1);

    wkc += SDO_write16 (slave, 0x1C13, 1, 0x1A00);
    wkc += SDO_write8  (slave, 0x1C13, 0, 1);

    /* Explicitly set flags that are (probably) invalid in EEPROM */
    ec_slave[slave].SM[2].SMflags = 0x10024l;

    /* Explicitly disable the sync manager that are activated by EEPROM */
    ec_slave[slave].SM[4].StartAddr = 0;
    ec_slave[slave].SM[5].StartAddr = 0;

    return wkc;
}

/** PDO mapping for motor
 *
 * @param[in] slave	=  Slave number.
 * @return -1 if motor setup fails.
 */
int motor_setup (uint16 slave)
{
    ec_slave[slave].PO2SOconfig = pdo_map;

//    if (wkc != 9)
//    {
//	printf ("Motor setup failed!");
//        return -1;
//    }

    return 0; 
}

/** Saturate encoder value within 2*Pi
 *
 * @param[in] encoder	=  Raw value.
 * return saturated value.
 */
double saturation (double encoder)
{
    double val = 2*M_PI; 
    while (fabs(encoder) > val)
    {
	if (encoder > 0) encoder = encoder - 2*M_PI;
	else if(encoder < 0) encoder = encoder + 2*M_PI;
    }
    return encoder;
}

/** Set target position
 * @param[in] slave	=  Slave number
 * @param[in] value	=  Position value
 */
/*
int TargetPosition(uint16 slave, int32 value)
{
    out_motor_t *out_motor = (out_motor_t *)ec_slave[slave].outputs;
    out_motor->tg_pos = value;

    return 0;
}
*/

/** Set target torque
 * @param[in] slave	=  Slave number
 * @param[in] value	=  Torque value
 */
/*
int TargetTorque(uint16 slave, int16 value)
{
    out_motor_t *out_motor = (out_motor_t *)ec_slave[slave].outputs;
    out_motor->tg_tau = value;

    return 0;
}
*/

/** Get actual position
 * @param[in] slave	=  Slave number
 * @param[out] ac_pos	=  Actual position
 */
/*
int GetActualPosition(uint16 slave)
{
    in_motor_t *in_motor = (in_motor_t *)ec_slave[slave].inputs;
    return in_motor->ac_pos;
}
*/

/** Get actual velocity
 * @param[in] slave	=  Slave number
 * @param[out] ac_vel	=  Actual velocity
 */
/*
int GetActualVelocity(uint16 slave)
{
    in_motor_t *in_motor = (in_motor_t *)ec_slave[slave].inputs;
    return in_motor->ac_vel;
}
*/










