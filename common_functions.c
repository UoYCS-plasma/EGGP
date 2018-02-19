#include "common_functions.h"

void add_function(FunctionSet* functionSet, string name, int arity, double (*func)(double* inputs)){
    Function* f = malloc(sizeof(Function));
    f->name = strdup(name);
    f->arity = arity;
    f->func = func;
    f->next = NULL;
    if(functionSet->functionCount > 0){
      functionSet->last->next = f;
      functionSet->last = f;
    }
    else{
      functionSet->first = f;
      functionSet->last = f;
    }
    functionSet->functionCount = functionSet->functionCount + 1;
}

FunctionSet* get_common_fset(string fsetV){
    FunctionSet* functionSet = malloc(sizeof(FunctionSet));
    functionSet->functionCount = 0;
    functionSet->first = NULL;
    int f = 0;
    int i = 0;
    int o = 0;
    while(o < (int)strlen(fsetV)){
      if(fsetV[o] == ',' || o == (int)strlen(fsetV) - 1){
        char* name;
        if(o == (int)strlen(fsetV) - 1){
          name = malloc((o + 2 - i) * sizeof(char));
          for(int j = i; j <= o; j++){
            name[j - i] = fsetV[j];
          }
        }
        else{
          name = malloc((1 + o - i) * sizeof(char));
          for(int j = i; j < o; j++){
            name[j - i] = fsetV[j];
          }
        }
        name[o - i] = '\0';
        printf("Function set: %s\n", name);
        if(strcmp(name, "and") == 0){
          add_function(functionSet, "AND", 2, common_and);
        }
        else if(strcmp(name, "or") == 0){
          add_function(functionSet, "OR", 2, common_or);
        }
        else if(strcmp(name, "nand") == 0){
          add_function(functionSet, "NAND", 2, common_nand);
        }
        else if(strcmp(name, "nor") == 0){
          add_function(functionSet, "NOR", 2, common_nor);
        }
        else if(strcmp(name, "xor") == 0){
          add_function(functionSet, "XOR", 2, common_xor);
        }
        else if(strcmp(name, "not") == 0){
          add_function(functionSet, "NOT", 1, common_not);
        }
        else if(strcmp(name, "id") == 0){
          add_function(functionSet, "ID", 1, common_id);
        }
        else if(strcmp(name, "add") == 0){
          add_function(functionSet, "ADD", 2, common_add);
        }
        else if(strcmp(name, "sub") == 0){
          add_function(functionSet, "SUB", 2, common_sub);
        }
        else if(strcmp(name, "div") == 0){
          add_function(functionSet, "DIV", 2, common_div);
        }
        else if(strcmp(name, "mul") == 0){
          add_function(functionSet, "MUL", 2, common_mul);
        }
        else{
          printf("Unknown function %s\n", name);
        }
        i = o + 1;
        f++;
      }
      o++;
    }
    return functionSet;
}

Function* get_function(FunctionSet* functionSet, string name){
    Function* f = functionSet->first;
    for(int i = 0; i < functionSet->functionCount; i++){
        if(strcmp(f->name, name) == 0){
          return f;
        }
        else{
          f = f->next;
        }
    }
    return NULL;
}

double common_and(double* inputs){
    bool i0 = (inputs[0] == 1.0);
    bool i1 = (inputs[1] == 1.0);
    return i0 && i1;
}

double common_or(double* inputs){
    bool i0 = (inputs[0] == 1.0);
    bool i1 = (inputs[1] == 1.0);
    return i0 || i1;
}

double common_nand(double* inputs){
    bool i0 = (inputs[0] == 1.0);
    bool i1 = (inputs[1] == 1.0);
    return !(i0 && i1);
}

double common_nor(double* inputs){
    bool i0 = (inputs[0] == 1.0);
    bool i1 = (inputs[1] == 1.0);
    return !(i0 || i1);
}

double common_xor(double* inputs){
    bool i0 = (inputs[0] == 1.0);
    bool i1 = (inputs[1] == 1.0);
    return ((i0 && !i1) || (!i0 && i1));
}

double common_not(double* inputs){
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
