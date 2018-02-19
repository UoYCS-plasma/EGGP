#include <time.h>
#include "common.h"
#include "debug.h"
#include "graph.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

#include "MutateFeedForwardEdge_pickEdge.h"
Morphism *M_MutateFeedForwardEdge_pickEdge = NULL;
#include "MutateFeedForwardEdge_markOutputs.h"
Morphism *M_MutateFeedForwardEdge_markOutputs = NULL;
#include "MutateFeedForwardEdge_pickNewTarget.h"
Morphism *M_MutateFeedForwardEdge_pickNewTarget = NULL;
#include "MutateFeedForwardEdge_mutateEdge.h"
Morphism *M_MutateFeedForwardEdge_mutateEdge = NULL;
#include "MutateFeedForwardEdge_unmarkOutputs.h"
Morphism *M_MutateFeedForwardEdge_unmarkOutputs = NULL;

static void MutateFeedForwardEdge_freeMorphisms(void)
{
   freeMorphism(M_MutateFeedForwardEdge_pickEdge);
   freeMorphism(M_MutateFeedForwardEdge_markOutputs);
   freeMorphism(M_MutateFeedForwardEdge_pickNewTarget);
   freeMorphism(M_MutateFeedForwardEdge_mutateEdge);
   freeMorphism(M_MutateFeedForwardEdge_unmarkOutputs);
}

Graph* MutateFeedForwardEdge_host = NULL;
int* MutateFeedForwardEdge_node_map = NULL;
MorphismPot* MutateFeedForwardEdge_pot = NULL;

static void MutateFeedForwardEdge_garbageCollect(void)
{
   MutateFeedForwardEdge_freeMorphisms();
   freePot(MutateFeedForwardEdge_pot);
}

bool MutateFeedForwardEdge_success = true;

int MutateFeedForwardEdge_execute(Graph* host_graph)
{
   MutateFeedForwardEdge_host = host_graph;
   MutateFeedForwardEdge_success = true;
   MutateFeedForwardEdge_pot = makeMorphismPot();
   emptyPot(MutateFeedForwardEdge_pot);
   M_MutateFeedForwardEdge_pickEdge = makeMorphism(2, 1, 3);
   M_MutateFeedForwardEdge_markOutputs = makeMorphism(2, 1, 3);
   M_MutateFeedForwardEdge_pickNewTarget = makeMorphism(1, 0, 2);
   M_MutateFeedForwardEdge_mutateEdge = makeMorphism(3, 1, 4);
   M_MutateFeedForwardEdge_unmarkOutputs = makeMorphism(1, 0, 1);

   /* Try Statement */
   /* Condition */
   int restore_point0 = graph_change_stack == NULL ? 0 : topOfGraphChangeStack();
   do
   {
      /* Rule Call */
      emptyPot(MutateFeedForwardEdge_pot);
      fillpotMutateFeedForwardEdge_pickEdge(MutateFeedForwardEdge_pot, M_MutateFeedForwardEdge_pickEdge);
      if(potSize(MutateFeedForwardEdge_pot) > 0){
         MorphismHolder *holder = drawFromPot(MutateFeedForwardEdge_pot);
         duplicateMorphism(holder->morphism, M_MutateFeedForwardEdge_pickEdge, MutateFeedForwardEdge_host);
         freeMorphism(holder->morphism);
         free(holder);
         applyMutateFeedForwardEdge_pickEdge(M_MutateFeedForwardEdge_pickEdge, true);
         MutateFeedForwardEdge_success = true;
      }
      else
      {
         MutateFeedForwardEdge_success = false;
         break;
      }
      emptyPot(MutateFeedForwardEdge_pot);
      /* Loop Statement */
      while(MutateFeedForwardEdge_success)
      {
         /* Rule Call */
         if(matchMutateFeedForwardEdge_markOutputs(M_MutateFeedForwardEdge_markOutputs))
         {
            applyMutateFeedForwardEdge_markOutputs(M_MutateFeedForwardEdge_markOutputs, true);
            MutateFeedForwardEdge_success = true;
         }
         else
         {
            MutateFeedForwardEdge_success = false;
         }
      }
      MutateFeedForwardEdge_success = true;
      /* Rule Call */
      emptyPot(MutateFeedForwardEdge_pot);
      fillpotMutateFeedForwardEdge_pickNewTarget(MutateFeedForwardEdge_pot, M_MutateFeedForwardEdge_pickNewTarget);
      if(potSize(MutateFeedForwardEdge_pot) > 0){
         MorphismHolder *holder = drawFromPot(MutateFeedForwardEdge_pot);
         duplicateMorphism(holder->morphism, M_MutateFeedForwardEdge_pickNewTarget, MutateFeedForwardEdge_host);
         freeMorphism(holder->morphism);
         free(holder);
         applyMutateFeedForwardEdge_pickNewTarget(M_MutateFeedForwardEdge_pickNewTarget, true);
         MutateFeedForwardEdge_success = true;
      }
      else
      {
         MutateFeedForwardEdge_success = false;
         break;
      }
      emptyPot(MutateFeedForwardEdge_pot);
      /* Rule Call */
      if(matchMutateFeedForwardEdge_mutateEdge(M_MutateFeedForwardEdge_mutateEdge))
      {
         applyMutateFeedForwardEdge_mutateEdge(M_MutateFeedForwardEdge_mutateEdge, true);
         MutateFeedForwardEdge_success = true;
      }
      else
      {
         MutateFeedForwardEdge_success = false;
         break;
      }
      /* Loop Statement */
      while(MutateFeedForwardEdge_success)
      {
         /* Rule Call */
         if(matchMutateFeedForwardEdge_unmarkOutputs(M_MutateFeedForwardEdge_unmarkOutputs))
         {
            applyMutateFeedForwardEdge_unmarkOutputs(M_MutateFeedForwardEdge_unmarkOutputs, true);
            MutateFeedForwardEdge_success = true;
         }
         else
         {
            MutateFeedForwardEdge_success = false;
         }
      }
      MutateFeedForwardEdge_success = true;
   } while(false);

   /* Then Branch */
   if(MutateFeedForwardEdge_success)
   {
      discardChanges(restore_point0);
      /* Skip Statement */
      MutateFeedForwardEdge_success = true;
   }
   /* Else Branch */
   else
   {
      undoChanges(MutateFeedForwardEdge_host, restore_point0);
      MutateFeedForwardEdge_success = true;
      /* Skip Statement */
      MutateFeedForwardEdge_success = true;
   }
   MutateFeedForwardEdge_garbageCollect();
   return 0;
}

