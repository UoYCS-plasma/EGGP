#include "generic_operators.h"

//Function for comparing two scores
static bool compare(double candidate, double champion, bool maximise, bool neutral_drift);
static int tournament(double* scores, int pop_size, int tournament_size, bool maximise);

//Loads a dataset for Genetic Programming.
GP_Dataset* load_data_set(char* file, int inputs, int rand_inputs, double rand_min, double rand_max, int outputs, int rows){
  GP_Dataset* dataset = malloc(sizeof(GP_Dataset));
  dataset->inputs = inputs + rand_inputs;
  dataset->outputs = outputs;
  dataset->rows = rows;
  dataset->data = malloc((rows * (inputs + rand_inputs + outputs)) * sizeof(double));
  for(int i = 0; i < rows; i++){
    dataset->data[i] = malloc((inputs + rand_inputs + outputs) * sizeof(double));
    for(int j = 0; j < inputs + outputs; j++){
      dataset->data[i][j] = -1.0;
    }
  }
  FILE* fp = fopen(file, "r");
  if(fp == NULL){
      printf("Error: file %s cannot be found.\n", file);
      exit(0);
  }

  for(int j = 0; j < rand_inputs; j++){
    double r = (((double)rand() / (double)RAND_MAX) * (rand_max - rand_min)) + rand_min;
    for (int i = 0; i < rows; i++){
      dataset->data[i][j] = r;
    }
  }
  for (int i = 0; i < rows; i++){
    for(int j = 0; j < (inputs + outputs); j++){
      int r = fscanf(fp," %lf %*c",&dataset->data[i][j + rand_inputs]);
      if(r != 1){
        printf("Error: file %s does not match the given row and columns dimensions.\n", file);
        exit(0);
      }
    }
  }
  return dataset;
}

//Evaluates a Genetic Programming individual. Used in CGP, EGGP, PDGP and TinyGP implementations
double gp_evaluate(Graph* host_graph, GP_Dataset* dataset, Function_Set* fset){
	mark_active_blue(host_graph);

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

     HostListItem *item = label.list->first;
     HostListItem *item2 = label.list->last;
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
       Function* f = get_function(fset, function);
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
	unmark_graph(host_graph);
  return totalError;
}

//Evaluates a Genetic Programming individual. Used in CGP, EGGP, PDGP and TinyGP implementations
double gp_evaluate_square(Graph* host_graph, GP_Dataset* dataset, Function_Set* fset){
	mark_active_blue(host_graph);

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

     HostListItem *item = label.list->first;
     HostListItem *item2 = label.list->last;
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
       Function* f = get_function(fset, function);
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
    	totalError = totalError + pow(fabs(values[r][outputIndex[o]] - dataset->data[r][inputs + o]), 2);
		}
  }
	unmark_graph(host_graph);
  return totalError;
}

//Evaluates a Genetic Programming individual. Used in CGP, EGGP, PDGP and TinyGP implementations
double gp_evaluate_worst(Graph* host_graph, GP_Dataset* dataset, Function_Set* fset){
	mark_active_blue(host_graph);
  double worst = 0.0;
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

     HostListItem *item = label.list->first;
     HostListItem *item2 = label.list->last;
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
       Function* f = get_function(fset, function);
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
    	double error = fabs(values[r][outputIndex[o]] - dataset->data[r][inputs + o]);
      if(error > worst){
        worst = error;
      }
		}
  }
	unmark_graph(host_graph);
  return worst;
}


//Evaluates a Genetic Programming individual. Used in CGP, EGGP, PDGP and TinyGP implementations
double gp_print_evaluate(Graph* host_graph, GP_Dataset* dataset, Function_Set* fset){
	mark_active_blue(host_graph);

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

     HostListItem *item = label.list->first;
     HostListItem *item2 = label.list->last;
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
       Function* f = get_function(fset, function);
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

  double error[rows];
	for(int r = 0; r < rows; r++){
    error[r] = 0.0;
  }
  for(int o = 0; o < outputs; o++){
		for(int r = 0; r < rows; r++){
			//printf("Expected %lf got %lf diff %lf\n", data[r][inputs + o], values[r][outputIndex[o]], fabs(values[r][outputIndex[o]] - data[r][inputs + o]));
    	totalError = totalError + fabs(values[r][outputIndex[o]] - dataset->data[r][inputs + o]);
      error[r] = error[r] + fabs(values[r][outputIndex[o]] - dataset->data[r][inputs + o]);
		}
  }
  for(int r = 0; r < rows; r++){
        for(int i = 0; i < inputs; i++){
          printf("%lf, ", dataset->data[r][i]);
        }
        printf(" => [");
        for(int i = 0; i < outputs; i++){
          printf("%lf,", dataset->data[r][inputs + i]);
        }
        printf("] vs. [");
        for(int i = 0; i < outputs; i++){
          printf("%lf,", values[r][outputIndex[i]]);
        }
        printf("] error = %lf\n", error[r]);
  			//printf("Expected %lf got %lf diff %lf\n", data[r][inputs + o], values[r][outputIndex[o]], fabs(values[r][outputIndex[o]] - data[r][inputs + o]));
  }
	unmark_graph(host_graph);
  return totalError;
}

void freeDataset(GP_Dataset* dataset){
  for(int i = 0; i < dataset->rows; i++){
    free(dataset->data[i]);
  }
  free(dataset->data);
  free(dataset);
}

//A procedure for performing 1 + lambda selection and reproduction
Graph** GP_1_plus_lambda(Graph** population, double* scores, uintptr_t GP_1_plus_lambda_env_pointer){
  //Access 1 + lambda environment using pointer
  GP_1_plus_lambda_env* env = (GP_1_plus_lambda_env*)GP_1_plus_lambda_env_pointer;
  Function_Set* fset = env->fset;
  double mutation_rate = env->mutation_rate;
  int winner_index = env->winner_index;
  int winner_score = env->winner_score;
  int pop_size = env->pop_size;
  bool maximise = env->maximise;
  bool neutral_drift = env->neutral_drift;

  //Find generation's "winning" parent
  int parent = winner_index;
  double best_score = winner_score;
  for(int i = 0; i < pop_size; i++){
    if(i != winner_index && compare(scores[i], best_score, maximise, neutral_drift)){
      parent = i;
      best_score = scores[i];
    }
  }

  //We now know the next generation's parent
  env->winner_index = parent;
  env->winner_score = best_score;
  Graph** new_pop = malloc(pop_size * sizeof(Graph*));
  for(int i = 0; i < pop_size; i++){
    if(i != parent){
      new_pop[i] = env->mutate(population[parent], fset, mutation_rate);
    }
    else{
      new_pop[i] = duplicate_graph(population[parent]);
    }
  }
  free_graph_array(population, pop_size);
  return new_pop;
}

static int tournament(double* scores, int pop_size, int tournament_size, bool maximise){
  int winner = -1;
  double winner_score = -1;
  for(int i = 0; i < tournament_size; i++){
    int candidate = random_int(0, pop_size);
    if(winner == -1 || compare(scores[candidate], winner_score, maximise, false)){
      winner = candidate;
      winner_score = scores[candidate];
    }
  }
  return winner;
}

Graph** GP_tournament_selection(Graph** population, double* scores, uintptr_t GP_tournament_env_pointer){
  GP_tournament_env* env = (GP_tournament_env*)GP_tournament_env_pointer;
  Function_Set* fset = env->fset;
  double mutation_rate = env->mutation_rate;
  int pop_size = env->pop_size;
  double crossover_p = env->crossover_p;
  int tournament_size = env->tournament_size;
  bool maximise = env->maximise;

  Graph** new_pop = malloc(pop_size * sizeof(Graph*));
  for(int i = 0; i < pop_size; i++){
    double r = ((double)rand() / (double)RAND_MAX);
    if(r <= crossover_p){
      Graph* parent_1 = population[tournament(scores, pop_size, tournament_size, maximise)];
      Graph* parent_2 = population[tournament(scores, pop_size, tournament_size, maximise)];
      Graph* children = env->crossover(parent_1, parent_2);
      new_pop[i] = get_red(children);
      freeGraph(children);
    }
    else{
      int parent_id = tournament(scores, pop_size, tournament_size, maximise);
      Graph* parent = population[parent_id];
      Graph* child = env->mutate(parent, fset, mutation_rate);
      new_pop[i] = child;
    }
  }
  free_graph_array(population, pop_size);
  return new_pop;

}

static bool compare(double candidate, double champion, bool maximise, bool neutral_drift){
  if(maximise){
    if(candidate > champion || (candidate >= champion && neutral_drift)){
      return true;
    }
    return false;
  }
  else{
    if(candidate < champion || (candidate <= champion && neutral_drift)){
      return true;
    }
    return false;
  }
}

double* gp_evaluate_population(Graph** population, uintptr_t GP_eval_env_pointer){
  GP_eval_env* eval_env = (GP_eval_env*)GP_eval_env_pointer;
  GP_Dataset* dataset = eval_env->dataset;
  Function_Set* fset = eval_env->fset;
  int pop_size = eval_env->pop_size;
  double* scores = malloc(pop_size * sizeof(double));
  for(int i = 0; i < pop_size; i++){
    scores[i] = gp_evaluate(population[i], dataset, fset);
  }
  return scores;
}

double* gp_evaluate_square_population(Graph** population, uintptr_t GP_eval_square_env_pointer){
  GP_eval_square_env* eval_env = (GP_eval_square_env*)GP_eval_square_env_pointer;
  GP_Dataset* dataset = eval_env->dataset;
  Function_Set* fset = eval_env->fset;
  int pop_size = eval_env->pop_size;
  double* scores = malloc(pop_size * sizeof(double));
  for(int i = 0; i < pop_size; i++){
    scores[i] = gp_evaluate_square(population[i], dataset, fset);
  }
  return scores;
}

bool target_0(Graph** population, double* scores, uintptr_t target_0_env_pointer){
  Target_0_env* target_env = (Target_0_env*)target_0_env_pointer;
  int pop_size = target_env->pop_size;
  for(int i = 0; i < pop_size; i++){
    if(scores[i] <= 0.0){
      return true;
    }
  }
  return false;
}

bool target_x(Graph** population, double* scores, uintptr_t target_x_env_pointer){
  Target_x_env* target_env = (Target_x_env*)target_x_env_pointer;
  int pop_size = target_env->pop_size;
  double x = target_env->x;
  for(int i = 0; i < pop_size; i++){
    if(scores[i] <= x){
      return true;
    }
  }
  return false;
}

bool target_worst_x(Graph** population, double* scores, uintptr_t target_worst_x_env_pointer){
  Target_worst_x_env* target_env = (Target_worst_x_env*)target_worst_x_env_pointer;
  int pop_size = target_env->pop_size;
  Function_Set* fset = target_env->fset;
  GP_Dataset* dataset = target_env->dataset;
  double x = target_env->x;
  for(int i = 0; i < pop_size; i++){
    double worst_score = target_env->get_worst_score(population[i], dataset, fset);
    if(worst_score <= x){
      return true;
    }
  }
  return false;
}

int fixed_pop_size(Graph** population, uintptr_t pop_size_env){
  Fixed_pop_env* env = (Fixed_pop_env*)pop_size_env;
  return env->pop_size;
}
