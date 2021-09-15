#include <RTL.h>                      /* RTX kernel functions & defines      */
#include "RTX_CAN.h"                  /* CAN Generic functions & defines     */ 

//CAN_msg CONFIG_MSG;
void INIT_CAN(U32 cntlr, U32 speed);
//void CONFIG_MSG(U32 ID, U8 *Data_bytes, U8 Length, U8 Format, U8 Frame);
void Transmit(U32 cntlr, unsigned char *MSG,U8 *Data_bytes, U8 length);


__task void task_init (void);
__task void task_CAN_tx (void);
__task void task_CAN_rx (void);
__task void task_disp (void);
