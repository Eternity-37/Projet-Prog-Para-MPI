#include <iostream>
#include <mpi.h>

#include "monde.hpp"



// Répartition de l'île i suivant le nombre d'îles et le nombre de processus
// Retourne quel processus doit gérer l'île i
// i est l'indice de l'île, nb_iles est le nombre total d'îles, nprocs est le nombre de processus
int repartition_ile(int i, int nb_iles, int nprocs) {
    int nb_iles_par_proc_de_base = nb_iles/nprocs;
    int reste = nb_iles % nprocs;
    if (i < reste * (nb_iles_par_proc_de_base + 1)) {
        return i / (nb_iles_par_proc_de_base + 1);
    } else {
        return (i - reste * (nb_iles_par_proc_de_base + 1)) / nb_iles_par_proc_de_base + reste;
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int pid, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    char* nom_fichier = argv[1];
    int root = atoi(argv[2]);

    int nb_iles;

    ile* iles;
    int* mat_adj;

    if (pid == root) {
        lecture(nom_fichier, &nb_iles, &iles, &mat_adj);
        cout << "Fichier : " << nom_fichier << endl;
        cout << "Processus racine : " << root << endl;
    }
    // Diffusion du nombre d'îles à tous les processus
    MPI_Bcast(&nb_iles, 1, MPI_INT, root, MPI_COMM_WORLD);
    // Allocation de la matrice d'adjacence sur tous les processus
    if (pid != root) {
        mat_adj = (int*)calloc(nb_iles*nb_iles,sizeof(int));
    }
    // Diffusion de la matrice d'adjacence à tous les processus
    MPI_Bcast(mat_adj, nb_iles*nb_iles, MPI_INT, root, MPI_COMM_WORLD);


    MPI_Scatterv(iles, sizeof(ile), MPI_BYTE, NULL, sizeof(ile), MPI_BYTE, root, MPI_COMM_WORLD);

    struct monde* mm = generation("monde1.txt");
    
    // affichage_monde(mm);

    liberer_monde(&mm);

    if (pid == root) {
        delete[] iles;
        delete[] mat_adj;
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}