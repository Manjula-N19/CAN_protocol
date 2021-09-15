
#include <RTL.h>                      /* RTX kernel functions & defines      */
#include <LPC23xx.h>                  /* LPC23xx library definitions         */
#include "RTX_CAN.h"                  /* CAN Generic functions & defines     */ 
#include "CAN_node.h"

//U32 cntlr;

/* Initialize Controller  */
void INIT_CAN(U32 cntlr, U32 speed){
  
  CAN_init (cntlr, speed);               
  CAN_start (cntlr);                  /* Start controller                   */
  
  }

//void CONFIG_MSG(U32 ID, U8 *Data_bytes, U8 Length, U8 Format, U8 Frame){
 // int i;
 // MSG = { ID, Data_bytes , Length, 0, Format, Frame };
  //for(i=0;i<Length;i++){
  //MSG[1][i] = *Data_bytes;
  //Data_bytes++;
  //} 
//}

void Transmit(U32 cntlr, unsigned char *MSG, U8 *Data_bytes, U8 length){
  int i;
  CAN_msg message;
  message.id     = MSG[0];
  message.format = MSG[1];
  message.type   = MSG[2];    
  message.len    = length;
  for(i=0; i<length; i++,Data_bytes++){
  message.data[i] = *Data_bytes;
  }
  CAN_send(cntlr, &message, 0xF0);
} 




  
  


