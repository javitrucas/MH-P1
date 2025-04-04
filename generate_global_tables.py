#!/usr/bin/env python3
import os
import re
import statistics

def cargar_best_fitness(filepath):
    """
    Carga los valores óptimos conocidos desde un fichero.
    Se espera un formato: InstanceName ValorFitnessMin
    """
    best_fitness = {}
    if not os.path.exists(filepath):
        print(f"Archivo de best fitness no encontrado: {filepath}")
        return best_fitness

    with open(filepath, "r") as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) == 2:
                instancia, valor = parts
                try:
                    best_fitness[instancia] = float(valor)
                except ValueError:
                    print(f"No se pudo convertir el fitness de {instancia} a float.")
    return best_fitness

def parse_block(block):
    """
    Extrae:
      - nombre de la instancia (línea con "Instance:")
      - fitness mean (línea con "Fitness")
      - time mean (línea con "Time (s)")
    """
    instance = None
    fitness_mean = None
    time_mean = None

    re_instance = re.compile(r"Instance:\s*(\S+)")
    re_fitness = re.compile(r"Fitness\s*-\s*Mean:\s*([\d\.]+)")
    re_time = re.compile(r"Time \(s\)\s*-\s*Mean:\s*([\d\.]+)")

    for line in block.splitlines():
        if instance is None:
            m = re_instance.search(line)
            if m:
                instance = m.group(1)
        if fitness_mean is None:
            m = re_fitness.search(line)
            if m:
                fitness_mean = float(m.group(1))
        if time_mean is None:
            m = re_time.search(line)
            if m:
                time_mean = float(m.group(1))
        if instance and fitness_mean is not None and time_mean is not None:
            break
    return instance, fitness_mean, time_mean

def extract_size(instance):
    m = re.search(r"_n(\d+)_", instance)
    if m:
        return int(m.group(1))
    return None

def process_algorithm(alg_input_file, best_fitness_dict):
    if not os.path.exists(alg_input_file):
        print(f"No se encontró el fichero: {alg_input_file}")
        return []

    with open(alg_input_file, "r") as f:
        contenido = f.read()

    bloques = [bloque for bloque in contenido.split("==========================================================") if bloque.strip()]
    resultados = []
    for bloque in bloques:
        instance, fitness_mean, time_mean = parse_block(bloque)
        if instance is None or fitness_mean is None or time_mean is None:
            print("No se pudo extraer información completa de un bloque; se omite.")
            continue

        fitness_min = best_fitness_dict.get(instance)
        if fitness_min is None:
            print(f"Instancia {instance} no encontrada en best_fitness.txt, se omite.")
            continue

        if fitness_mean == 0:
            desviacion = 0
        else:
            desviacion = 100 * ((fitness_mean - fitness_min) / fitness_mean)

        tamaño = extract_size(instance)
        resultados.append((instance, desviacion, time_mean, tamaño))
    return resultados

def main():
    mapping = {
        "GreedySearch": "Greedy",
        "randLS": "LSrandom",
        "heurLS": "LSheur"
    }

    input_dir = "output"
    output_dir = os.path.join("output", "tables")
    os.makedirs(output_dir, exist_ok=True)

    best_fitness_path = os.path.join("datos_MDD", "best_fitness.txt")
    best_fitness_dict = cargar_best_fitness(best_fitness_path)

    global_results = {}

    for file_key, alg_nombre in mapping.items():
        input_file = os.path.join(input_dir, f"{file_key}.txt")
        resultados = process_algorithm(input_file, best_fitness_dict)
        global_results[alg_nombre] = [(desv, tiempo, tamaño) for (_, desv, tiempo, tamaño) in resultados]

    # Tabla 2
    tabla2_file = os.path.join(output_dir, "global_by_size.txt")
    with open(tabla2_file, "w") as out2:
        out2.write("Algoritmo\tTamaño\tDesv\tTiempo\n")
        for alg_nombre, data in global_results.items():
            grupos = {}
            for desv, tiempo, tamaño in data:
                if tamaño is None:
                    continue
                grupos.setdefault(tamaño, []).append((desv, tiempo))
            for tamaño in sorted(grupos.keys()):
                desv_list = [item[0] for item in grupos[tamaño]]
                tiempo_list = [item[1] for item in grupos[tamaño]]
                media_desv = statistics.mean(desv_list) if desv_list else 0
                media_tiempo = statistics.mean(tiempo_list) if tiempo_list else 0
                out2.write(f"{alg_nombre}\t{tamaño}\t{media_desv:.4f}\t{media_tiempo:.4f}\n")
    print(f"Tabla 2 generada en: {tabla2_file}")

    # Tabla 3
    tabla3_file = os.path.join(output_dir, "global_totals.txt")
    with open(tabla3_file, "w") as out3:
        out3.write("Algoritmo\tDesv\tTiempo\n")
        for alg_nombre, data in global_results.items():
            if data:
                desv_list = [item[0] for item in data]
                tiempo_list = [item[1] for item in data]
                media_desv = statistics.mean(desv_list)
                media_tiempo = statistics.mean(tiempo_list)
            else:
                media_desv, media_tiempo = 0, 0
            out3.write(f"{alg_nombre}\t{media_desv:.4f}\t{media_tiempo:.4f}\n")
    print(f"Tabla 3 generada en: {tabla3_file}")

if __name__ == "__main__":
    main()
