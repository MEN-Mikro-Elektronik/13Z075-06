/*********************  P r o g r a m  -  M o d u l e ***********************/
/*!
 *        \file  z75_drv.c
 *
 *      \author  rla
 *        $Date: 2010/04/22 11:06:07 $
 *    $Revision: 1.4 $
 *
 *      \brief   Low-level driver for Z75 module
 *
 *     Required: OSS, DESC, DBG, ID libraries
 *
 *     \switches _ONE_NAMESPACE_PER_DRIVER_
 */
 /*-------------------------------[ History ]--------------------------------
 *
 * $Log: z75_drv.c,v $
 * Revision 1.4  2010/04/22 11:06:07  amorbach
 * R: Porting to MDIS5
 * M: changed according to MDIS Porting Guide 0.8
 *
 * Revision 1.3  2006/06/14 09:53:48  rla
 * added Default values for Descriptors
 *
 * Revision 1.2  2006/02/21 14:21:26  rla
 * changed IRQ return value
 *
 * Revision 1.1  2006/02/21 13:54:03  rla
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2010 by MEN Mikro Elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/

#include "z75_int.h"


/****************************** Z75_GetEntry ********************************/
/** Initialize driver's jump table
 *
 *  \param drvP     \OUT Pointer to the initialized jump table structure
 */
#ifdef _ONE_NAMESPACE_PER_DRIVER_
    extern void LL_GetEntry( LL_ENTRY* drvP )
#else
    extern void Z75_GetEntry( LL_ENTRY* drvP )
#endif /* _ONE_NAMESPACE_PER_DRIVER_ */
{
    drvP->init        = Z75_Init;
    drvP->exit        = Z75_Exit;
    drvP->read        = Z75_Read;
    drvP->write       = Z75_Write;
    drvP->blockRead   = Z75_BlockRead;
    drvP->blockWrite  = Z75_BlockWrite;
    drvP->setStat     = Z75_SetStat;
    drvP->getStat     = Z75_GetStat;
    drvP->irq         = Z75_Irq;
    drvP->info        = Z75_Info;
}

/******************************** Z75_Init **********************************/
/** Allocate and return low-level handle, initialize hardware
 *
 * The function initializes all channels with the definitions made
 * in the descriptor. The interrupt is disabled.
 *
 * The function decodes \ref descriptor_entries "these descriptor entries"
 * in addition to the general descriptor keys.
 *
 *  \param descP      \IN  Pointer to descriptor data
 *  \param osHdl      \IN  OSS handle
 *  \param ma         \IN  HW access handle
 *  \param devSemHdl  \IN  Device semaphore handle
 *  \param irqHdl     \IN  IRQ handle
 *  \param llHdlP     \OUT Pointer to low-level driver handle
 *
 *  \return           \c 0 On success or error code
 */
static int32 Z75_Init(
    DESC_SPEC       *descP,
    OSS_HANDLE      *osHdl,
    MACCESS         *ma,
    OSS_SEM_HANDLE  *devSemHdl,
    OSS_IRQ_HANDLE  *irqHdl,
    LL_HANDLE       **llHdlP
)
{
    LL_HANDLE *llHdl = NULL;
    u_int32 gotsize;
    int32 error;
    u_int32 value;

    /*------------------------------+
    |  prepare the handle           |
    +------------------------------*/
	*llHdlP = NULL;		/* set low-level driver handle to NULL */

	/* alloc */
    if ((llHdl = (LL_HANDLE*)OSS_MemGet(
    				osHdl, sizeof(LL_HANDLE), &gotsize)) == NULL)
       return(ERR_OSS_MEM_ALLOC);

	/* clear */
    OSS_MemFill(osHdl, gotsize, (char*)llHdl, 0x00);

	/* init */
    llHdl->memAlloc   = gotsize;
    llHdl->osHdl      = osHdl;
    llHdl->irqHdl     = irqHdl;
    llHdl->ma		  = *ma;

    /*------------------------------+
    |  init id function table       |
    +------------------------------*/
	/* driver's ident function */
	llHdl->idFuncTbl.idCall[0].identCall = Ident;
	/* library's ident functions */
	llHdl->idFuncTbl.idCall[1].identCall = DESC_Ident;
	llHdl->idFuncTbl.idCall[2].identCall = OSS_Ident;
	/* terminator */
	llHdl->idFuncTbl.idCall[3].identCall = NULL;

    /*------------------------------+
    |  prepare debugging            |
    +------------------------------*/
	DBG_MYLEVEL = OSS_DBG_DEFAULT;	/* set OS specific debug level */
	DBGINIT((NULL,&DBH));

    /*------------------------------+
    |  scan descriptor              |
    +------------------------------*/
	/* prepare access */
    if ((error = DESC_Init(descP, osHdl, &llHdl->descHdl)))
		return( Cleanup(llHdl,error) );

    /* DEBUG_LEVEL_DESC */
    if ((error = DESC_GetUInt32(llHdl->descHdl, OSS_DBG_DEFAULT,
								&value, "DEBUG_LEVEL_DESC")) &&
		error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );

	DESC_DbgLevelSet(llHdl->descHdl, value);	/* set level */

    /* DEBUG_LEVEL */
    if ((error = DESC_GetUInt32(llHdl->descHdl, OSS_DBG_DEFAULT,
								&llHdl->dbgLevel, "DEBUG_LEVEL")) &&
		error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );

    DBGWRT_1((DBH, "LL - Z75_Init\n"));

    /*------------------------------------+
    |  init hardware and read descriptor  |
    +------------------------------------*/

	/**************************
	 * FPGA System core clock
	 */
	value = 0;

	if ((error = DESC_GetUInt32(llHdl->descHdl, Z075_DESC_DEF_SYSCLK,
							&value, "Z075_SYSCLK")) &&
		error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );

	llHdl->fpgaSysClk = value;


	/**************************
	 * Resolution
	 */
	value = 0;

	if ((error = DESC_GetUInt32(llHdl->descHdl, Z075_DESC_DEF_RESOLUTION,
							&value, "Z075_RESOLUTION")) &&
		error != ERR_DESC_KEY_NOTFOUND )
		return( Cleanup(llHdl,error) );

	if( value < 8 ){
		llHdl->res = (u_int16)value;
	}
	else{
		error = ERR_DESC_CORRUPTED;
		return( Cleanup(llHdl,error) );
	}

	/* write initial value to SPEED CTRL_STS register */
	MWRITE_D32( llHdl->ma, Z075_CTRL_STS_ADDR_OFFSET, value );

	/**************************
	 * Upper limit
	 */
	value = 0;

	if ((error = DESC_GetUInt32(llHdl->descHdl, Z075_DESC_DEF_UPLTD,
							&value, "Z075_A_UPPER_LIMIT")) &&
		error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );

	llHdl->upLimit = value;

	/**************************
	 * Lower limit
	 */
	value = 0;

	if ((error = DESC_GetUInt32(llHdl->descHdl, Z075_DESC_DEV_LOWLTD,
							&value, "Z075_A_LOWER_LIMIT")) &&
		error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );

	llHdl->upLimit = value;

	/**************************
	 * Check limits
	 */
	if( llHdl->lowLimit >= llHdl->upLimit ){
		llHdl->lowLimit = llHdl->upLimit - 1;
	}

	llHdl->startFlag = 0;
	llHdl->autoMode = 0;

	*llHdlP = llHdl;	/* set low-level driver handle */

	return(ERR_SUCCESS);
}

/****************************** Z75_Exit ************************************/
/** De-initialize hardware and clean up memory
 *
 *  The function deinitializes all channels by setting them to ???.
 *  The interrupt is disabled.
 *
 *  \param llHdlP      \IN  Pointer to low-level driver handle
 *
 *  \return           \c 0 On success or error code
 */
static int32 Z75_Exit(
   LL_HANDLE    **llHdlP
)
{
    LL_HANDLE *llHdl = *llHdlP;
	int32 error = 0;

    DBGWRT_1((DBH, "LL - Z75_Exit\n"));

    /*------------------------------+
    |  de-init hardware             |
    +------------------------------*/
    /* stop measurement */
	MWRITE_D32( llHdl->ma, Z075_CTRL_STS_ADDR_OFFSET, 0 );

    /*------------------------------+
    |  clean up memory               |
    +------------------------------*/
	*llHdlP = NULL;		/* set low-level driver handle to NULL */
	error = Cleanup(llHdl,error);

	return(error);
}

/****************************** Z75_Read ************************************/
/** Read a value from the device
 *
 *  The function reads the ??? state of the current channel.
 *
 *  If the channel's direction is not configured as input the function
 *  returns an ERR_LL_ILL_DIR error.
 *
 *  \param llHdl      \IN  Low-level handle
 *  \param ch         \IN  Current channel
 *  \param valueP     \OUT Read value
 *
 *  \return           \c 0 On success or error code
 */
static int32 Z75_Read(
    LL_HANDLE *llHdl,
    int32 ch,
    int32 *valueP
)
{
	return(ERR_LL_ILL_FUNC);
}

/****************************** Z75_Write ***********************************/
/** Description:  Write a value to the device
 *
 *  The function writes a value to the current channel.
 *
 *  If the channel's direction is not configured as output the function
 *  returns an ERR_LL_ILL_DIR error.
 *
 *  \param llHdl      \IN  Low-level handle
 *  \param ch         \IN  Current channel
 *  \param value      \IN  Read value
 *
 *  \return           \c 0 on success or error code
 */
static int32 Z75_Write(
    LL_HANDLE *llHdl,
    int32 ch,
    int32 value
)
{
	return(ERR_LL_ILL_FUNC);
}

/****************************** Z75_SetStat *********************************/
/** Set the driver status
 *
 *  The driver supports \ref getstat_setstat_codes "these status codes"
 *  in addition to the standard codes (see mdis_api.h).
 *
 *  \param llHdl  	        \IN  low-level handle
 *  \param code             \IN  \ref getstat_setstat_codes "status code"
 *  \param ch               \IN  current channel
 *  \param value32_or_64    \IN  data or
 *                               pointer to block data structure (M_SG_BLOCK) for
 *                               block status codes
 *  \return           \c 0 On success or error code
 */
static int32 Z75_SetStat(
    LL_HANDLE *llHdl,
    int32  code,
    int32  ch,
    INT32_OR_64 value32_or_64
)
{
    int32 value = (int32)value32_or_64;	    /* 32bit value */
    /* INT32_OR_64 valueP = value32_or_64;    stores 32/64bit pointer */
	int32 error = ERR_SUCCESS;
	DBGCMD( static const char functionName[] = "Z75 SetStat"; )

    DBGWRT_1((DBH, "LL - Z75_SetStat: ch=%d code=0x%04x value=0x%x\n",
			  ch,code,value));

    switch(code) {
        /*--------------------------+
        |  debug level              |
        +--------------------------*/
        case M_LL_DEBUG_LEVEL:
            llHdl->dbgLevel = value;
            break;
        /*--------------------------+
        |  enable interrupts        |
        +--------------------------*/
        case M_MK_IRQ_ENABLE:
			error = ERR_LL_UNK_CODE;	/* say: not supported */
            break;
        /*--------------------------+
        |  set irq counter          |
        +--------------------------*/
        case M_LL_IRQ_COUNT:
            llHdl->irqCount = value;
            break;
        /*--------------------------+
        |  set resolution           |
        +--------------------------*/
        case Z075_RESOLUTION:
        	value &= Z075_STS_RES_MSK;

        	if( value < 8 ){
        		MWRITE_D32( llHdl->ma, Z075_CTRL_STS_ADDR_OFFSET, value );
        		llHdl->res = (u_int16)value;
        	}
        	else{
        		error = ERR_LL_ILL_PARAM;
        	}

            break;

        /*--------------------------+
        |  install upper signal     |
        +--------------------------*/
        case Z075_SET_SIG_UPPER:
        	if( llHdl->upSigIdP ){
        		error = ERR_OSS_SIG_SET;
        		break;
        	}

        	error = OSS_SigCreate( llHdl->osHdl, value, &llHdl->upSigIdP );
        	break;
        /*--------------------------+
        |  install lower signal     |
        +--------------------------*/
        case Z075_SET_SIG_LOWER:
        	if( llHdl->lowSigIdP ){
        		error = ERR_OSS_SIG_SET;
        		break;
        	}

        	error = OSS_SigCreate( llHdl->osHdl, value, &llHdl->lowSigIdP );
        	break;

        /*--------------------------+
        |  unregister upper signal  |
        +--------------------------*/
        case Z075_CLR_SIG_UPPER:

            /* signal already installed ? */
            if( llHdl->upSigIdP == NULL ) {
                error = ERR_OSS_SIG_CLR;
                break;
            }

            error = OSS_SigRemove( llHdl->osHdl, &llHdl->upSigIdP );
            break;

        /*--------------------------+
        |  unregister lower signal  |
        +--------------------------*/
        case Z075_CLR_SIG_LOWER:

            /* signal already installed ? */
            if( llHdl->lowSigIdP == NULL ) {
                error = ERR_OSS_SIG_CLR;
                break;
            }

            error = OSS_SigRemove( llHdl->osHdl, &llHdl->lowSigIdP );
            break;


        /*--------------------------+
        |  set upper limit          |
        +--------------------------*/
        case Z075_A_UPPER_LTD:
        	llHdl->upLimit = value;
        	break;

        /*--------------------------+
        |  set lower limit          |
        +--------------------------*/
        case Z075_A_LOWER_LTD:
        	llHdl->lowLimit = value;
        	break;

        /*--------------------------+
        |  start/stop measurement   |
        +--------------------------*/
        case Z075_START_MEAS:
        	if( value ){
        		MWRITE_D32( llHdl->ma, Z075_CTRL_STS_ADDR_OFFSET,
        			( Z075_STS_EN_MSK | llHdl->res ) );
        		llHdl->startFlag = 1;
        	}
        	else{
        		MWRITE_D32( llHdl->ma, Z075_CTRL_STS_ADDR_OFFSET, llHdl->res);
        		llHdl->startFlag = 0;
        	}
        	break;

        /*--------------------------+
        |  set lower limit          |
        +--------------------------*/
        case Z075_AUTO_MODE:
        	llHdl->autoMode = (u_int8)value;

        	if( value ){	/* enable alarm */
				if( (error = OSS_AlarmCreate( llHdl->osHdl,
											  automatedTsk,
											  (void *)llHdl,
											  &llHdl->alarmHdl)) )
				{
					DBGWRT_ERR( (DBH, "*** %s: can't create alarm:" \
						" err=%d!!!\n",	functionName, error) );
					goto ERR_EXIT;
				}

				if( (error = OSS_AlarmSet( llHdl->osHdl,
										   llHdl->alarmHdl,
		    							   (u_int32)value,
		    							   1,
		    							   &llHdl->realMsec)) )
				{
					DBGWRT_ERR( (DBH, "*** %s: can't set alarm to" \
						" cyclic all %d ms: err=%d!!!\n",
						functionName, (int)value, error) );

					goto ERR_EXIT;
				}
        	}
        	else{		/* disable alarm */
				OSS_AlarmClear( llHdl->osHdl, llHdl->alarmHdl );

				/* remove alarm */
				if( llHdl->alarmHdl ){
					OSS_AlarmRemove( llHdl->osHdl, &llHdl->alarmHdl );
				}
        	}
        	break;

        /*--------------------------+
        |  (unknown)                |
        +--------------------------*/
        default:
            error = ERR_LL_UNK_CODE;
    }
ERR_EXIT:

	return(error);
}

/****************************** Z75_GetStat *********************************/
/** Get the driver status
 *
 *  The driver supports \ref getstat_setstat_codes "these status codes"
 *  in addition to the standard codes (see mdis_api.h).
 *
 *  \param llHdl            \IN  low-level handle
 *  \param code             \IN  \ref getstat_setstat_codes "status code"
 *  \param ch               \IN  current channel
 *  \param value32_or_64P   \IN  pointer to block data structure (M_SG_BLOCK) for
 *                               block status codes
 *  \param value32_or_64P   \OUT data pointer or pointer to block data structure
 *                               (M_SG_BLOCK) for block status codes
 *
 *  \return           \c 0 On success or error code
 */
static int32 Z75_GetStat(
    LL_HANDLE *llHdl,
    int32  code,
    int32  ch,
    INT32_OR_64 *value32_or_64P
)
{
    int32		*valueP		  = (int32*)value32_or_64P;	      /* pointer to 32bit value  */
    /* INT32_OR_64	*value64P	  = value32_or_64P;		 		  stores 32/64bit pointer  */
    /* M_SG_BLOCK	*blk 		  = (M_SG_BLOCK*)value32_or_64P;  stores block struct pointer */
	int32 error = ERR_SUCCESS;

    DBGWRT_1((DBH, "LL - Z75_GetStat: ch=%d code=0x%04x\n",
			  ch,code));

    switch(code)
    {
        /*--------------------------+
        |  debug level              |
        +--------------------------*/
        case M_LL_DEBUG_LEVEL:
            *valueP = llHdl->dbgLevel;
            break;
        /*--------------------------+
        |  number of channels       |
        +--------------------------*/
        case M_LL_CH_NUMBER:
            *valueP = CH_NUMBER;
            break;
        /*--------------------------+
        |  irq counter              |
        +--------------------------*/
        case M_LL_IRQ_COUNT:
            *valueP = llHdl->irqCount;
            break;
        /*--------------------------+
        |  FREQUENCY                |
        +--------------------------*/
        case Z075_FREQUENCY:
        	error = GetFreq( llHdl, (u_int32*)valueP );
            break;

        /*--------------------------+
        |  UPPER LIMIT              |
        +--------------------------*/
        case Z075_A_UPPER_LTD:
        	*valueP = llHdl->upLimit;
            break;

        /*--------------------------+
        |  LOWER LIMIT              |
        +--------------------------*/
        case Z075_A_LOWER_LTD:
        	*valueP = llHdl->lowLimit;
            break;

        /*--------------------------+
        |  RESOLUTION               |
        +--------------------------*/
        case Z075_RESOLUTION:
        	*valueP = llHdl->res;
            break;

        /*--------------------------+
        |  AUTOMATIC MODE           |
        +--------------------------*/
        case Z075_AUTO_MODE:
        	*valueP = llHdl->autoMode;
            break;

        /*--------------------------+
        |  START/STOP MEASUREMENT   |
        +--------------------------*/
        case Z075_START_MEAS:
        	*valueP = llHdl->startFlag;
            break;

        /*--------------------------+
        |  unknown                  |
        +--------------------------*/
        default:
            error = ERR_LL_UNK_CODE;
    }

	return(error);
}

/******************************* Z75_BlockRead ******************************/
/** Read a data block from the device
 *
 *  \param llHdl       \IN  Low-level handle
 *  \param ch          \IN  Current channel
 *  \param buf         \IN  Data buffer
 *  \param size        \IN  Data buffer size
 *  \param nbrRdBytesP \OUT Number of read bytes
 *
 *  \return            \c 0 On success or error code
 */
static int32 Z75_BlockRead(
     LL_HANDLE *llHdl,
     int32     ch,
     void      *buf,
     int32     size,
     int32     *nbrRdBytesP
)
{
	return(ERR_LL_ILL_FUNC);
}

/****************************** Z75_BlockWrite ******************************/
/** Write a data block from the device
 *
 *  \param llHdl  	   \IN  Low-level handle
 *  \param ch          \IN  Current channel
 *  \param buf         \IN  Data buffer
 *  \param size        \IN  Data buffer size
 *  \param nbrWrBytesP \OUT Number of written bytes
 *
 *  \return            \c 0 On success or error code
 */
static int32 Z75_BlockWrite(
     LL_HANDLE *llHdl,
     int32     ch,
     void      *buf,
     int32     size,
     int32     *nbrWrBytesP
)
{
	return(ERR_LL_ILL_FUNC);
}


/****************************** Z75_Irq ************************************/
/** Interrupt service routine
 *
 *  The interrupt is triggered when ??? occurs.
 *
 *  If the driver can detect the interrupt's cause it returns
 *  LL_IRQ_DEVICE or LL_IRQ_DEV_NOT, otherwise LL_IRQ_UNKNOWN.
 *
 *  \param llHdl  	   \IN  Low-level handle
 *  \return LL_IRQ_DEVICE	IRQ caused by device
 *          LL_IRQ_DEV_NOT  IRQ not caused by device
 *          LL_IRQ_UNKNOWN  Unknown
 */
static int32 Z75_Irq(
   LL_HANDLE *llHdl
)
{
    IDBGWRT_1((DBH, ">>> Z75_Irq:\n"));

	/* ... */

	llHdl->irqCount++;

	return(LL_IRQ_DEV_NOT);
}

/****************************** Z75_Info ***********************************/
/** Get information about hardware and driver requirements
 *
 *  The following info codes are supported:
 *
 * \code
 *  Code                      Description
 *  ------------------------  -----------------------------
 *  LL_INFO_HW_CHARACTER      Hardware characteristics
 *  LL_INFO_ADDRSPACE_COUNT   Number of required address spaces
 *  LL_INFO_ADDRSPACE         Address space information
 *  LL_INFO_IRQ               Interrupt required
 *  LL_INFO_LOCKMODE          Process lock mode required
 * \endcode
 *
 *  The LL_INFO_HW_CHARACTER code returns all address and
 *  data modes (ORed) which are supported by the hardware
 *  (MDIS_MAxx, MDIS_MDxx).
 *
 *  The LL_INFO_ADDRSPACE_COUNT code returns the number
 *  of address spaces used by the driver.
 *
 *  The LL_INFO_ADDRSPACE code returns information about one
 *  specific address space (MDIS_MAxx, MDIS_MDxx). The returned
 *  data mode represents the widest hardware access used by
 *  the driver.
 *
 *  The LL_INFO_IRQ code returns whether the driver supports an
 *  interrupt routine (TRUE or FALSE).
 *
 *  The LL_INFO_LOCKMODE code returns which process locking
 *  mode the driver needs (LL_LOCK_xxx).
 *
 *  \param infoType	   \IN  Info code
 *  \param ...         \IN  Argument(s)
 *
 *  \return            \c 0 On success or error code
 */
static int32 Z75_Info(
   int32  infoType,
   ...
)
{
    int32   error = ERR_SUCCESS;
    va_list argptr;

    va_start(argptr, infoType );

    switch(infoType) {
		/*-------------------------------+
        |  hardware characteristics      |
        |  (all addr/data modes ORed)   |
        +-------------------------------*/
        case LL_INFO_HW_CHARACTER:
		{
			u_int32 *addrModeP = va_arg(argptr, u_int32*);
			u_int32 *dataModeP = va_arg(argptr, u_int32*);

			*addrModeP = MDIS_MA08;
			*dataModeP = MDIS_MD08 | MDIS_MD16;
			break;
	    }
		/*-------------------------------+
        |  nr of required address spaces |
        |  (total spaces used)           |
        +-------------------------------*/
        case LL_INFO_ADDRSPACE_COUNT:
		{
			u_int32 *nbrOfAddrSpaceP = va_arg(argptr, u_int32*);

			*nbrOfAddrSpaceP = ADDRSPACE_COUNT;
			break;
	    }
		/*-------------------------------+
        |  address space type            |
        |  (widest used data mode)       |
        +-------------------------------*/
        case LL_INFO_ADDRSPACE:
		{
			u_int32 addrSpaceIndex = va_arg(argptr, u_int32);
			u_int32 *addrModeP = va_arg(argptr, u_int32*);
			u_int32 *dataModeP = va_arg(argptr, u_int32*);
			u_int32 *addrSizeP = va_arg(argptr, u_int32*);

			if (addrSpaceIndex >= ADDRSPACE_COUNT)
				error = ERR_LL_ILL_PARAM;
			else {
				*addrModeP = MDIS_MA08;
				*dataModeP = MDIS_MD16;
				*addrSizeP = ADDRSPACE_SIZE;
			}

			break;
	    }
		/*-------------------------------+
        |   interrupt required           |
        +-------------------------------*/
        case LL_INFO_IRQ:
		{
			u_int32 *useIrqP = va_arg(argptr, u_int32*);

			*useIrqP = USE_IRQ;
			break;
	    }
		/*-------------------------------+
        |   process lock mode            |
        +-------------------------------*/
        case LL_INFO_LOCKMODE:
		{
			u_int32 *lockModeP = va_arg(argptr, u_int32*);

			*lockModeP = LL_LOCK_CALL;
			break;
	    }
		/*-------------------------------+
        |   (unknown)                    |
        +-------------------------------*/
        default:
          error = ERR_LL_ILL_PARAM;
    }

    va_end(argptr);
    return(error);
}

/*******************************  Ident  ***********************************/
/** Return ident string
 *
 *  \return            Pointer to ident string
 */
static char* Ident( void )
{
    return( "Z75 - Z75 low-level driver: "
#ifdef MAC_BYTESWAP
    "swapped "
#else
    "non swapped "
#endif
"$Id: z75_drv.c,v 1.4 2010/04/22 11:06:07 amorbach Exp $" );

}

/********************************* Cleanup *********************************/
/** Close all handles, free memory and return error code
 *
 *	\warning The low-level handle is invalid after this function is called.
 *
 *  \param llHdl      \IN  Low-level handle
 *  \param retCode    \IN  Return value
 *
 *  \return           \IN   retCode
 */
static int32 Cleanup(
   LL_HANDLE    *llHdl,
   int32        retCode
)
{
    /*------------------------------+
    |  close handles                |
    +------------------------------*/
	/* clean up desc */
	if (llHdl->descHdl)
		DESC_Exit(&llHdl->descHdl);

	/* clean up debug */
	DBGEXIT((&DBH));

    /*------------------------------+
    |  free memory                  |
    +------------------------------*/
    /* free my handle */
    OSS_MemFree(llHdl->osHdl, (int8*)llHdl, llHdl->memAlloc);

    /*------------------------------+
    |  return error code            |
    +------------------------------*/
	return(retCode);
}

/********************************* GetFreq *********************************/
/** Read value of Result register and calculate the measured frequency
 *  if a new values is measured.
 *  \param llHdl      \IN  Low-level handle
 *  \param freqP      \IN  pointer to store frequency value
 *
 *  \return           \IN   0 if value could be determined or Error code
 */
static int32 GetFreq( LL_HANDLE *llHdl, u_int32 *freqP )
{
	u_int32 result = 0;
	u_int32 ctrl = 0;
	u_int32 meas = 0;
	u_int32 period = 0;
	u_int32 freq = 0;

	int32 error = ERR_SUCCESS;

	/* read result register */
	result = MREAD_D32( llHdl->ma, Z075_RESULT_ADDR_OFFSET);

	/* read status register */
	ctrl = MREAD_D32( llHdl->ma, Z075_CTRL_STS_ADDR_OFFSET);

	/* check new value */
	if( ctrl & Z075_STS_LOW_MSK || ctrl & Z075_STS_HIGH_MSK ||
		!(result & Z075_RES_NEW_VALUE_MSK) ){
		*freqP = 0;
		error = ERR_LL_READ;
	}

	/* calculate frequency */
	period = (result & Z075_RES_PERIOD_MSK) >> 20;
	meas = result & Z075_RES_MEAS_TIME_MSK;

	if( checkRange( llHdl->fpgaSysClk, period ) ){
		freq = period * ( llHdl->fpgaSysClk / meas);
	}
	else{
		freq = (period * llHdl->fpgaSysClk) / meas;
	}

    DBGWRT_1((DBH, "LL - Z75_GetFreq: freq=%d period=%d meas=%d\n", freq, \
    	period, meas ));

	*freqP = freq;

	return error;
}

/**************************** automatedTsk *******************************/
/** This task is called from OSS_Alarm if time llHdl->realMsec elapsed
 *
 *  \param hdl      \IN  Low-level handle
 *
 */
static void automatedTsk( void *hdl )
{
	LL_HANDLE *llHdl = (LL_HANDLE*)hdl;
	u_int32 freq = 0;

	if( llHdl->startFlag == 0 ){
		return;
	}

	if( GetFreq( llHdl, &freq ) == ERR_SUCCESS ){

		if( (llHdl->lowLimit > freq) && (llHdl->lowSigIdP != NULL) ){
			/* send signal to inform application */
			OSS_SigSend( llHdl->osHdl, llHdl->lowSigIdP );
		}

		if( (llHdl->upLimit < freq) && (llHdl->upSigIdP != NULL) ){
			/* send signal to inform application */
			OSS_SigSend( llHdl->osHdl, llHdl->upSigIdP );
		}
	}
}

/**************************** checkRange *******************************/
/** This routine evaluates the calculation range
 *
 *  \param clk      \IN  FPGA system core clock
 *  \param per      \IN  period value
 *
 */
static int checkRange( u_int32 clk, u_int32 per )
{
	if( per > (0xffffffff / clk) ){
		return 1;
	}
	else{
		return 0;
	}
}

