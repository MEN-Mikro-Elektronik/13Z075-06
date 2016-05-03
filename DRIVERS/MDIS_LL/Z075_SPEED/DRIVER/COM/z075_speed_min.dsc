#************************** MDIS5 device descriptor *************************
#
#        Author: rla
#         $Date: 2010/04/22 11:05:28 $
#     $Revision: 1.2 $
#
#   Description: Metadescriptor for Z75 SPEED
#
#****************************************************************************

speed_1  {
	#------------------------------------------------------------------------
	#	general parameters (don't modify)
	#------------------------------------------------------------------------
    DESC_TYPE        = U_INT32  1           # descriptor type (1=device)
    HW_TYPE          = STRING   Z75         # hardware name of device

	#------------------------------------------------------------------------
	#	reference to base board
	#------------------------------------------------------------------------
    BOARD_NAME   = STRING   chameleon_pcitbl_1 # device name of baseboard
    DEVICE_SLOT  = U_INT32  0                  # used slot on baseboard (0..n)

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
