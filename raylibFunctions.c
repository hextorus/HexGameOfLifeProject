#include "raylibFunctions.h"
#include "fonctions.h"

//reset le zoom et la pos de la camera
void resetCamera(Camera2D* camera){
    camera->zoom = 1.0f;
    camera->target = Vector2Zero();
    camera->offset = Vector2Zero();
}

void zoom(Camera2D* camera){
    //code pris de l'exemple de zoom sur balle jaune du site officiel, modifié en remplaçant par des pointeurs
    // Translate based on mouse right click
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f/camera->zoom);
        camera->target = Vector2Add(camera->target, delta);
    }

    // Zoom based on mouse wheel
    float wheel = GetMouseWheelMove();
    if (wheel != 0)
    {
        // Get the world point that is under the mouse
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), *camera);

        // Set the offset to where the mouse is
        camera->offset = GetMousePosition();

        // Set the target to match, so that the camera maps the world space point 
        // under the cursor to the screen space point under the cursor at any zoom
        camera->target = mouseWorldPos;

        // Zoom increment
        float scaleFactor = 1.0f + (0.25f*fabsf(wheel));
        if (wheel < 0) scaleFactor = 1.0f/scaleFactor;
        camera->zoom = Clamp(camera->zoom*scaleFactor, 0.125f, 64.0f);
    }

}

//première version d'affichage simple, sert à tester rapidement
void draw4pixel(int x, int y,Color couleur){
    //voir feuille 2 pour pk ces formules
    int xToDraw=x;
    int yToDraw=y*2;
    //Color col={x*5453,y*5453,x+y,100};
    //Color col={rand(),rand(),rand(),100};
    DrawRectangle(xToDraw,yToDraw,2,2,couleur);
}

//note si veut revenir aux premiers coeffs x : 1 et y : 1.8
//ces coeffs permettent des cases parfaitement collées les unes aux autres x : cos(PI/6) et y : 1.5
//mais donc ne permet pas de différencier les cellules les unes des autres dans un amas de cellules vivantes

#define CONSTANTERAYLIB 1.5/cos(PI/6)
//version plus jolie pour hexagone, sert à faire des images pour la présentation
void betterDraw(int x, int y,Color couleur){
    Vector2 center;
    center.x=(float)x;
    center.y=(float)(y*CONSTANTERAYLIB);
    //DrawPolyLines(center,6,1,90,couleur); //pour débugger
    DrawPoly(center,6,(float)1.05,90,couleur); //vraie version
}

//betterDraw, mais hexagone vide
void betterDrawVide(int x, int y,Color couleur){
    Vector2 center;
    center.x=(float)x; //angle de 30°
    center.y=(float)(y*CONSTANTERAYLIB);
    //DrawPolyLines(center,6,1,90,couleur); //pour débugger
    DrawPolyLines(center,6,(float)1.05,90,couleur); //vraie version
}

//betterDrawVide, mais pour les visuels de la présentation (slide 28)
void betterDrawVidePourPresentation(int x, int y,Color couleur){
    Vector2 center;
    center.x=(float)x;
    center.y=(float)(y*CONSTANTERAYLIB);
    //DrawPolyLines(center,6,1,90,couleur); //pour débugger
    DrawPolyLinesEx(center,6,(float)7.6,0.5,1,couleur); //vraie version
}


//appelle draw4pixel sur la grille en argument
void afficherRaylibVer (bool**gri,int h,int l, int mode){
    for (int ligne=0;ligne<h;ligne++){
        for (int colonne=0;colonne<l;colonne++){
            if ((colonne+ligne)%2==0){
                if (gri[colonne][ligne]) {
                    if (mode==0) draw4pixel(colonne,ligne,BLACK);
                    if (mode>0) betterDraw(colonne,ligne,BLACK);
                }
                //else draw4pixel(colonne,ligne,RAYWHITE); //pas besoin, fait juste lagger pour rien
                if (mode==2) betterDrawVide(colonne,ligne,BLACK);
            }
        }
    }
    //betterDrawVidePourPresentation(l/2+2,h/2,RED);
}


void explorerSimulationRaylibVer(bool*** all,int N,int h,int l,Camera2D*camera,int mode){
    //afficherTtEnregistre(all,N,h,l);
    int id=0;
    bool fastmode=false;//pour pouvoir laisser une touche enfoncée

    //pour pouvoir formater
    //char*charBuffer=malloc(sizeof(char)*100);



    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ZERO)) resetCamera(camera);

        zoom(camera);


        
        BeginDrawing();
            BeginMode2D(*camera);
                ClearBackground(WHITE);
                
                afficherRaylibVer(all[id],h,l,mode);

            EndMode2D();

            /*
            //permet d'afficher des informations, en commentaire pour pouvoir prendre des screenshot pour les diapo
            if (fastmode) sprintf(charBuffer,"Etape : %d, 1 lower, 2 higher,3 fastmode ON",id);
            else sprintf(charBuffer,"Etape : %d, 1 lower, 2 higher,3 fastmode OFF",id);
            DrawText(charBuffer, 20, 50, 20, DARKGRAY);
            DrawFPS(400,10);
            */
        EndDrawing();

        if (fastmode){
            if (IsKeyDown(KEY_ONE)&& id>0) id--;
            if (IsKeyDown(KEY_TWO)&& id<N-1) id++;
        }
        else{
            if (IsKeyPressed(KEY_ONE)&& id>0) id--;
            if (IsKeyPressed(KEY_TWO)&& id<N-1) id++;
        }
        if (IsKeyPressed(KEY_THREE)) fastmode= !fastmode;

    
    }

    CloseWindow();

}

//s'occupe du calcul
void explorerSimulationRaylibVerPLUSCalcul(bool*** all,int N,int h,int l,Camera2D*camera,int regles,int*coordonnesVoisins,int mode, int idLoadFrom, int idSaveTo){
    int id=0;//pour l'affichage de l'étape
    int idTab=id;//pour l'id de lecture dans le tableau
    //idmin non inclu borne inf
    int idmin=N-1;//pour ne pas revenir tlm en arrière qu'on boucle au futur
    bool fastmode=false;//pour pouvoir laisser une touche enfoncée
    bool gogo=false; //simule sans t'arrêter

    int idTabPlus1;
    int idTabMoins1;

    //pour pouvoir formater
    char*charBuffer=malloc(sizeof(char)*100);
    //pour stocker pos de souris pour détecter click
    Vector2 mousePos;
    int idCaseSourisX;int idCaseSourisY;


    while (!WindowShouldClose())
    {
        idTabPlus1=modAddition(idTab,1,N);
        idTabMoins1=modAddition(idTab,-1,N);

        etapeSuivanteTorique(all[idTab],all[idTabPlus1],h,l,regles,coordonnesVoisins);


        if (IsKeyPressed(KEY_ZERO)) resetCamera(camera);

        zoom(camera);


        
        BeginDrawing();
            BeginMode2D(*camera);
                ClearBackground(WHITE);
                
                afficherRaylibVer(all[idTab],h,l,mode);

            EndMode2D();

            
            //permet d'afficher des informations, en commentaire pour pouvoir prendre des screenshot pour les diapo sans texste superflux
            if (fastmode) sprintf(charBuffer,"Etape : %d, 1 lower, 2 higher,3 fastmode ON",id);
            else sprintf(charBuffer,"Etape : %d, 1 lower, 2 higher,3 fastmode OFF",id);
            DrawText(charBuffer, 20, 50, 20, DARKGRAY);
            DrawFPS(400,10);
            


            mousePos = GetScreenToWorld2D(GetMousePosition(), *camera);
            idCaseSourisY=(int)(mousePos.y/2);
            if (idCaseSourisY%2==0) idCaseSourisX=(int)(mousePos.x/2);
            else idCaseSourisX=(int)((mousePos.x-1)/2);
            int caseX=idCaseSourisX*2+(idCaseSourisY%2);
            int caseY=idCaseSourisY;
           /*
           //pour afficher les coordonnées pour comprendre comment detecter les clicks, les 4 lignes du dessus en font partie
            sprintf(charBuffer,"X: %f, Y: %f",mousePos.x,mousePos.y);
            DrawText(charBuffer, 20, 50, 20, DARKGRAY);
            sprintf(charBuffer,"carre X: %d, carre Y: %d",idCaseSourisX,idCaseSourisY);
            DrawText(charBuffer, 20, 80, 20, DARKGRAY);
           */

        EndDrawing();

        //les lignes suivantes detectent des inputs

        if (fastmode){
            if (IsKeyDown(KEY_ONE)&& idTabMoins1!=idmin){id--;idTab=idTabMoins1;}
            else if (IsKeyDown(KEY_TWO)||gogo) {id++;idTab=idTabPlus1;}
        }
        else{
            if (IsKeyPressed(KEY_ONE)&& idTabMoins1!=idmin){id--;idTab=idTabMoins1;}
            else if (IsKeyPressed(KEY_TWO)||gogo) {id++;idTab=idTabPlus1;}
        }
        if (IsKeyPressed(KEY_THREE)) fastmode= !fastmode;
        if (IsKeyPressed(KEY_FOUR)) gogo=!gogo;
        if (idmin==idTab) idmin=(idmin+1)%(N);

        //pour detecter les clicks et changer les cases en conséquence
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if (0<=caseX && caseX<l && 0<=caseY && caseY<h)
                all[idTab][caseX][caseY]=!all[idTab][caseX][caseY];
        }
        //pour save et load
        if (IsKeyDown(KEY_LEFT_CONTROL)){
            if (IsKeyPressed(KEY_S)) saveTab(all[idTab],h,l,idSaveTo);
            if (IsKeyPressed(KEY_L)) loadTab(all[idTab],h,l,idLoadFrom);
        }
        //pour poser étoile
        if (IsKeyPressed(KEY_E)) poserEtoile(all[idTab],caseX,caseY);
    
    }

    CloseWindow();

}