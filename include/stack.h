/*-------------------------------------------------------*/
/*  This file contains the declarations used in the 
    implementation and the use of different stacks 
    used in the program.
*/
/*-------------------------------------------------------*/

#ifndef STACK_H
#define STACK_H

struct Node_D
{
    double data;
    struct Node_D *next;
};

struct Node_CH
{
    char data;
    struct Node_CH *next;
};

struct Node_STR
{
    char *data;
    struct Node_STR *next;
};

typedef struct Node_D *Stack_D;
typedef struct Node_CH *Stack_CH;
typedef struct Node_STR *Stack_STR;

extern int status_err;
extern Stack_STR s;


/*-------------------------------------------------------*/

/* 
    Function declarations to handle the operand stack 
    used for evaluating the RPN expression
*/

struct Node_D *create_stack_D(void);
void push_D(double value, Stack_D s);
int is_empty_D(Stack_D s);
double pop_D(Stack_D s);

/* Used only for debugging purposes (to use the function
uncomment the next line) */
// void print_D(Stack_D s);


/*-------------------------------------------------------*/

/* 
    Function declarations to handle the operator stack 
    used in the Shunting-Yard Algorithm
*/

struct Node_CH *create_stack_CH(void);
void push_CH(char value, Stack_CH s);
int is_empty_CH(Stack_CH s);
char pop_CH(Stack_CH s);
char peek_CH(Stack_CH s);

/* Used only for debugging purposes (to use the function
uncomment the next line) */
// void print_ch(Stack_CH s);


/*-------------------------------------------------------*/

/* 
    Function declarations to handle the expression history stack
*/

void push_STR(const char *val, Stack_STR *s);
int is_empty_STR(Stack_STR *s);
char *pop_STR(Stack_STR *s);

#endif