#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#ifndef HASHTABRECODE
#define HASHTABRECODE

//objectif : recoder de 0 hashtableModifie de zero car code illisible + problèmeS dans hashtableModifie (fuite de mémoire ainsi que code et type modifié au fur et à mesure)

//contiens infos x y et generation dans infos
struct listeInfos_ {
    uint64_t infos; //où et quand le pattern a été detecté
    struct listeInfos_* InfoSuivante; //une autre instance du pattern
};
typedef struct listeInfos_ listeInfos;

//sert également en tant que tête de liste pour un pattern et donc pour stocker des infos
struct listeCollision_ {
    uint64_t pattern; //la clé du pattern (sa représentation binaire)
    uint64_t nb_elems; //le nombre de fois que ce pattern a été détecté
    listeInfos* listeInfoDuPattern; //la liste contenant les infos de où et quand le pattern a été detecté
    
    struct listeCollision_ *patternSuivant; //un autre pattern et donc une autre tête (ou NULL)
};
typedef struct listeCollision_ listeCollision;


//hashtable personalisée
struct hashtab_ {
    uint64_t nbCases; //le nb de cases dans le tableau
    uint64_t nbElem; //combien de patterns DIFFERENTS dans la hashtab
    listeCollision** tab; //le tab de la hashtab, double pointeur pour avoir NULL si case vide
};
typedef struct hashtab_ hashtab;





//crée hashtab (arbritrairement taille init = 64)
hashtab *hashtab_new();


//free listeInfos
void listeInfos_free(listeInfos *li);
//free listeCollision
void listeCollision_free(listeCollision *lc);
//free hashtab
void hashtab_free(hashtab *t);

//ajoute une info à un pattern, gere le cas si le pattern a déjà été rajouté
void hashtab_add(hashtab *t, uint64_t pattern, uint64_t infos);

//teste si un pattern a déjà été ajouté, renvoie pointeur vers tête de liste si vrai, NULL sinon
listeCollision* hashtab_contains(hashtab *t, uint64_t pattern);

//pour des tests, normalement non utilisé pour la version finale
uint64_t compteNbElements(listeInfos *linfos);



#endif