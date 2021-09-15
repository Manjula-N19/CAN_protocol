/*----------------------------------------------------------------------------
 *      R T L  -  C A N   D r i v e r
 *----------------------------------------------------------------------------
 *      Name:    CAN_cfg.h
 *      Purpose: Header for LPC2xxx CAN Configuration Settings
 *      Rev.:    V3.20
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2008 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#ifndef _CAN_CFG_H
#define _CAN_CFG_H

#include "CAN_Error.h"                /* CAN Error definition                */

/*
// *** <<< Use Configuration Wizard in Context Menu >>> ***
*/

/*
// Peripheral clock, depends on VPBDIV
// <o> PCLK value <1-1000000000>
*/
#define  PCLK            24000000


/*
// <e>Use CAN Controller 1
*/
#define USE_CAN_CTRL1           1
/* 
// </e>
*/


/*
// <e>Use CAN Controller 2
*/
#define USE_CAN_CTRL2           1
/*
// </e>
*/


/*
// <o>Number of transmit objects for controllers <1-1024>
// <i> Determines the size of software message buffer for transmitting.
// <i> Default: 20
*/
#define CAN_No_SendObjects     20


/*
// <o>Number of receive objects for controllers <1-1024>
// <i> Determines the size of software message buffer for receiving.
// <i> Default: 20
*/
#define CAN_No_ReceiveObjects  20

/*
// *** <<< End of Configuration section             >>> ***
*/

/* Maximum index of used CAN controller 1..2
   Needed for memory allocation for CAN messages.                            */

#if (USE_CAN_CTRL2)
  #define CAN_CTRL_MAX_NUM      2
#elif (USE_CAN_CTRL1)
  #define CAN_CTRL_MAX_NUM      1
#else
  #error "No CAN Controller defined"
#endif

#endif /* _CAN_CFG_H */


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

