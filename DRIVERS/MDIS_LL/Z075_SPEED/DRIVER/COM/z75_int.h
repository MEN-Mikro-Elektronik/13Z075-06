/***********************  I n c l u d e  -  F i l e  ***********************/
/*!
 *        \file  z75_int.h
 *
 *      \author  rla
 *
 *       \brief  Header file for Z75 driver containing
 *               Z75 specific status codes and
 *               Z75 function prototypes
 *
 *    \switches  _ONE_NAMESPACE_PER_DRIVER_
 *               _LL_DRV_
 */
 /*-------------------------------[ History ]--------------------------------
 *
 * $Log: z75_int.h,v $
 * Revision 1.4  2010/04/22 11:06:23  amorbach
 * R: Porting to MDIS5
 * M: changed according to MDIS Porting Guide 0.8
 *
 * Revision 1.3  2006/06/14 09:53:50  rla
 * added Default values for Descriptors
 *
 * Revision 1.2  2006/06/01 17:09:36  cs
 * changed ADDRSPACE_SIZE to 16 (Chameleon BBIS reports real size)
 *
 * Revision 1.1  2006/02/21 13:54:05  rla
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * Copyright 2010-2019, MEN Mikro Elektronik GmbH
 ****************************************************************************/

 /*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _Z75_INT_H
#define _Z75_INT_H

#ifdef __cplusplus
      extern "C" {
#endif

#define _NO_LL_HANDLE		/* ll_defs.h: don't define LL_HANDLE struct */

#include <MEN/men_typs.h>   /* system dependent definitions   */
#include <MEN/maccess.h>    /* hw access macros and types     */
#include <MEN/dbg.h>        /* debug functions                */
#include <MEN/oss.h>        /* oss functions                  */
#include <MEN/desc.h>       /* descriptor functions           */
#include <MEN/modcom.h>     /* ID PROM functions              */
#include <MEN/mdis_api.h>   /* MDIS global defs               */
#include <MEN/mdis_com.h>   /* MDIS common defs               */
#include <MEN/mdis_err.h>   /* MDIS error codes               */
#include <MEN/ll_defs.h>    /* low-level driver definitions   */

/*-----------------------------------------+
|  DEFINES                                 |
+-----------------------------------------*/
/* general defines */
#define CH_NUMBER			1			/**< number of device channels */
#define USE_IRQ				FALSE		/**< interrupt required  */
#define ADDRSPACE_COUNT		1			/**< nbr of required address spaces */
#define ADDRSPACE_SIZE		16			/**< size of address space */

/* debug defines */
#define DBG_MYLEVEL			llHdl->dbgLevel   /**< debug level */
#define DBH					llHdl->dbgHdl     /**< debug handle */

/** \name 16Z075_SPEED register and mask defines
 *  \anchor reg_defines
 */
/**@{*/
#define Z075_RESULT_ADDR_OFFSET		0x0
		/**< Result register address offset in SPEED device */
#define Z075_CTRL_STS_ADDR_OFFSET	0x00000004
		/**< CTRL_STS register address offset in SPEED device */
#define Z075_RES_MEAS_TIME_MSK		0x000FFFFF	/**< measurement time mask */
#define Z075_RES_PERIOD_MSK			0x7FF00000	/**< Period mask */
#define Z075_RES_NEW_VALUE_MSK		0x80000000	/**< Period mask */
#define Z075_STS_LOW_MSK			0x40000000	/**< too low mask */
#define Z075_STS_HIGH_MSK			0x20000000  /**< too high mask */
#define Z075_STS_EN_MSK				0x80000000	/**< enable mask */
#define Z075_STS_RES_MSK			0x00000007	/**< Resolution mask */
/**@}*/

/** \name 16Z075_SPEED default values
 *  \anchor default defines
 */
/**@{*/
#define Z075_DESC_DEF_SYSCLK		33000000
#define Z075_DESC_DEF_RESOLUTION	0
#define Z075_DESC_DEF_UPLTD			0xFFFFFFFF
#define Z075_DESC_DEV_LOWLTD		0
/**@}*/

/*-----------------------------------------+
|  TYPEDEFS                                |
+-----------------------------------------*/
/** low-level handle */
typedef struct {
	/* general */
    int32           memAlloc;		/**< Size allocated for the handle */
    OSS_HANDLE      *osHdl;         /**< OSS handle */
    OSS_IRQ_HANDLE  *irqHdl;        /**< IRQ handle */
    DESC_HANDLE     *descHdl;       /**< DESC handle */
    MACCESS         ma;             /**< HW access handle */
	MDIS_IDENT_FUNCT_TBL idFuncTbl;	/**< ID function table */
	OSS_ALARM_HANDLE *alarmHdl;		/**< alarm handle */
	/* debug */
    u_int32         dbgLevel;		/**< Debug level */
	DBG_HANDLE      *dbgHdl;        /**< Debug handle */
	/* misc */
    u_int32         irqCount;       /**< Interrupt counter */
    u_int32 		fpgaSysClk;		/**< FPGA core system clock */
    u_int32 		upLimit;		/**< upper limit for auto mode */
    u_int32 		lowLimit;		/**< lower limit for auto mode */
    u_int8			autoMode;		/**< automatic Mode */
    u_int16			res;			/**< measurement resolution */
    u_int16			startFlag;		/**< measurement running flag */
    OSS_SIG_HANDLE	*lowSigIdP;		/**< lower limit signal Id */
    OSS_SIG_HANDLE	*upSigIdP;		/**< upper limit signal Id */
    u_int32 		realMsec;		/**< real Milliseconds of polling */
} LL_HANDLE;

/* include files which need LL_HANDLE */
#include <MEN/ll_entry.h>   /* low-level driver jump table  */
#include <MEN/z75_drv.h>    /* Z75 driver header file */

/*-----------------------------------------+
|  PROTOTYPES                              |
+-----------------------------------------*/
static int32 Z75_Init(DESC_SPEC *descSpec, OSS_HANDLE *osHdl,
					   MACCESS *ma, OSS_SEM_HANDLE *devSemHdl,
					   OSS_IRQ_HANDLE *irqHdl, LL_HANDLE **llHdlP);
static int32 Z75_Exit(LL_HANDLE **llHdlP );
static int32 Z75_Read(LL_HANDLE *llHdl, int32 ch, int32 *value);
static int32 Z75_Write(LL_HANDLE *llHdl, int32 ch, int32 value);
static int32 Z75_SetStat(LL_HANDLE *llHdl,int32 ch, int32 code, INT32_OR_64 value32_or_64);
static int32 Z75_GetStat(LL_HANDLE *llHdl, int32 ch, int32 code, INT32_OR_64 *value32_or_64P);
static int32 Z75_BlockRead(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size,
							int32 *nbrRdBytesP);
static int32 Z75_BlockWrite(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size,
							 int32 *nbrWrBytesP);
static int32 Z75_Irq(LL_HANDLE *llHdl );
static int32 Z75_Info(int32 infoType, ... );

static char* Ident( void );
static int32 Cleanup(LL_HANDLE *llHdl, int32 retCode);
static int32 GetFreq( LL_HANDLE *llHdl, u_int32 *freqP );
static void automatedTsk( void *hdl );
static int checkRange( u_int32 clk, u_int32 per );

#ifdef __cplusplus
      }
#endif

#endif /* _Z75_INT_H */

