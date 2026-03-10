#include "fonctions.h"
#include "raylibFunctions.h"


int main(){
    printf("ETAPE -1\n");
    FILE*parametre=fopen("arguments.txt","r");
    int h;//hauteur
    int l;//largeur
    int N;//nombre de grilles stockés/initialisés
    int environMemUtilisee;
    fscanf(parametre,"h = %d\n",&h);
    fscanf(parametre,"l = %d\n",&l);
    fscanf(parametre,"memoireUtilise(Mo) = %d\n",&environMemUtilisee);

    N=environMemUtilisee*1000000/(h*l);//REMETTRE 1000000 pour bon facteur
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
    int hexSize; fscanf(parametre,"hexSize = %d\n",&hexSize);
    //randomiserv2(all,hexSize,h,l,coordonnesVoisins);
    initialPetiteEtoilePourTestHashtable(all,h,l,coordonnesVoisins); //A SUPPR,PERMET DE TEST HASHTAB
    //genereCarreAuxCooFourniesPourTest(all[0],250,250,30,30,h,l);
    printf("Quelle règle simuler ?\n");
    int regles; fscanf(parametre,"regle = %d\n",&regles);



    //copié collé de raylib_renderer.c, très modifié

    printf("fin calculs, affichage\n");
    // Initialization
    int screenWidth;
    int screenHeight;
    fscanf(parametre,"screenWidth = %d\n",&screenWidth);
    fscanf(parametre,"screenHeight = %d",&screenHeight);
    fclose(parametre);


    InitWindow(screenWidth, screenHeight, "Jeu de la vie Hexagonal");

    Vector2 cameraPosInit={-(float)l/8,-(float)h/2};
    Camera2D camera = { 0 };
    camera.offset=cameraPosInit;
    camera.zoom = 1.0f;
    

    SetTargetFPS(60);

    explorerSimulationRaylibVerPLUSCalcul(all, N, h, l,&camera,regles,coordonnesVoisins);

    //fin render

    
    free(result);
    libereTableauMatrices(all,l,N);
    printf("FINIT\n");
    return 0;
}


