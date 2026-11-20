#include "monde.hpp"

struct habitant* creation_habitant(char* nom, int suffixe) {
    struct habitant* h = (struct habitant*) malloc(sizeof(struct habitant));
    h->age = rand()%50;
    sprintf(h->nom, "%d", suffixe);
    strcat(h->nom,nom);
    h->revenu = revenu_min + ((float)rand()/(float)RAND_MAX)*(float)(revenu_max-revenu_min);
    return h;
}

void lecture(const char* filename, int* nb_iles, struct ile** ile_info, int** mat_adj) {
    ifstream f(filename);
    // Lecture du nombre d'îles
    f >> *nb_iles;
    *ile_info = new struct ile[*nb_iles];
    // Lecture des informations sur les îles (numéro, nom, nombre d'habitants)
    for (int i=0; i<*nb_iles; i++) {
        f >> (*ile_info)[i].num >> (*ile_info)[i].nom >> (*ile_info)[i].nb_hab;
    }
    //matrice d'adjacence pour stocker les ponts
    *mat_adj = (int*)calloc(*nb_iles**nb_iles,sizeof(int));
    int nb_ponts, ile1, ile2;
    f >> nb_ponts;
    for (int i=0; i<nb_ponts; i++) {
        f >> ile1 >> ile2;
        (*mat_adj)[ile1*(*nb_iles)+ile2] = 1;
        (*mat_adj)[ile2*(*nb_iles)+ile1] = 1;
    }

}

struct monde* generation(const char* filename) {
    
    struct monde* m = (struct monde*)malloc(sizeof(struct monde));
    
    ifstream f(filename);

    f >> m->nb_iles;
    int nb = m->nb_iles;
    m->mes_iles = (struct ile**)malloc(m->nb_iles*sizeof(struct ile*));


    for (int i=0; i<nb; i++) {
        m->mes_iles[i] = (struct ile*)malloc(sizeof(struct ile));
        f>>(m->mes_iles[i])->num>>(m->mes_iles[i])->nom >> (m->mes_iles[i])->nb_hab;
        m->mes_iles[i]->liste_hab = (struct habitant**)malloc(m->mes_iles[i]->nb_hab*sizeof(struct habitant*));
        for (int j=0; j<m->mes_iles[i]->nb_hab; j++)
            m->mes_iles[i]->liste_hab[j] = creation_habitant(m->mes_iles[i]->nom,j);
    }

    m->mat_adj = (int*)calloc(nb*nb,sizeof(int));

    int nb_arete;
    f>>nb_arete;

    int a, b;
    for (int i=0; i<nb_arete; i++) {
        f>>a>>b;
        m->mat_adj[a*nb+b] = 1;
        m->mat_adj[b*nb+a] = 1;
    }

    m->inflation = inflation_initiale;

    return m;    
}



   



void affichage_ile(struct ile* ile) {
    cout << ile->nom << "->" << ile->nb_hab << " habitants : ";
    for (int i=0; i<ile->nb_hab; i++) {
        affichage_habitant(ile->liste_hab[i]);
        cout << endl;
    }
}

void affichage_monde(struct monde* m) {
    cout << m->nb_iles << " iles dans ce monde :" << endl;
    for (int i=0; i<m->nb_iles; i++)
        affichage_ile(m->mes_iles[i]);
    cout << "connectivité : " << endl;
    for (int i=0; i<m->nb_iles; i++) {
        for (int j=0; j<m->nb_iles; j++)
            cout << m->mat_adj[i*m->nb_iles+j] << " ";
        cout << endl;
    }
}

void affichage_habitant(struct habitant* h) {
    cout << h->nom << " \t" << h->age << " ans \t(" << h->revenu << "€)"; 
}

void liberer_monde(struct monde** m){
    for (int i=0; i<(*m)->nb_iles; i++) {
        free((*m)->mes_iles[i]);
        (*m)->mes_iles[i] = NULL;
    }
    free((*m)->mes_iles);
    (*m)->mes_iles = NULL;
    free((*m)->mat_adj);
    (*m)->mat_adj = NULL;
    free(*m);
    *m=NULL;
}