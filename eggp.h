#ifndef EGGP_H
#define EGGP_H

#include <stdint.h>
#include "common_functions.h"
#include "evolutionary_algorithm.h"
#include "generic_operators.h"
#include "utils.h"

#include "graph.h"
#include "common.h"

typedef struct EGGP_init_env{
  Function_Set* fset;
  GP_Dataset* dataset;
  int nodes;
  int pop_size;
} EGGP_init_env;

EGGP_init_env* default_eggp_init_env(GP_Dataset* dataset, Function_Set* fset);
GP_1_plus_lambda_env* default_eggp_select_env(Function_Set* fset);
GP_eval_env* default_eggp_eval_env(GP_Dataset* dataset, Function_Set* fset);
Target_0_env* default_eggp_termination_env();
EAArgs* default_eggp_EAArgs(GP_Dataset* dataset, Function_Set* fset);
Fixed_pop_env* default_eggp_pop_size_env();

Graph** eggp_init(uintptr_t env_pointer);
Graph* eggp_mutate(Graph* host, Function_Set* fset, double mutation_rate);

#endif
