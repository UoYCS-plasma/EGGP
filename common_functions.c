/** COMMON FUNCTIONS - common_functions.c
Generic functions for defining EGGP function sets.
*/

#include "common_functions.h"

/**print_fset. Prints a function set.
Args:
  *Function_Set* fsetV - The function set to print.
*/
void print_fset(Function_Set* fsetV){
  printf("Function set with %d members:\n", fsetV->functionCount);
  Function* f = fsetV->first;
  //Loop through function set printinf each function.
  while(f != NULL){
    printf("%s (%d)\n", f->name, f->arity);
    f = f->next;
  }
}

/**get_max_arity. Finds the maximum arity of a function set.
Args:
  *Function_Set* fsetV - The function set to find the maximum arity of.
Returns:
  *int max_arity. The maximum arity of fsetV.
*/
int get_max_arity(Function_Set* fset){
  Function* f = fset->first;
  int max_arity = 0;
  //Loop through function set finding the function with highest arity.
  while(f != NULL){
    if(f->arity > max_arity){
      max_arity = f->arity;
    }
    f = f->next;
  }
  //Return highest arity.
  return max_arity;
}

/**add_function. Adds a new function to a function set.
Args:
  *Function_Set* fset - The function set to append a function to.
  *string name - The unique name to be associated with the appended function.
  *int arity. The arity of the function to append.
  *double (*func)(double* inputs). A function which takes an array of doubles and returns a double.
*/
void add_function(Function_Set* fset, string name, int arity, double (*func)(double* inputs)){
    Function* f = malloc(sizeof(Function));
    f->name = strdup(name);
    f->arity = arity;
    f->func = func;
    f->next = NULL;
    if(fset->functionCount > 0){
      fset->last->next = f;
      fset->last = f;
    }
    else{
      fset->first = f;
      fset->last = f;
    }
    fset->functionCount = fset->functionCount + 1;
}

/**get_common_fset. Generates a function set based on a string that indicates which common functions should be included.
Args:
  *string fsetV. The string representation of the function set to generate. It should be of the form "f1,f2,f3,f4" where each fi is the relevant
  name of a function intended to be included (see below for individual names).
Returns:
  Function_Set* fset. The generated function set.
*/
Function_Set* get_common_fset(string fsetV){
    Function_Set* fset = malloc(sizeof(Function_Set));
    fset->functionCount = 0;
    int f = 0;
    int i = 0;
    int o = 0;
    //Iterate through the string; every time we see a ',', the preceeding characters are the associated name of a function to be included.
    while(o < (int)strlen(fsetV)){
      if(fsetV[o] == ',' || o == (int)strlen(fsetV) - 1){
        char* name;
        if(o == (int)strlen(fsetV) - 1){
          name = malloc((o + 2 - i) * sizeof(char));
          for(int j = i; j <= o; j++){
            name[j - i] = fsetV[j];
          }
          name[o + 1 - i] = '\0';
        }
        else{
          name = malloc((1 + o - i) * sizeof(char));
          for(int j = i; j < o; j++){
            name[j - i] = fsetV[j];
          }
          name[o - i] = '\0';
        }
        if(strcmp(name, "and") == 0){
          add_function(fset, "AND", 2, common_and);
        }
        else if(strcmp(name, "or") == 0){
          add_function(fset, "OR", 2, common_or);
        }
        else if(strcmp(name, "nand") == 0){
          add_function(fset, "NAND", 2, common_nand);
        }
        else if(strcmp(name, "nor") == 0){
          add_function(fset, "NOR", 2, common_nor);
        }
        else if(strcmp(name, "xor") == 0){
          add_function(fset, "XOR", 2, common_xor);
        }
        else if(strcmp(name, "not") == 0){
          add_function(fset, "NOT", 1, common_not);
        }
        else if(strcmp(name, "not2") == 0){
          add_function(fset, "NOT", 2, common_not);
        }
        else if(strcmp(name, "id") == 0){
          add_function(fset, "ID", 1, common_id);
        }
        else if(strcmp(name, "add") == 0){
          add_function(fset, "ADD", 2, common_add);
        }
        else if(strcmp(name, "sub") == 0){
          add_function(fset, "SUB", 2, common_sub);
        }
        else if(strcmp(name, "div") == 0){
          add_function(fset, "DIV", 2, common_div);
        }
        else if(strcmp(name, "mul") == 0){
          add_function(fset, "MUL", 2, common_mul);
        }
        else if(strcmp(name, "sin") == 0){
          add_function(fset, "SIN", 1, common_sin);
        }
        else if(strcmp(name, "cos") == 0){
          add_function(fset, "COS", 1, common_cos);
        }
        else if(strcmp(name, "exp") == 0){
          add_function(fset, "EXP", 1, common_exp);
        }
        else if(strcmp(name, "log") == 0){
          add_function(fset, "LOG", 1, common_log);
        }
        else{
          printf("Unknown function %s\n", name);
        }
        i = o + 1;
        f++;
        free(name);
      }
      o++;
    }
    return fset;
}

/**get_function. Gets a function from a function set by its name.
Args:
  *Function_Set* fset. The function set to search.
  *string name. The name of the function to search for.
Returns:
  *Function* f, the found function, if it exists. Otherwise NULL.
*/
Function* get_function(Function_Set* fset, string name){
    Function* f = fset->first;
    //Loop through the function set, checking for the given function name.
    for(int i = 0; i < fset->functionCount; i++){
        if(strcmp(f->name, name) == 0){
          return f;
        }
        else{
          f = f->next;
        }
    }
    return NULL;
}

/**boolcheck. Checks if a set of inputs are valid for a boolean function.
Args:
  *double* inputs. The set of inputs to check.
  *int count. The number of inputs to count.
Crashes if fails*/
void boolcheck(double* inputs, int count){
  for(int i = 0; i < count; i++){
    if(inputs[i] != 0.0 && inputs[i] != 1.0){
      printf("Broken logic gate!\n\n");
      exit(0);
    }
  }
}


/** VARIOUS COMMON FUNCTIONS */
//Each function declaration is self explanatory.

double common_and(double* inputs){
    boolcheck(inputs, 2);
    bool i0 = (inputs[0] == 1.0);
    bool i1 = (inputs[1] == 1.0);
    return i0 && i1;
}

double common_or(double* inputs){
    boolcheck(inputs, 2);
    bool i0 = (inputs[0] == 1.0);
    bool i1 = (inputs[1] == 1.0);
    return i0 || i1;
}

double common_nand(double* inputs){
    boolcheck(inputs, 2);
    bool i0 = (inputs[0] == 1.0);
    bool i1 = (inputs[1] == 1.0);
    return !(i0 && i1);
}

double common_nor(double* inputs){
    boolcheck(inputs, 2);
    bool i0 = (inputs[0] == 1.0);
    bool i1 = (inputs[1] == 1.0);
    return !(i0 || i1);
}

double common_xor(double* inputs){
    boolcheck(inputs, 2);
    bool i0 = (inputs[0] == 1.0);
    bool i1 = (inputs[1] == 1.0);
    return ((i0 && !i1) || (!i0 && i1));
}

double common_not(double* inputs){
    boolcheck(inputs, 1);
    bool i0 = (inputs[0] == 1.0);
    return !i0;
}

double common_id(double* inputs){
  return inputs[0];
}

double common_add(double* inputs){
    return inputs[0] + inputs[1];
}

double common_sub(double* inputs){
    return inputs[0] - inputs[1];
}

double common_div(double* inputs){
    if(inputs[1] == 0.0){
      return 1.0;
    }
    return inputs[0] / inputs[1];
}

double common_mul(double* inputs){
    return inputs[0] * inputs[1];
}

double common_sin(double* inputs){
    return sin(inputs[0]);
}

double common_cos(double* inputs){
    return cos(inputs[0]);
}

double common_log(double* inputs){
    if(inputs[0] <= 0.0){
      return 1.0;
    }
    return log(inputs[0]);
}

double common_exp(double* inputs){
    return exp(inputs[0]);
}

/**freeFset. Frees a given Function_Set.
Args:
  *Function_Set* fset. The function set to free.
*/
void freeFset(Function_Set* fset){
    Function* f = fset->first;
    while(f != NULL){
      Function* f2 = f->next;
      free(f->name);
      f->next = NULL;
      free(f);
      f = f2;
    }
    fset->first = NULL;
    fset->last = NULL;
    free(fset);
}
