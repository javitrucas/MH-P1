#include <iostream>
#include <problem.h>
#include <random.hpp>
#include <string>
#include <util.h>
#include <fstream>      // BBORRAR
#include <filesystem>
// Real problem class
#include "pincrem.h"

// All all algorithms
#include "brutesearch.h"
#include "greedy.h"
#include "randomsearch.h"
#include "localsearch.h"

using namespace std;
namespace fs = std::filesystem;

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
  RandomSearch alg = RandomSearch();
  //BruteSearch rbrute = BruteSearch();
  //GreedySearch rgreedy = GreedySearch();
  //LocalSearch rlocal = LocalSearch(EstrategiaExploracion::heurLS);


  std::string data_folder = "../datos_MDD";
  std::string output_csv = "../results/results_random.csv";

  fs::path abs_data_folder = fs::absolute(data_folder);

  // 1. Recolectar y ordenar archivos
  std::vector<fs::path> archivos;
  for (const auto& entry : fs::directory_iterator(data_folder)) {
      if (entry.is_regular_file()) {
          archivos.push_back(fs::absolute(entry.path()));
          std::cout << "Archivo encontrado: " << entry.path() << std::endl;
      }
  }
  
  // Ordenar alfabéticamente
  std::sort(archivos.begin(), archivos.end());
  std::cout << "Archivos ordenados." << std::endl;


  // Open the CSV file for writing
  std::ofstream csv_file(output_csv);
  if (!csv_file) {
      std::cerr << "Error: No se pudo abrir el archivo " << output_csv << " para escritura." << std::endl;
      return 1;
  }

  // Write CSV header
  csv_file << "Caso,MejorCoste,Tiempo(ms)" << std::endl;

  for (const auto& archivo : archivos) {

        std::cout << "Procesando archivo: " << archivo << std::endl;

        ProblemIncrem rproblem;
        rproblem.leerArchivo(archivo.string());

        Problem *problem = dynamic_cast<Problem *>(&rproblem);
        Random::seed(seed);
        MH * algoritmo = &alg;
        
        // Measure execution time
        auto start_time = std::chrono::high_resolution_clock::now();

        ResultMH result = algoritmo->optimize(problem, 100000);
        cout << "Best solution: " << result.solution << endl;
        cout << "Best fitness: " << result.fitness << endl;
        cout << "Evaluations: " << result.evaluations << endl;

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed_time = end_time - start_time;

        // Write results to CSV
        std::string nombre_caso = archivo.filename().stem().string(); // Nombre sin extensión
        csv_file << nombre_caso << "," << result.fitness << "," << elapsed_time.count() << std::endl;

        std::cout << "Archivo: " << archivo << " procesado. Mejor coste: " << result.fitness 
                  << ", Tiempo: " << elapsed_time.count() << " ms" << std::endl;
    
}

csv_file.close();
std::cout << "Resultados guardados en " << output_csv << std::endl;



  // Create the specific problem
  //ProblemIncrem rproblem = ProblemIncrem(10);
  //ProblemIncrem rproblem;
  //rproblem.leerArchivo("../datos_MDD/GKD-b_6_n25_m7.txt") ;

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
  /*pair<string, MH *> algoritmo = make_pair("LocalSearch", &rlocal);
  Problem *problem = dynamic_cast<Problem *>(&rproblem);
  Random::seed(seed);
  cout << algoritmo.first << endl;
  MH *mh = algoritmo.second;
  ResultMH result = mh->optimize(problem, 5);
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

  //std::cout << "Presiona ENTER para salir...";
  //std::cin.get();  // Espera entrada del usuario

  return 0;
}
