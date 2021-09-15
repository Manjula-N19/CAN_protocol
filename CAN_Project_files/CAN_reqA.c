/*----------------------------------------------------------------------------
 *      R T L    C A N   D r i v e r
 *----------------------------------------------------------------------------
 *      Name:    CAN_reqA.c
 *      Purpose: RTX CAN Driver usage example
 *      Rev.:    V3.20
 * 
 * Remote Frame request example
 * ============================
 * This example demonstrates reception of CAN ID 0x120 remote frame
 * and how to respond to the requested ID.
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2008 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>                      /* RTX kernel functions & defines      */
#include <LPC23xx.h>                  /* LPC23xx library definitions         */
#include "RTX_CAN.h"                  /* CAN Generic functions & defines     */ 
#include "LCD.h"                      /* LCD functions prototypes            */

__task void task_init     (void) ;
__task void task_send_CAN (void) ;
__task void task_rece_CAN (void) ;
__task void task_disp     (void) ;

U32 Tx_val = 0, Rx_val = 0;           /* Global variables used for display   */
U32 status = 0;                     /* LEDs status indicators */
char hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};


/*----------------------------------------------------------------------------
 *  Function for converting 1 byte to string in hexadecimal notation
 *---------------------------------------------------------------------------*/

void Hex_Str (unsigned char hex, char *str) {
  *str++ = '0';
  *str++ = 'x';
  *str++ = hex_chars[hex >>  4];
  *str++ = hex_chars[hex & 0xF];
}

/*----------------------------------------------------------------------------
 *  Functions for init and getting input value
 *---------------------------------------------------------------------------*/

void In_Init (void) {
  /* Power enable, Setup pin, enable and setup AD converter interrupt        */
  PCONP        |= (1 << 12);          /* Enable power to AD block            */
  PINSEL1       = 0x00004000;         /* AD0.0 pin function select           */
  AD0CR         = 0x002E0301;         /* Power up, PCLK/4, sel AD0.0         */
}

unsigned char In_Get (void) {
  unsigned int val;

  AD0CR |=  0x01000000;               /* Start A/D Conversion                */
  while (!((val=AD0GDR)&0x80000000)); /* Wait for Conversion and Read Data   */
  AD0CR &= ~0x01000000;               /* Stop A/D Conversion                 */

  val = (val >> 8) & 0xFF;            /* Extract AIN0 Value                  */

  return (val);
}

/*----------------------------------------------------------------------------
 *  Functions for init and output of value on visual element
 *---------------------------------------------------------------------------*/

void Out_Init (void) {
  int n;

  PINSEL10 = 0;                       /* Disable ETM interface, enable LEDs  */
  FIO2DIR  = 0x000000FF;              /* P2.0..7 defined as Outputs          */
  FIO2MASK = 0x00000000;
  /* Setup LCD                                                               */
  lcd_init();                         /* Initialize the LCD display          */
  lcd_clear();                        /* Clear the LCD display               */
  lcd_print ("MCB2300 CAN A");     /* Display string on LCD display       */
  set_cursor (0, 1);                  /* Set cursor position on LCD display  */
  lcd_print ("  Alan Goude  ");
  for (n = 0; n < 30000000; n ++);    /* Wait for initial display (~5s)      */


  lcd_clear ();
  lcd_print ("CAN at 500kbit/s");
}

void Out_Val (void) {
  U32 val1, val2;
  static char disp_buf[] = "Tx:    , Rx:    ";

  val1 = Tx_val;                      /* Read values to local variable       */
  val2 = Rx_val;    

  FIO2CLR = 0xFF;                     /* Turn off all LEDs                   */
  FIO2SET = (status & 0xFF);          /* Turn on requested LEDs              */

  Hex_Str(val1, &disp_buf[ 3]);       /* Display Tx and Rx values to LCD disp*/ 
  Hex_Str(val2, &disp_buf[12]);
  set_cursor (0, 1);
  lcd_print  ((char *)disp_buf);
}



/*----------------------------------------------------------------------------
 *  Task 0: Initializes and starts other tasks
 *---------------------------------------------------------------------------*/

__task void task_init (void)  {
  os_tsk_create (task_send_CAN, 3);   /* Start          transmit task        */
  os_tsk_create (task_rece_CAN, 4);   /* Start           receive task        */
  os_tsk_create (task_disp    , 2);   /* Start displaying to LCD task        */
  os_tsk_delete_self();               /* End      initialization task        */
}

/*----------------------------------------------------------------------------
 *  Task 1: Sends message with input value in data[0] over CAN periodically
 *---------------------------------------------------------------------------*/

__task void task_send_CAN (void) {

  CAN_rx_object (1, 0,  0x120, STANDARD_FORMAT); /* Enable reception of         */
                                      /* message on controller 1, channel is    */
                                      /* not used for LPC (can be set to        */
                                      /* whatever value), with standard id 0x111*/
 
  CAN_init (1, 500000);               /* CAN controller 1 init, 500 kbit/s   */
  CAN_start (1);                      /* Start controller 1                  */
  
  for (;;)  {
        os_dly_wait(100);                /* Wait 1 second                       */
		// This task would normally be doing some sending of CAN messages
		// But remember the CAN message handling functions are not reentrant.
		// Which means that only one task at a time can call the CAN_send function!
		// You would need to control access to the CAN message handling functions or
		// only allow a single task to use each of the CAN functions.

  }
}

 /*----------------------------------------------------------------------------
 *  Task 2: Received CAN messages
 *---------------------------------------------------------------------------*/

__task void task_rece_CAN (void) {
  CAN_msg msg_rec;		// CAN message structure for recieved messages
  CAN_msg msg_send120       = { 0x120, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   
                              1, 0, STANDARD_FORMAT, DATA_FRAME };

  for (;;)  {
    if (CAN_receive (1, &msg_rec, 0x00FF) == CAN_OK)  
    {
        // check to see if this is a remote frame request
		if (msg_rec.type == REMOTE_FRAME)
        {
            switch(msg_rec.id)
            {
                case 0x120: 
                            Tx_val= In_Get();     
                            msg_send120.data[0] = Tx_val;     
                            CAN_send(1,&msg_send120,0X0F00);
                            break;
            }
        }
        else
        {
            // A normal Data Frame - process as normal received DATA FRAME
        }
    }
  }
}

/*----------------------------------------------------------------------------
 *  Task 3: Activate visual outputs
 *---------------------------------------------------------------------------*/

__task void task_disp (void) {
  for (;;)  {
    Out_Val ();                       /* Output info on visual outputs       */
    os_dly_wait (10);                 /* Wait 100 ms and display again       */
  }
}


/*----------------------------------------------------------------------------
 *        Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/

int main (void)  {                    /* Program execution starts here       */

  In_Init  ();                        /* Initialize input                    */
                                      /* - Analog voltage                    */
  Out_Init ();                        /* Initialize visual outputs           */
                                      /* - LEDs, LCD display (2x16 chars)    */

  os_sys_init (task_init);            /* Initialize OS and start init task   */
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
