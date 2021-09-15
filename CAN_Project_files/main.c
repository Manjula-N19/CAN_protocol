/*----------------------------------------------------------------------------
*      Project: C A N bus demonstrator
 *----------------------------------------------------------------------------
 *      Name:    main.c
 *      Purpose: To demonstrate the real-time communication using CAN protocol 
 *      Author:  Manjula Narayanaswamy			
 *----------------------------------------------------------------------------
 *      This code is implemented as part of MSc dissertation - 2011
 *      Sheffield Hallam University, UK
 *---------------------------------------------------------------------------*/




#include <RTL.h>                      /* RTX kernel functions & defines      */
#include <LPC23xx.h>                  /* LPC23xx library definitions         */
#include "RTX_CAN.h"                  /* CAN Generic functions & defines     */ 
#include "CAN_node.h"

/* Initialise CAN message */    
  unsigned char CAN_MSG_1[3] =    {0x20, STANDARD_FORMAT,DATA_FRAME}; // Message ID, Format, Frame        		     	 
  U8 DATA_BYTES_1[8] =    {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
  unsigned char CAN_MSG_2[3] =    {0x40, STANDARD_FORMAT,DATA_FRAME}; // Message ID, Format, Frame        		     	 
  U8 DATA_BYTES_2[8] =    {0x03,0x04,0x02,0x01,0x05,0x06,0x07,0x08};


__task void task_init (void)  {
  os_tsk_create (task_CAN_tx, 3);   /* Start          transmit task        */
  os_tsk_create (task_CAN_rx, 4);   /* Start           receive task        */
 // os_tsk_create (task_disp  , 2);   /* Start displaying to LCD task        */
  os_tsk_delete_self();             /* End      initialization task        */
}

__task void task_CAN_tx () { 
    
  unsigned int t=0;
  INIT_CAN(1, 500000);  // Initialize Controller and its baud rate
  INIT_CAN(2, 1000000);
    os_itv_set(10);
  for (;;)  {
    os_itv_wait();   /* Wait 1 second    */
	t++;
	if(t==10)
	Transmit(1, CAN_MSG_1, DATA_BYTES_1, sizeof (DATA_BYTES_1));    // Message on cntroller
    if(t==20)
	{
	Transmit(2, CAN_MSG_2, DATA_BYTES_2, sizeof (DATA_BYTES_2));    // Message on cntroller
	t=0;
	}
	}
}


__task void task_CAN_rx () {
  CAN_msg RXCAN_MSG_1, RXCAN_MSG_2;
  for (;;)  {
      CAN_receive(1, &RXCAN_MSG_1,0xFF0); 
	  CAN_receive(2, &RXCAN_MSG_2,0xFFF0);	
   
	  }
}

int main(){
 os_sys_init (task_init);            /* Initialize OS and start init task   */

}




 





