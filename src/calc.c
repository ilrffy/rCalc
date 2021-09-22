/*-------------------------------------------------------*/
/*  This file contains the implementation of the Shunting
    -Yard Algorithm used in the conversion from the infix
    expression entred in the calculator entry, to a postfix
    notation (RPN). It also contains the evaluation of the 
    converted expression and the handling of syntax errors. 
*/
/*-------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>

#include <math.h>
#include <limits.h>

#include "calc.h"
#include "stack.h"

int status_err;

// Array Output (contains the RPN expression):
struct
{
    enum
    {
        is_double,
        is_char
    } type;
    union
    {
        double dval;
        char cval;
    } val;
}arr[MAX_SIZE];


// Check the precedence of the operators:
int op_precedence(char op)
{
    if (op == '^')
        return 3;
    else if (op == '*' || op == '/')
        return 2;
    else if (op == '+' || op == '-')
        return 1;
    else
        return -1;
}


// Check if token is an operator:
int is_operator(char token)
{
    return (token == '+' || token == '-' || token == '*' || token == '/' || token == '^');
}


// Check if token is an number:
int is_number(const char *p)
{
    if (*p == '-')
        p++;    // The '-' is unary 
    if (*p == '.')
        p++;    // Numbers as ".3" are recognized
    return (isdigit((unsigned char) *p));
}

// Check if the expression contains a possible syntax error:
int check_syntax(double val)
{
    return (isnan(val) && status_err == -1) ? 1 : 0;
}


// Computation of the entred expression:
double calculate(const char *math_exp, int *status)
{
    setlocale(LC_NUMERIC ,"C");     // The comma operator ',' is not recognized

    const char *ptr = math_exp;

    Stack_D stack_d = create_stack_D();     // Operand Stack
    Stack_CH stack_ch = create_stack_CH();  // Operator Stack

    int j = 0; // For counting the tokens

    // Spaces are recongnized and skipped:
    while (isspace((unsigned char)*ptr)) ptr++;

    // While there are tokens:
    while (*ptr)
    {
        // If token is a number:
        if (is_number(ptr))
        {
            double num = 0;
            char *end;
            num = strtod(ptr, &end);
            if (ptr == end)
            {
                printf("%s\n", "error : Illegal number !");
                ptr++;
            }
            else
            {
                /*  If the number is preceded or followed by a letter,
                    and preceded by a right parenthesis or followed 
                    by a left parenthesis, handle the error:
                    ex : "3sqrt(9)" or "4(6)" are set as syntax error.
                */
                _Bool con1 = isalpha(*(ptr - 1)) || isalpha(*end);
                _Bool con2 = (*(ptr - 1) == ')') || (*(end) == '(');
                if (con1 || con2)
                {
                    goto OUT;  
                }
                
                // Put the number into the output array:
                arr[j].type = is_double;
                arr[j].val.dval = num;
                j++;
                ptr = end;
            }
        }
        else
        {
            // If token is an operator, a parenthesis or a function or everything else:

            int c = *ptr++;

            // Compare token with the ASCII table values:
            _Bool condition_1 = (((c >= 33) && (c <= 39)) || (c == 44) || ((c >= 58) && (c <= 93))
                                || ((c >= 95) && (c <= 126))) 
                                && (c != 's') && (c != 'q') && (c != 'r') && (c != 't');

            if (is_operator(c))
            {
                char cl = peek_CH(stack_ch);
                
                while (!is_empty_CH(stack_ch) && ((op_precedence(cl) >= op_precedence(c))))
                {
                    if (cl == '\0')
                    {
                        goto OUT;  
                    }
                    arr[j].type = is_char;
                    arr[j].val.cval = pop_CH(stack_ch);
                    j++;
                }
                push_CH(c, stack_ch);
            }
            else if (c == 's')  
            {
                // If token is the "sqrt(" function:
                if ((*ptr == 'q') && (*(ptr + 1) == 'r') && (*(ptr + 2) == 't') && (*(ptr + 3) == '('))
                {
                    push_CH(c, stack_ch);
                }
            }
            else if (c == '(')
            {
                push_CH(c, stack_ch);
            }
            else if (c == ')')
            {
                char cl = peek_CH(stack_ch);
        
                if (cl == '\0')
                {
                    goto OUT;   
                }
                while (!is_empty_CH(stack_ch))
                {
                    arr[j].type = is_char;
                    arr[j].val.cval = pop_CH(stack_ch);
                    j++;
                }
                pop_CH(stack_ch);
            } 
            else if (condition_1)   // If the expression contains a forbbiden character:
            {
                goto OUT;   
            }
        }

        while (isspace((unsigned char)*ptr)) ptr++;
    }

    while (!is_empty_CH(stack_ch))
    {
        char cl = peek_CH(stack_ch);
        
        if (cl == '(')
        {
            goto OUT;   
        }
        arr[j].type = is_char;
        arr[j].val.cval = pop_CH(stack_ch);
        j++;
    }

    // Used only for debugging purposes:
    // (Print the postfix expression after conversion from infix)
    /*
    for (int i = 0; i < j; i++)
    {
        switch (arr[i].type)
        {
            case is_double : printf("%lf ", arr[i].val.dval); break;
            case is_char: printf("%c ", arr[i].val.cval); break;
        }  
    }
    putchar('\n');
    */

    double num2 = 0;
    double num1 = 0;
    double result = 0;
    char op = 0;

    // Evaluation of the RPN expression:
    for (int i = 0; i < j; i++)
    {
        switch (arr[i].type)
        {
        case is_double:
            push_D(arr[i].val.dval, stack_d);
            break;
        case is_char:
            op = arr[i].val.cval;
            switch (op)
            {
                case '+':
                    num2 = pop_D(stack_d);
                    num1 = pop_D(stack_d);
                    if (check_syntax(num1))
                    {
                        goto OUT;  
                    } 
                    result = num1 + num2;
                    push_D(result, stack_d);
                    break;

                case '*':
                    num2 = pop_D(stack_d);
                    num1 = pop_D(stack_d);
                    if (check_syntax(num1))
                    {
                        goto OUT;   
                    } 
                    result = num1 * num2;
                    push_D(result, stack_d);
                    break;

                case '/':
                    num2 = pop_D(stack_d);
                    num1 = pop_D(stack_d);
                    if (check_syntax(num1))
                    {
                        goto OUT;
                    } 
                    result = num1 / num2;
                    push_D(result, stack_d);
                    break;
                case '-':
                    num2 = pop_D(stack_d);
                    num1 = pop_D(stack_d);
                    if (check_syntax(num1))
                    {
                        goto OUT;
                    } 
                    result = num1 - num2;
                    push_D(result, stack_d);
                    break;

                case '^':
                    num2 = pop_D(stack_d);
                    num1 = pop_D(stack_d);
                    if (check_syntax(num1))
                    {
                        goto OUT;
                    } 
                    result = pow(num1, num2);
                    push_D(result, stack_d);
                    break;
                case 's':
                    num1 = pop_D(stack_d);
                    result = sqrt(num1);
                    push_D(result, stack_d);
                    break;
            }
            break;
        }
    }
    
    double f_result = pop_D(stack_d);   

    *status = 0;  
    return f_result;

OUT:    // Syntax Error
    *status = -1;   
    return NAN;     
}
