#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMutateNode_mutateNode(Morphism *morphism);

void applyMutateNode_mutateNode(Morphism *morphism, bool record_changes);
bool fillpotMutateNode_mutateNode(MorphismPot *pot, Morphism *morphism);

