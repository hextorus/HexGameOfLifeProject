#include "raylib.h"
#include "raymath.h"

#ifndef RAYLIBFUNCTI
#define RAYLIBFUNCTI

//reset le zoom et la pos de la camera
void resetCamera(Camera2D* camPointer);

//a appeler (avant draw) pour pouvoir zoomer
void zoom(Camera2D* camPointer);

//pour colorier 4 pixel, voir feuille pour mieux comprendre pk 4
void draw4pixel(int x, int y,Color couleur);

void afficherRaylibVer (bool**gri,int h,int l, int mode);

void explorerSimulationRaylibVer(bool*** all,int N,int h,int l,Camera2D*camPointer, int mode);

//même que l'autre mais ne necessite pas de precalculs
void explorerSimulationRaylibVerPLUSCalcul(bool*** all,int N,int h,int l,Camera2D*camera,int regles,int*coordonnesVoisins, int mode, int idLoadFrom, int idSaveTo);

#endif