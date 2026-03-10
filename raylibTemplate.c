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
    N=environMemUtilisee*1000000/(h*l);

    int reglesFin = 16384;
    uint8_t*result=malloc(sizeof(uint8_t)*(unsigned)(reglesFin));
    int coordonnesVoisins[12]={-2,0, -1,-1, 1,-1, 2,0, 1,1, -1,1};
    bool***all=creeStockageGrilles(N,h,l);

    srand((unsigned)time(NULL));

    printf("ETAPE 0\n");

    viderGrille(all[0],h,l);
    int hexSize; fscanf(parametre,"hexSize = %d\n",&hexSize);
    
    //randomiserv2(all,hexSize,h,l,coordonnesVoisins);
    remplirInitial(all,hexSize,h,l,coordonnesVoisins);

    int regles; fscanf(parametre,"regle = %d\n",&regles);

    //affichage
    int screenWidth;
    int screenHeight;
    fscanf(parametre,"screenWidth = %d\n",&screenWidth);
    fscanf(parametre,"screenHeight = %d",&screenHeight);


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
    fclose(parametre);
    printf("FINIT\n");
    return 0;
}


