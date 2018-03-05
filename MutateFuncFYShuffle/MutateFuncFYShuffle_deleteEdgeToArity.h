#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMutateFuncFYShuffle_deleteEdgeToArity(Morphism *morphism);

void applyMutateFuncFYShuffle_deleteEdgeToArity(Morphism *morphism, bool record_changes);
bool fillpotMutateFuncFYShuffle_deleteEdgeToArity(MorphismPot *pot, Morphism *morphism);

