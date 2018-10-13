/**
 * \file
 * \All self-defined macros.
 * 
 * Edited by Zikun Yu -- yu.zik@husky.neu.edu
 *
 * Whitney Robotics Lab  10/2/2018 
 */

#ifndef _ECATTYPE_H_
#define _ECATTYPE_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "ethercat.h"

#define Counts_per_radian 52151.8917
#define Units_per_Nm 5000

/* Buffers for SDO transfer */
#define READ_SDO(slave, idx, sub, buf, comment) \
{ \
    buf=0; \
    int __s = sizeof(buf); \
    int __ret = ec_SDOread(slave, idx, sub, FALSE, &__s, &buf, EC_TIMEOUTRXM); \
    printf("Read at 0x%04x:%d => wkc: %d; data: 0x%.*x (%d)\t[%s]\n", idx, sub, __ret, __s, (unsigned int)buf, (unsigned int)buf, comment); \
}

#define WRITE_SDO(slave, idx, sub, buf, value, comment) \
{ \
    int __s = sizeof(buf); \
    buf = value; \
    int __ret = ec_SDOwrite(slave, idx, sub, FALSE, __s, &buf, EC_TIMEOUTRXM); \
    printf("Write at 0x%04x:%d => wkc: %d; data: 0x%.*x (%d)\t[%s]\n", idx, sub, __ret, __s, (unsigned int)buf, (unsigned int)buf, comment); \
}

#define CHECKERROR(slave) \
{ \
    ec_readstate(); \
    printf("EC> \"%s\" %x - %x [%s] \n", (char*)ec_elist2string(), ec_slave[slave].state, ec_slave[slave].ALstatuscode, (char*)ec_ALstatuscode2string(ec_slave[slave].ALstatuscode)); \
}


#endif //_ECATTYPE_H_
