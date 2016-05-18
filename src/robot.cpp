
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


Membre * membres[22];

enum m {	corps,
		   	torse,
			cou,
			tete,
			epaule_droite, 
			bras_droit,
			av_bras_droit,
			poignet_droit,
			main_droite,
			epaule_gauche, 
			bras_gauche,
			av_bras_gauche,
			poignet_gauche,
			main_gauche,
			boule_cuisse_droite,
			cuisse_droite,
			tibia_droit,
			pied_droit,
			boule_cuisse_gauche,
			cuisse_gauche,
			tibia_gauche,
			pied_gauche };

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
    
    membres[m::corps] = new Membre(NULL, make_translation(0,3.5,0));
    
   	membres[m::torse] = new Membre("data/membres/torse.obj", make_identity(), membres[m::corps]);
   	
    membres[m::cou] = new Membre("data/membres/boule.obj", make_translation(0.1, 3.28, 0), membres[m::torse]);
    membres[m::tete] = new Membre("data/membres/tete.obj", make_identity(), membres[m::cou]);
    
    membres[m::epaule_droite] = new Membre("data/membres/boule.obj", make_translation(0.1, 2.66, 1), membres[m::torse]); 
	membres[m::bras_droit] = new Membre("data/membres/bras.obj", make_identity(),  membres[m::epaule_droite]);
	membres[m::av_bras_droit] = new Membre("data/membres/avant_bras.obj", make_translation(0, 0, 1.25),  membres[m::bras_droit]);
	membres[m::poignet_droit] = new Membre("data/membres/poignet.obj", make_translation(0, 0, 1.25),  membres[m::av_bras_droit]);
	membres[m::main_droite] = new Membre("data/membres/main.obj", make_translation(0, 0, 0.16),  membres[m::poignet_droit]);

	membres[m::epaule_gauche] = new Membre("data/membres/boule.obj", make_translation(0.1, 2.66, -1), membres[m::torse]); 
	membres[m::bras_gauche] = new Membre("data/membres/bras.obj", make_rotationY(180),  membres[m::epaule_gauche]);
	membres[m::av_bras_gauche] = new Membre("data/membres/avant_bras.obj", make_translation(0, 0, 1.25),  membres[m::bras_gauche]);
	membres[m::poignet_gauche] = new Membre("data/membres/poignet.obj", make_translation(0, 0, 1.25),  membres[m::av_bras_gauche]);
	membres[m::main_gauche] = new Membre("data/membres/main.obj", make_translation(0, 0, 0.16),  membres[m::poignet_gauche]);
	
	membres[m::boule_cuisse_droite] = new Membre("data/membres/boule.obj", make_translation(0, 0, 0.6)* make_scale(1.5,1.5,1.5), membres[m::corps]);
	membres[m::cuisse_droite] = new Membre("data/membres/cuisse.obj", make_identity()* make_scale(2.0/3.0,2.0/3.0,2.0/3.0),  membres[m::boule_cuisse_droite]);
	membres[m::tibia_droit] = new Membre("data/membres/tibia.obj", make_translation(0, -1.53, 0),  membres[m::cuisse_droite]);
	membres[m::pied_droit] = new Membre("data/membres/pied.obj", make_translation(0, -1.37, 0),  membres[m::tibia_droit]);

	membres[m::boule_cuisse_gauche] = new Membre("data/membres/boule.obj", make_translation(0, 0, -0.6) * make_scale(1.5,1.5,1.5), membres[m::corps]);
	membres[m::cuisse_gauche] = new Membre("data/membres/cuisse.obj", make_identity()* make_scale(2.0/3.0,2.0/3.0,2.0/3.0),  membres[m::boule_cuisse_gauche]);
	membres[m::tibia_gauche] = new Membre("data/membres/tibia.obj", make_translation(0, -1.53, 0),  membres[m::cuisse_gauche]);
	membres[m::pied_gauche] = new Membre("data/membres/pied.obj", make_translation(0, -1.37, 0),  membres[m::tibia_gauche]);


    return 0;   // renvoyer 0 ras, pas d'erreur, sinon renvoyer -1
}

void reset() //retour à la position de départ
{
	for (int i = 0; i<22; i++)
	{
		 membres[i]->reset(); 
	}
}

void saut() //position du saut (bras en V, genoux pliés)
{
	reset();
	
	membres[m::epaule_droite]->move(make_rotationX(-60));
	membres[m::epaule_gauche]->move(make_rotationX(60));
	
	membres[m::tibia_droit]->move(make_rotationZ(-105));
	membres[m::tibia_gauche]->move(make_rotationZ(-105));
}

void guerrier() //position du guerrier (yoga)
{
	reset();
	
	membres[m::cou]->move(make_rotationY(90));
	
	membres[m::boule_cuisse_droite]->move(make_rotationY(90));
	membres[m::boule_cuisse_droite]->move(make_rotationZ(90));
	membres[m::tibia_droit]->move(make_rotationZ(-90));
		
	membres[m::boule_cuisse_gauche]->move(make_rotationY(45));
	membres[m::boule_cuisse_gauche]->move(make_rotationX(-55));
	membres[m::boule_cuisse_gauche]->move(make_rotationZ(-45));
	
	membres[m::pied_gauche]->move(make_rotationX(65));
	membres[m::pied_gauche]->move(make_rotationZ(20));
	membres[m::pied_gauche]->move(make_rotationY(-5));	
	
	membres[m::corps]->move(make_translation(0,-1.5,0));
	
}

void sur_une_main() // en équilibre sur une main, les jambes en V, l'une est pliée
{
	reset();
	
	membres[m::epaule_gauche]->move(make_rotationX(90));
	
	membres[m::main_gauche]->move(make_rotationX(-90));
		
	membres[m::tibia_droit]->move(make_rotationZ(-105));
	
	membres[m::boule_cuisse_gauche]->move(make_rotationX(-15));
	membres[m::boule_cuisse_droite]->move(make_rotationX(15));
	
	membres[m::corps]->move(make_rotationX(180));
	membres[m::corps]->move(make_translation(0,-2,0));
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
    
	for (int i = 0; i<22; i++)
	{
		 draw(membres[i]->getMaillage(),membres[i]->getTransform(), camera); 
	}
	
	if (key_state('p'))
		saut();

	if(key_state('o'))
		guerrier();
	
	if(key_state('i'))
		sur_une_main();

	
	if (key_state('r'))
		reset();
	
	if (key_state('z')) // penche le buste sans faire bouger les jambes
		membres[m::torse]->move(make_rotationZ(-0.2));
		
	if (key_state('e')) // penche le buste sans faire bouger les jambes
		membres[m::torse]->move(make_rotationZ(0.2));

    return 1;   // on continue, renvoyer 0 pour sortir de l'application
}

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
