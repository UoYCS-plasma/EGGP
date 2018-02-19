#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMutateFeedForwardEdge_unmarkOutputs(Morphism *morphism);

void applyMutateFeedForwardEdge_unmarkOutputs(Morphism *morphism, bool record_changes);
bool fillpotMutateFeedForwardEdge_unmarkOutputs(MorphismPot *pot, Morphism *morphism);

