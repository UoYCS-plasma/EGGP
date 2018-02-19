#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchGenerateIndividual_connectOutput(Morphism *morphism);

void applyGenerateIndividual_connectOutput(Morphism *morphism, bool record_changes);
bool fillpotGenerateIndividual_connectOutput(MorphismPot *pot, Morphism *morphism);

