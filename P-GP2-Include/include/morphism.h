/* ///////////////////////////////////////////////////////////////////////////

  Copyright 2015-2016 Christopher Bak

  This file is part of the GP 2 Compiler. The GP 2 Compiler is free software:
  you can redistribute it and/or modify it under the terms of the GNU General
  Public License as published by the Free Software Foundation, either version 3
  of the License, or (at your option) any later version.

  The GP 2 Compiler is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License
  along with the GP 2 Compiler. If not, see <http://www.gnu.org/licenses/>.

  ===============
  Morphism Module
  ===============

  Defines a data structure for a graph morphism. Morphisms represent matches
  from the left-hand graph of a rule to a subgraph of the host graph.
  Specifically, a morphism consists of a set of node-to-node mappings,
  a set of edge-to-edge mappings, and a set of variable-value mappings.

/////////////////////////////////////////////////////////////////////////// */

#ifndef INC_MATCH_H
#define INC_MATCH_H

#include "graph.h"
#include "label.h"

typedef struct Assignment {
   char type; /* (n)ot assigned, (i)nteger, (s)tring, (l)ist */
   union {
      int num;
      string str;
      struct HostList *list;
   };
} Assignment;

typedef struct Map {
   int host_index;
   /* The number of variable-value assignments added by this node map.
    * Needed when matching backtracks in order to remove the appropriate
    * number of assignments from the morphism. */
   int assignments;
} Map;

/* A graph morphism is a set of node-to-node mappings, a set of edge-to-edge
 * mappings and a variable-value assignment. Maps and assignments are
 * stored as static arrays, whose sizes are determined at compile time by
 * the number of nodes, edges and variables in the rule. */
typedef struct Morphism {
   int nodes;
   Map *node_map;

   int edges;
   Map *edge_map;

   int variables;
   Assignment *assignment;

   /* Stack to record the order of variable assignments during rule matching. */
   int *assigned_variables;
   int variable_index;
   double weight;
} Morphism;

/* Allocates memory for the morphism, and calls initialiseMorphism. */
Morphism *makeMorphism(int nodes, int edges, int variables);

/* Functions for moving morphisms around */
Morphism *copyMorphism(Morphism *morphism, Graph *graph);
void duplicateMorphism(Morphism *s, Morphism *t, Graph *graph);

/* This function is used to both initialise the morphism on creation and to
 * reset the morphism after each rule application. The data in the morphism
 * are reset to their default values.
 * The host graph is passed as an optional second argument to reset the matched flags
 * of all host graph items matched by the morphism. */
void initialiseMorphism(Morphism *morphism, Graph *graph);
void initialiseMorphism(Morphism *morphism, Graph *graph);
void addNodeMap(Morphism *morphism, int left_index, int host_index, int assignments);
void removeNodeMap(Morphism *morphism, int left_index);
void addEdgeMap(Morphism *morphism, int left_index, int host_index, int assignments);
void removeEdgeMap(Morphism *morphism, int left_index);

/* Tests a potential variable-value assignment against the assignments in the
 * morphism. If the variable is not in the assignment, its name and value are
 * added to the assignments array in the morphism.
 *
 * Returns -1 if the variable has already been assigned to a different value
 * in the assignment.
 * Returns 0 if the variable has a value in the assignment that is equal to
 * the passed value.
 * Returns 1 if the variable did not previously exist in the assignment. */
int addListAssignment(Morphism *morphism, int id, HostList *list);
int addIntegerAssignment(Morphism *morphism, int id, int num);
int addStringAssignment(Morphism *morphism, int id, string value);

void removeAssignments(Morphism *morphism, int number);
void pushVariableId(Morphism *morphism, int id);
int popVariableId(Morphism *morphism);

int lookupNode(Morphism *morphism, int left_index);
int lookupEdge(Morphism *morphism, int left_index);

/* These functions expect to be passed the id of a variable of the appropriate type. */
int getIntegerValue(Morphism *morphism, int id);
string getStringValue(Morphism *morphism, int id);
Assignment getAssignment(Morphism *morphism, int id);
/* Used in rule application to get the length of the value matched by a list variable. */
int getAssignmentLength(Assignment assignment);

/* Used to test string constants in the rule against a host string. If
 * rule_string is a prefix of the host_string, then the index of the host
 * character directly after this prefix is returned, so that the caller knows
 * where in the host string to resume matching.
 * For example, isPrefix("ab", "abcd") returns 2, the index of the first
 * character ('c') after the matched substring ("ab").
 * Returns -1 if it the rule string is not a prefix of the host string. */
int isPrefix(const string rule_string, const string host_string);

/* Analogous to isPrefix. Example: isSuffix("cd", "abcd") returns 1, the index
 * of the character ('b') directly preceding the matched suffix ("cd").
 * The exception is if rule_string equals host_string, in which case 0 is
 * returned. */
int isSuffix(const string rule_string, const string host_string);

void printMorphism(Morphism *morphism);
void freeMorphism(Morphism *morphism);

/* A morphism holder is used to form a linked list over morphisms */
typedef struct MorphismHolder {
   Morphism *morphism;
   string ruleName;
   struct MorphismHolder *next;
} MorphismHolder;

/* A morphism pot is used to store and sample morphism for prob mode */
typedef struct MorphismPot {
   MorphismHolder *first;
   MorphismHolder *last;
   double totalWeight;
} MorphismPot;


/* Allocates memory for a morphism holder */
MorphismHolder *makeMorphismHolder();

/* Allocates memory for the morphism pot */
MorphismPot *makeMorphismPot();

void putMorphism(MorphismPot *pot, Morphism *morphism, string ruleName, Graph *graph);

void emptyPot(MorphismPot *pot);

int potSize(MorphismPot *pot);

MorphismHolder *drawFromPot(MorphismPot *pot);

void freePot(MorphismPot *pot);

void printPot(MorphismPot *pot);

#endif /* INC_MATCH_H */
