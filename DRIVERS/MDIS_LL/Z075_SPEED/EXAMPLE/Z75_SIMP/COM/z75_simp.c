/****************************************************************************
 ************                                                    ************
 ************                   Z75_SIMP                         ************
 ************                                                    ************
 ****************************************************************************/
/*!
 *         \file z75_simp.c
 *       \author rla
 *        $Date: 2010/04/22 11:06:51 $
 *    $Revision: 1.3 $
 *
 *       \brief  Simple example program for the Z75 driver
 *
 *               Reads the frequency or monitors frequency
 *
 *     Required: libraries: mdis_api
 *     \switches (none)
 */
 /*-------------------------------[ History ]--------------------------------
 *
 * $Log: z75_simp.c,v $
 * Revision 1.3  2010/04/22 11:06:51  amorbach
 * R: Porting to MDIS5
 * M: changed according to MDIS Porting Guide 0.8
 *
 * Revision 1.2  2006/10/18 13:34:39  cs
 * added command line option to set resolution of counters
 *
 * Revision 1.1  2006/02/21 13:54:12  rla
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2010 by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/

static const char RCSid[]="$Id: z75_simp.c,v 1.3 2010/04/22 11:06:51 amorbach Exp $";

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <MEN/men_typs.h>
#include <MEN/usr_oss.h>
#include <MEN/usr_err.h>
#include <MEN/usr_utl.h>
#include <MEN/mdis_api.h>
#include <MEN/z75_drv.h>

/*--------------------------------------+
|   DEFINES                             |
+--------------------------------------*/
/* none */

/*--------------------------------------+
|   TYPDEFS                             |
+--------------------------------------*/
/* none */

/*--------------------------------------+
|   EXTERNALS                           |
+--------------------------------------*/
/* none */

/*--------------------------------------+
|   GLOBALS                             |
+--------------------------------------*/

/*--------------------------------------+
|   PROTOTYPES                          |
+--------------------------------------*/
static void PrintError(char *info);
static void __MAPILIB SigHandler( u_int32 sigCode );

/********************************* main ************************************/
/** Program main function
 *
 *  \param argc       \IN  argument counter
 *  \param argv       \IN  argument vector
 *
 *  \return	          success (0) or error (1)
 */
int main(int argc, char *argv[])
{
	MDIS_PATH	path;
	char	*device;
	int32 freq = 0;
	u_int32 value =0;
	char ret = 0;
	int retval = 0;
	u_int32  cnt = 0;
	int32 alarm = 0;
	int32 resolution = 7;

	if (argc < 2 || strcmp(argv[1],"-?")==0) {
		printf("Syntax: z75_simp <device> [<resolution> [<alarm>]]\n");
		printf("Function: Z75 example for Frequency counter\n");
		printf("Options:\n");
		printf("      device:       device name\n");
		printf("  resolution:       resolution of counter (0-7)\n");
		printf("       alarm:		0 = alarm off, frequency monitoring\n");
		printf("				    1 = activate alarm monitoring\n");
		printf("\n");
		return(1);
	}

	device     =       argv[1];
	if( argc >= 3 )
		resolution = atoi( argv[2] );
	if( argc >= 4 )
		alarm      = atoi( argv[3] );

	/*--------------------+
    |  open path          |
    +--------------------*/
	if ((path = M_open(device)) < 0) {
		PrintError("open");
		return(1);
	}

	/*--------------------+
    |  config             |
    +--------------------*/
	if( alarm ){

		printf("Initialize Automated mode\n");

		UOS_SigInit( SigHandler );
		UOS_SigInstall( UOS_SIG_USR1 );
		UOS_SigInstall( UOS_SIG_USR2 );

		/* Install signal */
		if ((M_setstat(path, Z075_SET_SIG_LOWER, UOS_SIG_USR1)) < 0) {
			PrintError("setstat Z075_SET_SIG_LOWER");
			goto abort;
		}

		if ((M_setstat(path, Z075_SET_SIG_UPPER, UOS_SIG_USR2)) < 0) {
			PrintError("setstat Z075_SET_SIG_UPPER");
			goto abort;
		}

		/* Set monitored frequency range upper Limit f = 100 kHz */
		if ((M_setstat(path, Z075_A_UPPER_LTD, 100000 )) < 0) {
			PrintError("setstat Z075_A_UPPER_LTD");
			goto abort;
		}

		/* lower limit f = 1 kHz */
		if ((M_setstat(path, Z075_A_LOWER_LTD, 1000 )) < 0) {
			PrintError("setstat Z075_A_LOWER_LTD");
			goto abort;
		}

		/* start automated mode, time = 100 ms */
		if ((M_setstat(path, Z075_AUTO_MODE, 100 )) < 0) {
			PrintError("setstat Z075_AUTO_MODE");
			goto abort;
		}
	}

	/* set resolution */
	if ((M_setstat(path, Z075_RESOLUTION, resolution )) < 0) {
		PrintError("setstat Z075_RESOLUTION (out of range?)");
		goto abort;
	} else {
		printf("resolution set to %d\n", (int)resolution);
	}


	/* start measurement */
	value = 1;
	if ((M_setstat(path, Z075_START_MEAS, value)) < 0) {
		PrintError("setstat Z075_START_MEAS");
		goto abort;
	}

	UOS_Delay( 10 );

	if( !alarm ){
		printf("Measuring Frequency\n" );
		printf("=================================\n" );
		printf("Press RETURN to stop measurement\n\n");
		printf("                " );

		/*--------------------+
	    |  MAIN-Loop          |
	    +--------------------*/
		for( ;; ){

			UOS_Delay( 40 );

			if( UOS_KeyPressed() == '\n' ){
				goto abort;
			}

			if( M_getstat( path, Z075_FREQUENCY, &freq ) < 0 ){
				/* restart measurement */
				value = 0;
				if ((M_setstat(path, Z075_START_MEAS, value)) < 0) {
					PrintError("setstat Z075_START_MEAS");
					goto abort;
				}

				UOS_Delay( 10 );

				value = 1;
				if ((M_setstat(path, Z075_START_MEAS, value)) < 0) {
					PrintError("setstat Z075_START_MEAS");
					goto abort;
				}

			}

			switch( cnt ){
				case 0:
					ret='|';
					cnt++;
					break;
				case 1:
					ret='/';
					cnt++;
					break;
				case 2:
					ret = '-';
					cnt++;
					break;
				case 3:
					ret = '\\';
					cnt = 0;
					break;
			}

			printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b%c %10d Hz",ret,(int)freq);
			fflush(stdout);
		}
	}
	else{
		for( ;; ){

			if( UOS_KeyPressed() == '\n' ){
				goto abort;
			}

			retval = 0;
			retval = UOS_SigWait( 20, (u_int32*)UOS_SIG_USR1 );

			switch( retval ){
				case 0:
					break;
				case ERR_UOS_TIMEOUT:
					break;
				default:
					PrintError("UOS_SigWait for sigLow");
					goto abort;
			}

			retval = 0;
			retval = UOS_SigWait( 20, (u_int32*)UOS_SIG_USR1 );

			switch( retval ){
				case 0:
					break;
				case ERR_UOS_TIMEOUT:
					break;
				default:
					PrintError("UOS_SigWait for sigUp");
					goto abort;
			}

			switch( cnt ){
				case 0:
					ret='|';
					cnt++;
					break;
				case 1:
					ret='/';
					cnt++;
					break;
				case 2:
					ret = '-';
					cnt++;
					break;
				case 3:
					ret = '\\';
					cnt = 0;
					break;
			}

			printf("\b%c",ret);
			fflush(stdout);
		}
	}

	/*--------------------+
    |  cleanup            |
    +--------------------*/
abort:
	if( alarm ){
		/* stop automated mode */
		if ( (M_setstat(path, Z075_AUTO_MODE, 0  )) < 0) {
			PrintError("setstat Z075_AUTO_MODE");
		}

		M_setstat( path, Z075_CLR_SIG_LOWER, 0 );
		M_setstat( path, Z075_CLR_SIG_UPPER, 0 );
		UOS_SigRemove( UOS_SIG_USR1 );
		UOS_SigRemove( UOS_SIG_USR2 );

		UOS_SigExit();
	}

	if (M_close(path) < 0)
		PrintError("close");

	return(0);
}




/********************************* SigHanlder ******************************/
/** Signal Handler
 *
 *  \param sigCode       \IN  signal Code of received signal
*/
static void __MAPILIB SigHandler( u_int32 sigCode )
{
	switch( sigCode ){
		case UOS_SIG_USR2:
			printf(" --> Upper Range Limit violation\n");
			break;

		case UOS_SIG_USR1:
			printf(" --> Lower Range Limit violation\n");
			break;

		default:
			printf(" --> Unexpected Signal\n");
	}
}

/********************************* PrintError ******************************/
/** Print MDIS error message
 *
 *  \param info       \IN  info string
*/
static void PrintError(char *info)
{
	printf("*** can't %s: %s\n", info, M_errstring(UOS_ErrnoGet()));
}


