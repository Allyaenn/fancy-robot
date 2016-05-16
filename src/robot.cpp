
#include "window.h"

#include "vec.h"
#include "mat.h"
#include "color.h"

#include "mesh.h"
#include "orbiter.h"

#include "wavefront.h"
#include "draw.h"
#include "membre.h"

Orbiter camera;
int pose;
bool sens;
Mesh grid;

/*Parties du corps du robot*/
Membre * torse;

Membre * cou; //boule
Membre * tete;

Membre * epaule_droite; //boule
Membre * bras_droit;
Membre * av_bras_droit;
Membre * poignet_droit;
Membre * main_droite;

Membre * epaule_gauche; //boule
Membre * bras_gauche;
Membre * av_bras_gauche;
Membre * poignet_gauche;
Membre * main_gauche;

Membre * boule_cuisse_droite;
Membre * cuisse_droite;
Membre * tibia_droit;
Membre * pied_droit;

Membre * boule_cuisse_gauche;
Membre * cuisse_gauche;
Membre * tibia_gauche;
Membre * pied_gauche;

// utilitaire. creation d'une grille / repere.
Mesh make_grid( int size )
{
    Mesh grid= create_mesh(GL_LINES);
    
    for(int x= 0; x < size; x++)
    {
        float offset = (float) size / 2 - 0.5f ;
        float px= (float) x - offset;
        push_vertex(grid, make_point(px, 0, -offset)); 
        push_vertex(grid, make_point(px, 0, offset)); 
    }

    for(int z= 0; z < size; z++)
    {
        float offset = (float) size / 2 - 0.5f ;
        float pz= (float) z - offset;
        push_vertex(grid, make_point(-offset, 0, pz)); 
        push_vertex(grid, make_point(offset, 0, pz)); 
    }
    
    return grid;
}

// creation des objets openGL
int init( )
{
    camera= make_orbiter();
    grid = make_grid(50);
    grid.color = make_black() ;
    
    // etat par defaut
    glClearColor(0.2, 0.2, 0.2, 1);           // couleur par defaut de la fenetre
    
    glClearDepthf(1);                   // profondeur par defaut
    glDepthFunc(GL_LEQUAL);               // ztest, conserver l'intersection la plus proche de la camera
    glEnable(GL_DEPTH_TEST);            // activer le ztest
    
    glLineWidth(2.5f);                  // epaisseur des lignes de la grille (pixels)
    
    //création du robot : 
   	torse = new Membre("data/membres/torse.obj", make_identity());
   	
    cou = new Membre("data/membres/boule.obj", make_translation(0.1, 3.28, 0), torse);
    tete = new Membre("data/membres/tete.obj", make_identity(), cou);
    
    epaule_droite = new Membre("data/membres/boule.obj", make_translation(0.1, 2.66, 1), torse); 
	bras_droit = new Membre("data/membres/bras.obj", make_identity(), epaule_droite);
	av_bras_droit = new Membre("data/membres/avant_bras.obj", make_translation(0, 0, 1.25), bras_droit);
	poignet_droit = new Membre("data/membres/poignet.obj", make_translation(0, 0, 1.25), av_bras_droit);
	main_droite = new Membre("data/membres/main.obj", make_translation(0, 0, 0.16), poignet_droit);

	epaule_gauche = new Membre("data/membres/boule.obj", make_translation(0.1, 2.66, -1), torse); 
	bras_gauche = new Membre("data/membres/bras.obj", make_rotationY(180), epaule_gauche);
	av_bras_gauche = new Membre("data/membres/avant_bras.obj", make_translation(0, 0, 1.25), bras_gauche);
	poignet_gauche = new Membre("data/membres/poignet.obj", make_translation(0, 0, 1.25), av_bras_gauche);
	main_gauche = new Membre("data/membres/main.obj", make_translation(0, 0, 0.16), poignet_gauche);

	boule_cuisse_droite = new Membre("data/membres/boule.obj", make_translation(0, 0, -0.6), torse);
	cuisse_droite = new Membre("data/membres/cuisse.obj", make_identity(), boule_cuisse_droite);
	tibia_droit = new Membre("data/membres/tibia.obj", make_translation(0, -1.53, 0), cuisse_droite);
	pied_droit = new Membre("data/membres/pied.obj", make_translation(0, -1.37, 0), tibia_droit);

	boule_cuisse_gauche = new Membre("data/membres/boule.obj", make_translation(0, 0, 0.6), torse);
	cuisse_gauche = new Membre("data/membres/cuisse.obj", make_identity(), boule_cuisse_gauche);
	tibia_gauche = new Membre("data/membres/tibia.obj", make_translation(0, -1.53, 0), cuisse_gauche);
	pied_gauche = new Membre("data/membres/pied.obj", make_translation(0, -1.37, 0), tibia_gauche);
	
	//appliquer une transformation sans repercussions sur les enfants
    boule_cuisse_droite->transformWithoutSpreading(make_scale(1.5,1.5,1.5));
    boule_cuisse_gauche->transformWithoutSpreading(make_scale(1.5,1.5,1.5));
    pose = 0;
	sens = true;
    return 0;   // renvoyer 0 ras, pas d'erreur, sinon renvoyer -1
}

// affichage
int draw( )
{
    // on commence par effacer la fenetre avant de dessiner quelquechose
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // on efface aussi le zbuffer
    
    // recupere les mouvements de la souris, utilise directement SDL2
    int mx, my;
    unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);
    
    // deplace la camera
    if(mb & SDL_BUTTON(1))              // le bouton gauche est enfonce
        // tourne autour de l'objet
        orbiter_rotation(camera, mx, my);
    
    else if(mb & SDL_BUTTON(3))         // le bouton droit est enfonce
        // approche / eloigne l'objet
        orbiter_move(camera, mx);
    
    else if(mb & SDL_BUTTON(2))         // le bouton du milieu est enfonce
        // deplace le point de rotation
        orbiter_translation(camera, (float) mx / (float) window_width(), (float) my / (float) window_height()); 
    
    draw(grid,camera);
    //torse->move(make_rotationY(1));
    draw(torse->getMaillage(),torse->getTransform(), camera); 
       
    draw(cou->getMaillage(),cou->getTransform(), camera);
    draw(tete->getMaillage(),tete->getTransform(), camera);  
    
    draw(epaule_droite->getMaillage(),epaule_droite->getTransform(), camera);    
    draw(bras_droit->getMaillage(),bras_droit->getTransform(), camera);
    draw(av_bras_droit->getMaillage(),av_bras_droit->getTransform(), camera);  
    draw(poignet_droit->getMaillage(),poignet_droit->getTransform(), camera);    
    draw(main_droite->getMaillage(),main_droite->getTransform(), camera);
    
    draw(epaule_gauche->getMaillage(),epaule_gauche->getTransform(), camera);    
    draw(bras_gauche->getMaillage(),bras_gauche->getTransform(), camera);
    draw(av_bras_gauche->getMaillage(),av_bras_gauche->getTransform(), camera);  
    draw(poignet_gauche->getMaillage(),poignet_gauche->getTransform(), camera);    
    draw(main_gauche->getMaillage(),main_gauche->getTransform(), camera);
    
    draw(boule_cuisse_droite->getMaillage(),boule_cuisse_droite->getTransform(), camera);  
    draw(cuisse_droite->getMaillage(),cuisse_droite->getTransform(), camera);    
    draw(tibia_droit->getMaillage(),tibia_droit->getTransform(), camera);
    draw(pied_droit->getMaillage(),pied_droit->getTransform(), camera);  
    
    draw(boule_cuisse_gauche->getMaillage(),boule_cuisse_gauche->getTransform(), camera);  
    draw(cuisse_gauche->getMaillage(),cuisse_gauche->getTransform(), camera);    
    draw(tibia_gauche->getMaillage(),tibia_gauche->getTransform(), camera);
    draw(pied_gauche->getMaillage(),pied_gauche->getTransform(), camera); 


	SDL_Event event;
	SDL_PollEvent(&event);
	Transform tboule = make_identity();

	switch(event.type){
		case SDL_KEYUP:
			switch(event.key.keysym.sym){
				case SDLK_z :
					if(sens){
						torse->move(make_rotationZ(-10));
						cuisse_droite->move(make_rotationZ(10));
						cuisse_gauche->move(make_rotationZ(10));
						printf("%i\n", pose);
						pose++;
					}else{
						torse->move(make_rotationZ(10));
						cuisse_droite->move(make_rotationZ(-10));
						cuisse_gauche->move(make_rotationZ(-10));
						pose--;
					}
					if(pose == 9)
						sens = false;
					if(pose== 0)
						sens = true;
					break;
				case SDLK_t :
					bras_droit->move(make_rotationY(10));
					bras_gauche->move(make_rotationY(10));
					//bras_droit->afficherTransform();//Debug des matrices
					break;
				case SDL_q :
					return 0;
					break;
			}
			break;
		case SDL_KEYDOWN:
			break;
	}

    return 1;   // on continue, renvoyer 0 pour sortir de l'application
}

//la matrice la plus a droite est la première appliquée, ici on veut que le cube tourne d'abors puis ensuite on le déplace

// destruction des objets openGL
int quit( )
{
    return 0;   // ras, pas d'erreur
}


int main( int argc, char **argv )
{
    // etape 1 : creer la fenetre
    Window window= create_window(1024, 640);
    if(window == NULL) 
        return 1;       // erreur lors de la creation de la fenetre ou de l'init de sdl2
    
    // etape 2 : creer un contexte opengl pour pouvoir dessiner
    Context context= create_context(window);
    if(context == NULL) 
        return 1;       // erreur lors de la creation du contexte opengl
    
    // etape 3 : creation des objets 
    if(init() < 0)
    {
        printf("[error] init failed.\n");
        return 1;
    }
    
    // etape 4 : affichage de l'application, tant que la fenetre n'est pas fermee. ou que draw() ne renvoie pas 0
    run(window, draw);

    // etape 5 : nettoyage
    quit();
    release_context(context);
    release_window(window);

    return 0;
}
