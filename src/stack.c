/*-------------------------------------------------------*/
/*  This file contains the definitions of the
    functions used in the creation and the use of 
    of different stacks used in the program.
    Stacks are implemented using linked lists.
*/
/*-------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include "stack.h"

int status_err;     /* Used in the double data type stack,
                    equals to -1 if the stack is empty, 
                    otherwise 0. */


/*-------------------------------------------------------*/
//  Function definitions of the operand stack:


struct Node_D *create_stack_D(void)
{
    struct Node_D *node;
    node = malloc(sizeof(struct Node_D));

    if (node == NULL)
    {
        printf("%s\n", "error : cannot create stack");
        exit(EXIT_FAILURE);
    }

    node->next = NULL;

    return node;
}

void push_D(double value, Stack_D s)
{
    struct Node_D *new_node;
    new_node = malloc(sizeof(struct Node_D));

    if (new_node == NULL)
    {
        printf("%s\n", "error : out of memory");
        exit(EXIT_FAILURE);
    }

    new_node->data = value;

    new_node->next = s->next;
    s->next = new_node;
}

int is_empty_D(Stack_D s)
{
    return s->next == NULL;
}

double pop_D(Stack_D s)
{
    double popped_item;
    struct Node_D *node;

    if (is_empty_D(s))
    {
        status_err = -1;
        return NAN;
    }

    node = s->next;

    popped_item = node->data;
    s->next = node->next;

    free(node);

    status_err = 0;
    return popped_item;
}

// Used only for debugging purposes:
/* 
void print_D(Stack_D s)
{
    struct Node_D *e = s->next;
    while (e != NULL)
    {
        printf("%lf ", e->data);
        e = e->next;
    }
    putchar('\n');
}
*/


/*-------------------------------------------------------*/
//  Function definitions of the operator stack :


struct Node_CH *create_stack_CH(void)
{
    struct Node_CH *node;
    node = malloc(sizeof(struct Node_CH));

    if (node == NULL)
    {
        printf("%s\n", "error : cannot create stack");
        exit(EXIT_FAILURE);
    }

    node->next = NULL;

    return node;
}

void push_CH(char value, Stack_CH s)
{
    struct Node_CH *new_node;
    new_node = malloc(sizeof(struct Node_CH));

    if (new_node == NULL)
    {
        printf("%s\n", "error : out of memory");
        exit(EXIT_FAILURE);
    }

    new_node->data = value;

    new_node->next = s->next;
    s->next = new_node;
}

int is_empty_CH(Stack_CH s)
{
    return s->next == NULL;
}

char pop_CH(Stack_CH s)
{
    char popped_item;
    struct Node_CH *node;

    if (is_empty_CH(s))
    {
        return '\0';
    }

    node = s->next;

    popped_item = node->data;
    s->next = node->next;

    free(node);

    return popped_item;
}

char peek_CH(Stack_CH s)
{
    char peeked_item;
    struct Node_CH *node;

    if (is_empty_CH(s))
    {
        return '\0';
    }
    node = s->next;
    peeked_item = node->data;

    return peeked_item;
}


// Used only for debugging purposes:
/* 
void print_CH(Stack_CH s)
{
    struct Node_CH *e = s->next;
    while (e != NULL)
    {
        printf("%c ", e->data);
        e = e->next;
    }
    putchar('\n');
}
*/


/*-------------------------------------------------------*/
//  Function definitions of the expression history stack:


void push_STR(const char *val, Stack_STR *s)
{
    struct Node_STR *new_node;
    new_node = malloc(sizeof(struct Node_STR));

    if (new_node == NULL)
    {
        printf("%s\n", "error : out of memory");
        exit(EXIT_FAILURE);
    }
    
    new_node->data = malloc(strlen(val) + 1);
    strcpy(new_node->data, val);

    new_node->next = *s;
    *s = new_node;
}

int is_empty_STR(Stack_STR *s)
{
    return *s == NULL;
}   

char *pop_STR(Stack_STR *s)
{
    struct Node_STR *node;
    static char *popped_item;
    
    if (is_empty_STR(s))       // No undo history
    {
        return NULL;
    }

    popped_item = (*s)->data;

    node = *s;
    *s = node->next;

    free(node);

    return popped_item;
}