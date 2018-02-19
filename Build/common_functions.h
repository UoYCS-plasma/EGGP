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

typedef struct FunctionSet {
    int functionCount;
    Function* first;
    Function* last;
} FunctionSet;

FunctionSet* get_common_fset(string fsetV);
void add_function(FunctionSet* functionSet, string name, int arity, double (*func)(double* inputs));
Function* get_function(FunctionSet* functionSet, string name);
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
