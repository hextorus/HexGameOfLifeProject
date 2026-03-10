#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
//pour randomiser
#include <time.h>
//pour l'id à mettre dans la hashtable
#include <stdint.h>
//pour sleep
#include <unistd.h>
#include "hashtableRecode.h"
#ifndef FONCTIONS
#define FONCTIONS


//return une grille remplie de false
bool ** creeGrille(int h,int l);

bool*** creeStockageGrilles(int N,int h,int l);

void libere (bool **gri,int l);

void libereTableauMatrices (bool ***all,int l,int N);

//fonction avec boucle colonne et ligne échangé car sinon affichage sur le côté
void afficher (bool**gri,int h,int l);

void afficherFull_DEBUG (bool**gri,int h,int l);

void viderGrille(bool**gri,int h,int l);

//ne calcule pas les bords de taille 2 sur les côtés, et de taille 1 en haut et en bas
void etapeSuivante(bool**griAvant,bool**griApres,int h,int l,int regles,int*cooV);

int modAddition(int a,int b,int modulo);

void etapeSuivanteTorique(bool**griAvant,bool**griApres,int h,int l,int regles,int*cooV);

//bornes inclues
void randomiser(bool**gri,int coteCarreRandom,int h,int l);

void remplirInitial(bool***all,int tailleHexagone,int h,int l,int*cooV);

void randomiserv2(bool***all,int tailleHexagone,int h,int l,int*cooV);
//nom assez explicite
void genereCarreAuxCooFourniesPourTest(bool**grille,int x,int y,int tailleX,int tailleY,int h,int l);

int min(int a,int b);

bool aToucheMur(bool**gri,int h,int l);

void afficherTtEnregistre(bool*** all,int N,int h,int l);

void explorerSimulation(bool*** all,int N,int h,int l);

struct tableauBool{
    int x;
    int y;
    bool**tab;
};typedef struct tableauBool tableauBool;
void freetableauBool(tableauBool*tabBool);

tableauBool* hexagoneCoordonnes(bool***all,int tailleHexagone,int h,int l,int*cooV);

void printBinary (uint64_t n,int taille);

uint64_t patternTO64int(bool**grille,int x,int y,int h, int l,tableauBool*coordHexagone,int * nbcellulesEnVieDansPattern);

bool** int64TOpattern (uint64_t int64,tableauBool*coordHexagone);


struct valeurDeHashTab{
    int x;
    int y;
    int generation;
}; typedef struct valeurDeHashTab valeurDeHashTab;

uint64_t valeurDeHashTabTO64int(valeurDeHashTab* val);

void int64TOvaleurDeHashTab(uint64_t int64, valeurDeHashTab* val);

void ajoutePatternToHashTable(bool**grille,int x, int y,int h,int l,int generation,tableauBool*coordHexagone,hashtab *table,int seuil);

void ajouteGrilleToriqueToHashTable(bool**grille,int h,int l,int generation,tableauBool*coordHexagone,hashtab *table,int stockerAPartirDe);
//nom explicite
void initialPetiteEtoilePourTestHashtable(bool***all,int h,int l,int*cooV);

//print un UNIQUE pattern
void print_pattern_joli(uint64_t pattern,tableauBool*coordHexagone);
//print la hashtab avec des int64
void hashtab_print_raw(hashtab* t);
//print la hashtab visuellement
void hashtab_print_joli(hashtab* t,tableauBool*coordHexagone, int printAPartirDe);

//format de la sauvegarde :
//pattern nb_elems
//puis nb_elems lignes
void sauvegardeToTXTlisteInfos(listeCollision* tete);
//réciproque de sauvegardeToTXTlisteInfos
listeCollision* TXTlisteInfosToListeInfosTete();

//save et load de la grille

void saveTab(bool**tabASave,int h,int l, int id);
void loadTab(bool**tabALoadDedans,int h,int l, int id);


void poserEtoile(bool**tab,int x, int y);
#endif