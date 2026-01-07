import matplotlib.pyplot as plt
import numpy as np

def lire_fichier_perf(nom_fichier):
    perf = {}
    with open(nom_fichier, 'r') as f:
        for ligne in f:
            if ':' not in ligne:
                continue
            cle, valeur = ligne.strip().split(':', 1)
            cle = cle.strip().lower().replace(' ', '_')
            valeur = valeur.strip()
            if not valeur:
                continue
            valeur = valeur.split()[0]
            try:
                perf[cle] = float(valeur)
            except ValueError:
                perf[cle] = 0
    return perf

def tracer_histogrammes(perfs, labels=None):
    if labels is None:
        labels = [f"Fichier {i+1}" for i in range(len(perfs))]

    cumul_alloc = [p.get("cumul_alloc", 0) for p in perfs]
    cumul_desalloc = [p.get("cumul_desalloc", 0) for p in perfs]
    max_alloc = [p.get("allocation_max", 0) for p in perfs]
    temps_exec = [p.get("temps_execution", 0) for p in perfs]

    x = np.arange(len(labels))
    width = 0.35  

    plt.figure(figsize=(12, 8))

    plt.subplot(2, 2, 1)
    plt.bar(x, cumul_alloc, color="#132F69", width=width)
    plt.xticks(x, labels, rotation=45)
    plt.title("Cumul allocations")
    plt.ylabel("Octets")

    plt.subplot(2, 2, 2)
    plt.bar(x, cumul_desalloc, color='#C8A2C8', width=width) 
    plt.xticks(x, labels, rotation=45)
    plt.title("Cumul désallocations")
    plt.ylabel("Octets")

    plt.subplot(2, 2, 3)
    plt.bar(x, max_alloc, color="#465F90", width=width)
    plt.xticks(x, labels, rotation=45)
    plt.title("Allocation max")
    plt.ylabel("Octets")

    plt.subplot(2, 2, 4)
    plt.bar(x, temps_exec, color="#672E67", width=width)
    plt.xticks(x, labels, rotation=45)
    plt.title("Temps d'exécution")
    plt.ylabel("Secondes")

    plt.tight_layout()
    plt.savefig("graphique_perf_histogramme.png")
    print("Histogrammes sauvegardés dans graphique_perf_histogramme.png")

if __name__ == "__main__":
    fichiers_perf = [
        "bilan_memoire_algo1_fichier1.txt",
        "bilan_memoire_algo2_fichier1.txt",
        "bilan_memoire_algo3_fichier1.txt"
    ]
    labels = ["Algo 1", "Algo 2", "Algo 3"]
    perfs = [lire_fichier_perf(f) for f in fichiers_perf]

    tracer_histogrammes(perfs, labels)