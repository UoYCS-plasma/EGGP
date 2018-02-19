#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchGenerateIndividual_deleteNodeCounter(Morphism *morphism);

void applyGenerateIndividual_deleteNodeCounter(Morphism *morphism, bool record_changes);
bool fillpotGenerateIndividual_deleteNodeCounter(MorphismPot *pot, Morphism *morphism);

