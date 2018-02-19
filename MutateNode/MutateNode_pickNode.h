#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMutateNode_pickNode(Morphism *morphism);

void applyMutateNode_pickNode(Morphism *morphism, bool record_changes);
bool fillpotMutateNode_pickNode(MorphismPot *pot, Morphism *morphism);

