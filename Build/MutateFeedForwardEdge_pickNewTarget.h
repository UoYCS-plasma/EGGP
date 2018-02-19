#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMutateFeedForwardEdge_pickNewTarget(Morphism *morphism);

void applyMutateFeedForwardEdge_pickNewTarget(Morphism *morphism, bool record_changes);
bool fillpotMutateFeedForwardEdge_pickNewTarget(MorphismPot *pot, Morphism *morphism);

