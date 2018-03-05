#include "eggp.h"

int main(void){
  printf("Hello world!\n");
  srand(time(NULL));
  int inputs = 1;
  int outputs = 1;
  int rows =50;
  string datasetpath = "Datasets/SymbolicRegression/x4+x3+x2+x1.csv";
  Dataset* dataset = loadDataSet(datasetpath, inputs, outputs, rows);
  Params* params = default_params("add,sub,mul,div,", dataset);
  params->neutral_mutations = 0;
  params->target_min_error = 0.01;
  params->size_pressure = false;
  params->permitted_size = 0;
  params->runs = 100;
  params->nodes = 30;
  params->mutation_rate = 0.03;
  one_plus_lambda_multi(dataset, params);
}
