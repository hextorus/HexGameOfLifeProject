#include "fonctions.h"



int main(){
    int h=30;//hauteur
    int l=30;//largeur
    int N=500;//nombre de grilles stockés/initialisés
    //int repetition=50;//nombre de répétitions pour chaque set de règles
    
    //A REMETTRE A 16384 POUR VRAI
    int reglesFin = 16384; //utilise la représentation binaire pour représenter 2 bool array de 7 cases 
    //(donc utilise 14 bits)
    //bits de 0 à 6 utilisé pour reglesVivant et de 7 à 13
    //16 384=2^14
    uint8_t*result=malloc(sizeof(uint8_t)*(unsigned)(reglesFin));
    
    int coordonnesVoisins[12]={-2,0, -1,-1, 1,-1, 2,0, 1,1, -1,1};

    bool***all=creeStockageGrilles(N,h,l);

    //initialisation pour la fonction random
    srand((unsigned)time(NULL));
    //int coteCarreRandom=16;

    printf("ETAPE 0\n");


    viderGrille(all[0],h,l);
    //randomiser(all[0],coteCarreRandom,h,l);
    printf("hexSize = \n");
    int hexSize; scanf("%d",&hexSize);
    remplirInitial(all,hexSize,h,l,coordonnesVoisins);
    //printf("Quelle règle simuler ?\n");
    int regles=0;
    //scanf("%d",&regles);
    printf("Simulation sur %d\n",regles);
    //simulation
    for (int i=0;i<N-1;i++){
        etapeSuivante(all[i],all[i+1],h,l,regles,coordonnesVoisins);
    }

    explorerSimulation(all,N,h,l);

    /*
    int coupActuel=0;
    int ordre;

    while (coupActuel<N){
        printf("Entrer nb etapes à exécuter : ");
        scanf("%d",&ordre);
        for (int i=coupActuel;i<ordre+coupActuel && i<N-1;i++){
            etapeSuivante(all[i],all[i+1],h,l,regles,coordonnesVoisins);
            afficher(all[i+1],h,l);
            printf("ETAPE %d\n",i+1);
        }
        coupActuel+=ordre;
    }*/
    
    free(result);
    libereTableauMatrices(all,l,N);
    printf("FINIT\n");
    return 0;
}


