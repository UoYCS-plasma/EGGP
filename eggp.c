#include "eggp.h"

//P-GP2 Libraries
#include "common.h"
#include "debug.h"
#include "graph.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

//C Libraries
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

//Compiled P-GP2 Programs
#include "GenerateIndividual.h"
#include "MutateFeedForwardEdge.h"
#include "MutateNode.h"

//Local function headers
static Graph* duplicateGraph(Graph* host);
static Graph* buildEmptyHostGraph();
static void markActiveBlue(Graph* hostG);
static void markActiveChildrenBlue(Graph* hostG, Node *node);
static void unmarkGraph(Graph* hostG);
static int countActiveNodes(Graph* hostG, Params* params);

Graph* host = NULL;
int* node_map = NULL;

static int countActiveNodes(Graph* hostG, Params* params){
	markActiveBlue(hostG);
	int count = 0;
	for(int host_index = 0; host_index < hostG->nodes.size; host_index++)
	{
		 Node *host_node = getNode(hostG, host_index);
		 if(host_node == NULL || host_node->index == -1) continue;
		 HostLabel label = host_node->label;
		 if(label.mark > 0){
		 	count++;
		 }
	}
	unmarkGraph(hostG);
	if(count < (params->inputs + params->outputs)){
		return 0;
	}
	return count - (params->inputs + params->outputs);
}


Params* default_params(char* fset, Dataset* dataset){
  Params* params = malloc(sizeof(Params));
  params->inputs = dataset->inputs;
  params->outputs = dataset->outputs;
  params->mutation_rate = 0.01;
  params->lambda = 4;
  params->maxgens = 20000000;
  params->runs = 100;
  params->debug = true;
  params->nodes = 100;
  params->neutral_mutations = 0;
  params->functionSet = get_common_fset(fset);
	params->size_pressure = false;
	params->permitted_size = 100;
	params->mutations = -1;
  return params;
}

Result* one_plus_lambda(Dataset* dataset, Params* params){

    Graph* parent = default_generate_individual(params);
    double parentScore = default_evaluate_individual(parent, dataset, params);
    int gen = 0;
		double scores[params->lambda];
    while(gen < params->maxgens && default_max_error(parent, dataset, params) > params->target_min_error){
      Graph* newparent = duplicateGraph(parent);
			double reval = default_evaluate_individual(newparent, dataset, params);
			if(reval != parentScore){
				printf("\n\n\n WARNING Reval (%lf) != Score (%lf)\n\n\n", reval, parentScore);
				printfGraph(newparent);
			}
      if(params->debug){
        printf("Generation %d, score %lf, active nodes %d", gen, parentScore, countActiveNodes(parent, params));
      }
			if(countActiveNodes(parent, params) < 0){
				printfGraph(parent);
			}
			if(gen > 0){
				for(int i = 0; i < params->lambda; i++){
					if(scores[i] < 99999.9){
						printf(" %lf,", scores[i]);
					}
					else{
						printf(" Big Number!,");
					}
				}
				printf("                                                                                                        \r");
			}
			else{
				printf("\r");
			}
      gen = gen + 1;
      for(int i = 0; i < params->lambda; i++){
        Graph* child = duplicateGraph(parent);
        default_mutate_individual(child, params);
        double childScore = default_evaluate_individual(child, dataset, params);
				scores[i] = childScore;
        if(childScore <= parentScore + 0.00001 || default_max_error(child, dataset, params) <= params->target_min_error ){
          freeGraph(newparent);
          newparent = child;
          parentScore = childScore;
        }
        else{
          freeGraph(child);
        }
      }
      freeGraph(parent);
      parent = newparent;
    }
    if(params->debug){
      printf("\r");
    }
    Result* res = malloc(sizeof(Result));
    res->generation = gen;
    res->final_graph = parent;
    res->final_score = parentScore;
    return res;
}


Result* one_plus_lambda_multi(Dataset* dataset, Params* params){
    int gen_buffer = floor(log10(abs(params->maxgens))) + 1;
    char* gen_buffer_s = malloc((gen_buffer - 2) * sizeof(char));
    for(int j = 0; j < gen_buffer - 1; j++){
      gen_buffer_s[j] = ' ';
    }
    gen_buffer_s[gen_buffer - 1] = '\0';
    printf("                                                                                        \r");
    printf("Gen%s | Final Score | Active Nodes\n", gen_buffer_s);
    for(int i = 0; i < params->runs; i++){
         Result* result = one_plus_lambda(dataset, params);
        // free(gen_buffer_s);
        // int gen_digits = floor(log10(abs(result->generation))) + 1;
        // gen_buffer_s = malloc((gen_buffer - (gen_digits)) * sizeof(char));
        // for(int j = 0; j < gen_buffer - (gen_digits - 1); j++){
        //   gen_buffer_s[j] = ' ';
        // }
        // gen_buffer_s[gen_buffer - (gen_digits)] = '\0';
        printf("                                                                                        \r");
        printf("%d                      , %lf,      %d                                                                       \n", result->generation, result->final_score, countActiveNodes(result->final_graph, params));
    }
    return NULL;
}



Result* n_plus_lambda(Dataset* dataset, Params* params){

    Graph* parents[params->n];
		double parentScore[params->n];
		for(int i = 0; i < params->n; i++){
			parents[i] = default_generate_individual(params);
			parentScore[i] = default_evaluate_individual(parents[i], dataset, params);
		}
    int gen = 0;
		bool success = false;
		Graph* winner = NULL;
		double winningScore;

    while(gen < params->maxgens && !success){
			printf("Generation %d scores:", gen);
			for(int i = 0; i < params->n; i++){
	      Graph* newparent = duplicateGraph(parents[i]);
				double reval = default_evaluate_individual(newparent, dataset, params);
				if(reval != parentScore[i]){
					printf("\n\n\n WARNING Reval (%lf) != Score (%lf)\n\n\n", reval, parentScore[i]);
					printfGraph(newparent);
				}
	      if(params->debug){
	        printf("[ %lf, active nodes %d ] ", parentScore[i], countActiveNodes(parents[i], params));
	      }
				if(countActiveNodes(parents[i], params) < 0){
					printfGraph(parents[i]);
				}
				if(gen > 0){
					printf("                                                                                                        \r");
				}
				else{
					printf("\r");
				}
	      gen = gen + 1;
	      for(int i = 0; i < params->lambda; i++){
	        Graph* child = duplicateGraph(parents[i]);
	        default_mutate_individual(child, params);
	        double childScore = default_evaluate_individual(child, dataset, params);
	        if(childScore <= parentScore[i] + 0.00001 || default_max_error(child, dataset, params) <= params->target_min_error ){
	          freeGraph(newparent);
	          newparent = child;
	          parentScore[i] = childScore;
	        }
	        else{
	          freeGraph(child);
	        }
	      }
	      freeGraph(parents[i]);
	      parents[i] = newparent;
				if(default_max_error(parents[i], dataset, params)){
					success = true;
					winner = parents[i];
					winningScore = parentScore[i];
				}
			}
    }
    if(params->debug){
      printf("\r");
    }
    Result* res = malloc(sizeof(Result));
    res->generation = gen;
    res->final_graph = duplicateGraph(winner);
    res->final_score = winningScore;
    return res;
}


Result* n_plus_lambda_multi(Dataset* dataset, Params* params){
    int gen_buffer = floor(log10(abs(params->maxgens))) + 1;
    char* gen_buffer_s = malloc((gen_buffer - 2) * sizeof(char));
    for(int j = 0; j < gen_buffer - 1; j++){
      gen_buffer_s[j] = ' ';
    }
    gen_buffer_s[gen_buffer - 1] = '\0';
    printf("                                                                                        \r");
    printf("Gen%s | Final Score | Active Nodes\n", gen_buffer_s);
    for(int i = 0; i < params->runs; i++){
         Result* result = n_plus_lambda(dataset, params);
        // free(gen_buffer_s);
        // int gen_digits = floor(log10(abs(result->generation))) + 1;
        // gen_buffer_s = malloc((gen_buffer - (gen_digits)) * sizeof(char));
        // for(int j = 0; j < gen_buffer - (gen_digits - 1); j++){
        //   gen_buffer_s[j] = ' ';
        // }
        // gen_buffer_s[gen_buffer - (gen_digits)] = '\0';
        printf("                                                                                        \r");
        printf("%d                      , %lf,      %d                                                                       \n", result->generation, result->final_score, countActiveNodes(result->final_graph, params));
    }
    return NULL;
}

//Graph duplication function
static Graph* duplicateGraph(Graph *graph)
{
   Graph *graph_copy = newGraph(100000, 100000);
	 int node_mapping[graph->nodes.size];
	 //Copy nodes
	 for(int i = 0; i < graph->nodes.size; i++){
			Node *host_node = getNode(graph, i);
			if(host_node == NULL || host_node->index == -1) continue;
			node_mapping[i] = addNode(graph_copy, host_node->root, makeHostLabel(host_node->label.mark, host_node->label.length, copyHostList(host_node->label.list)));
	 }
	 //Copy edges
	 for(int i = 0; i < graph->edges.size; i++){
			Edge *host_edge = getEdge(graph, i);
			if(host_edge == NULL || host_edge->index == -1) continue;
			addEdge(graph_copy, makeHostLabel(host_edge->label.mark, host_edge->label.length, copyHostList(host_edge->label.list)), node_mapping[host_edge->source], node_mapping[host_edge->target]);
	 }
	 return graph_copy;
}

Dataset* loadDataSet(char* file, int inputs, int outputs, int rows){
  Dataset* dataset = malloc(sizeof(Dataset));
  dataset->inputs = inputs;
  dataset->outputs = outputs;
  dataset->rows = rows;
  dataset->data = malloc((rows * (inputs + outputs)) * sizeof(double));
  double * raw[rows];
  for(int i = 0; i < rows; i++){
    dataset->data[i] = malloc((inputs + outputs) * sizeof(double));
    for(int j = 0; j < inputs + outputs; j++){
      dataset->data[i][j] = -1.0;
    }
  }
  FILE* fp = fopen(file, "r");
  if(fp == NULL){
      printf("Error: file %s cannot be found.\n", file);
      exit(0);
  }
  for (int i = 0; i < rows; i++){
    for(int j = 0; j < (inputs + outputs); j++){
      int r = fscanf(fp," %lf %*c",&dataset->data[i][j]);
      if(r != 1){
        printf("Error: file %s does not match the given row and columns dimensions.\n", file);
        exit(0);
      }
    }
  }
	printf("Rows: %d\n", rows);
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < inputs + outputs; j++){
      printf("%lf, ", dataset->data[i][j]);
    }
    printf("\n");
  }
  return dataset;
}

static Graph *buildEmptyHostGraph()
{
   Graph* new_host = newGraph(100000, 100000);
   node_map = calloc(100000, sizeof(int));
   if(node_map == NULL)
   {
      freeGraph(new_host);
      return NULL;
   }
   return new_host;
}

static void markActiveBlue(Graph* hostG){

	for(int host_index = 0; host_index < hostG->nodes.size; host_index++)
	{
		 Node *host_node = getNode(hostG, host_index);
		 if(host_node == NULL || host_node->index == -1) continue;

		 HostLabel label = host_node->label;
		 if(label.length != 2) break;

		 HostListItem *item = label.list->first;
		 HostListItem *item2 = item->next;
		 if(item->atom.type != 'i') break;
		 if(item2->atom.type != 's') break;
		 if(strcmp(item2->atom.str, "OUT") == 0){
	   	 changeNodeMark(hostG, host_index, 3);
			 markActiveChildrenBlue(hostG, host_node);
		 }
	}
}

static void markActiveChildrenBlue(Graph* hostG, Node *node){
	int counter;
	for(counter = 0; counter < node->out_edges.size + 2; counter++)
	{
		Edge *host_edge = getNthOutEdge(hostG, node, counter);
		if(host_edge == NULL || host_edge->index == -1) continue;
		Node *target = getNode(hostG, host_edge->target);
		HostLabel label = target->label;
		if(label.mark == 0){
 	   	changeNodeMark(hostG, target->index, 3);
			markActiveChildrenBlue(hostG, target);
		}
	}
}

static void unmarkGraph(Graph* hostG){

		for(int host_index = 0; host_index < hostG->nodes.size; host_index++)
		{
			 Node *host_node = getNode(hostG, host_index);
			 if(host_node == NULL || host_node->index == -1) continue;
			 if(host_node->label.mark != 0){
		   	changeNodeMark(hostG, host_index, 0);
				if(host_node->root){
					host_node->root = false;
				}
			}
		}
}

Graph* default_generate_individual(Params* params)
{
   int inputs = params->inputs;
   int outputs = params->outputs;
   int nodes = params->nodes;
   FunctionSet* fSet = params->functionSet;
   host = buildEmptyHostGraph();

   /* Initialise host graph */
   /* INPUTS */
   for(int i = 0; i < inputs; i++){
     HostAtom array[2];
     HostLabel label;
     array[0].type = 'i';
     array[0].num = i;
     array[1].type = 's';
     array[1].str = "IN";
     HostList *list = makeHostList(array, 2, false);
     label = makeHostLabel(0, 2, list);
     addNode(host, 0, label);
   }

   /* OUTPUTS */
   for(int i = 0; i < outputs; i++){
     HostAtom array[2];
     HostLabel label;
     array[0].type = 'i';
     array[0].num = i;
     array[1].type = 's';
     array[1].str = "OUT";
     HostList *list = makeHostList(array, 2, false);
     label = makeHostLabel(0, 2, list);
     addNode(host, 0, label);
   }

   /* FUNCTIONSET */
   Function* f = fSet->first;
   for(int i = 0; i < fSet->functionCount; i++){
      HostAtom array[3];
      HostLabel label;
      array[0].type = 's';
      array[0].str = "FunctionSet";
      array[1].type = 's';
      array[1].str = strdup(f->name);
      array[2].type = 'i';
      array[2].num = f->arity;
      HostList *list = makeHostList(array, 3, false);
      label = makeHostLabel(0, 3, list);
      addNode(host, 1, label);
      f = f->next;
   }

   /* NODES */
    HostAtom array[2];
    HostLabel label;
    array[0].type = 's';
    array[0].str = "Nodes";
    array[1].type = 'i';
    array[1].num = nodes;
    HostList *list = makeHostList(array, 2, false);
    label = makeHostLabel(0, 2, list);
    addNode(host, 1, label);

    GenerateIndividual_execute(host);


     /* REMOVE FUNCTION SET AND NODES COUNTER. These are the only rooted nodes. */
     for(int i = 0; i < host->nodes.size; i++){
        Node *host_node = getNode(host, i);
      if(host_node == NULL) continue;
      if(!host_node->root) continue;
      removeNode(host, i);
     }

    return host;
}

void default_mutate_individual(Graph* host_graph, Params* params){

   int nodes = params->nodes;
   FunctionSet* fSet = params->functionSet;
   double mutation_rate = params->mutation_rate;

  /* FUNCTIONSET */
  Function* f = fSet->first;
  for(int i = 0; i < fSet->functionCount; i++){
     HostAtom array[3];
     HostLabel label;
     array[0].type = 's';
     array[0].str = "FunctionSet";
     array[1].type = 's';
     array[1].str = strdup(f->name);
     array[2].type = 'i';
     array[2].num = f->arity;
     HostList *list = makeHostList(array, 3, false);
     label = makeHostLabel(0, 3, list);
     addNode(host_graph, 1, label);
     f = f->next;
  }

	bool mutated = false;
	int mutations = 0;
  int num = host_graph->nodes.size + host_graph->edges.size;
	if(params->mutations == -1){
  for(int i = 0; i < num; i++){
	     double r = ((double)rand() / (double)RAND_MAX);
	     if(r <= mutation_rate){
				  mutated = true;
					mutations++;
	        double r2 = ((double)rand() / (double)RAND_MAX);
	        if(r2 <= (double)(host_graph->edges.size) / (double)(num)){
	          MutateFeedForwardEdge_execute(host_graph);
	        }
	        else{
	          MutateNode_execute(host_graph);
	        }
	        unmarkGraph(host_graph);
	     }
	  }
	}
	else{
		for(int i = 0; i < params->mutations; i++){
			double r2 = ((double)rand() / (double)RAND_MAX);
			if(r2 <= (double)(host_graph->edges.size) / (double)(num)){
				MutateFeedForwardEdge_execute(host_graph);
			}
			else{
				MutateNode_execute(host_graph);
			}
			unmarkGraph(host_graph);
				 mutated = true;

		}
	}
	if(!mutated){
			double r2 = ((double)rand() / (double)RAND_MAX);
			if(r2 <= (double)(host_graph->edges.size) / (double)(num)){
				MutateFeedForwardEdge_execute(host_graph);
			}
			else{
				MutateNode_execute(host_graph);
			}
			unmarkGraph(host_graph);
	}
   /* REMOVE FUNCTION SET AND NODES COUNTER. These are the only rooted nodes. */
   for(int i = 0; i < host_graph->nodes.size; i++){
      Node *host_node = getNode(host_graph, i);
    if(host_node == NULL) continue;
    if(!host_node->root) continue;
    removeNode(host_graph, i);
  }
  unmarkGraph(host_graph);
}

void default_neutral_mutation(Graph* host_graph, Params* params){
// 	int num = host_graph->nodes.size + host_graph->edges.size;
//   for(int i = 0; i < params->neutral_mutations; i++){
//      double r = ((double)rand() / (double)RAND_MAX);
//      if(r <= (double)host_graph->edges.size / (double)num){
//        //NeuCircuitGrowth_execute(host_graph);
// 			 NeutralEdge_execute(host_graph);
//      }
//      else{
//        // NeuCircuitShrink_execute(host_graph);
// 			 NeutralNode_execute(host_graph);
//      }
//      unmarkGraph(host_graph);
//   }
 }

double default_evaluate_individual(Graph* host_graph, Dataset* dataset, Params* params){
  FunctionSet* fSet = params->functionSet;
	markActiveBlue(host_graph);

  int rows = dataset->rows;
  int inputs = dataset->inputs;
  int outputs = dataset->outputs;
  int width = host_graph->nodes.size;
  double values[rows][width];
  int ready[width];
  double totalError = 0.0;
  //Find inputs and outputs
  int inputIndex[inputs];
  for(int i = 0; i<inputs; i++){
    inputIndex[i] = -1;
  }
  int outputIndex[outputs];
  for(int j = 0; j<outputs; j++){
    outputIndex[j] = -1;
  }

  for(int host_index = 0; host_index < host_graph->nodes.size; host_index++)
  {
     Node *host_node = getNode(host_graph, host_index);
     if(host_node == NULL || host_node->index == -1) continue;

     HostLabel label = host_node->label;
     if(label.length != 2) break;

     HostListItem *item = label.list->first;
     HostListItem *item2 = item->next;
     if(item->atom.type != 'i') break;
     if(item2->atom.type != 's') break;
     if(strcmp(item2->atom.str, "IN") == 0){
       inputIndex[item->atom.num] = host_index;
     }
     if(strcmp(item2->atom.str, "OUT") == 0){
       outputIndex[item->atom.num] = host_index;
     }
  }

  int queue[host_graph->nodes.size];
  int head = 0;
  int tail = 0;

	for(int x = 0; x < width; x++){
      ready[x] = 0;
	}
  for(int r = 0; r < rows; r++){
    for(int x = 0; x < width; x++){
      values[r][x] = -1.0;
    }
	}
  for(int q = 0; q < width; q++){
    queue[q] = 0;
  }

  //Load inputs
  for(int i = 0; i < inputs; i++){
		for(int r = 0; r < rows; r++){
    	values[r][inputIndex[i]] = (dataset->data)[r][i];
		}
    ready[inputIndex[i]] = 1;
    Node *host_node = getNode(host_graph, inputIndex[i]);
    int counter;
    for(counter = 0; counter < host_node->in_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthInEdge(host_graph, host_node, counter);
      if(host_edge == NULL) continue;
      int source = host_edge->source;
      ready[source]++;
			Node* source_node = getNode(host_graph, source);
			if(source_node->label.mark == 0) continue;
       if(ready[source] >= source_node->outdegree){
        queue[tail] = source;
        tail++;
      }
    }
  }



  while(head < tail){
    int node_index = queue[head];
    head++;
    Node *host_node = getNode(host_graph, node_index);

    HostLabel label = host_node->label;
    HostListItem *item = label.list->last;
    char* function = item->atom.str;

    double in_values[rows][host_node->outdegree];
    int counter;
    for(counter = 0; counter < host_node->out_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthOutEdge(host_graph, host_node, counter);
      if(host_edge == NULL) continue;
      int target = host_edge->target;

			HostLabel label = host_edge->label;
			int vIndex = label.list->first->atom.num;
			for(int r = 0; r < rows; r++){
      	in_values[r][vIndex] = values[r][target];
			}
    }

    if(strcmp(function, "OUT") == 0){
	 	   //printf("OUT function\n");
  		 for(int r = 0; r < rows; r++){
          values[r][node_index] = in_values[r][0];
		// 		//printf("%d: %lf\n", node_index, values[r][node_index]);
	 	   }
     }
     else{
       Function* f = get_function(fSet, function);
  		 for(int r = 0; r < rows; r++){
         values[r][node_index] = f->func(in_values[r]);
       }
     }

    for(counter = 0; counter < host_node->in_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthInEdge(host_graph, host_node, counter);
      if(host_edge == NULL) continue;
      int source = host_edge->source;
      ready[source]++;
			Node* source_node = getNode(host_graph, source);
			if(source_node->label.mark == 0) continue;
      if(ready[source] >= source_node->outdegree){
        queue[tail] = source;
        tail++;
      }
    }
  }

  for(int o = 0; o < outputs; o++){
		for(int r = 0; r < rows; r++){
			//printf("Expected %lf got %lf diff %lf\n", data[r][inputs + o], values[r][outputIndex[o]], fabs(values[r][outputIndex[o]] - data[r][inputs + o]));
    	totalError = totalError + fabs(values[r][outputIndex[o]] - dataset->data[r][inputs + o]);
		}
  }
	unmarkGraph(host_graph);
	if(params->size_pressure == true && countActiveNodes(host_graph, params) >= params->permitted_size){
		totalError = totalError + ((double)countActiveNodes(host_graph, params) / (10.0 * (double)params->nodes));
	}
  return totalError;
}

double default_max_error(Graph* host_graph, Dataset* dataset, Params* params){
  FunctionSet* fSet = params->functionSet;
	markActiveBlue(host_graph);
	double max_error = 0.0;

  int rows = dataset->rows;
  int inputs = dataset->inputs;
  int outputs = dataset->outputs;
  int width = host_graph->nodes.size;
  double values[rows][width];
  int ready[width];
  //Find inputs and outputs
  int inputIndex[inputs];
  for(int i = 0; i<inputs; i++){
    inputIndex[i] = -1;
  }
  int outputIndex[outputs];
  for(int j = 0; j<outputs; j++){
    outputIndex[j] = -1;
  }

  for(int host_index = 0; host_index < host_graph->nodes.size; host_index++)
  {
     Node *host_node = getNode(host_graph, host_index);
     if(host_node == NULL || host_node->index == -1) continue;

     HostLabel label = host_node->label;
     if(label.length != 2) break;

     HostListItem *item = label.list->first;
     HostListItem *item2 = item->next;
     if(item->atom.type != 'i') break;
     if(item2->atom.type != 's') break;
     if(strcmp(item2->atom.str, "IN") == 0){
       inputIndex[item->atom.num] = host_index;
     }
     if(strcmp(item2->atom.str, "OUT") == 0){
       outputIndex[item->atom.num] = host_index;
     }
  }

  int queue[host_graph->nodes.size];
  int head = 0;
  int tail = 0;

	for(int x = 0; x < width; x++){
      ready[x] = 0;
	}
  for(int r = 0; r < rows; r++){
    for(int x = 0; x < width; x++){
      values[r][x] = -1.0;
    }
	}
  for(int q = 0; q < width; q++){
    queue[q] = 0;
  }

  //Load inputs
  for(int i = 0; i < inputs; i++){
		for(int r = 0; r < rows; r++){
    	values[r][inputIndex[i]] = (dataset->data)[r][i];
		}
    ready[inputIndex[i]] = 1;
    Node *host_node = getNode(host_graph, inputIndex[i]);
    int counter;
    for(counter = 0; counter < host_node->in_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthInEdge(host_graph, host_node, counter);
      if(host_edge == NULL) continue;
      int source = host_edge->source;
      ready[source]++;
			Node* source_node = getNode(host_graph, source);
			if(source_node->label.mark == 0) continue;
       if(ready[source] >= source_node->outdegree){
        queue[tail] = source;
        tail++;
      }
    }
  }



  while(head < tail){
    int node_index = queue[head];
    head++;
    Node *host_node = getNode(host_graph, node_index);

    HostLabel label = host_node->label;
    HostListItem *item = label.list->last;
    char* function = item->atom.str;

    double in_values[rows][host_node->outdegree];
    int counter;
    for(counter = 0; counter < host_node->out_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthOutEdge(host_graph, host_node, counter);
      if(host_edge == NULL) continue;
      int target = host_edge->target;

			HostLabel label = host_edge->label;
			int vIndex = label.list->first->atom.num;
			for(int r = 0; r < rows; r++){
      	in_values[r][vIndex] = values[r][target];
			}
    }

    if(strcmp(function, "OUT") == 0){
	 	   //printf("OUT function\n");
  		 for(int r = 0; r < rows; r++){
          values[r][node_index] = in_values[r][0];
		// 		//printf("%d: %lf\n", node_index, values[r][node_index]);
	 	   }
     }
     else{
       Function* f = get_function(fSet, function);
  		 for(int r = 0; r < rows; r++){
         values[r][node_index] = f->func(in_values[r]);
       }
     }

    for(counter = 0; counter < host_node->in_edges.size + 2; counter++)
    {
      Edge *host_edge = getNthInEdge(host_graph, host_node, counter);
      if(host_edge == NULL) continue;
      int source = host_edge->source;
      ready[source]++;
			Node* source_node = getNode(host_graph, source);
			if(source_node->label.mark == 0) continue;
      if(ready[source] >= source_node->outdegree){
        queue[tail] = source;
        tail++;
      }
    }
  }

  for(int o = 0; o < outputs; o++){
		for(int r = 0; r < rows; r++){

			//printf("Expected %lf got %lf diff %lf\n", data[r][inputs + o], values[r][outputIndex[o]], fabs(values[r][outputIndex[o]] - data[r][inputs + o]));
    	double error =  fabs(values[r][outputIndex[o]] - dataset->data[r][inputs + o]);
			if(error > max_error){
				max_error = error;
			}
		}
  }
	unmarkGraph(host_graph);
  return max_error;
}
