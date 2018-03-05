#include "graph.h"
#include "label.h"
#include "graphStacks.h"
#include "parser.h"
#include "morphism.h"

bool matchMutateFuncFYShuffle_markOutputsRed(Morphism *morphism);

void applyMutateFuncFYShuffle_markOutputsRed(Morphism *morphism, bool record_changes);
bool fillpotMutateFuncFYShuffle_markOutputsRed(MorphismPot *pot, Morphism *morphism);

