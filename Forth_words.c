/*
 * Forth_words.c
 *
 * Created: 2014-01-05 08:43:13
 *  Author: jost1099
 */ 

//#include "Main.h"
//#include "BM700_app.h"
//#include "WCU.h"
#include "Forth_words.h"
#include "Forth_system.h"

/* Forth standard words to be included in the default dictionary */
//
void revision_info(void)
/* Printout revision info - same as on boot*/
{
    puts(STRING_HEADER);
}

/* Stack manipulation words */
void Drop(void)
/* remove the top item from the stack */
{
    Cpop();
}


void Dup(void)
/* Duplicate the top item of the stack */
{
    Stack_item temp =Cpop();
    Cpush(temp);
    Cpush(temp);
}

void Nip(void) /* (x1 x2 - x2) */
{
    Stack_item x2 =Cpop();
    Cpop();
    Cpush(x2);
}

void Over(void) /* x1 x2 - x1 x2 x1 */
{
    Stack_item x2 =Cpop();
    Stack_item x1 =Cpop();
    Cpush(x1);
    Cpush(x2);
    Cpush(x1);
}

void Swap(void) /* x1 x2 - x2 x1 */
{
    Stack_item x2 =Cpop();
    Stack_item x1 =Cpop();
    Cpush(x2);
    Cpush(x1);
}

/* Return stack manipulation words */
void To_R(void) /* (S: x -) (R: - x) */
{
    Stack_item temp =Cpop();
    Rpush(temp);
}

void R_from(void) /* (S: - x) (R: x -) */
{
    Cpush(Rpop());
}

void R_fetch(void) /* (S: - x) (R: x- x) */
{
    Stack_item x =Rpop();
    Rpush(x);
}

/* Single precision math words */
void Hex(void)
{
    BASE =16;
}

void DEC(void)
{
    BASE =10;
}

void Dot(void) /* (n -) */
{
    uputi(Cpop().n, BASE);
}

void Star(void) /* (n1 n2 - n3) */
{
    Stack_item n2 =Cpop();
    n2.n *= Cpop().n;
    Cpush(n2);
}

void Slash(void) /* (n1 n2 - n3) */
{
    Stack_item n2 =Cpop();
    Stack_item n1 =Cpop();
    n1.n /= n2.n;
    Cpush(n1);
}

void Plus(void) /* (n1 n2 - n3) */
{
    Stack_item n2 =Cpop();
    n2.n += Cpop().n;
    Cpush(n2);
}

void Minus(void) /* (n1 n2 - n3) */
{
    Stack_item n2 =Cpop();
    Stack_item n1 =Cpop();
    n1.n -= n2.n;
    Cpush(n1);
}

void Slash_mod(void) /* (n1 n2 - n3 n4) */
{
    Stack_item n3, n2 =Cpop();
    Stack_item n1 =Cpop();
    n3.n =n1.n /n2.n;
    Cpush(n3);
    n3.n = n1.n %n2.n;
    Cpush(n3);
}

void One_plus(void) /* (n1 - n2) */
{
    Stack_item n1 =Cpop();
    n1.n += 1;
    Cpush(n1);
}

void Two_plus(void) /* (n1 - n2) */
{
    Stack_item n1 =Cpop();
    n1.n += 2;
    Cpush(n1);
}

void One_minus(void) /* (n1 - n2) */
{
    Stack_item n1 =Cpop();
    n1.n -= -1;
    Cpush(n1);
}

void Two_minus(void) /* (n1 - n2) */
{
    Stack_item n1 =Cpop();
    n1.n -= 2;
    Cpush(n1);
}

void Two_star(void) /* (n1 - n2) */
{
    Stack_item n1 =Cpop();
    n1.n *= 2;
    Cpush(n1);
}

void Two_slash(void) /* (n1 - n2) */
{
    Stack_item n1 =Cpop();
    n1.n /= 2;
    Cpush(n1);
}

/* Memory manipulation words */
void C_store(void) /* (c c-address - ) */
{
    char *c_address =Cpop().c_address;
    *c_address = Cpop().c;
}

void C_fetch(void) /* (c-address - c) */
{
    Stack_item mychar;
    mychar.c = *(Cpop().c_address);
    Cpush(mychar);
}

void Fetch(void)
{
    Stack_item myint;
    myint.n =*(Cpop().a_address);
    Cpush(myint);
}

void Store(void)
{
    int *a_address =Cpop().a_address;
    *a_address = Cpop().n;
}

/* Logic and testing */
void OR_bitwise(void) /* n1 n2 - flag) */
{
    Stack_item n2 =Cpop();
    n2.n |= Cpop().n;
    Cpush(n2);
}

void AND_bitwise(void) /* n1 n2 - flag) */
{
    Stack_item n2 =Cpop();
    n2.n &=Cpop().n;
    Cpush(n2);
}

void Less_than(void) /* n1 n2 - flag) */
{
    Stack_item n2 =Cpop();
    if (Cpop().n <n2.n)
    n2.n =-1;
    else
    n2.n =0;
    Cpush(n2);
}

void Equal(void) /* n1 n2 - flag) */
{
    Stack_item n2 =Cpop();
    if (Cpop().n ==n2.n)
    n2.n =-1;
    else
    n2.n =0;
    Cpush(n2);
}

void Zero_equal(void) /* n1 - flag) */
{
    Stack_item n;
    if (Cpop().n ==0)
    n.n =-1;
    else
    n.n =0;
    Cpush(n);
}

void Zero_less_than(void) /* n1 - flag) */
{
    Stack_item n;
    if (Cpop().n <=0)
    n.n =-1;
    else
    n.n =0;
    Cpush(n);
}

void Zero_greater_than(void) /* n1 - flag) */
{
    Stack_item n;
    if (Cpop().n >0)
    n.n =-1;
    else
    n.n =0;
    Cpush(n);
}

void Zero_not_equal(void) /* n1 - flag) */
{
    Stack_item n;
    if (Cpop().n !=0)
    n.n =-1;
    else
    n.n =0;
    Cpush(n);
}

void RAM_dump(void) /* c-address n+ -) */
/* Syntax "<Start adress> <Number of bytes> dump" */
{
    int const per_row =16;
    int i =0;
    int temp, n =Cpop().n;
    char *c_address =Cpop().c_address;

    while (i <n){
        printf("%04x: ", (int)c_address);
        for (temp =0; temp <per_row; temp++, i++ )
        printf("%02x ", (int)(*c_address));
        printf("\n\r");
    }
}


