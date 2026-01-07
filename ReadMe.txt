**Analyseur de mots et bilan mémoire**

Ce programme en C lit un ou plusieurs fichiers texte et génère un compte des mots ainsi qu’un bilan mémoire des allocations utilisées par le programme.
Il permet d’utiliser trois algorithmes différents pour stocker et compter les mots, et d’afficher les mots les plus fréquents.

---

**Compilation**

Pour compiler le programme :

gcc main.c -o nom_executable

On peut choisir n’importe quel nom pour l’exécutable.

---

**Exécution**

./nom_executable [ALGO] fichier1.txt [fichier2.txt ...] [-n N]

**Arguments :**

1. **[ALGO] (optionnel)** : choix de l’algorithme.

   * -1 pour l'algorithme 1
   * -2 pour l'algorithme 2
   * -3 pour l'algorithme 3
     Par défaut, l'algorithme 1 est utilisé.

2. **[fichier.txt]** : un ou plusieurs fichiers texte à analyser, qui peuvent avoir n'importe quel nom.

3. **[-n N] (optionnel)** : nombre de mots à afficher.
   Par défaut, N = 5.
   On peut le modifier avec cet argument.

---

**Fichiers générés**

1. **resultats.txt** : contient la liste des mots et le nombre d’occurrences.
2. **bilan_memoire_algoX_fichierY.txt** : contient un résumé des allocations mémoire pour l’algorithme utilisé.

---

**Conseils**

* Pour analyser de gros fichiers, privilégiez l’algorithme 3.
* Pour un petit fichier, l’algorithme 1 ou 2 suffit.

---

**Comment utiliser le bilan.py ?**

Il suffit d’exécuter le programme main.c une fois par algorithme pour avoir assez de données.
*(Veillez à bien utiliser les mêmes fichiers à chaque test pour ne pas fausser les résultats.)*

Ensuite, vous pourrez exécuter le fichier bilan.py qui affichera :

* les temps d’exécution,
* le nombre maximal d’allocations simultanées,
* le nombre total d’allocations et de désallocations,
* l’histogramme des 20 mots les plus utilisés.