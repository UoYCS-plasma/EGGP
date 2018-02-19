#include "MutateNode_mutateNode.h"

#include "MutateNode.h"

bool MutateNode_b2 = true;

static bool evaluateCondition(void)
{
   return (MutateNode_b2);
}

static void evaluatePredicateMutateNode_2(Morphism *morphism)
{
   Assignment assignment_1 = getAssignment(morphism, 1);
   /* If the variable is not yet assigned, return. */
   if(assignment_1.type == 'n') return;
   string var_1 = getStringValue(morphism, 1);

   Assignment assignment_2 = getAssignment(morphism, 2);
   /* If the variable is not yet assigned, return. */
   if(assignment_2.type == 'n') return;
   string var_2 = getStringValue(morphism, 2);

   int list_var_length0 = 0;
   int list_length0 = list_var_length0 + 1;
   HostAtom array0[list_length0];
   int index0 = 0;

   array0[index0].type = 's';
   array0[index0++].str = var_1;

   int list_var_length1 = 0;
   int list_length1 = list_var_length1 + 1;
   HostAtom array1[list_length1];
   int index1 = 0;

   array1[index1].type = 's';
   array1[index1++].str = var_2;

   if(!equalHostLists(array0, array1, list_length0, list_length1)) MutateNode_b2 = true;
   else MutateNode_b2 = false;
}

static bool match_n1(Morphism *morphism);
static bool match_n0(Morphism *morphism);

bool matchMutateNode_mutateNode(Morphism *morphism)
{
   if(2 > MutateNode_host->number_of_nodes || 0 > MutateNode_host->number_of_edges) return false;
   if(match_n1(morphism)) return true;
   else
   {
      initialiseMorphism(morphism, MutateNode_host);
      return false;
   }
}

static bool match_n1(Morphism *morphism)
{
   RootNodes *nodes;
   for(nodes = getRootNodeList(MutateNode_host); nodes != NULL; nodes = nodes->next)
   {
      Node *host_node = getNode(MutateNode_host, nodes->index);
      if(host_node == NULL) continue;
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
         if(label.length < 2) break;
         /* Matching from the start of the host list. */
         HostListItem *item = label.list->first;
         /* Check if the end of the host list has been reached. */
         if(item == NULL) break;
         /* Matching rule atom 1. */
         if(item->atom.type != 's') break;
         else if(strcmp(item->atom.str, "FunctionSet") != 0) break;
         item = item->next;

         /* Check if the end of the host list has been reached. */
         if(item == NULL) break;
         /* Matching rule atom 2. */
         int result = -1;
         /* Matching string variable 2. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 2, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicateMutateNode_2(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               MutateNode_b2 = true;
               break;
            }
         }
         else break;
         item = item->next;

         HostListItem *start = item;
         /* Matching list variable 0. */
         if(start == NULL) result = addListAssignment(morphism, 0, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 0, item->atom.num);
            else result = addStringAssignment(morphism, 0, item->atom.str);
         }
         else
         {
            /* Assign to variable 0 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 2];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 2, false);
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
         addNodeMap(morphism, 1, host_node->index, new_assignments);
         host_node->matched = true;
         if(match_n0(morphism)) return true;
         else
         {
         removeNodeMap(morphism, 1);
         host_node->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool match_n0(Morphism *morphism)
{
   int host_index;
   for(host_index = 0; host_index < MutateNode_host->nodes.size; host_index++)
   {
      Node *host_node = getNode(MutateNode_host, host_index);
      if(host_node == NULL || host_node->index == -1) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark != 3) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         /* The rule list does not contain a list variable, so there is no
          * match if the host list has a different length. */
         if(label.length != 1) break;
         HostListItem *item = label.list->first;
         /* Matching rule atom 1. */
         int result = -1;
         /* Matching string variable 1. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 1, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicateMutateNode_2(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               MutateNode_b2 = true;
               break;
            }
         }
         else break;
         match = true;
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 0, host_node->index, new_assignments);
         host_node->matched = true;
         /* All items matched! */
            return true;
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

void applyMutateNode_mutateNode(Morphism *morphism, bool record_changes)
{
   string var_2 = getStringValue(morphism, 2);
   int host_node_index = lookupNode(morphism, 0);
   HostLabel label_n0 = getNodeLabel(MutateNode_host, host_node_index);
   HostLabel label;
   int list_var_length0 = 0;
   int list_length0 = list_var_length0 + 1;
   HostAtom array0[list_length0];
   int index0 = 0;

   array0[index0].type = 's';
   array0[index0++].str = var_2;
   if(list_length0 > 0)
   {
      HostList *list0 = makeHostList(array0, list_length0, false);
      label = makeHostLabel(0, list_length0, list0);
   }
   else label = makeEmptyLabel(0);

   if(equalHostLabels(label_n0, label)) removeHostList(label.list);
   else
   {
      if(record_changes) pushRelabelledNode(host_node_index, label_n0);
      relabelNode(MutateNode_host, host_node_index, label);
   }
   /* Reset the morphism. */
   initialiseMorphism(morphism, MutateNode_host);
}

static bool fillpot_n1(MorphismPot *pot, Morphism *morphism);
static bool fillpot_n0(MorphismPot *pot, Morphism *morphism);

bool fillpotMutateNode_mutateNode(MorphismPot *pot, Morphism *morphism)
{
   if(2 > MutateNode_host->number_of_nodes || 0 > MutateNode_host->number_of_edges) return false;
   int oldPotsize = potSize(pot);
   morphism->weight = 1.000000;
   fillpot_n1(pot, morphism);
   if(potSize(pot) > oldPotsize){
      initialiseMorphism(morphism, MutateNode_host);
      return true;
   }
   else
   {
      initialiseMorphism(morphism, MutateNode_host);
      return false;
   }
}

static bool fillpot_n1(MorphismPot *pot, Morphism *morphism)
{
   RootNodes *nodes;
   for(nodes = getRootNodeList(MutateNode_host); nodes != NULL; nodes = nodes->next)
   {
      Node *host_node = getNode(MutateNode_host, nodes->index);
      if(host_node == NULL) continue;
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
         if(label.length < 2) break;
         /* Matching from the start of the host list. */
         HostListItem *item = label.list->first;
         /* Check if the end of the host list has been reached. */
         if(item == NULL) break;
         /* Matching rule atom 1. */
         if(item->atom.type != 's') break;
         else if(strcmp(item->atom.str, "FunctionSet") != 0) break;
         item = item->next;

         /* Check if the end of the host list has been reached. */
         if(item == NULL) break;
         /* Matching rule atom 2. */
         int result = -1;
         /* Matching string variable 2. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 2, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicateMutateNode_2(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               MutateNode_b2 = true;
               break;
            }
         }
         else break;
         item = item->next;

         HostListItem *start = item;
         /* Matching list variable 0. */
         if(start == NULL) result = addListAssignment(morphism, 0, NULL);
         else if(item == start)
         {
            if(item->atom.type == 'i') result = addIntegerAssignment(morphism, 0, item->atom.num);
            else result = addStringAssignment(morphism, 0, item->atom.str);
         }
         else
         {
            /* Assign to variable 0 the unmatched sublist of the host list. */
            HostAtom sublist[label.length - 2];
            int list_index = 0;
            HostListItem *iterator = start;
            while(iterator != item->next)
            {
               sublist[list_index++] = iterator->atom;
               iterator = iterator->next;
            }
            HostList *list = makeHostList(sublist, label.length - 2, false);
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
         addNodeMap(morphism, 1, host_node->index, new_assignments);
         host_node->matched = true;
         if(fillpot_n0(pot, morphism)) return true;
         else
         {
         removeNodeMap(morphism, 1);
         host_node->matched = false;
         }
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

static bool fillpot_n0(MorphismPot *pot, Morphism *morphism)
{
   int host_index;
   for(host_index = 0; host_index < MutateNode_host->nodes.size; host_index++)
   {
      Node *host_node = getNode(MutateNode_host, host_index);
      if(host_node == NULL || host_node->index == -1) continue;
      if(host_node->matched) continue;
      if(host_node->label.mark != 3) continue;
      if(host_node->indegree < 0 || host_node->outdegree < 0 ||
         ((host_node->outdegree + host_node->indegree - 0 - 0 - 0) < 0)) continue;

      HostLabel label = host_node->label;
      bool match = false;
      /* Label Matching */
      int new_assignments = 0;
      do
      {
         /* The rule list does not contain a list variable, so there is no
          * match if the host list has a different length. */
         if(label.length != 1) break;
         HostListItem *item = label.list->first;
         /* Matching rule atom 1. */
         int result = -1;
         /* Matching string variable 1. */
         if(item->atom.type != 's') break;
         result = addStringAssignment(morphism, 1, item->atom.str);
         if(result >= 0)
         {
            new_assignments += result;
            /* Update global booleans for the variable's predicates. */
            evaluatePredicateMutateNode_2(morphism);
            if(!evaluateCondition())
            {
               /* Reset the boolean variables in the predicates of this variable. */
               MutateNode_b2 = true;
               break;
            }
         }
         else break;
         match = true;
      } while(false);

      if(match)
      {
         addNodeMap(morphism, 0, host_node->index, new_assignments);
         host_node->matched = true;
         /* All items matched! */
            putMorphism(pot, morphism, "MutateNode_mutateNode", MutateNode_host);
            removeNodeMap(morphism, 0);
            host_node->matched = false;
      }
      else removeAssignments(morphism, new_assignments);
   }
   return false;
}

