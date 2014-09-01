/*
 * Forth_system.h
 *
 * Created: 2013-02-13 13:36:40
 *  Author: 23050089
 */ 

#ifndef FORTH_SYSTEM_H_
#define FORTH_SYSTEM_H_

#include "F_Defs.h"
//#include "Main.h"
//#include "HW_words.h"
#include <stdio.h>
#include <stdlib.h>
#include "Forth_words.h"

extern int BASE;

extern void Forth_interpreter(void);

/* Supporting function used by the various Forth words */
extern void Cpush(Stack_item param);    //Push one stack item to the CSTACK
extern Stack_item Cpop(void);           //Pop one stack item from the CSTACK
extern void Rpush(Stack_item param);    //Push one item to the RSTACK
extern Stack_item Rpop(void);           //Pop one item from the RSTACK
extern int Cdepth(void);                //Calculate the amount of items on the stack
extern void uputi(int i, int base);     //

extern void Here(void); /* (- c-addr). Push the next available data location on the stack */


#endif /* FORTH_SYSTEM_H_ */


