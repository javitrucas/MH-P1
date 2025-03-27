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
#include "localsearch.h"

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

  // Create the algorithms
  RandomSearch ralg = RandomSearch();
  BruteSearch rbrute = BruteSearch();
  GreedySearch rgreedy = GreedySearch();
  LocalSearch rlocal = LocalSearch();

  // Create the specific problem
  //ProblemIncrem rproblem = ProblemIncrem(10);
  ProblemIncrem rproblem;
  rproblem.leerArchivo("../datos_MDD/GKD-b_6_n25_m7.txt") ;

  // PRUEBA SOLO RANDOM SEARCH
  /*pair<string, MH *> algoritmo = make_pair("RandomSearch", &ralg);
  Problem *problem = dynamic_cast<Problem *>(&rproblem);
  Random::seed(seed);
  cout << algoritmo.first << endl;
  MH *mh = algoritmo.second;
  // Cambiando a solo 1. Cambiar a 100000 evaluaciones
  ResultMH result = mh->optimize(problem, 100);
  cout << "Best solution: " << result.solution << endl;
  cout << "Best fitness: " << result.fitness << endl;
  cout << "Evaluations: " << result.evaluations << endl;*/

  // PRUEBA SOLO GREEDY
  /*pair<string, MH *> algoritmo = make_pair("Greedy", &rgreedy);
  Problem *problem = dynamic_cast<Problem *>(&rproblem);
  Random::seed(seed);
  cout << algoritmo.first << endl;
  MH *mh = algoritmo.second;
  ResultMH result = mh->optimize(problem, 1);
  cout << "Best solution: " << result.solution << endl;
  cout << "Best fitness: " << result.fitness << endl;
  cout << "Evaluations: " << result.evaluations << endl;*/

  // PRUEBA SOLO LOCALSEARCH
  pair<string, MH *> algoritmo = make_pair("LocalSearch", &rlocal);
  Problem *problem = dynamic_cast<Problem *>(&rproblem);
  Random::seed(seed);
  cout << algoritmo.first << endl;
  MH *mh = algoritmo.second;
  ResultMH result = mh->optimize(problem, 5);
  cout << "Best solution: " << result.solution << endl;
  cout << "Best fitness: " << result.fitness << endl;
  cout << "Evaluations: " << result.evaluations << endl;

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

  //std::cout << "Presiona ENTER para salir...";
  //std::cin.get();  // Espera entrada del usuario

  return 0;
}
