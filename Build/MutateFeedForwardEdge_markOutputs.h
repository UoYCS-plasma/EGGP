#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMutateFeedForwardEdge_markOutputs(Morphism *morphism);

void applyMutateFeedForwardEdge_markOutputs(Morphism *morphism, bool record_changes);
bool fillpotMutateFeedForwardEdge_markOutputs(MorphismPot *pot, Morphism *morphism);

