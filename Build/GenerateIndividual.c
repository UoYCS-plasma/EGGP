#include <time.h>
#include "common.h"
#include "debug.h"
#include "graph.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

#include "GenerateIndividual_initNodeCounter.h"
Morphism *M_GenerateIndividual_initNodeCounter = NULL;
#include "GenerateIndividual_addNode.h"
Morphism *M_GenerateIndividual_addNode = NULL;
#include "GenerateIndividual_addEdgeToArity.h"
Morphism *M_GenerateIndividual_addEdgeToArity = NULL;
#include "GenerateIndividual_decolourNode.h"
Morphism *M_GenerateIndividual_decolourNode = NULL;
#include "GenerateIndividual_connectOutput.h"
Morphism *M_GenerateIndividual_connectOutput = NULL;
#include "GenerateIndividual_deleteNodeCounter.h"
Morphism *M_GenerateIndividual_deleteNodeCounter = NULL;

static void GenerateIndividual_freeMorphisms(void)
{
   freeMorphism(M_GenerateIndividual_initNodeCounter);
   freeMorphism(M_GenerateIndividual_addNode);
   freeMorphism(M_GenerateIndividual_addEdgeToArity);
   freeMorphism(M_GenerateIndividual_decolourNode);
   freeMorphism(M_GenerateIndividual_connectOutput);
   freeMorphism(M_GenerateIndividual_deleteNodeCounter);
}

Graph* GenerateIndividual_host = NULL;
int* GenerateIndividual_node_map = NULL;
MorphismPot* GenerateIndividual_pot = NULL;

static void GenerateIndividual_garbageCollect(void)
{
   GenerateIndividual_freeMorphisms();
   freePot(GenerateIndividual_pot);
}

bool GenerateIndividual_success = true;

int GenerateIndividual_execute(Graph* host_graph)
{
   GenerateIndividual_host = host_graph;
   GenerateIndividual_success = true;
   GenerateIndividual_pot = makeMorphismPot();
   emptyPot(GenerateIndividual_pot);
   M_GenerateIndividual_initNodeCounter = makeMorphism(1, 0, 1);
   M_GenerateIndividual_addNode = makeMorphism(2, 0, 3);
   M_GenerateIndividual_addEdgeToArity = makeMorphism(3, 0, 5);
   M_GenerateIndividual_decolourNode = makeMorphism(1, 0, 1);
   M_GenerateIndividual_connectOutput = makeMorphism(2, 0, 3);
   M_GenerateIndividual_deleteNodeCounter = makeMorphism(1, 0, 1);

   /* Rule Call */
   if(matchGenerateIndividual_initNodeCounter(M_GenerateIndividual_initNodeCounter))
   {
      applyGenerateIndividual_initNodeCounter(M_GenerateIndividual_initNodeCounter, false);
      GenerateIndividual_success = true;
   }
   else
   {
      printf("No output graph: rule GenerateIndividual_initNodeCounter not applicable.\n");
      GenerateIndividual_garbageCollect();
      return 0;
   }
   /* Loop Statement */
   int restore_point0 = graph_change_stack == NULL ? 0 : topOfGraphChangeStack();
   while(GenerateIndividual_success)
   {
      /* Rule Call */
      emptyPot(GenerateIndividual_pot);
      fillpotGenerateIndividual_addNode(GenerateIndividual_pot, M_GenerateIndividual_addNode);
      if(potSize(GenerateIndividual_pot) > 0){
         MorphismHolder *holder = drawFromPot(GenerateIndividual_pot);
         duplicateMorphism(holder->morphism, M_GenerateIndividual_addNode, GenerateIndividual_host);
         freeMorphism(holder->morphism);
         free(holder);
         applyGenerateIndividual_addNode(M_GenerateIndividual_addNode, true);
         GenerateIndividual_success = true;
      }
      else
      {
         GenerateIndividual_success = false;
         undoChanges(GenerateIndividual_host, restore_point0);
      }
      emptyPot(GenerateIndividual_pot);
      if(!GenerateIndividual_success) break;

      /* Loop Statement */
      while(GenerateIndividual_success)
      {
         /* Rule Call */
         emptyPot(GenerateIndividual_pot);
         fillpotGenerateIndividual_addEdgeToArity(GenerateIndividual_pot, M_GenerateIndividual_addEdgeToArity);
         if(potSize(GenerateIndividual_pot) > 0){
            MorphismHolder *holder = drawFromPot(GenerateIndividual_pot);
            duplicateMorphism(holder->morphism, M_GenerateIndividual_addEdgeToArity, GenerateIndividual_host);
            freeMorphism(holder->morphism);
            free(holder);
            applyGenerateIndividual_addEdgeToArity(M_GenerateIndividual_addEdgeToArity, true);
            GenerateIndividual_success = true;
         }
         else
         {
            GenerateIndividual_success = false;
         }
         emptyPot(GenerateIndividual_pot);
      }
      GenerateIndividual_success = true;
      if(!GenerateIndividual_success) break;

      /* Rule Call */
      if(matchGenerateIndividual_decolourNode(M_GenerateIndividual_decolourNode))
      {
         applyGenerateIndividual_decolourNode(M_GenerateIndividual_decolourNode, true);
         GenerateIndividual_success = true;
      }
      else
      {
         GenerateIndividual_success = false;
         undoChanges(GenerateIndividual_host, restore_point0);
      }
      /* Graph changes from loop body may not have been used.
         Discard them so that future graph roll backs are uncorrupted. */
      if(GenerateIndividual_success) discardChanges(restore_point0);
   }
   GenerateIndividual_success = true;
   /* Loop Statement */
   while(GenerateIndividual_success)
   {
      /* Rule Call */
      emptyPot(GenerateIndividual_pot);
      fillpotGenerateIndividual_connectOutput(GenerateIndividual_pot, M_GenerateIndividual_connectOutput);
      if(potSize(GenerateIndividual_pot) > 0){
         MorphismHolder *holder = drawFromPot(GenerateIndividual_pot);
         duplicateMorphism(holder->morphism, M_GenerateIndividual_connectOutput, GenerateIndividual_host);
         freeMorphism(holder->morphism);
         free(holder);
         applyGenerateIndividual_connectOutput(M_GenerateIndividual_connectOutput, false);
         GenerateIndividual_success = true;
      }
      else
      {
         GenerateIndividual_success = false;
      }
      emptyPot(GenerateIndividual_pot);
   }
   GenerateIndividual_success = true;
   /* Rule Call */
   if(matchGenerateIndividual_deleteNodeCounter(M_GenerateIndividual_deleteNodeCounter))
   {
      applyGenerateIndividual_deleteNodeCounter(M_GenerateIndividual_deleteNodeCounter, false);
      GenerateIndividual_success = true;
   }
   else
   {
      printf("No output graph: rule GenerateIndividual_deleteNodeCounter not applicable.\n");
      GenerateIndividual_garbageCollect();
      return 0;
   }
   GenerateIndividual_garbageCollect();
   return 0;
}

