/*
 * Forth_system.c
 *
 * Created: 2013-02-13 13:35:39
 *  Author: 23050089
 */ 

#include "Forth_system.h"

#define PER_ROW 16
#define ENABLE_ENTRY
#define use_create
#define ARM4S_DAC
/* define EE_enable */
#define ENABLE_HEAP
#define NEW_UPUTI

Stack_item CSTACK[STACKSIZE];
int CStack_pointer = 0;

#define RSTACKSIZE 32
static Stack_item RSTACK[STACKSIZE];
static int RStack_pointer = 0;

#ifdef ENABLE_HEAP
#define FHEAP_SIZE 1024
static Stack_item Data_segment[FHEAP_SIZE];
static Stack_item *Datapointer = Data_segment;
#endif

static struct Entry_type *Hash_dictionary[HASHSIZE];

/* Length of an command line from the terminal */
#define CMD_LENGTH 96
static char Command_Line[CMD_LENGTH];
static unsigned char Cmdline_index;
static char substring[CMD_LENGTH];

/* Forth interpreter variables */
int BASE = 16;
static int STATE = 0;
static struct Entry_type *Compile_entry;


void uputi(int i, int base)
{
#ifdef NEW_UPUTI
    switch (base){
    case 8:
        printf("%o", i);
    break;
    case 10:
        printf("%d", i);
    break;
    default:
        printf("%x", i);
    break;
    }
#else
    int temp;
    char s[] = "0123456789abcdef";
    char temp2[16];
    
    if (i == 0)
    putchar('0');
    else{
        if (i <0){
            putchar('-');
            i = -i;
        }
        for (temp = 0; (i >= 1) &&(temp < 15);)
        {
            temp2[temp++] = s[i %base];
            i /= base;
        }
        for (--temp; temp >= 0; temp--)
        putchar(temp2[temp]);
    }
#endif /* NEW_UPUTI */
}

void Cpush(Stack_item param)
//Push one stack item to the CSTACK
{
    CSTACK[CStack_pointer++] = param;
}

Stack_item Cpop(void)
//Pop one stack item from the CSTACK
{
    if (CStack_pointer >0)
        return (CSTACK[--CStack_pointer]);
    else{
        printf("\n\rStack underflow!!\n\r");
        return (CSTACK[CStack_pointer]);
    }
}

void Rpush(Stack_item param)
{
    RSTACK[RStack_pointer++] = param;
}

Stack_item Rpop(void)
{
    return (RSTACK[--RStack_pointer]);
}

int Cdepth(void)
{
    return CStack_pointer;
}

void Here(void) /* (- c-addr) */
/* Push the next available data location on the stack */
{
    Stack_item temp;
    temp.default_ptr = Datapointer;
    Cpush(temp);
}


static int Get_number(char *chrstr, int base, int *errno)
{
    int temp1 = 0, temp2 = 1;
    int i;
    *errno = 0;
    for (i = 0; (*chrstr) != '\0';chrstr++, i++)
        ;
    for (--i, --chrstr; i >= 0;i--)
    {
        if ((*chrstr >= '0') &&(*chrstr <= '9')){
            temp1 += ((*chrstr--)-'0')*temp2;
            temp2 *= base;
        }
        else if ((*chrstr >= 'a') &&(*chrstr <= 'f')){
            temp1 += ((*chrstr--)-'a'+10)*temp2;
            temp2 *= base;
        }
        else            
            *errno = 1;
    }       
    return temp1;               
}

/* Conditional words */
#ifdef IF_THEN_ELSE
void If_then(void)
{
    char Wordstring[32];
    int i;
    char temp;
    /* Get the true/false flag from the stack */
    if (Cpop() ==- 1) /* == true */{
        /* Discard leading characters blank and tab */
        for (temp = Command_Line[Cmdline_index]; (temp == '\t')||(temp == ' '); temp = Command_Line[++Cmdline_index])
        ;
        for (i = 0; (temp != '\t') &&(temp != ' ') &&(temp != '\0');)
        {
            substring[i++] = temp;
            temp = Command_Line[++Cmdline_index];
        }
        if (i == 0)
        Forth_state = 4;
        else{
            Wordstring[i] = '\0';
            Current_word = Find_in_dict(&substring[0]);
            if (Current_word != NULL)
            Current_word->funk();
            else{
                Cpush(Get_number(substring, BASE, Error_no));
    
}
#endif

static struct Entry_type *New_entry(void)
{
    return calloc(1, sizeof(struct Entry_type));
}

static void Into_Hash(struct Entry_type *Entry)
{
    /* Find which hash list to insert into and insert */
    /* as first element in the list */
    int i = ((int)(Entry->Name[0]) %HASHSIZE);
    Entry->Previous_def = Hash_dictionary[i];
    Hash_dictionary[i] = Entry;
}

static void Init_hash(void)
{
    int i;
    struct Entry_type *entry;
    for (i = 0; i <HASHSIZE;)
    Hash_dictionary[i++] = NULL;
    /* Add a test constant to the hash table */
    entry = New_entry();
    strcpy(entry->Name,"apa");
    entry->parameter.i = 127;
    entry->Xtoken = 0;
    Into_Hash(entry);
}

static struct Entry_type *Find_in_hash(char *chrstr)
{
    struct Entry_type *entryptr;
    for(entryptr = Hash_dictionary[(int)(*chrstr) %HASHSIZE]; entryptr != NULL; )
    {
        if (strncmp(entryptr->Name,chrstr,3) == 0)
        return entryptr;
        entryptr = entryptr->Previous_def;
    }
    return NULL;
}

static void Word(void)
{
    char temp;
    int i;
    Stack_item temp2;
    /* Discard leading characters blank and tab */
    for (temp = Command_Line[Cmdline_index]; (temp == '\t')||(temp == ' '); temp = Command_Line[++Cmdline_index])
    ;
    for (i = 0; (temp != '\t') &&(temp != ' ') &&(temp != '\0');)
    {
        substring[i++] = temp;
        temp = Command_Line[++Cmdline_index];
    }
    if (i != 0){
        substring[i] = '\0';
        temp2.c_address = &substring[0];
    }
    else
        temp2.c_address = NULL;
    Cpush(temp2);
}

static struct Entry_type *Create(void) /* (--) */
{
    char *tempname;
    struct Entry_type *entry;
    Word(); /* (n - n c_addr) */
    if((tempname = Cpop().c_address) != NULL){ /* */
        entry = New_entry();
        strcpy(entry->Name,tempname);
        /* entry->parameter.data = Datapointer++; */
        entry->parameter.data = Datapointer;
        /* uputi(entry->parameter.i, BASE); */
        entry->Xtoken = 1;
        Into_Hash(entry);
        return entry;
    }
    else
        return NULL;
}

static void allot(void) /* (u -) */
/* Allocate 'u' number of bytes of data memory */
{
    Datapointer += Cpop().n;
}

static void Const_word(void) /* (x -) */
/* Constant <name> takes an integer from the stack and creates a dictionary */
/* entry with the name and the integer */
{
    //char *tempname;
    struct Entry_type *entry;
    
    entry = Create();
    entry->Xtoken = 0;
    entry->parameter.i = Cpop().n;
}

static void Var_word(void)
/* Variable <name> creates a dictionary entry and allocates one cell of data for the variable */
/* 'entry->parameter.data' is a pointer to a Stack_item in which parameter.i stores the data for the variable*/
{
    //char *tempname;
    struct Entry_type *entry;
    
    entry = Create();
    entry->Xtoken = 1;
    entry->parameter.data = Datapointer++; /* Allocate one cell for the variable */
}

static void comma(void)
{
    *Datapointer++ = Cpop();
}

static void Colon(void)
/* Set State flag and enter compilation mode. It creates a new entry under the name that */
/* follows in the command string */
{
    //char *tempname;
    struct Entry_type *entry;
    entry = Create();
    entry->Xtoken = 2;
    entry->parameter.code = New_entry(); /*Add one element to the linked list */
    entry->Previous_def = NULL;
    Compile_entry = entry->parameter.code; /* Set the pointer to the new element */
    STATE = -1;  
}

static void Semicolon(void)
/* Semicolon clear the STATE flag and causes the Forth VM to exit the compilation mode */
/* and return to interpret mode */
{
    STATE = 0;
}

static struct Wordtype {
    const char *abbr;
    void (*funk)(void);
    const char behavior;
}   Dictionary[] =
    {   {"revision", revision_info, 0},
		{"hex", Hex, 0},
        {"dec", DEC, 0},
        {".", Dot, 0},
        /* Stack manipulation words */
        {"drop", Drop, 0},
		{"dup", Dup, 0},
		{"nip", Nip, 0},
        {"over", Over, 0},
		{"swap", Swap, 0},
        /* Return stack manipulation words */
        {">R", To_R, 0},
		{"R>", R_from, 0},
		{"R@", R_fetch, 0},
        /* Single precision math words */
        {"/", Slash, 0},
		{"*", Star, 0},
		{"-", Minus, 0},
		{"+", Plus, 0},
        {"/mod", Slash_mod, 0},
		{"1+", One_plus, 0},
		{"2+", Two_plus, 0},
        {"1-", One_minus, 0},
		{"2-",Two_minus, 0},
		{"2*", Two_star, 0},
		{"2/", Two_slash, 0},
        /* Memory manipulation words */
        {"c!", C_store, 0},
		{"c@", C_fetch, 0},
        {"dump", RAM_dump, 0},
		{"@", Fetch, 0},
		{"!", Store, 0},
        /* Logical and test words */
        {"or", OR_bitwise, 0},
		{"and", AND_bitwise, 0},
        /* Comparison and testing */
        {"0=", Zero_equal, 0},
		{"0<", Zero_less_than, 0},
		{"0>", Zero_greater_than, 0},
        {"0<>", Zero_not_equal, 0},
		{"<", Less_than, 0},
		{"=", Equal, 0},
        /* Memory manipulation words */
        {"allot", allot, 0},
		{"constant", Const_word, 0},
		{"variable", Var_word, 0},
        {"cell+", Two_plus, 0},
		{",", comma, 0},
		{"c,", comma, 0},
		{"here", Here, 0},
        /* Defining (compilation ) words */
        {":", Colon, 0},
		{";", Semicolon, 3},
        /* Words defined in HW_words.c */
        HW_INTO_DICTIONARY
        /*Words that are not fully or incorrectly implemented */
        //{"create", Create, 0}
    };
    
static struct Wordtype *Find_in_dict(char *chrstr)
{
    struct Wordtype *wordptr;
    for (wordptr = Dictionary; wordptr <(Dictionary+(sizeof(Dictionary)/sizeof(Dictionary[0])));wordptr++)
        if (strncmp(wordptr->abbr,chrstr,3) == 0)
            return wordptr;
    return NULL;
}

void Forth_interpreter(void)
{
    static unsigned char Forth_state = 0;
    //char temp;
    //char *scrathpad, *c_address;
    struct Wordtype *Current_word;
    struct Entry_type *Current_entry;
    unsigned char i;
    static int Error_no = 0;
    Stack_item temp2;
	unsigned char uc_char;
    //uint8_t uc_char;
    //uint8_t uc_flag;
    
    switch (Forth_state){
    case 0:
        /* Start the interpreter with version info */
        revision_info();
        printf("\n\rForth Cortex M4 v0.37b 2013-11-25\n\r");
        printf("Kernel v0.38 2014-01-06\n\r\n");
        printf("\n\r");
        Init_hash();
        Forth_state = 1;
    break;
    case 1:
        /* Forth_state1; Reset the text input */
        Cmdline_index = 0;
        Forth_state = 2;
    break;
    case 2:
        /* Forth_state2; Compile the command line string */
        uc_char = getchar();
        //uc_char = 0;
        //uc_flag = uart_read(CONSOLE_UART, &uc_char);
        //if (!uc_flag) {
        if (uc_char) {
            cpu_irq_disable();
            switch ((char)uc_char){
            case '\r':
            case '\n':
                if (Cmdline_index != 0){
                    Command_Line[Cmdline_index] = '\0';
                    Cmdline_index = 0;
                    Forth_state = 3;
                }
                printf("\n\r");         
            break;
            case '\b':
                if (Cmdline_index != 0)
                    Cmdline_index--;
                    printf("\b \b");
            break;          
            default: /* Echo character and insert into command string */
                putchar(uc_char);
                Command_Line[Cmdline_index++] = (char)uc_char;
                Cmdline_index %= CMD_LENGTH;
            break;
            }
            cpu_irq_enable();
        }                       
    break;
    case 3:
        /* Forth_state3; The command line in parsed and the different words are looked up in the dictionaries */
        if (STATE == 0){ /* Compilation flag STATE == 0 means NOT in compilation mode */
            Word();
            if (Cpop().c_address == NULL) /* NULL indicates the end of the command line */
                Forth_state = 4;
            else{
                /* Start with the hash dictionary for new word definitions */
                if ((Current_entry = Find_in_hash(substring)) != NULL)
                    switch (Current_entry->Xtoken){
                    case 1:
                        /* Found a variable in 'heap'. Push a stack item that contains */
                        /* the address to the variable */
                        temp2.a_address = (int *)(&(Current_entry->parameter));
#ifdef DEBUG_PRINT                      
                        printf("Address to variable: %lx \n\r", (uint32_t)temp2.a_address);
#endif
                        Cpush(temp2);
                    break;
                    default: /* xt 0 */
                        temp2.n = Current_entry->parameter.i;
                        /* uputi(temp2.n, BASE); */
                        Cpush(temp2);
                    break;
                    }
                else
                    if ((Current_word = Find_in_dict(substring)) != NULL)
                        switch (Current_word->behavior){
                        case 1: /* Behaviour 1 is not used */
                            //temp2.a_address = (int)(Current_word->behavior);
                        break;
                        case 6: /* Behavior 6 is not used */
                        
                        break;
                        default: /* Behaviour 0 */
                            Current_word->funk();
                        break;
                        }
                    else{
                        temp2.n = Get_number(substring, BASE, &Error_no);
                        Cpush(temp2);
                    }       
            }
        }           
        else /* STATE == -1 i.e VM is in compilation mode */
            Forth_state = 5;                     
    break;
    case 4:
        /* Forth_state4; Show the depth of the stack with N dots followed by "ok"*/
        for (i = Cdepth(); i>0; i--)
            putchar('.');
        printf(" ok \n\r\n\r");
        Forth_state = 1;
    break;
    case 5:
        /* Forth_state5; The virtual machine is now in compile mode. Compile each word into an */
        /* element of a single linked list */
        Word();
        if ((Current_entry = Find_in_hash(substring)) != NULL){
            Compile_entry->parameter.code = Current_entry;
            Compile_entry->Xtoken = Current_entry->Xtoken;
        }
        else{
            if ((Current_word = Find_in_dict(substring)) != NULL){
                if (Current_word->behavior == 3){ /* Semicolon => Stop compiling */
                    Forth_state = 4;
                }
                else{                   
                    Compile_entry->parameter.funk = Current_word->funk;
                    Compile_entry->Xtoken = 0;
                }               
            }
            else{
                Compile_entry->parameter.i = Get_number(substring, BASE, &Error_no);
                Compile_entry->Xtoken = 4;
            }
        }
        Compile_entry->Previous_def = New_entry();
    break;
    case 6:
        /* Empty */
    break;
    default:
        /* The virtual machine exits compile mode and enters interpret mode */
    break;
    }   
}
