#ifndef COMFUN_H
#define COMFUN_H

//C Libraries
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

//P-GP2 Libraries
#include "common.h"
#include "debug.h"
#include "graph.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

typedef struct Function {
    string name;
    int arity;
    double (*func)(double* inputs);
    struct Function* next;
} Function;

typedef struct Function_Set {
    int functionCount;
    struct Function* first;
    struct Function* last;
} Function_Set;

int get_max_arity(Function_Set* fset);
void print_fset(Function_Set* fsetV);
Function_Set* get_common_fset(string fsetV);
void add_function(Function_Set* fset, string name, int arity, double (*func)(double* inputs));
Function* get_function(Function_Set* fset, string name);
void freeFset(Function_Set* fset);
double common_and(double* inputs);
double common_or(double* inputs);
double common_nand(double* inputs);
double common_nor(double* inputs);
double common_xor(double* inputs);
double common_not(double* inputs);
double common_add(double* inputs);
double common_sub(double* inputs);
double common_div(double* inputs);
double common_mul(double* inputs);
double common_id(double* inputs);
double common_sin(double* inputs);
double common_log(double* inputs);
double common_cos(double* inputs);
double common_exp(double* inputs);

#endif
