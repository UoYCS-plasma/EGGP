#include "eggp.h"

int main(void){
  printf("Hello world!\n");
  srand(time(NULL));
  int inputs = 6;
  int outputs = 1;
  int rows =64;
  string datasetpath = "Datasets/Circuits/6op.csv";
  Dataset* dataset = loadDataSet(datasetpath, inputs, outputs, rows);
  Params* params = default_params("and,or,nand,nor,", dataset);
  params->neutral_mutations = 0;
  params->target_min_error = 0.01;
  params->mutation_rate = 0.01;
  params->size_pressure = false;
  params->permitted_size = 0;
  params->runs = 100;
  params->nodes = 100;
  one_plus_lambda_multi(dataset, params);
}
