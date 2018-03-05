#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMutateFuncFYShuffle_markOutputsBlue(Morphism *morphism);

void applyMutateFuncFYShuffle_markOutputsBlue(Morphism *morphism, bool record_changes);
bool fillpotMutateFuncFYShuffle_markOutputsBlue(MorphismPot *pot, Morphism *morphism);

