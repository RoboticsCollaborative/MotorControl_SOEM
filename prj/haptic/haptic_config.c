/**
 * \file
 * \Configuration for running EtherCAT threads.
 *
 * Edited by Zikun Yu -- yu.zik@husky.neu.edu
 *
 * Whitney Robotics Lab  9/10/2018
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "ethercat.h"
#include "config_map.h"
#include "ecattype.h"
#include "haptic_config.h"
#include "controller.h"
#include "config_SDOparam.h"

#define EC_TIMEOUTMON 500

char IOmap[4096];
int expectedWKC;
boolean needlf;
volatile int wkc;
boolean inOP;
uint8 currentgroup = 0;
double ActualPosition = 0, ActualVelocity = 0, InputTorque = 0, ReferencePosition = 0;

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

void haptic_config(char *ifname)
{
    int i, oloop, iloop, chk; //int j;
    needlf = FALSE;
    inOP = FALSE;
    
    uint16 BUF16; 
    // uint8 BUF8;  //uint32 BUF32;
    uint16 motor1; //uint16 motor2;
	    
    printf("Starting to configure robot\n");

    /* initialise SOEM, bind socket to ifname */
    if (ec_init(ifname))
    {
      	printf("ec_init on %s succeeded.\n",ifname);
      	/* find and auto-config slaves */


       	if ( ec_config_init(FALSE) > 0 )
      	{
            printf("%d slaves found and configured.\n",ec_slavecount);

	    /* CompleteAccess disabled for Bel driver */
	    ec_slave[1].CoEdetails ^= ECT_COEDET_SDOCA;

	    ec_statecheck(0, EC_STATE_PRE_OP, EC_TIMEOUTSTATE);

	    int slaveIdx;

	    /* user Def */
	    /* locate slave (BEL motor drive) */
	    if (ec_slavecount > 0)
	    {
//		ec_slavet * slave = &ec_slave[slaveIdx];
	    	for (slaveIdx = 1; slaveIdx <= ec_slavecount; slaveIdx ++)
	    	{
		    if ((ec_slave[slaveIdx].eep_man == 0x000000ab) && (ec_slave[slaveIdx].eep_id == 0x00001110))
		    {
			motor1 = slaveIdx;
			/* Set PDO mapping */
		    	printf("Found %s at position %d\n", ec_slave[motor1].name, motor1);
			motor_setup(motor1);
		    }
	    	}
	    }

	    /* If CA disabled => auto-mapping work */
            ec_config_map(&IOmap);

	    /* Let DC off for the time being */
//            ec_configdc();

            printf("Slaves mapped, state to SAFE_OP.\n");
            /* wait for all slaves to reach SAFE_OP state */
            ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);

            oloop = ec_slave[0].Obytes;
            if ((oloop == 0) && (ec_slave[0].Obits > 0)) oloop = 1;
            if (oloop > 8) oloop = 8;
            iloop = ec_slave[0].Ibytes;
            if ((iloop == 0) && (ec_slave[0].Ibits > 0)) iloop = 1;
            if (iloop > 8) iloop = 8;

            printf("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

            printf("Request operational state for all slaves\n");
            expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
            printf("Calculated workcounter %d\n", expectedWKC);

	    /* Going operational */
            ec_slave[0].state = EC_STATE_OPERATIONAL;
            /* send one valid process data to make outputs in slaves happy*/
            ec_send_processdata();
            ec_receive_processdata(EC_TIMEOUTRET);

            /* request OP state for all slaves */
            ec_writestate(0);
            chk = 40;
            /* wait for all slaves to reach OP state */
            do
            {
                ec_send_processdata();
            	ec_receive_processdata(EC_TIMEOUTRET);
            	ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
            }
            while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));

            if (ec_slave[0].state == EC_STATE_OPERATIONAL )
            {
            	printf("Operational state reached for all slaves.\n");
            	inOP = TRUE;

		/* Output torque and velocity values to a file */
/*
		FILE *fp = NULL;
		char *filename = "/home/protectli/velocity.dat";
		remove(filename);
		fp = fopen(filename, "w");
*/
	  	/**
	  	* Initialize static parameters by setting SDO
	 	*/
	     	motor_init(motor1);
		out_motor_t *out_motor = (out_motor_t *)ec_slave[motor1].outputs;
		in_motor_t *in_motor = (in_motor_t *)ec_slave[motor1].inputs;
			

		/* Initialize origin */
		ec_send_processdata();
               	wkc = ec_receive_processdata(EC_TIMEOUTRET);
		ActualPosition = (in_motor->ac_pos)/Counts_per_radian;
		ActualPosition = saturation(ActualPosition);
		ec_send_processdata();
               	wkc = ec_receive_processdata(EC_TIMEOUTRET);
		ReferencePosition = ActualPosition;

		/* Activate motor drive */
		WRITE_SDO(motor1, 0x6040, 0, BUF16, 15, "*Control word: motor1*");

                /* cyclic loop */
//            	for(i = 1; i <= 20000; i++)
		while(inOP)
            	{
		    i += 1;
		    ec_send_processdata();
               	    wkc = ec_receive_processdata(EC_TIMEOUTRET);
			
		    /* Add data to the file */
//		    fprintf(fp, "%d %lf %lf \n", i, InputTorque, ActualVelocity);

                    if(wkc >= expectedWKC)
                    {
                    	printf("ac_pos: %lf, ac_vel: %lf, cmd_tau: %lf, Processdata cycle %4d, WKC %d , O:\r", ActualPosition, ActualVelocity, InputTorque, i, wkc);

		 	/* Set target position */			
			ActualPosition = (in_motor->ac_pos)/Counts_per_radian;
			ActualPosition = saturation(ActualPosition);
			ActualVelocity = (in_motor->ac_vel)/Counts_per_radian/10;
			InputTorque = PDcontroller(ReferencePosition, ActualPosition, ActualVelocity);
			/* For damping meassure */
//			InputTorque = 0.35;
			out_motor->tg_tau = (int16)(InputTorque * Units_per_Nm);

/*
                        for(j = 0 ; j < oloop; j++)
                        {
                            printf(" %2.2x", *(ec_slave[0].outputs + j));
                        }

                        printf(" I:");
                        for(j = 0 ; j < iloop; j++)
                        {
                            printf(" %2.2x", *(ec_slave[0].inputs + j));
                        }
                        printf(" T:%"PRId64"\r",ec_DCtime);
*/
//			fflush(stdout);
                        needlf = TRUE;
                    }
//                    osal_usleep(50);

                }
                inOP = FALSE;
//		fclose(fp);
            }
            else
            {
                printf("Not all slaves reached operational state.\n");
                ec_readstate();
                for(i = 1; i<=ec_slavecount ; i++)
                {
                    if(ec_slave[i].state != EC_STATE_OPERATIONAL)
                    {
                        printf("Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
                            i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
                    }
                }
            }
            printf("\nRequest init state for all slaves\n");
            ec_slave[0].state = EC_STATE_INIT;
            /* request INIT state for all slaves */
            ec_writestate(0);
        }
        else
        {
            printf("No slaves found!\n");
        }
        printf("End haptic_run, close socket\n");
        /* stop SOEM, close socket */
        ec_close();
    }
    else
    {
        printf("No socket connection on %s\nExcecute as root\n",ifname);
    }
}

OSAL_THREAD_FUNC ecatcheck( void *ptr )
{
    int slave;
    (void)ptr;                  /* Not used */

    while(1)
    {
        if( inOP && ((wkc < expectedWKC) || ec_group[currentgroup].docheckstate))
        {
            if (needlf)
            {
            	needlf = FALSE;
            	printf("\n");
            }
            /* one ore more slaves are not responding */
            ec_group[currentgroup].docheckstate = FALSE;
            ec_readstate();
            for (slave = 1; slave <= ec_slavecount; slave++)
            {
            	if ((ec_slave[slave].group == currentgroup) && (ec_slave[slave].state != EC_STATE_OPERATIONAL))
            	{
                    ec_group[currentgroup].docheckstate = TRUE;
                    if (ec_slave[slave].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
                    {
                    	printf("ERROR : slave %d is in SAFE_OP + ERROR, attempting ack.\n", slave);
                    	ec_slave[slave].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
                     	ec_writestate(slave);
                    }
                    else if(ec_slave[slave].state == EC_STATE_SAFE_OP)
                    {
                    	printf("WARNING : slave %d is in SAFE_OP, change to OPERATIONAL.\n", slave);
                    	ec_slave[slave].state = EC_STATE_OPERATIONAL;
                    	ec_writestate(slave);
                    }
                    else if(ec_slave[slave].state > EC_STATE_NONE)
                    {
                    	if (ec_reconfig_slave(slave, EC_TIMEOUTMON))
                    	{
                            ec_slave[slave].islost = FALSE;
                            printf("MESSAGE : slave %d reconfigured\n",slave);
                    	}
                    }
                    else if(!ec_slave[slave].islost)
                    {
                    	/* re-check state */
                    	ec_statecheck(slave, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
                    	if (ec_slave[slave].state == EC_STATE_NONE)
                    	{
                            ec_slave[slave].islost = TRUE;
                            printf("ERROR : slave %d lost\n",slave);
                     	}
                    }
                }
                if (ec_slave[slave].islost)
                {
                    if(ec_slave[slave].state == EC_STATE_NONE)
                    {
                    	if (ec_recover_slave(slave, EC_TIMEOUTMON))
                    	{
                            ec_slave[slave].islost = FALSE;
                            printf("MESSAGE : slave %d recovered\n",slave);
                     	}
                    }
                    else
                    {
                    	ec_slave[slave].islost = FALSE;
                    	printf("MESSAGE : slave %d found\n",slave);
                    }
                }
            }
            if(!ec_group[currentgroup].docheckstate)
            	printf("OK : all slaves resumed OPERATIONAL.\n");
        }
        osal_usleep(10000);
    }
}

OSAL_THREAD_FUNC switch_off( void *ptr )
{
    int c; uint16 BUF16;
    (void)ptr;                  /* Not used */

    do
    {
	c = getchar();
	putchar(c);
    }while (c != 'q');

    /* Activate motor drive */
    WRITE_SDO(1, 0x6040, 0, BUF16, 0, "*Control word: motor1*");
    inOP = FALSE;
}
