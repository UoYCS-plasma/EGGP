#include "GenerateIndividual_connectOutput.h"

#include "GenerateIndividual.h"

bool GenerateIndividual_b3 = true;
bool GenerateIndividual_b4 = true;

static bool evaluateCondition(void)
{
   return (GenerateIndividual_b3 && GenerateIndividual_b4);
}

static void evaluatePredicateGenerateIndividual_3(Morphism *morphism)
{
   Assignment assignment_2 = getAssignment(morphism, 2);
   /* If the variable is not yet assigned, return. */
   if(assignment_2.type == 'n') return;
   string var_2 = getStringValue(morphism, 2);

   int list_var_length0 = 0;
   int list_length0 = list_var_length0 + 1;
   HostAtom array0[list_length0];
   int index0 = 0;

   array0[index0].type = 's';
   array0[index0++].str = var_2;

   int list_var_length1 = 0;
   int list_length1 = list_var_length1 + 1;
   HostAtom array1[list_length1];
   int index1 = 0;

   array1[index1].type = 's';
   array1[index1++].str = "OUT";

   if(!equalHostLists(array0, array1, list_length0, list_length1)) GenerateIndividual_b3 = true;
   else GenerateIndividual_b3 = false;
}

static void evaluatePredicateGenerateIndividual_4(Morphism *morphism)
{
   int n0 = lookupNode(morphism, 0);
   /* If the node is not yet matched by the morphism, return. */
   if(n0 == -1) return;

   if(getOutdegree(GenerateIndividual_host, n0) < 1) GenerateIndividual_b4 = true;
   else GenerateIndividual_b4 = false;
}

static bool match_n0(Morphism *morphism);
static bool match_n1(Morphism *morphism);

bool matchGenerateIndividual_connectOutput(Morphism *morphism)
{
   if(2 > GenerateIndividual_host->number_of_nodes || 0 > GenerateIndividual_host->number_of_edges) return false;
   if(match_n0(morphism)) return true;
   else
   {
      initialiseMorphism(morphism, GenerateIndividual_host);
      return false;
   }
}

static bool match_n0(Morphism *morphism)
{
   int host_index;
   for(host_index = 0; host_index < GenerateIndividual_host->nodes.size; host_index++)
   {
      Node *host_node = getNode(GenerateIndividual_host, host_index);
      if(host_node == NULL || host_node->index == -1) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark != 0) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         if(label.length < 1) break;
         /* Matching from the start of the host list. */
         HostListItem *item = label.list->first;
         int result = -1;
         HostListItem *start = item;
         /* The current host list position marks the start of the list that is
            assigned to the list variable. */
         /* More rule atoms to match. If the end of the host list is reached, break. */
         if(start == NULL) break;

         /* Matching from the end of the host list. */
         item = label.list->last;
         /* Check if the host list has passed "start". */
         if(item == start->prev) break;
         /* Matching rule atom 2 */
         if(item->atom.type != 's') break;
         else if(strcmp(item->atom.str, "OUT") != 0) break;
         item = item->prev;
         /* Matching list variable 0. */
         if(item == start->prev) result = addListAssignment(morphism, 0, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 0, item->atom.num);
            else result = addStringAssignment(morphism, 0, item->atom.str);
         }
         else
         {
            /* Assign to variable 0 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 1];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 1, false);
            result = addListAssignment(morphism, 0, list);
            freeHostList(list);
         }
         if(result >= 0)
         {
            new_assignments += result;
            match = true;
         }
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 0, host_node->index, new_assignments);
         host_node->matched = true;
         /* Update global booleans representing the node's predicates. */
         evaluatePredicateGenerateIndividual_4(morphism);
         bool next_match_result = false;
         if(evaluateCondition()) next_match_result = match_n1(morphism);
         if(next_match_result) return true;
         else
         {
            /* Reset the boolean variables in the predicates of this node. */
            GenerateIndividual_b4 = true;
            removeNodeMap(morphism, 0);
            host_node->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool match_n1(Morphism *morphism)
{
   int host_index;
   for(host_index = 0; host_index < GenerateIndividual_host->nodes.size; host_index++)
   {
      Node *host_node = getNode(GenerateIndividual_host, host_index);
      if(host_node == NULL || host_node->index == -1) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark != 0) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         if(label.length < 1) break;
         /* Matching from the start of the host list. */
         HostListItem *item = label.list->first;
         int result = -1;
         HostListItem *start = item;
         /* The current host list position marks the start of the list that is
            assigned to the list variable. */
         /* More rule atoms to match. If the end of the host list is reached, break. */
         if(start == NULL) break;

         /* Matching from the end of the host list. */
         item = label.list->last;
         /* Check if the host list has passed "start". */
         if(item == start->prev) break;
         /* Matching rule atom 2 */
         /* Matching string variable 2. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 2, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicateGenerateIndividual_3(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               GenerateIndividual_b3 = true;
               break;
            }
         }
         else break;
         item = item->prev;
         /* Matching list variable 1. */
         if(item == start->prev) result = addListAssignment(morphism, 1, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 1, item->atom.num);
            else result = addStringAssignment(morphism, 1, item->atom.str);
         }
         else
         {
            /* Assign to variable 1 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 1];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 1, false);
            result = addListAssignment(morphism, 1, list);
            freeHostList(list);
         }
         if(result >= 0)
         {
            new_assignments += result;
            match = true;
         }
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 1, host_node->index, new_assignments);
         host_node->matched = true;
         /* All items matched! */
            return true;
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

void applyGenerateIndividual_connectOutput(Morphism *morphism, bool record_changes)
{
   int host_edge_index;
   int source, target;
   int edge_array_size0 = GenerateIndividual_host->edges.size;
   source = lookupNode(morphism, 0);
   target = lookupNode(morphism, 1);
   HostLabel label;
   int list_var_length0 = 0;
   int list_length0 = list_var_length0 + 1;
   HostAtom array0[list_length0];
   int index0 = 0;

   array0[index0].type = 'i';
   array0[index0++].num = 0;
   if(list_length0 > 0)
   {
      HostList *list0 = makeHostList(array0, list_length0, false);
      label = makeHostLabel(0, list_length0, list0);
   }
   else label = makeEmptyLabel(0);

   host_edge_index = addEdge(GenerateIndividual_host, label, source, target);
   /* If the edge array size has not increased after the edge addition, then
      the edge was added to a hole in the array. */
   if(record_changes)
      pushAddedEdge(host_edge_index, edge_array_size0 == GenerateIndividual_host->edges.size);
   /* Reset the morphism. */
   initialiseMorphism(morphism, GenerateIndividual_host);
}

static bool fillpot_n0(MorphismPot *pot, Morphism *morphism);
static bool fillpot_n1(MorphismPot *pot, Morphism *morphism);

bool fillpotGenerateIndividual_connectOutput(MorphismPot *pot, Morphism *morphism)
{
   if(2 > GenerateIndividual_host->number_of_nodes || 0 > GenerateIndividual_host->number_of_edges) return false;
   int oldPotsize = potSize(pot);
   morphism->weight = 1.000000;
   fillpot_n0(pot, morphism);
   if(potSize(pot) > oldPotsize){
      initialiseMorphism(morphism, GenerateIndividual_host);
      return true;
   }
   else
   {
      initialiseMorphism(morphism, GenerateIndividual_host);
      return false;
   }
}

static bool fillpot_n0(MorphismPot *pot, Morphism *morphism)
{
   int host_index;
   for(host_index = 0; host_index < GenerateIndividual_host->nodes.size; host_index++)
   {
      Node *host_node = getNode(GenerateIndividual_host, host_index);
      if(host_node == NULL || host_node->index == -1) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark != 0) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         if(label.length < 1) break;
         /* Matching from the start of the host list. */
         HostListItem *item = label.list->first;
         int result = -1;
         HostListItem *start = item;
         /* The current host list position marks the start of the list that is
            assigned to the list variable. */
         /* More rule atoms to match. If the end of the host list is reached, break. */
         if(start == NULL) break;

         /* Matching from the end of the host list. */
         item = label.list->last;
         /* Check if the host list has passed "start". */
         if(item == start->prev) break;
         /* Matching rule atom 2 */
         if(item->atom.type != 's') break;
         else if(strcmp(item->atom.str, "OUT") != 0) break;
         item = item->prev;
         /* Matching list variable 0. */
         if(item == start->prev) result = addListAssignment(morphism, 0, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 0, item->atom.num);
            else result = addStringAssignment(morphism, 0, item->atom.str);
         }
         else
         {
            /* Assign to variable 0 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 1];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 1, false);
            result = addListAssignment(morphism, 0, list);
            freeHostList(list);
         }
         if(result >= 0)
         {
            new_assignments += result;
            match = true;
         }
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 0, host_node->index, new_assignments);
         host_node->matched = true;
         /* Update global booleans representing the node's predicates. */
         evaluatePredicateGenerateIndividual_4(morphism);
         bool next_match_result = false;
         if(evaluateCondition()) next_match_result = fillpot_n1(pot, morphism);
         if(next_match_result) return true;
         else
         {
            /* Reset the boolean variables in the predicates of this node. */
            GenerateIndividual_b4 = true;
            removeNodeMap(morphism, 0);
            host_node->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool fillpot_n1(MorphismPot *pot, Morphism *morphism)
{
   int host_index;
   for(host_index = 0; host_index < GenerateIndividual_host->nodes.size; host_index++)
   {
      Node *host_node = getNode(GenerateIndividual_host, host_index);
      if(host_node == NULL || host_node->index == -1) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark != 0) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         if(label.length < 1) break;
         /* Matching from the start of the host list. */
         HostListItem *item = label.list->first;
         int result = -1;
         HostListItem *start = item;
         /* The current host list position marks the start of the list that is
            assigned to the list variable. */
         /* More rule atoms to match. If the end of the host list is reached, break. */
         if(start == NULL) break;

         /* Matching from the end of the host list. */
         item = label.list->last;
         /* Check if the host list has passed "start". */
         if(item == start->prev) break;
         /* Matching rule atom 2 */
         /* Matching string variable 2. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 2, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicateGenerateIndividual_3(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               GenerateIndividual_b3 = true;
               break;
            }
         }
         else break;
         item = item->prev;
         /* Matching list variable 1. */
         if(item == start->prev) result = addListAssignment(morphism, 1, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 1, item->atom.num);
            else result = addStringAssignment(morphism, 1, item->atom.str);
         }
         else
         {
            /* Assign to variable 1 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 1];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 1, false);
            result = addListAssignment(morphism, 1, list);
            freeHostList(list);
         }
         if(result >= 0)
         {
            new_assignments += result;
            match = true;
         }
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 1, host_node->index, new_assignments);
         host_node->matched = true;
         /* All items matched! */
            putMorphism(pot, morphism, "GenerateIndividual_connectOutput", GenerateIndividual_host);
            removeNodeMap(morphism, 1);
            host_node->matched = false;
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

