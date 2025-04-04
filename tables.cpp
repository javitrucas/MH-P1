#include <iostream>
#include <problem.h>
#include <random.hpp>
#include <string>
#include <util.h>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "pincrem.h"
#include "brutesearch.h"
#include "greedy.h"
#include "randomsearch.h"
#include "localsearch.h"
#include <chrono> // Para medir el tiempo
#include <vector> // Para el vector de semillas
#include <map>    // Para almacenar los mejores valores conocidos

using namespace std;
namespace fs = std::filesystem;

// Estructura para almacenar estadísticas
struct Stats {
    double mean;
    double min;
    double max;
};

// Función para calcular estadísticas (solo mean, min y max)
Stats calculateStats(const vector<double>& values) {
    Stats result;
    
    // Calcular la media
    double sum = 0.0;
    for (const auto& val : values) {
        sum += val;
    }
    result.mean = sum / values.size();
    
    // Encontrar mínimo y máximo
    result.min = *min_element(values.begin(), values.end());
    result.max = *max_element(values.begin(), values.end());
    
    return result;
}

// Función para cargar los mejores valores conocidos de fitness
map<string, double> loadBestKnownFitness(const string& filePath) {
    map<string, double> bestFitness;
    ifstream file(filePath);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open best fitness file: " << filePath << endl;
        return bestFitness;
    }
    
    // Saltar la línea de cabecera
    string header;
    getline(file, header);
    
    string instance;
    double cost;
    while (file >> instance >> cost) {
        bestFitness[instance] = cost;
    }
    
    file.close();
    cout << "Loaded " << bestFitness.size() << " best known fitness values" << endl;
    return bestFitness;
}

// Estructura para almacenar resultados de cada ejecución
// Se almacena el objeto ResultMH completo para poder llamar a printSolution()
struct RunResults {
    vector<double> fitness;
    vector<double> evaluations;
    vector<double> times;
    vector<ResultMH> results;
};

// Función para ejecutar un algoritmo en una instancia del problema
void runAlgorithm(const string& algorithmName, MH* algorithm, ProblemIncrem& problem, long int seed, RunResults& results) {
    cout << "\n=== " << algorithmName << " (Seed: " << seed << ") ===" << endl;

    // Reinicializar el generador de números aleatorios con la semilla actual
    Random::seed(seed);

    // Inicio del temporizador
    auto start = chrono::high_resolution_clock::now();

    ResultMH result = algorithm->optimize(&problem, 100000);

    // Fin del temporizador
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    
    // Guardar resultados
    results.fitness.push_back(result.fitness);
    results.evaluations.push_back(result.evaluations);
    results.times.push_back(elapsed.count());
    results.results.push_back(result);

    // Mostrar resultados por consola
    cout << "Best fitness: " << result.fitness << endl;
    cout << "Evaluations: " << result.evaluations << endl;
    cout << "Time: " << elapsed.count() << " seconds" << endl;

    cout << "Best solution: ";
    result.printSolution();  // Imprime en std::cout
    cout << endl;
}

// Función para escribir estadísticas en un archivo
void writeStats(const string& algorithmName, const string& instanceName, const RunResults& results, const map<string, double>& bestKnownFitness) {
    // Crear carpeta de salida si no existe
    fs::create_directories("../output");
    
    // Nombre del archivo de salida
    string outputFile = "../output/" + algorithmName + ".txt";
    
    // Abrir archivo para añadir contenido
    ofstream outFile(outputFile, ios::app);
    
    if (!outFile.is_open()) {
        cerr << "Error: Could not open output file " << outputFile << endl;
        return;
    }
    
    // Calcular estadísticas de fitness, evaluaciones y tiempo
    Stats fitnessStats = calculateStats(results.fitness);
    Stats evalStats = calculateStats(results.evaluations);
    Stats timeStats = calculateStats(results.times);
    
    // Calcular la desviación usando el mejor valor conocido
    double bestKnown = 0.0;
    bool bestKnownFound = false;
    
    // Buscar el mejor valor conocido para esta instancia
    auto it = bestKnownFitness.find(instanceName);
    if (it != bestKnownFitness.end()) {
        bestKnown = it->second;
        bestKnownFound = true;
        cout << "Found best known fitness for " << instanceName << ": " << bestKnown << endl;
    } else {
        cout << "Warning: No best known fitness found for " << instanceName << endl;
    }
    
    // Calcular la desviación: 100*((mean_fitness - best_known)/mean_fitness)
    double deviation;
    if (bestKnownFound) {
        deviation = (fitnessStats.mean == 0) ? 0 : 100 * ((fitnessStats.mean - bestKnown) / fitnessStats.mean);
    } else {
        // Si no hay mejor valor conocido, usar el mínimo como antes
        deviation = (fitnessStats.mean == 0) ? 0 : 100 * ((fitnessStats.mean - fitnessStats.min) / fitnessStats.mean);
    }
    
    // Escribir resultados
    outFile << "==========================================================\n";
    outFile << "Instance: " << instanceName << "\n";
    outFile << "----------------------------------------------------------\n";
    outFile << fixed << setprecision(4);
    
    if (bestKnownFound) {
        outFile << "Fitness - Mean: " << fitnessStats.mean 
                << " - Desv: " << deviation 
                << " - Min: " << fitnessStats.min 
                << " - Max: " << fitnessStats.max 
                << " - Best Known: " << bestKnown << "\n";
    } else {
        outFile << "Fitness - Mean: " << fitnessStats.mean 
                << " - Desv: " << deviation 
                << " - Min: " << fitnessStats.min 
                << " - Max: " << fitnessStats.max 
                << " - Best Known: Not available\n";
    }
            
    outFile << "Evaluations - Mean: " << evalStats.mean 
            << " - Min: " << evalStats.min 
            << " - Max: " << evalStats.max << "\n";
            
    outFile << "Time (s) - Mean: " << timeStats.mean 
            << " - Min: " << timeStats.min 
            << " - Max: " << timeStats.max << "\n";
    
    // Escribir todas las soluciones encontradas usando printSolution()
    outFile << "Results per run:\n";
    for (size_t i = 0; i < results.results.size(); i++) {
        outFile << "Run " << i+1 << ": Fitness=" << results.fitness[i] 
                << ", Evals=" << results.evaluations[i] 
                << ", Time=" << results.times[i] << "\n";
        
        outFile << "  Solution: [";
        results.results[i].printSolution(outFile);  // Imprime la solución en el archivo
        outFile << "]\n";
    }
    
    outFile << "==========================================================\n\n";
    outFile.close();
}

int main(int argc, char *argv[]) {
    // Vector de semillas para ejecutar
    vector<long int> seeds;
    
    // Verificar argumentos de línea de comandos
    if (argc < 2) {
        // Si no hay argumentos, usar 5 semillas predeterminadas
        seeds = {42, 123, 567, 1024, 9999};
        cout << "Using default seeds: 42, 123, 567, 1024, 9999" << endl;
    } else if (argc == 2) {
        // Si hay un argumento, usarlo como semilla base y generar otras 4
        long int baseSeed = atoi(argv[1]);
        seeds = {baseSeed, baseSeed + 100, baseSeed + 200, baseSeed + 300, baseSeed + 400};
        cout << "Using seed base " << baseSeed << " to generate 5 seeds" << endl;
    } else if (argc >= 6) {
        // Si hay al menos 5 argumentos, usar los primeros 5 como semillas
        for (int i = 1; i <= 5; i++) {
            seeds.push_back(atoi(argv[i]));
        }
        cout << "Using command line seeds: ";
        for (size_t i = 0; i < seeds.size(); i++) {
            cout << seeds[i];
            if (i < seeds.size() - 1) cout << ", ";
        }
        cout << endl;
    } else {
        // Si hay entre 2 y 4 argumentos, completar hasta 5 semillas
        for (int i = 1; i < argc; i++) {
            seeds.push_back(atoi(argv[i]));
        }
        while (seeds.size() < 5) {
            seeds.push_back(42 + seeds.size() * 100);
        }
        cout << "Using mixed seeds: ";
        for (size_t i = 0; i < seeds.size(); i++) {
            cout << seeds[i];
            if (i < seeds.size() - 1) cout << ", ";
        }
        cout << endl;
    }

    // Cargar los mejores valores conocidos
    string bestFitnessFile = "../datos_MDD/best_fitness.txt";
    map<string, double> bestKnownFitness = loadBestKnownFitness(bestFitnessFile);

    // Carpeta que contiene las instancias
    string folderPath = "../datos_MDD";

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

    // Iterar sobre todos los archivos .txt en la carpeta
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt" && 
            entry.path().filename() != "best_fitness.txt") {  // Excluir el archivo de mejores valores
            
            string instance_path = entry.path().string();
            string instance_name = fs::path(instance_path).stem().string();

            cout << "\nProcessing instance: " << instance_name << endl;

            // Cargar la instancia del problema
            ProblemIncrem problem;
            problem.leerArchivo(instance_path);

            // Para cada algoritmo
            for (auto& alg : algorithms) {
                cout << "\nRunning algorithm: " << alg.first << endl;
                
                // Estructura para almacenar los resultados de este algoritmo
                RunResults results;
                
                // Para cada semilla
                for (const auto& seed : seeds) {
                    cout << "\n--- Executing with seed: " << seed << " ---" << endl;
                    
                    // Ejecutar el algoritmo con esta semilla
                    runAlgorithm(alg.first, alg.second, problem, seed, results);
                }
                
                // Calcular y guardar estadísticas usando los mejores valores conocidos
                writeStats(alg.first, instance_name, results, bestKnownFitness);
                cout << "\nStatistics for " << alg.first << " on " << instance_name 
                     << " saved to ../output/" << alg.first << ".txt" << endl;
            }
        }
    }

    return 0;
}