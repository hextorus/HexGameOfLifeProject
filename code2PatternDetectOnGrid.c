#include "fonctions.h"
#include "raylibFunctions.h"
//pour la hashtable pour l'hexagone d'analyse
#include "hashtableRecode.h"

int main(){
    printf("ETAPE -1\n");
    int hexaSize=5;//est à 5 au max, donne 55 cellules dans ce cas, si pattern detection marche pas bien, l'un des paramètres à modifier
    FILE*parametre=fopen("argumentsPatternDetect.txt","r");
    int h;//hauteur
    int l;//largeur
    int N=10;//nombre de grilles stockés/initialisés
    int nbGeneration;
    fscanf(parametre,"h = %d\n",&h);
    fscanf(parametre,"l = %d\n",&l);
    fscanf(parametre,"nbGeneration = %d\n",&nbGeneration);

    //int reglesFin = 16384;
    //uint8_t*result=malloc(sizeof(uint8_t)*(unsigned)(reglesFin));
    int coordonnesVoisins[12]={-2,0, -1,-1, 1,-1, 2,0, 1,1, -1,1};
    bool***all=creeStockageGrilles(N,h,l);

    //srand((unsigned)time(NULL));
    srand(1); // A SUPPRIMER POUR VRAI TEST

    printf("ETAPE 0\n");

    viderGrille(all[0],h,l);
    int hexSizeINIT; fscanf(parametre,"hexSizeINIT = %d\n",&hexSizeINIT);
    
    //le code ajouté de cette version
    tableauBool*hexagoneCoo=hexagoneCoordonnes(all,hexaSize,h,l,coordonnesVoisins);
    hashtab*hashtable=hashtab_new();

/*
    //randomiserv2(all,hexSize,h,l,coordonnesVoisins);
    remplirInitial(all,hexSize,h,l,coordonnesVoisins);

    int regles; fscanf(parametre,"regle = %d\n",&regles);
*/


    viderGrille(all[0],h,l);
    randomiserv2(all,hexSizeINIT,h,l,coordonnesVoisins);
    //initialPetiteEtoilePourTestHashtable(all,h,l,coordonnesVoisins); //A SUPPR,PERMET DE TEST HASHTAB
    //genereCarreAuxCooFourniesPourTest(all[0],250,250,30,30,h,l);
    printf("Quelle règle simuler ?\n");
    int regles; fscanf(parametre,"regle = %d\n",&regles);
    int printAPartirDe; fscanf(parametre,"printAPartirDe occurrences = %d\n",&printAPartirDe);//le nombre d'occurence à partir duquel on print/traite
    int stockerAPartirDe; fscanf(parametre,"stockerAPartirDe pixels = %d\n",&stockerAPartirDe);//le nombre de pixel à partir duquel un pattern est considéré (cad stocké dans la hashtab)







    fclose(parametre);




    int id=0;//pour l'affichage de l'étape
    int idTab=id;//pour l'id de lecture dans le tableau
    //idmin non inclu borne inf
    int idmin=N-1;//pour ne pas revenir tlm en arrière qu'on boucle au futur
    int idTabPlus1;




    for (int generation=0;generation<nbGeneration;generation++){
        idTabPlus1=modAddition(idTab,1,N);

        ajouteGrilleToriqueToHashTable(all[idTab],h,l,generation,hexagoneCoo,hashtable,stockerAPartirDe);
        etapeSuivanteTorique(all[idTab],all[idTabPlus1],h,l,regles,coordonnesVoisins);


        idTab=idTabPlus1;
        if (idmin==idTab) idmin=(idmin+1)%(N);
        printf("Generation finie : %d\n",generation);
    };
    

    printf("finit boucles\n");



    //METTRE ICI L'AFFICHAGE DE LA HASHTABLE
    hashtab_print_joli(hashtable,hexagoneCoo,printAPartirDe);
    printf("fin print hashtab\n");

    
    //free(result);
    libereTableauMatrices(all,l,N);
    printf("free 1 done\n");
    freetableauBool(hexagoneCoo);
    printf("free 2 done\n");
    hashtab_free(hashtable);
    printf("free 3 done\n");
    printf("FINIT\n");
    return 0;
}


