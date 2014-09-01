/*
 * Forth_words.h
 *
 * Created: 2014-01-05 08:43:44
 *  Author: jost1099
 */ 


#ifndef FORTH_WORDS_H_
#define FORTH_WORDS_H_

#include "F_Defs.h"

extern void revision_info(void);    /* SW revision to the console output */

/* Stack manipulation words */
extern void Drop(void);     /* remove the top item from the stack */
extern void Dup(void);      /* Duplicate the top item of the stack */
extern void Nip(void);      /* (x1 x2 - x2) */
extern void Over(void);     /* x1 x2 - x1 x2 x1 */
extern void Swap(void);     /* x1 x2 - x2 x1 */

/* Return stack manipulation words */
extern void To_R(void);     /* (S: x -) (R: - x) */
extern void R_from(void);   /* (S: - x) (R: x -) */
extern void R_fetch(void);  /* (S: - x) (R: x- x) */

/* Single precision math words */
extern void Hex(void);      /* Set the global BASE to hexadeciaml (16) */
extern void DEC(void);      /* Set BASE to 10 */
extern void Dot(void);      /* (n -) */
extern void Star(void);     /* (n1 n2 - n3) */
extern void Slash(void);    /* (n1 n2 - n3) */
extern void Plus(void);     /* (n1 n2 - n3) */
extern void Minus(void);    /* (n1 n2 - n3) */
extern void Slash_mod(void);    /* (n1 n2 - n3 n4) */
extern void One_plus(void);     /* (n1 - n2) */
extern void Two_plus(void);     /* (n1 - n2) */
extern void One_minus(void);    /* (n1 - n2) */
extern void Two_minus(void);    /* (n1 - n2) */
extern void Two_star(void);     /* (n1 - n2) */
extern void Two_slash(void);    /* (n1 - n2) */

/* Memory manipulation words */
extern void C_store(void);      /* (c c-address - ) */
extern void C_fetch(void);      /* (c-address - c) */
extern void Fetch(void);
extern void Store(void);

/* Logic and testing */
extern void OR_bitwise(void);   /* n1 n2 - flag) */
extern void AND_bitwise(void);  /* n1 n2 - flag) */
extern void Less_than(void);    /* n1 n2 - flag) */
extern void Equal(void);        /* n1 n2 - flag) */
extern void Zero_equal(void);       /* n1 - flag) */
extern void Zero_less_than(void);   /* n1 - flag) */
extern void Zero_greater_than(void);    /* n1 - flag) */
extern void Zero_not_equal(void);       /* n1 - flag) */

/* Miscellaneous functions */
extern void RAM_dump(void);     /* c-address n+ -) */
/* Syntax "<Start adress> <Number of bytes> dump" */

/* Words that operate on the data area 'heap' */


#endif /* FORTH_WORDS_H_ */
