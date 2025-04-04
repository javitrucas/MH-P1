#!/usr/bin/env python3
import os
import re

def load_best_fitness():
    """
    Carga los mejores valores de fitness conocidos desde el archivo best_fitness.txt
    Retorna un diccionario con instancia -> mejor fitness
    """
    best_fitness = {}
    best_fitness_path = os.path.join("datos_MDD", "best_fitness.txt")
    
    try:
        with open(best_fitness_path, "r") as f:
            # Saltamos la cabecera
            next(f)
            for line in f:
                parts = line.strip().split()
                if len(parts) >= 2:
                    instance = parts[0]
                    fitness = float(parts[1])
                    best_fitness[instance] = fitness
                    # También almacenamos con el nombre simplificado para facilitar la comparación
                    simplified_name = simplify_instance_name(instance)
                    if simplified_name != instance:
                        best_fitness[simplified_name] = fitness
        print(f"Cargados {len(best_fitness)} valores de mejor fitness conocido")
    except Exception as e:
        print(f"Error al cargar el archivo de mejores fitness: {e}")
    
    return best_fitness

def simplify_instance_name(name):
    """
    Simplifica el nombre de la instancia para hacerlo coincidir entre formatos.
    Elimina extensiones de archivo y normaliza.
    """
    # Eliminar extensión si existe
    name = os.path.splitext(name)[0]
    # Eliminar path si existe
    name = os.path.basename(name)
    return name

def parse_block(block):
    """
    Dado un bloque de texto (separado por líneas con "="),
    extrae:
      - el nombre de la instancia (línea que contiene "Instance:")
      - el fitness mean (línea que contiene "Fitness")
      - el time mean (línea que contiene "Time (s)")
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

def process_algorithm(nombre_algoritmo, best_fitness):
    """
    Procesa el fichero de resultados del algoritmo dado y genera la tabla con
    la media total al final, usando los mejores valores conocidos para el cálculo
    de la desviación.
    """
    # Definir rutas de entrada y salida
    input_file = os.path.join("output", f"{nombre_algoritmo}.txt")
    output_dir = os.path.join("output", "tables")
    os.makedirs(output_dir, exist_ok=True)
    output_file = os.path.join(output_dir, f"{nombre_algoritmo}.txt")

    if not os.path.exists(input_file):
        print(f"No se encontró el fichero de entrada: {input_file}")
        return

    with open(input_file, "r") as f:
        contenido = f.read()

    # Separamos el fichero en bloques usando las líneas de separación (líneas con "=")
    bloques = [bloque for bloque in contenido.split("==========================================================") if bloque.strip()]

    resultados = []
    instancias_sin_best = []
    
    for bloque in bloques:
        instance, fitness_mean, time_mean = parse_block(bloque)
        if instance is None or fitness_mean is None or time_mean is None:
            print("No se pudo extraer información completa de un bloque; se omite.")
            continue
            
        # Intentar varias formas de buscar el mejor fitness
        original_instance = instance
        found = False
        
        # 1. Buscar directamente
        if instance in best_fitness:
            fitness_min = best_fitness[instance]
            found = True
        else:
            # 2. Intentar simplificar el nombre
            simplified_instance = simplify_instance_name(instance)
            if simplified_instance in best_fitness:
                fitness_min = best_fitness[simplified_instance]
                found = True
            else:
                # 3. Intentar con el nombre de patrón GKD-b_X_...
                match = re.search(r'(GKD-b_\d+_[^_]+)', simplified_instance)
                if match and match.group(1) in best_fitness:
                    fitness_min = best_fitness[match.group(1)]
                    found = True
                else:
                    # 4. Intentar con búsqueda menos estricta
                    for key in best_fitness.keys():
                        if key in simplified_instance or simplified_instance in key:
                            fitness_min = best_fitness[key]
                            found = True
                            print(f"Coincidencia aproximada: {instance} -> {key}")
                            break
        
        if not found:
            instancias_sin_best.append(original_instance)
            print(f"No se encontró mejor fitness para: {original_instance}")
            # Si no hay mejor fitness conocido, usamos el fitness_mean
            fitness_min = fitness_mean
        
        if fitness_mean == 0:
            print(f"Advertencia: fitness_mean es 0 en la instancia {instance}. Se asigna desviacion=0.")
            desviacion = 0
        else:
            desviacion = 100 * ((fitness_mean - fitness_min) / fitness_mean)
        resultados.append((instance, desviacion, time_mean))
    
    if instancias_sin_best:
        print(f"Advertencia: No se encontró mejor fitness conocido para {len(instancias_sin_best)} instancias")

    # Ordenar los resultados según el número de instancia (se extrae el número de "GKD-b_X_...")
    def orden_key(item):
        m = re.search(r'GKD-b_(\d+)_', item[0])
        return int(m.group(1)) if m else 0
    resultados.sort(key=orden_key)

    # Calcular medias totales
    if resultados:
        media_desv = sum(r[1] for r in resultados) / len(resultados)
        media_tiempo = sum(r[2] for r in resultados) / len(resultados)
    else:
        media_desv, media_tiempo = 0, 0

    # Escribir la tabla en el fichero de salida, añadiendo la línea de medias totales
    with open(output_file, "w") as out:
        out.write("Caso\tDesv\tTiempo\n")
        for instance, desv, tiempo in resultados:
            out.write(f"{instance}\t{desv:.4f}\t{tiempo:.4f}\n")
        out.write(f"MEDIA TOTAL:\t{media_desv:.4f}\t{media_tiempo:.4f}\n")

    print(f"Tabla generada en: {output_file}")

def main():
    # Cargar los mejores valores de fitness conocidos
    best_fitness = load_best_fitness()
    
    if not best_fitness:
        print("ADVERTENCIA: No se pudo cargar ningún valor de mejor fitness conocido.")
        print("Verificar la ruta: ../datos_MDD/best_fitness.txt")
    
    # Lista de algoritmos a procesar
    algoritmos = ["GreedySearch", "RandomSearch", "heurLS", "randLS"]

    for algoritmo in algoritmos:
        process_algorithm(algoritmo, best_fitness)

if __name__ == "__main__":
    main()