#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchGenerateIndividual_addNode(Morphism *morphism);

void applyGenerateIndividual_addNode(Morphism *morphism, bool record_changes);
bool fillpotGenerateIndividual_addNode(MorphismPot *pot, Morphism *morphism);

