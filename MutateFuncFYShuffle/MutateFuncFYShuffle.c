#include <time.h>
#include "common.h"
#include "debug.h"
#include "graph.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

#include "MutateFuncFYShuffle_pickNode.h"
Morphism *M_MutateFuncFYShuffle_pickNode = NULL;
#include "MutateFuncFYShuffle_mutateNode.h"
Morphism *M_MutateFuncFYShuffle_mutateNode = NULL;
#include "MutateFuncFYShuffle_markOutputsBlue.h"
Morphism *M_MutateFuncFYShuffle_markOutputsBlue = NULL;
#include "MutateFuncFYShuffle_markOutputsRed.h"
Morphism *M_MutateFuncFYShuffle_markOutputsRed = NULL;
#include "MutateFuncFYShuffle_addEdgeToArity.h"
Morphism *M_MutateFuncFYShuffle_addEdgeToArity = NULL;
#include "MutateFuncFYShuffle_deleteEdgeToArity.h"
Morphism *M_MutateFuncFYShuffle_deleteEdgeToArity = NULL;
#include "MutateFuncFYShuffle_resetArityCounter.h"
Morphism *M_MutateFuncFYShuffle_resetArityCounter = NULL;
#include "MutateFuncFYShuffle_bucketEdge.h"
Morphism *M_MutateFuncFYShuffle_bucketEdge = NULL;
#include "MutateFuncFYShuffle_readdEdge.h"
Morphism *M_MutateFuncFYShuffle_readdEdge = NULL;
#include "MutateFuncFYShuffle_deleteArityC.h"
Morphism *M_MutateFuncFYShuffle_deleteArityC = NULL;
#include "MutateFuncFYShuffle_unmarkBlue.h"
Morphism *M_MutateFuncFYShuffle_unmarkBlue = NULL;
#include "MutateFuncFYShuffle_unmarkRed.h"
Morphism *M_MutateFuncFYShuffle_unmarkRed = NULL;

static void MutateFuncFYShuffle_freeMorphisms(void)
{
   freeMorphism(M_MutateFuncFYShuffle_pickNode);
   freeMorphism(M_MutateFuncFYShuffle_mutateNode);
   freeMorphism(M_MutateFuncFYShuffle_markOutputsBlue);
   freeMorphism(M_MutateFuncFYShuffle_markOutputsRed);
   freeMorphism(M_MutateFuncFYShuffle_addEdgeToArity);
   freeMorphism(M_MutateFuncFYShuffle_deleteEdgeToArity);
   freeMorphism(M_MutateFuncFYShuffle_resetArityCounter);
   freeMorphism(M_MutateFuncFYShuffle_bucketEdge);
   freeMorphism(M_MutateFuncFYShuffle_readdEdge);
   freeMorphism(M_MutateFuncFYShuffle_deleteArityC);
   freeMorphism(M_MutateFuncFYShuffle_unmarkBlue);
   freeMorphism(M_MutateFuncFYShuffle_unmarkRed);
}

Graph* MutateFuncFYShuffle_host = NULL;
int* MutateFuncFYShuffle_node_map = NULL;
MorphismPot* MutateFuncFYShuffle_pot = NULL;

static void MutateFuncFYShuffle_garbageCollect(void)
{
   MutateFuncFYShuffle_freeMorphisms();
   freePot(MutateFuncFYShuffle_pot);
}

bool MutateFuncFYShuffle_success = true;

int MutateFuncFYShuffle_execute(Graph* host_graph)
{
   MutateFuncFYShuffle_host = host_graph;
   MutateFuncFYShuffle_success = true;
   MutateFuncFYShuffle_pot = makeMorphismPot();
   emptyPot(MutateFuncFYShuffle_pot);
   M_MutateFuncFYShuffle_pickNode = makeMorphism(1, 0, 2);
   M_MutateFuncFYShuffle_mutateNode = makeMorphism(2, 0, 4);
   M_MutateFuncFYShuffle_markOutputsBlue = makeMorphism(2, 1, 3);
   M_MutateFuncFYShuffle_markOutputsRed = makeMorphism(2, 1, 3);
   M_MutateFuncFYShuffle_addEdgeToArity = makeMorphism(4, 0, 6);
   M_MutateFuncFYShuffle_deleteEdgeToArity = makeMorphism(4, 1, 7);
   M_MutateFuncFYShuffle_resetArityCounter = makeMorphism(1, 0, 1);
   M_MutateFuncFYShuffle_bucketEdge = makeMorphism(2, 1, 3);
   M_MutateFuncFYShuffle_readdEdge = makeMorphism(3, 1, 4);
   M_MutateFuncFYShuffle_deleteArityC = makeMorphism(2, 0, 2);
   M_MutateFuncFYShuffle_unmarkBlue = makeMorphism(1, 0, 1);
   M_MutateFuncFYShuffle_unmarkRed = makeMorphism(1, 0, 1);

   /* Rule Call */
   emptyPot(MutateFuncFYShuffle_pot);
   fillpotMutateFuncFYShuffle_pickNode(MutateFuncFYShuffle_pot, M_MutateFuncFYShuffle_pickNode);
   if(potSize(MutateFuncFYShuffle_pot) > 0){
      MorphismHolder *holder = drawFromPot(MutateFuncFYShuffle_pot);
      duplicateMorphism(holder->morphism, M_MutateFuncFYShuffle_pickNode, MutateFuncFYShuffle_host);
      freeMorphism(holder->morphism);
      free(holder);
      applyMutateFuncFYShuffle_pickNode(M_MutateFuncFYShuffle_pickNode, false);
      MutateFuncFYShuffle_success = true;
   }
   else
   {
      printf("No output graph: rule MutateFuncFYShuffle_pickNode not applicable.\n");
      MutateFuncFYShuffle_garbageCollect();
      return 0;
   }
   emptyPot(MutateFuncFYShuffle_pot);
   /* Rule Call */
   emptyPot(MutateFuncFYShuffle_pot);
   fillpotMutateFuncFYShuffle_mutateNode(MutateFuncFYShuffle_pot, M_MutateFuncFYShuffle_mutateNode);
   if(potSize(MutateFuncFYShuffle_pot) > 0){
      MorphismHolder *holder = drawFromPot(MutateFuncFYShuffle_pot);
      duplicateMorphism(holder->morphism, M_MutateFuncFYShuffle_mutateNode, MutateFuncFYShuffle_host);
      freeMorphism(holder->morphism);
      free(holder);
      applyMutateFuncFYShuffle_mutateNode(M_MutateFuncFYShuffle_mutateNode, false);
      MutateFuncFYShuffle_success = true;
   }
   else
   {
      printf("No output graph: rule MutateFuncFYShuffle_mutateNode not applicable.\n");
      MutateFuncFYShuffle_garbageCollect();
      return 0;
   }
   emptyPot(MutateFuncFYShuffle_pot);
   /* Loop Statement */
   while(MutateFuncFYShuffle_success)
   {
      /* Rule Set Call */
      do
      {
         if(matchMutateFuncFYShuffle_markOutputsBlue(M_MutateFuncFYShuffle_markOutputsBlue))
         {
            applyMutateFuncFYShuffle_markOutputsBlue(M_MutateFuncFYShuffle_markOutputsBlue, false);
            MutateFuncFYShuffle_success = true;
            break;
         }
         if(matchMutateFuncFYShuffle_markOutputsRed(M_MutateFuncFYShuffle_markOutputsRed))
         {
            applyMutateFuncFYShuffle_markOutputsRed(M_MutateFuncFYShuffle_markOutputsRed, false);
            MutateFuncFYShuffle_success = true;
         }
         else
         {
            MutateFuncFYShuffle_success = false;
         }
      } while(false);
   }
   MutateFuncFYShuffle_success = true;
   /* Loop Statement */
   while(MutateFuncFYShuffle_success)
   {
      /* Rule Set Call */
      do
      {
      bool valid[2];
      double weight[2];
      double totalWeight = 0.0;
      bool someValid = false;
      valid[0] = false;
      weight[0] = 1.0;
      valid[1] = false;
      weight[1] = 1.0;
      valid[0] = false;
      weight[0] = 1.000000;
      if(matchMutateFuncFYShuffle_addEdgeToArity(M_MutateFuncFYShuffle_addEdgeToArity)){
         valid[0] = true;
         totalWeight = totalWeight + weight[0];
         someValid = true;
      }
      initialiseMorphism(M_MutateFuncFYShuffle_addEdgeToArity, MutateFuncFYShuffle_host);
      valid[1] = false;
      weight[1] = 1.000000;
      if(matchMutateFuncFYShuffle_deleteEdgeToArity(M_MutateFuncFYShuffle_deleteEdgeToArity)){
         valid[1] = true;
         totalWeight = totalWeight + weight[1];
         someValid = true;
      }
      initialiseMorphism(M_MutateFuncFYShuffle_deleteEdgeToArity, MutateFuncFYShuffle_host);
      if (someValid){
         double r = ((double)rand() / (double)RAND_MAX);
         r = r * totalWeight;
         double sum = 0.0;
         if(valid[0]){
            sum = sum + weight[0];
            if(r <= sum){
               emptyPot(MutateFuncFYShuffle_pot);
               fillpotMutateFuncFYShuffle_addEdgeToArity(MutateFuncFYShuffle_pot, M_MutateFuncFYShuffle_addEdgeToArity);
               if(potSize(MutateFuncFYShuffle_pot) > 0){
                  MorphismHolder *holder = drawFromPot(MutateFuncFYShuffle_pot);
                  duplicateMorphism(holder->morphism, M_MutateFuncFYShuffle_addEdgeToArity, MutateFuncFYShuffle_host);
                  freeMorphism(holder->morphism);
                  free(holder);
                  applyMutateFuncFYShuffle_addEdgeToArity(M_MutateFuncFYShuffle_addEdgeToArity, false);
                  MutateFuncFYShuffle_success = true;
                  break;
               }
               emptyPot(MutateFuncFYShuffle_pot);
            }
         }
         if(valid[1]){
            sum = sum + weight[1];
            if(r <= sum && r > sum - weight[1]){
               emptyPot(MutateFuncFYShuffle_pot);
               fillpotMutateFuncFYShuffle_deleteEdgeToArity(MutateFuncFYShuffle_pot, M_MutateFuncFYShuffle_deleteEdgeToArity);
               if(potSize(MutateFuncFYShuffle_pot) > 0){
                  MorphismHolder *holder = drawFromPot(MutateFuncFYShuffle_pot);
                  duplicateMorphism(holder->morphism, M_MutateFuncFYShuffle_deleteEdgeToArity, MutateFuncFYShuffle_host);
                  freeMorphism(holder->morphism);
                  free(holder);
                  applyMutateFuncFYShuffle_deleteEdgeToArity(M_MutateFuncFYShuffle_deleteEdgeToArity, false);
                  MutateFuncFYShuffle_success = true;
                  break;
               }
               emptyPot(MutateFuncFYShuffle_pot);
            }
         }
      } else {
      MutateFuncFYShuffle_success = false;
      }
      } while(false);
   }
   MutateFuncFYShuffle_success = true;
   /* Loop Statement */
   while(MutateFuncFYShuffle_success)
   {
      /* Rule Call */
      if(matchMutateFuncFYShuffle_unmarkBlue(M_MutateFuncFYShuffle_unmarkBlue))
      {
         applyMutateFuncFYShuffle_unmarkBlue(M_MutateFuncFYShuffle_unmarkBlue, false);
         MutateFuncFYShuffle_success = true;
      }
      else
      {
         MutateFuncFYShuffle_success = false;
      }
   }
   MutateFuncFYShuffle_success = true;
   /* Rule Call */
   if(matchMutateFuncFYShuffle_resetArityCounter(M_MutateFuncFYShuffle_resetArityCounter))
   {
      applyMutateFuncFYShuffle_resetArityCounter(M_MutateFuncFYShuffle_resetArityCounter, false);
      MutateFuncFYShuffle_success = true;
   }
   else
   {
      printf("No output graph: rule MutateFuncFYShuffle_resetArityCounter not applicable.\n");
      MutateFuncFYShuffle_garbageCollect();
      return 0;
   }
   /* Loop Statement */
   while(MutateFuncFYShuffle_success)
   {
      /* Rule Call */
      if(matchMutateFuncFYShuffle_bucketEdge(M_MutateFuncFYShuffle_bucketEdge))
      {
         applyMutateFuncFYShuffle_bucketEdge(M_MutateFuncFYShuffle_bucketEdge, false);
         MutateFuncFYShuffle_success = true;
      }
      else
      {
         MutateFuncFYShuffle_success = false;
      }
   }
   MutateFuncFYShuffle_success = true;
   /* Loop Statement */
   while(MutateFuncFYShuffle_success)
   {
      /* Rule Call */
      emptyPot(MutateFuncFYShuffle_pot);
      fillpotMutateFuncFYShuffle_readdEdge(MutateFuncFYShuffle_pot, M_MutateFuncFYShuffle_readdEdge);
      if(potSize(MutateFuncFYShuffle_pot) > 0){
         MorphismHolder *holder = drawFromPot(MutateFuncFYShuffle_pot);
         duplicateMorphism(holder->morphism, M_MutateFuncFYShuffle_readdEdge, MutateFuncFYShuffle_host);
         freeMorphism(holder->morphism);
         free(holder);
         applyMutateFuncFYShuffle_readdEdge(M_MutateFuncFYShuffle_readdEdge, false);
         MutateFuncFYShuffle_success = true;
      }
      else
      {
         MutateFuncFYShuffle_success = false;
      }
      emptyPot(MutateFuncFYShuffle_pot);
   }
   MutateFuncFYShuffle_success = true;
   /* Rule Call */
   if(matchMutateFuncFYShuffle_deleteArityC(M_MutateFuncFYShuffle_deleteArityC))
   {
      applyMutateFuncFYShuffle_deleteArityC(M_MutateFuncFYShuffle_deleteArityC, false);
      MutateFuncFYShuffle_success = true;
   }
   else
   {
      printf("No output graph: rule MutateFuncFYShuffle_deleteArityC not applicable.\n");
      MutateFuncFYShuffle_garbageCollect();
      return 0;
   }
   /* Loop Statement */
   while(MutateFuncFYShuffle_success)
   {
      /* Rule Set Call */
      do
      {
         if(matchMutateFuncFYShuffle_unmarkBlue(M_MutateFuncFYShuffle_unmarkBlue))
         {
            applyMutateFuncFYShuffle_unmarkBlue(M_MutateFuncFYShuffle_unmarkBlue, false);
            MutateFuncFYShuffle_success = true;
            break;
         }
         if(matchMutateFuncFYShuffle_unmarkRed(M_MutateFuncFYShuffle_unmarkRed))
         {
            applyMutateFuncFYShuffle_unmarkRed(M_MutateFuncFYShuffle_unmarkRed, false);
            MutateFuncFYShuffle_success = true;
         }
         else
         {
            MutateFuncFYShuffle_success = false;
         }
      } while(false);
   }
   MutateFuncFYShuffle_success = true;
   MutateFuncFYShuffle_garbageCollect();
   return 0;
}

