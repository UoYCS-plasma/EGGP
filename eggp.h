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
#include "common_functions.h"

//Compiled P-GP2 Programs
#include "GenerateIndividual.h"

typedef struct Result {
    int generation;
    Graph* final_graph;
    double final_score;
} Result;

typedef struct Dataset {
    int inputs;
    int outputs;
    int rows;
    double** data;
} Dataset;

typedef struct Params {
    FunctionSet* functionSet;
    int nodes;
    int inputs;
    int outputs;
    int lambda;
    int maxgens;
    bool debug;
    int runs;
    int neutral_mutations;
    double mutation_rate;
    double target_min_error;
    bool size_pressure;
    int permitted_size;
    int mutations;
    int n;
} Params;

Params* default_params(char* fset, Dataset* dataset);
void default_mutate_individual(Graph* host_graph, Params* params);
void default_neutral_mutation(Graph* host_graph, Params* params);
double default_evaluate_individual(Graph* host, Dataset* dataset, Params* params);
double default_max_error(Graph* host, Dataset* dataset, Params* params);
Graph* default_generate_individual(Params* params);
Dataset* loadDataSet(char* file, int inputs, int outputs, int rows);
Result* one_plus_lambda(Dataset* dataset, Params* params);
Result* one_plus_lambda_multi(Dataset* dataset, Params* params);
Result* n_plus_lambda(Dataset* dataset, Params* params);
Result* n_plus_lambda_multi(Dataset* dataset, Params* params);
