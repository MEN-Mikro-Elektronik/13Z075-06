#************************** MDIS5 device descriptor *************************
#
#        Author: rla
#         $Date: 2010/04/22 11:05:30 $
#     $Revision: 1.2 $
#
#   Description: Metadescriptor for Z75 SPEED, swapped variant
#
#****************************************************************************

speed_sw_1  {
	#------------------------------------------------------------------------
	#	general parameters (don't modify)
	#------------------------------------------------------------------------
    DESC_TYPE        = U_INT32  1           # descriptor type (1=device)
    HW_TYPE          = STRING   Z75_SW      # hardware name of device

	#------------------------------------------------------------------------
	#	reference to base board
	#------------------------------------------------------------------------
    BOARD_NAME   = STRING   chameleon_pcitbl_1 # device name of baseboard
    DEVICE_SLOT  = U_INT32  0                  # used slot on baseboard (0..n)

	#------------------------------------------------------------------------
	#	debug levels (optional)
	#   this keys have only effect on debug drivers
	#------------------------------------------------------------------------
    DEBUG_LEVEL      = U_INT32  0xc0008000  # LL driver
    DEBUG_LEVEL_MK   = U_INT32  0xc0008000  # MDIS kernel
    DEBUG_LEVEL_OSS  = U_INT32  0xc0008000  # OSS calls
    DEBUG_LEVEL_DESC = U_INT32  0xc0008000  # DESC calls
    DEBUG_LEVEL_MBUF = U_INT32  0xc0008000  # MBUF calls

	#------------------------------------------------------------------------
	#	device parameters
	#------------------------------------------------------------------------

	# FPGA core system clock
	# range: 0..0xFFFFFFFF
	Z075_SYSCLK = U_INT32	32000000

	# Frequency counter resolution
	# range: 0..7
	Z075_RESOLUTIOM = U_INT16 0

	# Upper frequency limit for the automated mode
	# range: 0..0xFFFFFFFF
	Z075_A_UPPER_LIMIT = U_INT32 0xFFFFFFFF

	# Lower frequency limit for the automated mode
	# range: 0..0xFFFFFFFF
	Z075_A_LOWER_LIMIT = U_INT32 0
}
