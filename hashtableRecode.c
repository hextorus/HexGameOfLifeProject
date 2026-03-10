#include "hashtableRecode.h"

//recodage de la lib de hashtab v3



hashtab *hashtab_new(){
    hashtab *t = malloc(sizeof(hashtab));
    t -> nbElem = 0;
    t -> nbCases = 64;
    t -> tab = malloc(t->nbCases * sizeof(listeCollision*));

    for (uint64_t i = 0; i < t->nbCases; i++)
        t->tab[i] = NULL;

    return t;
}



//free listeInfos
void listeInfos_free(listeInfos *li){
    listeInfos* pointeur=li;
    listeInfos* aFree;
    while (pointeur!=NULL){
        aFree=pointeur;
        pointeur=pointeur->InfoSuivante;
        free(aFree);
    }
}
//free listeCollision
void listeCollision_free(listeCollision *lc){
    listeCollision* pointeur=lc;
    listeCollision* aFree;
    while (pointeur!=NULL){
        aFree=pointeur;
        listeInfos_free(pointeur->listeInfoDuPattern);
        pointeur=pointeur->patternSuivant;
        free(aFree);
    }
}
//free hashtab
void hashtab_free(hashtab *t){

    for (uint64_t i = 0; i < t->nbCases; i++)
        listeCollision_free(t->tab[i]);

    free(t->tab);
    free(t);
}


//la seule fonction conservée depuis la première version (ici on est à v3)
uint64_t hash(uint64_t v) {
  v ^= v >> 33;
  v *= 0xff51afd7ed558ccdL;
  v ^= v >> 33;
  v *= 0xc4ceb9fe1a85ec53L;
  v ^= v >> 33;
  return v;
}




//teste si un pattern a déjà été ajouté, renvoie pointeur vers tête de liste si vrai, NULL sinon
listeCollision* hashtab_get_info(hashtab *t, uint64_t pattern){
    uint64_t h=hash(pattern);
    uint64_t idCase = h%(t->nbCases);
    listeCollision* pointeur=t->tab[idCase];

    while (pointeur!=NULL){
        if (pointeur->pattern == pattern) return pointeur;
        pointeur=pointeur->patternSuivant;
    }

    return NULL;
}

//double la taille de la hashtab
void hashtab_resize(hashtab* t){
    listeCollision** newTab = malloc(2* (t->nbCases) * sizeof(listeCollision*));
    for (uint64_t i = 0; i < 2*t->nbCases; i++)
        newTab[i] = NULL;
    
    //mettre les ancienne infos dans la nouvelle table
    for (uint64_t i = 0; i < t->nbCases; i++){
        listeCollision* nextAtraiter = t->tab[i];
        listeCollision* enTraitement;
        while (nextAtraiter!=NULL){
            enTraitement=nextAtraiter;
            nextAtraiter=nextAtraiter->patternSuivant;

            uint64_t idCase=hash(enTraitement->pattern) % (2*(t->nbCases));
            enTraitement->patternSuivant=newTab[idCase];
            newTab[idCase]=enTraitement;
        }
    }
    free(t->tab);
    t->tab=newTab;
    t->nbCases=2*t->nbCases;
}




//ajoute une info à un pattern, gere le cas si le pattern a déjà été rajouté
void hashtab_add(hashtab *t, uint64_t pattern, uint64_t infos){

    //dans tous les cas, besoin de stocker la nouvelle info
    listeInfos* newInfo=malloc(sizeof(listeInfos));
    newInfo->infos=infos;
    //newInfo->InfoSuivante à déterminer


    listeCollision* pointeur = hashtab_get_info(t,pattern);

    //si nouveau pattern -> allouer mémoire et ajouter info
    if (pointeur==NULL){
        //si remplie à moitié
        if (2*t->nbElem >= t->nbCases) hashtab_resize(t);

        t->nbElem++;
        uint64_t h=hash(pattern);

        listeCollision* newListeColli = malloc(sizeof(listeCollision));
        newListeColli->pattern=pattern;
        newListeColli->nb_elems=1;
        newListeColli->listeInfoDuPattern=newInfo;
        newInfo->InfoSuivante=NULL;
        newListeColli->patternSuivant=t->tab[h%(t->nbCases)];

        t->tab[h%(t->nbCases)]=newListeColli;
    }

    //si existe déjà -> rajouter info
    else{
        //on sait que pointeur->pattern == pattern
        newInfo->InfoSuivante=pointeur->listeInfoDuPattern;
        pointeur->listeInfoDuPattern=newInfo;
        pointeur->nb_elems++;
    }
}



//pour des tests, normalement non utilisé pour la version finale
uint64_t compteNbElements(listeInfos *linfos){
    listeInfos* pointeur=linfos;
    uint64_t count=0;
    while (pointeur!=NULL){
        count++;
        pointeur=pointeur->InfoSuivante;
    }
    return(count);
}
