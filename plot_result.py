#!/usr/bin/env python3
import os
import pandas as pd
import matplotlib.pyplot as plt

def plot_global_by_size():
    """
    Lee la tabla global por tamaño (global_by_size.txt) y genera gráficos
    que muestran la evolución de la desviación y el tiempo en función del tamaño
    para cada algoritmo.
    """
    file_path = os.path.join("output", "tables", "global_by_size.txt")
    if not os.path.exists(file_path):
        print(f"No se encontró el fichero: {file_path}")
        return
    
    # Se asume que el fichero está separado por tabuladores y tiene columnas:
    # Algoritmo, Tamaño, Desv, Tiempo
    df = pd.read_csv(file_path, sep="\t")
    
    # Convertir 'Tamaño' a numérico (en caso de que se lea como objeto)
    df['Tamaño'] = pd.to_numeric(df['Tamaño'], errors='coerce')
    
    # Ordenar por tamaño para cada algoritmo
    df.sort_values(by=['Algoritmo', 'Tamaño'], inplace=True)
    
    # Crear dos subplots: uno para la desviación y otro para el tiempo
    fig, axs = plt.subplots(1, 2, figsize=(14, 6))
    
    # Gráfico de Desviación
    for alg in df['Algoritmo'].unique():
        sub_df = df[df['Algoritmo'] == alg]
        axs[0].plot(sub_df['Tamaño'], sub_df['Desv'], marker='o', label=alg)
    axs[0].set_title("Desviación vs Tamaño")
    axs[0].set_xlabel("Tamaño")
    axs[0].set_ylabel("Desviación (%)")
    axs[0].legend()
    axs[0].grid(True)
    
    # Gráfico de Tiempo
    for alg in df['Algoritmo'].unique():
        sub_df = df[df['Algoritmo'] == alg]
        axs[1].plot(sub_df['Tamaño'], sub_df['Tiempo'], marker='o', label=alg)
    axs[1].set_title("Tiempo vs Tamaño")
    axs[1].set_xlabel("Tamaño")
    axs[1].set_ylabel("Tiempo (s)")
    axs[1].legend()
    axs[1].grid(True)
    
    plt.tight_layout()
    plt.show()

def plot_global_totals():
    """
    Lee la tabla global total (global_totals.txt) y genera un gráfico de barras
    que compara la desviación y el tiempo promedio para cada algoritmo.
    """
    file_path = os.path.join("output", "tables", "global_totals.txt")
    if not os.path.exists(file_path):
        print(f"No se encontró el fichero: {file_path}")
        return

    # Se asume que el fichero está separado por tabuladores y tiene columnas:
    # Algoritmo, Desv, Tiempo
    df = pd.read_csv(file_path, sep="\t")
    
    # Crear un gráfico de barras agrupado para Desviación y Tiempo
    fig, ax = plt.subplots(figsize=(8,6))
    
    # Configuración de los datos para el gráfico
    algorithms = df['Algoritmo']
    desv = df['Desv']
    tiempo = df['Tiempo']
    
    x = range(len(algorithms))
    width = 0.35  # ancho de las barras
    
    # Barras para Desviación y Tiempo
    bars1 = ax.bar([p - width/2 for p in x], desv, width, label='Desv (%)')
    bars2 = ax.bar([p + width/2 for p in x], tiempo, width, label='Tiempo (s)')
    
    ax.set_xlabel("Algoritmo")
    ax.set_ylabel("Valor medio")
    ax.set_title("Resultados Globales Totales en el MDD")
    ax.set_xticks(x)
    ax.set_xticklabels(algorithms)
    ax.legend()
    ax.grid(axis="y")
    
    # Mostrar valores encima de cada barra (opcional)
    for bar in bars1 + bars2:
        height = bar.get_height()
        ax.annotate(f'{height:.2f}',
                    xy=(bar.get_x() + bar.get_width() / 2, height),
                    xytext=(0, 3),  # desplazamiento vertical
                    textcoords="offset points",
                    ha='center', va='bottom')
    
    plt.tight_layout()
    plt.show()

def main():
    plot_global_by_size()
    plot_global_totals()

if __name__ == "__main__":
    main()
