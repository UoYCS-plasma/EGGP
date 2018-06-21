#include "utils.h"

Graph* disjoint_union(Graph* red, Graph* blue){
  Graph *graph_copy = newGraph(100000, 100000);
  int node_mapping_red[red->nodes.size];
  int node_mapping_blue[blue->nodes.size];
  //Copy nodes
  for(int i = 0; i < red->nodes.size; i++){
     Node *host_node = getNode(red, i);
     if(host_node == NULL || host_node->index == -1) continue;
     node_mapping_red[i] = addNode(graph_copy, host_node->root, makeHostLabel(1, host_node->label.length, copyHostList(host_node->label.list)));
  }
  for(int i = 0; i < blue->nodes.size; i++){
     Node *host_node = getNode(blue, i);
     if(host_node == NULL || host_node->index == -1) continue;
     node_mapping_blue[i] = addNode(graph_copy, host_node->root, makeHostLabel(3, host_node->label.length, copyHostList(host_node->label.list)));
  }
  //Copy edges
  for(int i = 0; i < red->edges.size; i++){
     Edge *host_edge = getEdge(red, i);
     if(host_edge == NULL || host_edge->index == -1) continue;
     addEdge(graph_copy, makeHostLabel(1, host_edge->label.length, copyHostList(host_edge->label.list)), node_mapping_red[host_edge->source], node_mapping_red[host_edge->target]);
  }
  for(int i = 0; i < blue->edges.size; i++){
     Edge *host_edge = getEdge(blue, i);
     if(host_edge == NULL || host_edge->index == -1) continue;
     addEdge(graph_copy, makeHostLabel(3, host_edge->label.length, copyHostList(host_edge->label.list)), node_mapping_blue[host_edge->source], node_mapping_blue[host_edge->target]);
  }
  return graph_copy;
}

Graph* get_red(Graph* red_blue){
  return get_mark(red_blue, 1, 0);
}

Graph* get_blue(Graph* red_blue){
  return get_mark(red_blue, 3, 0);
}

Graph* get_mark(Graph* multi_mark, int mark, int target_mark){
  Graph *graph_copy = newGraph(100000, 100000);
  int node_mapping[multi_mark->nodes.size];
  //Copy nodes
  for(int i = 0; i < multi_mark->nodes.size; i++){
     Node *host_node = getNode(multi_mark, i);
     if(host_node == NULL || host_node->index == -1 || host_node->label.mark != mark) continue;
     node_mapping[i] = addNode(graph_copy, host_node->root, makeHostLabel(target_mark, host_node->label.length, copyHostList(host_node->label.list)));
  }
  //Copy edges
  for(int i = 0; i < multi_mark->edges.size; i++){
     Edge *host_edge = getEdge(multi_mark, i);
     if(host_edge == NULL || host_edge->index == -1 || host_edge->label.mark != mark) continue;
     addEdge(graph_copy, makeHostLabel(target_mark, host_edge->label.length, copyHostList(host_edge->label.list)), node_mapping[host_edge->source], node_mapping[host_edge->target]);
  }
  return graph_copy;
}

//Graph duplication function
Graph* duplicate_graph(Graph *graph)
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

//Counts the active nodes in a GP individual.
int count_active_nodes(Graph* hostG, int inputs, int outputs){
	mark_active_blue(hostG);
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
	unmark_graph(hostG);
	if(count < (inputs + outputs)){
		return 0;
	}
	return count - (inputs + outputs);
}

//Counts the active nodes in a GP individual.
int count_active_edges(Graph* hostG){
	mark_active_blue(hostG);
	int count = 0;
	for(int host_index = 0; host_index < hostG->nodes.size; host_index++)
	{
		 Node *host_node = getNode(hostG, host_index);
		 if(host_node == NULL || host_node->index == -1) continue;
		 HostLabel label = host_node->label;
		 if(label.mark > 0){
        int counter = 0;
       	for(counter = 0; counter < host_node->out_edges.size + 2; counter++)
       	{
       		Edge *host_edge = getNthOutEdge(hostG, host_node, counter);
       		if(host_edge == NULL || host_edge->index == -1) continue;
          count++;
    		}
    }
	}
	unmark_graph(hostG);
  return count;
}


void mark_active_blue(Graph* hostG){

	for(int host_index = 0; host_index < hostG->nodes.size; host_index++)
	{
		 Node *host_node = getNode(hostG, host_index);
		 if(host_node == NULL || host_node->index == -1) continue;

		 HostLabel label = host_node->label;
		 HostListItem *item = label.list->last;
		 if(item->atom.type != 's') break;
		 if(strcmp(item->atom.str, "OUT") == 0){
	   	 changeNodeMark(hostG, host_index, 3);
			 mark_active_children_blue(hostG, host_node);
		 }
	}
}

void mark_active_children_blue(Graph* hostG, Node *node){
	int counter;
	for(counter = 0; counter < node->out_edges.size + 2; counter++)
	{
		Edge *host_edge = getNthOutEdge(hostG, node, counter);
		if(host_edge == NULL || host_edge->index == -1) continue;
		Node *target = getNode(hostG, host_edge->target);
		HostLabel label = target->label;
		if(label.mark == 0){
 	   	changeNodeMark(hostG, target->index, 3);
			mark_active_children_blue(hostG, target);
		}
	}
}

void unmark_graph(Graph* hostG){

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

Graph* build_empty_host_graph()
{
   Graph* new_host = newGraph(100000, 100000);
   return new_host;
}

void free_graph_array(Graph** array, int graphs){
    for(int i = 0; i < graphs; i++){
     freeGraph(array[i]);
   }
   free(array);
}

void free_graph_data(Graph* graph){
  if(graph == NULL) return;
  printf("Freeing graph\n");
  printfGraph(graph);
  int index;
  for(index = 0; index < graph->nodes.size; index++)
  {
     Node *node = getNode(graph, index);
     if(node == NULL) continue;
     if(node->out_edges.items != NULL) free(node->out_edges.items);
     if(node->in_edges.items != NULL) free(node->in_edges.items);
     removeHostList(node->label.list);
  }
  if(graph->nodes.holes.items) free(graph->nodes.holes.items);
  if(graph->nodes.items) free(graph->nodes.items);

  for(index = 0; index < graph->edges.size; index++)
  {
     Edge *edge = getEdge(graph, index);
     if(edge == NULL) continue;
     removeHostList(edge->label.list);
  }
  if(graph->edges.holes.items != NULL) free(graph->edges.holes.items);
  if(graph->edges.items != NULL) free(graph->edges.items);
  if(graph->root_nodes != NULL)
  {
     RootNodes *iterator = graph->root_nodes;
     while(iterator != NULL)
     {
        RootNodes *temp = iterator;
        iterator = iterator->next;
        free(temp);
     }
  }
}

//Random integer from min (inclusive) to max (exclusive)
int random_int(int min, int max){
   int nu_max = max - 1;
   return min + rand() / (RAND_MAX / (nu_max - min + 1) + 1);
}
