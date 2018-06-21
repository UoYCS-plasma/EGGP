#include "eggp.h"
#include "evolutionary_algorithm.h"

int main(void){
  //Reset random seed.
  srand(time(NULL));
  printf("Hello world... EGGP Demo here!\n");

  printf("AND OR NAND NOR 6-Bit Odd-Parity\n");

  //Generate a function set AND OR NAND NOR
  Function_Set* fset = get_common_fset("and,or,nand,nor");
  print_fset(fset);

  //Load the local 6-bit odd parity dataset.
  GP_Dataset* dataset = load_data_set("Datasets/Circuits/6op.csv", 6, 0, 0, 0, 1, 64);

  //Load parameters.
  EAArgs** args = malloc(100 * sizeof(EAArgs*));
  for(int i = 0; i < 100; i++){
    //Use default EGGP arguments.
    args[i] = default_eggp_EAArgs(dataset, fset);
    ((GP_1_plus_lambda_env*)args[i]->select_repopulate_env_pointer)->mutate = eggp_mutate;
  }

  //Run the experiment 100 times.
  run_multi(args, 100);

  //Free the 6-bit odd parity dataset.
  freeDataset(dataset);

  printf("AND OR NAND NOR 2-Bit Adder\n");

  //Load the local 2-Bit adder dataset.
  dataset = load_data_set("Datasets/Circuits/2BitAdder.csv", 5, 0, 0, 0, 3, 32);

  //Reload the parameters.
  free(args);
  args = malloc(100 * sizeof(EAArgs*));
  for(int i = 0; i < 100; i++){
    //Use default EGGP arguments.
    args[i] = default_eggp_EAArgs(dataset, fset);
    ((GP_1_plus_lambda_env*)args[i]->select_repopulate_env_pointer)->mutate = eggp_mutate;
  }

  //Run the experiment 100 times.
  run_multi(args, 100);

  //Free the 2-bit adder dataset.
  freeDataset(dataset);
}
