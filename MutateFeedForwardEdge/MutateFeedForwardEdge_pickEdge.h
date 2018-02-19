#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMutateFeedForwardEdge_pickEdge(Morphism *morphism);

void applyMutateFeedForwardEdge_pickEdge(Morphism *morphism, bool record_changes);
bool fillpotMutateFeedForwardEdge_pickEdge(MorphismPot *pot, Morphism *morphism);

