/***********************  I n c l u d e  -  F i l e  ***********************/
/*!
 *        \file  z75_drv.h
 *
 *      \author  rla
 *        $Date: 2010/04/22 11:08:00 $
 *    $Revision: 3.2 $
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
 * $Log: z75_drv.h,v $
 * Revision 3.2  2010/04/22 11:08:00  amorbach
 * R: driver ported to MDIS5, new MDIS_API and men_typs
 * M: for backward compatibility to MDIS4 optionally define new types
 *
 * Revision 3.1  2006/02/21 13:54:14  rla
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2010 by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/

#ifndef _Z75_DRV_H
#define _Z75_DRV_H

#ifdef __cplusplus
      extern "C" {
#endif


/*-----------------------------------------+
|  TYPEDEFS                                |
+-----------------------------------------*/
/* none */

/*-----------------------------------------+
|  DEFINES                                 |
+-----------------------------------------*/
/** \name Z75 specific Getstat/Setstat standard codes
 *  \anchor getstat_setstat_codes
 */
/**@{*/
#define Z075_RESOLUTION      M_DEV_OF+0x00   /**<  S/G: Access CTRL to set/get
											  *    resolution */
#define Z075_FREQUENCY     	 M_DEV_OF+0x01   /**<  G: Get frequency */
#define Z075_SET_SIG_UPPER   M_DEV_OF+0x02   /**<  S: install signal for
											  *    upper limit violation in
											  *    automated mode */
#define Z075_SET_SIG_LOWER   M_DEV_OF+0x03   /**<  S: install signal for
											  *    lower limit violation in
											  *    automated mode */
#define Z075_CLR_SIG_UPPER   M_DEV_OF+0x04   /**<  S: uninstall signal for
											  *    upper limit violation in
											  *    automated mode */
#define Z075_CLR_SIG_LOWER   M_DEV_OF+0x05   /**<  S: uninstall signal for
											  *    lower limit violation in
											  *    automated mode */
#define Z075_A_UPPER_LTD     M_DEV_OF+0x06   /**<  S/G: Upper Limit for
											  *    automated mode */
#define Z075_A_LOWER_LTD     M_DEV_OF+0x07   /**<  S/G: Lower limit for
										      *	 automated mode */
#define Z075_START_MEAS      M_DEV_OF+0x08   /**<  S/G: Start/Stop Measurement*/
#define Z075_AUTO_MODE	     M_DEV_OF+0x09   /**<  S/G: En/Disable automated
											  *    mode */
/**@}*/


/*-----------------------------------------+
|  PROTOTYPES                              |
+-----------------------------------------*/
#ifdef _LL_DRV_
#	ifdef _ONE_NAMESPACE_PER_DRIVER_
#		define Z75_GetEntry LL_GetEntry
#	else
#		ifdef Z75_SW
#			define Z75_GetEntry Z75_SW_GetEntry
#		endif /* Z75_SW */
		extern void Z75_GetEntry(LL_ENTRY* drvP);
#	endif
#endif /* _LL_DRV_ */

/*-----------------------------------------+
|  BACKWARD COMPATIBILITY TO MDIS4         |
+-----------------------------------------*/
#ifndef U_INT32_OR_64
 /* we have an MDIS4 men_types.h and mdis_api.h included */
 /* only 32bit compatibility needed!                     */
 #define INT32_OR_64  int32
 #define U_INT32_OR_64 u_int32
 typedef INT32_OR_64  MDIS_PATH;
#endif /* U_INT32_OR_64 */

#ifdef __cplusplus
      }
#endif

#endif /* _Z75_DRV_H */

