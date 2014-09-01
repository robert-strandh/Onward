/*
 * F_Defs.h
 *
 * Created: 2014-01-05 09:33:58
 *  Author: jost1099
 */ 


#ifndef F_DEFS_H_
#define F_DEFS_H_

#include <string.h>
// #include "asf.h"

/* Forth stacks */
typedef union {
    int n;
    char c;
    char *c_address;
    int *a_address;
    void *default_ptr; //Default pointer for new words all
} Stack_item;

/* Hash dictionary and function to create, insert and delete dictionary entries */
#define HASHSIZE 16
#define NAMELENGTH 16
struct Entry_type {
    char Name[NAMELENGTH];
    struct Entry_type *Previous_def;
    int Xtoken;
    union {
        void (*funk)(void);
        int i;
        struct Entry_type *code;
        Stack_item *data;
    } parameter;
};

#define STACKSIZE 32



#endif /* F_DEFS_H_ */
