#include <time.h>
#include "common.h"
#include "debug.h"
#include "graph.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

#include "MutateNode_pickNode.h"
Morphism *M_MutateNode_pickNode = NULL;
#include "MutateNode_mutateNode.h"
Morphism *M_MutateNode_mutateNode = NULL;

static void MutateNode_freeMorphisms(void)
{
   freeMorphism(M_MutateNode_pickNode);
   freeMorphism(M_MutateNode_mutateNode);
}

Graph* MutateNode_host = NULL;
int* MutateNode_node_map = NULL;
MorphismPot* MutateNode_pot = NULL;

static void MutateNode_garbageCollect(void)
{
   MutateNode_freeMorphisms();
   freePot(MutateNode_pot);
}

bool MutateNode_success = true;

int MutateNode_execute(Graph* host_graph)
{
   MutateNode_host = host_graph;
   MutateNode_success = true;
   MutateNode_pot = makeMorphismPot();
   emptyPot(MutateNode_pot);
   M_MutateNode_pickNode = makeMorphism(1, 0, 1);
   M_MutateNode_mutateNode = makeMorphism(2, 0, 3);

   /* Rule Call */
   emptyPot(MutateNode_pot);
   fillpotMutateNode_pickNode(MutateNode_pot, M_MutateNode_pickNode);
   if(potSize(MutateNode_pot) > 0){
      MorphismHolder *holder = drawFromPot(MutateNode_pot);
      duplicateMorphism(holder->morphism, M_MutateNode_pickNode, MutateNode_host);
      freeMorphism(holder->morphism);
      free(holder);
      applyMutateNode_pickNode(M_MutateNode_pickNode, false);
      MutateNode_success = true;
   }
   else
   {
      printf("No output graph: rule MutateNode_pickNode not applicable.\n");
      MutateNode_garbageCollect();
      return 0;
   }
   emptyPot(MutateNode_pot);
   /* Rule Call */
   emptyPot(MutateNode_pot);
   fillpotMutateNode_mutateNode(MutateNode_pot, M_MutateNode_mutateNode);
   if(potSize(MutateNode_pot) > 0){
      MorphismHolder *holder = drawFromPot(MutateNode_pot);
      duplicateMorphism(holder->morphism, M_MutateNode_mutateNode, MutateNode_host);
      freeMorphism(holder->morphism);
      free(holder);
      applyMutateNode_mutateNode(M_MutateNode_mutateNode, false);
      MutateNode_success = true;
   }
   else
   {
      printf("No output graph: rule MutateNode_mutateNode not applicable.\n");
      MutateNode_garbageCollect();
      return 0;
   }
   emptyPot(MutateNode_pot);
   MutateNode_garbageCollect();
   return 0;
}

