#include <RTL.h>                      /* RTX kernel functions & defines      */
#include <LPC23xx.h>                  /* LPC23xx library definitions         */
#include "RTX_CAN.h"                  /* CAN Generic functions & defines     */ 
#include "CAN_node.h"

U8 tx_val;
U8 rx_val1,rx_val2;

void Out_Val (void) {
  
  FIO2PIN |= rx_val1;
  //printf("tx_val = %d", tx_val);
  //printf("rx_val = %d", rx_val1)
}


__task void task_init (void)  {
  os_tsk_create (task_CAN_tx, 3);   /* Start          transmit task        */
  os_tsk_create (task_CAN_rx, 4);   /* Start           receive task        */
 // os_tsk_create (task_disp  , 2);   /* Start displaying to LCD task        */
  os_tsk_delete_self();             /* End      initialization task        */
}

__task void task_CAN_tx () {
  
  CONFIG_MSG(0x10, {0x01, 0x02, 0x03, 0xFF,0xF0,0x60,0x80, 0x90}, 
  					 8, 0, STANDARD_FORMAT, DATA_FRAME, 0xF0),
		  //message2 = {0x20, {0x03, 0x04, 0x05, 0xFF,0xF0,0x60,0x80, 0x90}, 
  		//			 8, 0, STANDARD_FORMAT, DATA_FRAME};
  INIT_CAN(1, 500000);

    
  for (;;)  {
        CAN_send(1, &, 0x0F0);
		CAN_send(1, &message2, 0x0F0);
		tx_val = message2.data[0];
        os_dly_wait(100);                /* Wait 1 second                       */
	  }
}

__task void task_CAN_rx () {
  CAN_msg rx_message = {0x20, {0x0, 0x0, 0x0, 0x00,0x00,0x00,0x0, 0x0}, 
  				        8, 0, STANDARD_FORMAT, DATA_FRAME};	  

  init_CAN(2, 500000);        
  for (;;)  {
      CAN_receive(2, &rx_message,0xFFF0);
	  if(rx_message.id == 0x10)
	  rx_val1 = rx_message.data[0];
	  else if(rx_message.id == 0x20)
	  rx_val2 = rx_message.data[0]; 
   
	  }
}

__task void task_disp (void) {
  for (;;)  {
    Out_Val ();                       /* Output info on visual outputs       */
    os_dly_wait (10);                 /* Wait 100 ms and display again       */
  }
}

 
int main(void){
  PINSEL10 = 0;                       /* Disable ETM interface, enable LEDs  */
  FIO2DIR  = 0x000000FF;              /* P2.0..7 defined as Outputs          */
  FIO2MASK = 0x00000000;
  FIO2PIN  = 0x00;
  Serial_init();		  /* Init UART */
  //printf("Serial port working!");
  os_sys_init (task_init);            /* Initialize OS and start init task   */

}




