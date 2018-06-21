#ifndef UTILS_H
#define UTILS_H

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

//Util graph marking functions
void mark_active_blue(Graph* hostG);
void mark_active_children_blue(Graph* hostG, Node *node);
void unmark_graph(Graph* hostG);
//Counts the number of nodes connected to an output in a computational network
int count_active_nodes(Graph* hostG, int inputs, int outputs);
//Counts the number of edges connected to an output in a computational network
int count_active_edges(Graph* hostG);
//Copies 2 graphs into 1 graph, marking the first argument red and the second argument mark_active_blue
Graph* disjoint_union(Graph* red, Graph* blue);
//Returns the red component of a graph
Graph* get_red(Graph* red_blue);
//Returns the blue component of a graph
Graph* get_blue(Graph* red_blue);
//Returns the argument mark component of a graph, marked the target_mark argument.
Graph* get_mark(Graph* multi_mark, int mark, int target_mark);
//Duplicates a graph
Graph* duplicate_graph(Graph *graph);
Graph* build_empty_host_graph();

void free_graph_array(Graph** array, int graphs);
void free_graph_data(Graph* graph);
int random_int(int min, int max);
#endif
