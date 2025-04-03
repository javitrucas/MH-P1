#include <iostream>
#include <problem.h>
#include <random.hpp>
#include <string>
#include <util.h>
#include <filesystem>
#include "pincrem.h"
#include "brutesearch.h"
#include "greedy.h"
#include "randomsearch.h"
#include "localsearch.h"
#include <chrono> // Para medir el tiempo

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    // Variables para la ruta del archivo y la semilla
    string instance_path;
    long int seed;

    // Verificar argumentos de línea de comandos
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <path_to_instance_file> <seed>" << endl;
        instance_path = "../datos_MDD/GKD-b_6_n25_m7.txt"; // Instancia por defecto
        seed = 42; // Semilla por defecto
    } else {
        instance_path = argv[1]; // Ruta del archivo pasada como argumento
        seed = atoi(argv[2]); // Semilla pasada como argumento
    }

    // Inicializar generador de números aleatorios (una sola vez)
    Random::seed(seed);
    cout << "Using fixed random seed: " << seed << endl;

    // Cargar la instancia del problema
    cout << "Loading problem instance from: " << instance_path << endl;
    ProblemIncrem problem;
    problem.leerArchivo(instance_path);

    // Mostrar información básica del problema
    cout << "Instance: " << fs::path(instance_path).stem().string() << endl;

    // Crear los algoritmos
    RandomSearch randomSearch;
    GreedySearch greedySearch;
    LocalSearch randLS(SearchStrategy::randLS);
    LocalSearch heurLS(SearchStrategy::heurLS);

    // Vector de algoritmos a ejecutar
    vector<pair<string, MH*>> algorithms = {
        make_pair("RandomSearch", &randomSearch),
        make_pair("GreedySearch", &greedySearch),
        make_pair("randLS", &randLS),
        make_pair("heurLS", &heurLS)
    };

    // Ejecutar cada algoritmo SIN reinicializar la semilla
    for (auto& alg : algorithms) {
        cout << "\n=== " << alg.first << " ===" << endl;

        MH* mh = alg.second;

        // Inicio del temporizador
        auto start = chrono::high_resolution_clock::now();

        ResultMH result = mh->optimize(&problem, 100000);

        // Fin del temporizador
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;

        // Mostrar resultados
        cout << "Best fitness: " << result.fitness << endl;
        cout << "Evaluations: " << result.evaluations << endl;
        cout << "Time: " << elapsed.count() << " seconds" << endl;

        cout << "Best solution: [";
        result.printSolution();
        cout << "]" << endl;
    }

    return 0;
}