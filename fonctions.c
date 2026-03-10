#include "fonctions.h"
//pour la hashtable pour l'hexagone d'analyse
#include "hashtableRecode.h"
//str pour gérer le noms de fichier
#include <string.h>
/*
recommence car je suis perdu dans l'autre code

trucs à modif
passer de 
  1 1 1
 1 1 1
1 1 1
à
01010
10101
01010
gachi de la motié de l'espace mais permettra un meilleur affichage entre autre
doit par contre def voisins pour ligne paire et ligne impaire

les cases utilisées seront les cases dont la somme des coo sont paire
contraintes :
on ne prendra que des VN pairs pour ne pas s'occuper de cas particulier : il y aura toujours VN/2 cases par ligne
VN sera la vraie taille, N le nombre de cases par ligne (VN/2)
*/
//return une grille remplie de false
bool ** creeGrille(int h,int l){
    bool **gri = malloc(sizeof(bool*)*(unsigned)l);
    for (int i=0;i<l;i++) gri[i]=malloc(sizeof(bool)*(unsigned)h);
    for (int colonne=0;colonne<l;colonne++){
        for (int ligne=0;ligne<h;ligne++){
            gri[colonne][ligne]=false;
        }
    }
    return gri;
}

bool*** creeStockageGrilles(int N,int h,int l){
    bool***all=malloc(sizeof(bool**)*(unsigned)N);
    for (int i=0;i<N;i++) all[i]=creeGrille(h,l);
    return all;
}

void libere (bool **gri,int l){
    for (int i=0;i<l;i++) free(gri[i]);
    free(gri);
}

void libereTableauMatrices (bool ***all,int l,int N){
    for (int i=0;i<N;i++) libere(all[i],l);
    free(all);
}

//fonction avec boucle colonne et ligne échangé car sinon affichage sur le côté
void afficher (bool**gri,int h,int l){
    for (int ligne=0;ligne<h;ligne++){
        for (int colonne=0;colonne<l;colonne++){
            if ((colonne+ligne)%2==0){
                if (gri[colonne][ligne]) {
                    printf("\033[0;31m");
                    printf("1");
                    printf("\033[0m");
                }
                else printf("0");
            }
            else printf(" ");
        }
        printf("\n");
    }
}

void afficherFull_DEBUG (bool**gri,int h,int l){
    for (int ligne=0;ligne<h;ligne++){
        for (int colonne=0;colonne<l;colonne++){
            if (gri[colonne][ligne]) {
                printf("\033[0;31m");
                printf("1");
                printf("\033[0m");
            }
            else printf("0");
        }
        printf("\n");
    }
}

void viderGrille(bool**gri,int h,int l){
    for (int colonne=0;colonne<l;colonne++){
        for (int ligne=0;ligne<h;ligne++){
            gri[colonne][ligne]=false;
        }
    }
}

//ne calcule pas les bords de taille 2 sur les côtés, et de taille 1 en haut et en bas
void etapeSuivante(bool**griAvant,bool**griApres,int h,int l,int regles,int*cooV){
    int sum=0;
    for (int colonne=2;colonne<l-2;colonne++){
        for (int ligne=1;ligne<h-1;ligne++){
            if (((colonne+ligne)&1)==0){
                sum=0;
                for (int v=0;v<6;v++) sum+=griAvant[colonne+cooV[v*2]][ligne+cooV[v*2+1]];

                if (griAvant[colonne][ligne]) griApres[colonne][ligne]=(regles>>sum)&1; //ça a modif pour la nouvelle représentation des règles (passage de bool array à int)
                else griApres[colonne][ligne]=(regles>>(sum+7))&1;
            }
        }
    }
}
//addition modulo N
int modAddition(int a,int b,int modulo) {
    return (((a+b)%modulo)+modulo)%modulo;
}

//même chose que la basique mais recolle les bords opposés h ET l DOIVENT ÊTRE PAIR
void etapeSuivanteTorique(bool**griAvant,bool**griApres,int h,int l,int regles,int*cooV){
    int sum=0;
    for (int colonne=0;colonne<l;colonne++){
        for (int ligne=0;ligne<h;ligne++){
            if (((colonne+ligne)&1)==0){
                sum=0;
                for (int v=0;v<6;v++) sum+=griAvant[modAddition(colonne,cooV[v*2],l)][modAddition(ligne,cooV[v*2+1],h)];
                if (griAvant[colonne][ligne]) griApres[colonne][ligne]=(regles>>sum)&1; //ça a modif pour la nouvelle représentation des règles (passage de bool array à int)
                else griApres[colonne][ligne]=(regles>>(sum+7))&1;
            }
        }
    }
}

//bornes inclues (ancien randomiser dans carre) NE PLUS UTILISER
void randomiser(bool**gri,int coteCarreRandom,int h,int l){
    for (int colonne=(l-coteCarreRandom)/2;colonne<(l+coteCarreRandom)/2;colonne++){
        for (int ligne=(h-coteCarreRandom)/2;ligne<(h+coteCarreRandom)/2;ligne++){
            if ((colonne+ligne)%2==0){
                gri[colonne][ligne]=rand()&1;
            }
        }
    }
}


void copie(bool**init,bool**cible,int h, int l){
    for (int colonne=0;colonne<l;colonne++){
        for (int ligne=0;ligne<h;ligne++){
            cible[colonne][ligne]=init[colonne][ligne];
        }
    }
}


//pour la génération dans hexagone, se sert de la seconde grille comme buffer
//stocke resultat dans all[0]
//note : si les grille de all sont trop petite, le pattern en sortie "suinte", voir testTIPE13 ; ce problème n'arrive pas dans mon cas car la grille est bcp plus grosse que l'hexagone de détection
void remplirInitial(bool***all,int tailleHexagone,int h,int l,int*cooV){
    //pas besoin de check toute la grille car l'hexagone à l'étape i tien dans un carré de côté 2*i+5
    int idLecture=0;
    int idEcriture=1;
    int sum=0;
    //coloriage initial
    int milieuX=l/2;
    int milieuY=h/2;
    //truc que j'avais oublié initialement, FAIRE ATTENTION QUE LES MILIEUX SOIENT PAIR
    if ((milieuX&1)==1) milieuX+=1;
    if ((milieuY&1)==1) milieuY+=1;

    all[0][milieuX][milieuY]=true;
    for (int v=0;v<6;v++) all[0][milieuX+cooV[v*2]][milieuY+cooV[v*2+1]]=true;

    //boucle, coloration si 2 voisins au min
    for (int i=1;i<=tailleHexagone;i++){
        for (int colonne=milieuX-i-3;colonne<=milieuX+i+3;colonne++){
            for (int ligne=milieuY-i-3;ligne<=milieuY+i+3;ligne++){
                if (((colonne+ligne)&1)==0){
                    if (all[idLecture][colonne][ligne]) all[idEcriture][colonne][ligne]=true;
                    else{
                        sum=0;
                        for (int v=0;v<6;v++) sum+=all[idLecture][colonne+cooV[v*2]][ligne+cooV[v*2+1]];
                        if (sum>=2) all[idEcriture][colonne][ligne]=true;
                    }
                }
            }
        }
        //inversion lecture ecriture
        if (idLecture==0){
            idLecture=1;
            idEcriture=0;
        }
        else{
            idLecture=0;
            idEcriture=1;
        }
        //printf("%d\n",i);afficher(all[idLecture],h,l);printf("\n");
    }
    //copie du res dans la 0 et vidage de la 1
    if (idEcriture==0) copie (all[1],all[0],h,l);
    viderGrille(all[1],h,l);
}

//randomise mtn dans un hexagone
void randomiserv2(bool***all,int tailleHexagone,int h,int l,int*cooV){
    remplirInitial(all,tailleHexagone,h,l,cooV);
    for (int colonne=0;colonne<l;colonne++){
        for (int ligne=0;ligne<h;ligne++){
            if((colonne+ligne)%2==0 && all[0][colonne][ligne]){
                all[1][colonne][ligne]=rand()&1;
            }
        }
    }
    copie(all[1],all[0],h,l);
    viderGrille(all[1],h,l);
}

void genereCarreAuxCooFourniesPourTest(bool**grille,int x,int y,int tailleX,int tailleY,int h,int l){
    for (int colonne=x;colonne<x+tailleX;colonne++){
        for (int ligne=y;ligne<y+tailleY;ligne++){
            if ((colonne+ligne)%2==0){
                grille[colonne%l][ligne%h]=true;
            }
        }
    }
}

int min(int a,int b){
    if (a>b) return b;
    return a;
}

int max(int a,int b){
    if (a<b) return b;
    return a;
}

bool aToucheMur(bool**gri,int h,int l){
    //verif en haut et en bas les ligne offset 1
    for (int i=2;i<l-2;i++){
        if(gri[i][1]||gri[i][h-2]) return true;
    }
    //verif gauche et droite les colonnes offset 2
    for (int i=1;i<h-1;i++){
        if(gri[2][i]||gri[l-3][i]) return true;
    }
    //else
    return false;
}

void afficherTtEnregistre(bool*** all,int N,int h,int l){
    for (int i=0;i<N;i++) {
        afficher(all[i],h,l);
        printf("Etape : %d\n",i);
        usleep(200000);//microsecl
    }
}

void explorerSimulation(bool*** all,int N,int h,int l){
    //afficherTtEnregistre(all,N,h,l);
    int id=0;
    int commande=0;
    while (commande!=2){
        afficher(all[id],h,l);
        printf("Etape : %d, command : ",id);
        scanf("%d",&commande);
        if (commande==0 && id<N-1)id++;
        if (commande==1 && id>0)id--;
    }
}


/*
int nb_aleatoire_entre(int min, int max) {
    double x = rand() / (RAND_MAX + 1.); // conversion en flottant car valeur trop grandes
    return min + (int)((max - min) * x); // reconversion en int
}

void randomiser(bool**gri,int N,int debut,int fin){
    for (int ligne=debut+1;ligne<N-1-fin;ligne++){
        for (int colone=debut+1;colone<N-1-fin;colone++){
            gri[ligne][colone]=rand()&1;
        }
    }
}

void afficherListe(int*li,int n){
    for (int i=0;i<n;i++) printf("%d",li[i]);
    printf("\n");
}
*/




void freetableauBool(tableauBool*tabBool){
    for(int i=0;i<tabBool->x;i++){
        free(tabBool->tab[i]);
    }
    free(tabBool->tab);
    free(tabBool);
}

//A APPELER AVANT SIMULATION ET GENERATION INIT
//renvoie un tableau contenant des bool pour savoir si une case appartient à l'hexagone de taille demandée
//a appeler avant simulation car ecrase les contenus des 2 premiers tableaux
//pour pattern detection : l'hexagone a une taille 5 au max (cela donne 55 cellules)
tableauBool* hexagoneCoordonnes(bool***all,int tailleHexagone,int h,int l,int*cooV){
    remplirInitial(all,tailleHexagone,h,l,cooV);
    //afficherFull_DEBUG(all[0],h,l);
    //premier scan pour savoir la taille du tableau à reserver
    int xmin=l-1;
    int ymin=h-1;
    int xmax=0;
    int ymax=0;
    for (int colonne=0;colonne<l;colonne++){
        for (int ligne=0;ligne<h;ligne++){
            if ((colonne+ligne)%2==0){
                if (all[0][colonne][ligne]){
                    xmin=min(xmin,colonne);
                    ymin=min(ymin,ligne);
                    xmax=max(xmax,colonne);
                    ymax=max(ymax,ligne);
                }
            }
        }
    }

    int sizeX=xmax-xmin+1;
    int sizeY=ymax-ymin+1;

    //problème possible : grille est sur la grille impaire au lieu de pair (c'est ce qui ce passe dans le cas tailleHexagone=5), dans ce cas on rajoute une ligne vide en haut
    int decallageY=0;
    if ((xmin+ymin)%2==1) {sizeY++; decallageY++;}

    bool**tabRet=malloc(sizeof(bool*)*(unsigned)(sizeX));
    for (int i=0;i<sizeX;i++) tabRet[i]=malloc(sizeof(bool)*(unsigned)(sizeY));

    //ne pas oublier de vider la mémoire réservée AHHHHHHHHHHH J'AI PRIS AU MOINS 2 MOIS AVANT DE VOIR CA
    for (int i=0;i<sizeX;i++) for (int j=0;j<sizeY;j++) tabRet[i][j]=0;
    
    for (int colonne=xmin;colonne<=xmax;colonne++){
        for (int ligne=ymin;ligne<=ymax;ligne++){
            if ((colonne+ligne)%2==0){
                tabRet[colonne-xmin][ligne-ymin+decallageY]=all[0][colonne][ligne];
            }
        }
    }
    tableauBool* fulltab=malloc(sizeof(tableauBool));
    fulltab->tab=tabRet;
    fulltab->x=sizeX;
    fulltab->y=sizeY;
    return fulltab;
}


void printBinary (uint64_t n,int taille){
    for (int i=0; i<taille;i++){
        printf("%llu",(n>>(taille-1-i))&1);
    }
    printf("\n");
}



//l'hexagone a une taille 5 au max (cela donne 55 cellules)
//le dernier paramètre est le nombre de cellules allumées pour savoir si on supprime ou pas (ptete que ne sera pas utilisé au final)
uint64_t patternTO64int(bool**grille,int x,int y,int h, int l,tableauBool*coordHexagone,int * nbcellulesEnVieDansPattern){
    *nbcellulesEnVieDansPattern=0;
    uint64_t result=0;
    int finBouclex=x+coordHexagone->x;
    int finBoucley=y+coordHexagone->y;
    for (int colonne=x;colonne<finBouclex;colonne++){
        for (int ligne=y;ligne<finBoucley;ligne++){
            if (((colonne+ligne)&1)==0 && coordHexagone->tab[(colonne-x)%l][(ligne-y)%h]){
                result=result<<1;
                if (grille[colonne%l][ligne%h]){
                    //décale et rajoute un 1
                    result=result|1;
                    *nbcellulesEnVieDansPattern+=1;
                }
            }
        }
    }
    return result;
}

//coordHexagone doit venir de hexagoneCoordonnes
bool** int64TOpattern (uint64_t int64,tableauBool*coordHexagone){
    bool**tab=malloc(sizeof(bool*)*(unsigned)(coordHexagone->x));
    for (int i=0;i<coordHexagone->x;i++) tab[i]=malloc(sizeof(bool)*(unsigned)(coordHexagone->y));

    for (int colonne=coordHexagone->x-1;colonne>=0;colonne--){
        for (int ligne=coordHexagone->y-1;ligne>=0;ligne--){
            tab[colonne][ligne]=false;

            if (((colonne+ligne)&1)==0 && coordHexagone->tab[colonne][ligne]){
                if (int64&1){
                    tab[colonne][ligne]=true;
                }
                int64=int64>>1;
            }
        }
    }
    return tab;
}




//les int sont stocké sur 4 octets (4 294 967 296) mais on fera la supposition raisonnable que l et h ne dépasseront pas 65 536 (2 octets)
//seront stocké dans l'ordre : 2 octets pour x, 2 pour y et 4 pour generation
uint64_t valeurDeHashTabTO64int(valeurDeHashTab*val){
    uint64_t res=0;
    res=res|(val->x & 65535);
    res=res<<16;
    res=res|(val->y & 65535);
    res=res<<32;
    res=res|(val->generation & 4294967295);
    return res;    
}

void int64TOvaleurDeHashTab(uint64_t int64, valeurDeHashTab* val){
    val->generation=(int)(int64&4294967295);
    int64=int64>>32;
    val->y=int64&65535;
    int64=int64>>16;
    val->x=int64&65535;
}



//la clé est le pattern et les valeurs sont x y et generation
//n'ajoute pas si on essaie d'ajouter un pattern avec un nb de cellules en vie strictement inférieur à seuil
void ajoutePatternToHashTable(bool**grille,int x, int y,int h, int l,int generation,tableauBool*coordHexagone,hashtab *table,int seuil){
    int nbcellulesEnVieDansPattern=0;
    uint64_t cle=patternTO64int(grille,x,y,h,l,coordHexagone,&nbcellulesEnVieDansPattern);
    // on supprime le cas vide car il prendra de la place inutile sinon 
    // on supprime également le cas pattern full, n'est pas censé arriver car blacklist les règles qui font naitre à 0 voisins mais je le met pour de tests manuels
    if (nbcellulesEnVieDansPattern<seuil ||nbcellulesEnVieDansPattern==55) return;//||nbcellulesEnVieDansPattern==55 DONE ADD IT BACK
    valeurDeHashTab val={x,y,generation};
    hashtab_add(table,cle,valeurDeHashTabTO64int(&val));
}

void ajouteGrilleToriqueToHashTable(bool**grille,int h,int l,int generation,tableauBool*coordHexagone,hashtab *table,int stockerAPartirDe){
    for (int colonne=0;colonne<l;colonne++){
        for (int ligne=0;ligne<h;ligne++){
            if (((colonne+ligne)&1)==0){
                ajoutePatternToHashTable(grille,colonne,ligne,h,l,generation,coordHexagone,table,stockerAPartirDe);
            }
        }
    }
}

void initialPetiteEtoilePourTestHashtable(bool***all,int h,int l,int*cooV){
    //coloriage initial
    int milieuX=l/2;
    int milieuY=h/2;
    all[0][milieuX][milieuY]=true;
    for (int v=0;v<3;v++) all[0][milieuX+cooV[v*2*2]][milieuY+cooV[(v*2*2+1)]]=true;
}


//format de la sauvegarde :
//pattern nb_elems
//puis nb_elems lignes
void sauvegardeToTXTlisteInfos(listeCollision* tete){
    FILE* fichier = fopen("sauvegarde des infos d'un pattern.txt", "w");
    fprintf(fichier,"%lld %lld\n",tete->pattern,tete->nb_elems);
    listeInfos* pointeur=tete->listeInfoDuPattern;
    while (pointeur!=NULL){
        fprintf (fichier,"%lld\n",pointeur->infos);
        pointeur=pointeur->InfoSuivante;
    }
    fclose(fichier);
}


//les 3 fonctions suivantes ont besoin d'être ici au lieu de dans hashtableRecode à cause de pb de compil

void print_pattern_joli(uint64_t pattern,tableauBool*coordHexagone){
    bool**grillePat=int64TOpattern(pattern,coordHexagone);
    afficher(grillePat,coordHexagone->y,coordHexagone->x);
    libere(grillePat,coordHexagone->x);
}
  
void _listeUnSeulPattern_print_joli(listeInfos* li){
    valeurDeHashTab val2Hash;
    int compteur=0;
    while (li !=NULL){
        compteur++;printf("info n° %d ; ",compteur);
        int64TOvaleurDeHashTab(li->infos,&val2Hash);
        printf("x : %d, y : %d, gen : %d\n",val2Hash.x,val2Hash.y,val2Hash.generation);
        li=li->InfoSuivante;
    }
}
  
void hashtab_print_joli(hashtab* t,tableauBool*coordHexagone,int printAPartirDe){
    for (uint64_t i = 0; i < t->nbCases; i++) {
        listeCollision *tete = t -> tab[i];
        while (tete != NULL) {
            if (tete->nb_elems >= (unsigned)printAPartirDe){
                printf("pattern : %lld\n",tete->pattern);
                printf("detecte : %lld\n",tete->nb_elems);
                //printf("Nb Elems compté : %lld\n", compteNbElements(tete->listeInfoDuPattern)); //plus nécessaire, cette fois-ci j'ai bien codé la hashtable (ou au moins je ne vois pas les effets des problèmes)
                print_pattern_joli(tete->pattern,coordHexagone);
                
                //_listeUnSeulPattern_print_joli(l->listeDuPattern);
                printf("\n");
                //sauvegardeToTXTlisteInfos(tete);//ajouté pour des tests TODO a mettre en commentaire pour version finale
            }
            tete = tete->patternSuivant;
        }
    }
}


//réciproque de sauvegardeToTXTlisteInfos
listeCollision* TXTlisteInfosToListeInfosTete(){
    FILE* fichier = fopen("sauvegarde des infos d'un pattern.txt", "r");
    listeCollision*res=malloc(sizeof(listeCollision));
    uint64_t pattern;
    uint64_t nb_elems;
    uint64_t infos;
    fscanf(fichier, "%llu %llu\n",&pattern,&nb_elems);
    res->nb_elems=nb_elems;
    res->pattern=pattern;
    res->patternSuivant=NULL;
    res->listeInfoDuPattern=NULL;
    for (uint64_t i=0;i<nb_elems;i++){
        listeInfos* pointeur=malloc(sizeof(listeInfos));
        fscanf(fichier,"%llu\n",&infos);
        pointeur->infos=infos;
        pointeur->InfoSuivante=res->listeInfoDuPattern;
        res->listeInfoDuPattern=pointeur;
    }
    fclose(fichier);
    return res;
}


//les fonctions suivantes sont pour sauvegarder et load des patterns (je viens de rajouter le moyen de modifier manuellement les cellules, j'ai donc besoin de ça pour pouvoir faire encore plus de chose)

//id est à rajouter à la fin du fichier pour pouvoir en stocker plusieurs
void saveTab(bool**tabASave,int h,int l, int id){
    char*charBuffer=malloc(sizeof(char)*100);
    sprintf(charBuffer,"patternSave/PatternID_%d",id);
    FILE* fichier = fopen(charBuffer,"w");
    for (int colonne=0;colonne<l;colonne++){
        for (int ligne=0;ligne<h;ligne++){
            fprintf(fichier,"%d ",tabASave[colonne][ligne]);
        }
    }
    fclose(fichier);
}

void loadTab(bool**tabALoadDedans,int h,int l, int id){
    char*charBuffer=malloc(sizeof(char)*100);
    sprintf(charBuffer,"patternSave/PatternID_%d",id);
    FILE* fichier = fopen(charBuffer,"r");
    int info;
    for (int colonne=0;colonne<l;colonne++){
        for (int ligne=0;ligne<h;ligne++){
            fscanf(fichier,"%d ",&info);
            tabALoadDedans[colonne][ligne]=(bool)info;
        }
    }
    fclose(fichier);
}

//l'étoile semble être une bonne brique de base, je fais donc une fonction pour pouvoir la coller facilement
//WARNING : ne pas utiliser au bord car pas de test de validité
//            1
//         1 1 1 1
//          1 0 1
//         1 1 1 1
//            1
//est également sa réciproque
void poserEtoile(bool**tab,int x, int y){
    //ligne par ligne
    tab[x][y-2]=!tab[x][y-2];
    tab[x-3][y-1]=!tab[x-3][y-1];
    tab[x-1][y-1]=!tab[x-1][y-1];
    tab[x+1][y-1]=!tab[x+1][y-1];
    tab[x+3][y-1]=!tab[x+3][y-1];
    tab[x-2][y]=!tab[x-2][y];
    
    tab[x+2][y]=!tab[x+2][y];
    tab[x-3][y+1]=!tab[x-3][y+1];
    tab[x-1][y+1]=!tab[x-1][y+1];
    tab[x+1][y+1]=!tab[x+1][y+1];
    tab[x+3][y+1]=!tab[x+3][y+1];
    tab[x][y+2]=!tab[x][y+2];
}