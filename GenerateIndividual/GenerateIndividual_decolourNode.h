#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchGenerateIndividual_decolourNode(Morphism *morphism);

void applyGenerateIndividual_decolourNode(Morphism *morphism, bool record_changes);
bool fillpotGenerateIndividual_decolourNode(MorphismPot *pot, Morphism *morphism);

