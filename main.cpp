#include <iostream>
#include <problem.h>
#include <random.hpp>
#include <string>
#include <util.h>
#include <fstream>      // BBORRAR
// Real problem class
#include "pincrem.h"

// All all algorithms
#include "brutesearch.h"
#include "greedy.h"
#include "randomsearch.h"

using namespace std;
int main(int argc, char *argv[]) {
  long int seed;

  // Seed the random number generator
  if (argc == 1) {
    // Use a fixed seed for reproducibility
    seed = 42;
  } else {
    seed = atoi(argv[1]);
  }

  std::string filename = "../datos_MDD/GKD-b_1_n25_m2.txt";  // Cambia esto por el archivo que usa tu programa

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: No se pudo abrir el archivo: " << filename << std::endl;
        return 1;
    }

    std::cout << "Archivo abierto correctamente: " << filename << std::endl;
    
    // Agrega una pausa para ver la salida en depuración
    std::cin.get();
    return 0;


  // Create the algorithms
 /* RandomSearch ralg = RandomSearch();
  BruteSearch rbrute = BruteSearch();
  GreedySearch rgreedy = GreedySearch();

  // Create the specific problem
  //ProblemIncrem rproblem = ProblemIncrem(10);
  ProblemIncrem rproblem;
  rproblem.leerArchivo("../datos_MDD/GKD-b_1_n25_m2.txt") ;

  // PRUEBA SOLO RANDOM SEARCH
  pair<string, MH *> algoritmo = make_pair("RandomSearch", &ralg);
  Problem *problem = dynamic_cast<Problem *>(&rproblem);
  Random::seed(seed);
  cout << algoritmo.first << endl;
  MH *mh = algoritmo.second;
  ResultMH result = mh->optimize(problem, 100);
  cout << "Best solution: " << result.solution << endl;
  cout << "Best fitness: " << result.fitness << endl;
  cout << "Evaluations: " << result.evaluations << endl;*/

  // Solve using evaluations
  /*vector<pair<string, MH *>> algoritmos = {make_pair("RandomSearch", &ralg),
                                           make_pair("BruteSearch", &rbrute),
                                           make_pair("Greedy", &rgreedy)};

  Problem *problem = dynamic_cast<Problem *>(&rproblem);

  for (int i = 0; i < algoritmos.size(); i++) {
    Random::seed(seed);
    cout << algoritmos[i].first << endl;
    MH *mh = algoritmos[i].second;
    ResultMH result = mh->optimize(problem, 100);
    cout << "Best solution: " << result.solution << endl;
    cout << "Best fitness: " << result.fitness << endl;
    cout << "Evaluations: " << result.evaluations << endl;
  }*/

  std::cout << "Presiona ENTER para salir...";
  std::cin.get();  // Espera entrada del usuario

  return 0;
}
