#ifndef EA_H
#define EA_H

//C Libraries
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

//P-GP2 Libraries
#include "common.h"
#include "debug.h"
#include "graph.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

typedef struct EAArgs {
  Graph** (*initialisation)(uintptr_t init_env_pointer);
  uintptr_t init_env_pointer;
  double* (*evaluate)(Graph** population, uintptr_t evaluation_env_pointer);
  uintptr_t evaluation_env_pointer;
  Graph** (*select_repopulate)(Graph** population, double* scores, uintptr_t select_repopulate_env_pointer);
  uintptr_t select_repopulate_env_pointer;
  bool (*termination)(Graph** population, double* scores, uintptr_t termination_env_pointer);
  uintptr_t termination_env_pointer;
  int (*pop_size)(Graph** population, uintptr_t pop_size_env_pointer);
  uintptr_t pop_size_env_pointer;
  bool maximise;
  int update;
  int generations;
} EAArgs;

typedef struct Result {
  double winning_score;
  Graph* winning_graph;
  int generation;
  bool terminated;
} Result;

Result** run_multi(EAArgs** args, int runs);
Result* run_EA(EAArgs* args);

#endif
