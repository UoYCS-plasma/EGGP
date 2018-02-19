#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMutateFeedForwardEdge_mutateEdge(Morphism *morphism);

void applyMutateFeedForwardEdge_mutateEdge(Morphism *morphism, bool record_changes);
bool fillpotMutateFeedForwardEdge_mutateEdge(MorphismPot *pot, Morphism *morphism);

