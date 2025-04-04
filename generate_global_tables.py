#!/usr/bin/env python3
import os
import re
import statistics

def parse_block(block):
    """
    Dado un bloque de texto (separado por líneas con "="),
    extrae:
      - el nombre de la instancia (línea que contiene "Instance:")
      - el fitness mean y fitness min (línea que contiene "Fitness")
      - el time mean (línea que contiene "Time (s)")
    """
    instance = None
    fitness_mean = None
    fitness_min = None
    time_mean = None

    re_instance = re.compile(r"Instance:\s*(\S+)")
    re_fitness = re.compile(r"Fitness\s*-\s*Mean:\s*([\d\.]+).*Min:\s*([\d\.]+)")
    re_time = re.compile(r"Time \(s\)\s*-\s*Mean:\s*([\d\.]+)")

    for line in block.splitlines():
        if instance is None:
            m = re_instance.search(line)
            if m:
                instance = m.group(1)
        if fitness_mean is None or fitness_min is None:
            m = re_fitness.search(line)
            if m:
                fitness_mean = float(m.group(1))
                fitness_min = float(m.group(2))
        if time_mean is None:
            m = re_time.search(line)
            if m:
                time_mean = float(m.group(1))
        if instance and fitness_mean is not None and fitness_min is not None and time_mean is not None:
            break
    return instance, fitness_mean, fitness_min, time_mean

def extract_size(instance):
    """
    Extrae el tamaño de la instancia a partir del nombre.
    Se asume que el nombre tiene el formato: GKD-b_XX_nYYY_mZZ
    donde YYY es el tamaño (n).
    """
    m = re.search(r"_n(\d+)_", instance)
    if m:
        return int(m.group(1))
    return None

def process_algorithm(alg_input_file):
    """
    Lee el fichero de resultados del algoritmo y devuelve una lista
    de tuplas (instance, desviacion, tiempo, tamaño) para cada bloque.
    """
    if not os.path.exists(alg_input_file):
        print(f"No se encontró el fichero: {alg_input_file}")
        return []

    with open(alg_input_file, "r") as f:
        contenido = f.read()

    # Separar en bloques usando las líneas con muchas "="
    bloques = [bloque for bloque in contenido.split("==========================================================") if bloque.strip()]
    resultados = []
    for bloque in bloques:
        instance, fitness_mean, fitness_min, time_mean = parse_block(bloque)
        if instance is None or fitness_mean is None or fitness_min is None or time_mean is None:
            print("No se pudo extraer información completa de un bloque; se omite.")
            continue
        if fitness_mean == 0:
            print(f"Advertencia: fitness_mean es 0 en la instancia {instance}. Se asigna desviacion=0.")
            desviacion = 0
        else:
            desviacion = 100 * ((fitness_mean - fitness_min) / fitness_mean)
        tamaño = extract_size(instance)
        resultados.append((instance, desviacion, time_mean, tamaño))
    return resultados

def main():
    # Mapeo de ficheros y nombres a mostrar en la tabla global
    # Se usan los ficheros en output/ y se mapea a nombres de salida:
    # GreedySearch  -> Greedy
    # randLS        -> LSrandom
    # heurLS        -> LSheur
    mapping = {
        "GreedySearch": "Greedy",
        "randLS": "LSrandom",
        "heurLS": "LSheur"
    }

    # Directorio donde se encuentran los ficheros de cada algoritmo
    input_dir = "output"
    # Directorio donde se generarán las tablas globales
    output_dir = os.path.join("output", "tables")
    os.makedirs(output_dir, exist_ok=True)

    # Almacenamos los resultados globales por algoritmo
    global_results = {}  # {nombre_algoritmo_mostrado: [ (desv, tiempo, tamaño), ... ]}

    for file_key, alg_nombre in mapping.items():
        input_file = os.path.join(input_dir, f"{file_key}.txt")
        resultados = process_algorithm(input_file)
        if resultados:
            # Guardamos solo los valores (desviacion, tiempo, tamaño)
            global_results[alg_nombre] = [(desv, tiempo, tamaño) for (_, desv, tiempo, tamaño) in resultados]
        else:
            global_results[alg_nombre] = []
    
    # Generar Tabla 2: Resultados globales por Tamaño
    tabla2_file = os.path.join(output_dir, "global_by_size.txt")
    with open(tabla2_file, "w") as out2:
        out2.write("Algoritmo\tTamaño\tDesv\tTiempo\n")
        # Para cada algoritmo, agrupar por tamaño
        for alg_nombre, data in global_results.items():
            # Agrupar en un diccionario: tamaño -> lista de (desv, tiempo)
            grupos = {}
            for desv, tiempo, tamaño in data:
                if tamaño is None:
                    continue
                grupos.setdefault(tamaño, []).append((desv, tiempo))
            # Ordenar los tamaños de menor a mayor
            for tamaño in sorted(grupos.keys()):
                desv_list = [item[0] for item in grupos[tamaño]]
                tiempo_list = [item[1] for item in grupos[tamaño]]
                media_desv = statistics.mean(desv_list) if desv_list else 0
                media_tiempo = statistics.mean(tiempo_list) if tiempo_list else 0
                out2.write(f"{alg_nombre}\t{tamaño}\t{media_desv:.4f}\t{media_tiempo:.4f}\n")
    print(f"Tabla 2 generada en: {tabla2_file}")

    # Generar Tabla 3: Resultados globales totales
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
