
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

// tableau contenant tous les membres
Membre * membres[22];

// enum permettant de designer facilement les elements du robot
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
			
// indice permettant de se repérer dans l'animation
int indice_anim  = 1001;

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
    glClearColor(0.2, 0.2, 0.2, 1);        // couleur par defaut de la fenetre
    
    glClearDepthf(1);                     // profondeur par defaut
    glDepthFunc(GL_LEQUAL);               // ztest, conserver l'intersection la plus proche de la camera
    glEnable(GL_DEPTH_TEST);              // activer le ztest
    
    glLineWidth(2.5f);                    // epaisseur des lignes de la grille (pixels)
    
    // création du robot : 
    
    // corps
    membres[m::corps] = new Membre(NULL, make_translation(0,3.5,0));
    
    // partie haute
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
	
	// partie basse
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
	indice_anim  = 1001; //stoppe l'animation si elle est en cours
}

void saut() //animation (aller et retour) du saut (bras en V, genoux pliés)
{
	if(indice_anim<500)
	{
		if (indice_anim<250) // "aller" de l'animation
		{
			if (indice_anim<100) 
			membres[m::corps]->move(make_translation(0,0.005,0)); //translation vers le haut
			if (indice_anim<150)
			{
				//mouvements des bras
				membres[m::epaule_droite]->move(make_rotationX(-0.5));
				membres[m::epaule_gauche]->move(make_rotationX(0.5));
			}	
			
			//mouvements des jambes
			membres[m::tibia_droit]->move(make_rotationZ(-0.5));
			membres[m::tibia_gauche]->move(make_rotationZ(-0.5));
		}
		else // "retour"" de l'animation
		{
			if (indice_anim>400 && indice_anim<500)
				membres[m::corps]->move(make_translation(0,-0.005,0)); //translation vers le bas
			if (indice_anim<400)
			{
				//mouvements des bras
				membres[m::epaule_droite]->move(make_rotationX(0.5));
				membres[m::epaule_gauche]->move(make_rotationX(-0.5));
			}	
			//mouvements des jambes
			membres[m::tibia_droit]->move(make_rotationZ(0.5));
			membres[m::tibia_gauche]->move(make_rotationZ(0.5));
		}
	
		indice_anim++;
	}	
}

void guerrier() //position du guerrier (yoga)
{
	reset();
	
	//mouvement de la tête
	membres[m::cou]->move(make_rotationY(90));
	
	//mouvements de la jambe gauche
	membres[m::boule_cuisse_gauche]->move(make_rotationY(90));
	membres[m::boule_cuisse_gauche]->move(make_rotationZ(90));
	membres[m::tibia_gauche]->move(make_rotationZ(-90));
		
	// mouvements de la jambe droite
	membres[m::boule_cuisse_droite]->move(make_rotationY(45));
	membres[m::boule_cuisse_droite]->move(make_rotationX(-55));
	membres[m::boule_cuisse_droite]->move(make_rotationZ(-45));
	
	//mouvement du pied droit
	membres[m::pied_droit]->move(make_rotationX(65));
	membres[m::pied_droit]->move(make_rotationZ(20));
	membres[m::pied_droit]->move(make_rotationY(-5));	
	
	//translation vers le bas
	membres[m::corps]->move(make_translation(0,-1.5,0));	
}

void sur_une_main() // en équilibre sur une main, les jambes en V, l'une est pliée
{
	reset();
	
	//mouvements du bras gauche
	membres[m::epaule_gauche]->move(make_rotationX(90));
	membres[m::main_gauche]->move(make_rotationX(-90));
		
	//mouvement de la jambe gauche
	membres[m::tibia_gauche]->move(make_rotationZ(-105));
	
	//mouvement des jambes
	membres[m::boule_cuisse_droite]->move(make_rotationX(-15));
	membres[m::boule_cuisse_gauche]->move(make_rotationX(15));
	
	//deplaccement du corps
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
    
    //dessin de tous les membres (sauf le corps qui n'a pas de maillage associé)
	for (int i = 1; i<22; i++)
	{
		 draw(membres[i]->getMaillage(),membres[i]->getTransform(), camera); 
	}
	
	if (key_state('p')) // lance l'animation saut
	{
		reset();
		indice_anim = 0;
	}
	
	saut();

	if(key_state('o')) // le robot prend la posture du guerrier
		guerrier();
	
	if(key_state('i')) // le robot prend la position "sur une main"
		sur_une_main();

	
	if (key_state('r')) // le robot revient à sa position de départ
		reset();
	
	if (key_state('z')) // penche le buste sans faire bouger les jambes
		membres[m::torse]->move(make_rotationZ(-0.2));
	if (key_state('e')) // remonte le buste sans faire bouger les jambes
		membres[m::torse]->move(make_rotationZ(0.2));
		
	if(key_state('q')) // fait tourner l'épaule gauche sur Y dans le sens horaire
		membres[m::epaule_gauche]->move(make_rotationY(-1));
	if(key_state('s')) // fait tourner l'épaule gauche sur Y dans le sens anti horaire
		membres[m::epaule_gauche]->move(make_rotationY(1));
		
	if(key_state('d')) // fait tourner l'avant bras gauche sur X dans le sens horaire
		membres[m::av_bras_gauche]->move(make_rotationX(1));
	if(key_state('f')) // fait tourner l'avant bras gauche sur X dans le sens anti-horaire
		membres[m::av_bras_gauche]->move(make_rotationX(-1));
		
	if(key_state('g')) // fait tourner le poignet gauche sur Z dans le sens anti-horaire
		membres[m::poignet_gauche]->move(make_rotationZ(1));
	if(key_state('h')) // fait tourner le poignet gauche sur Z dans le sens horaire
		membres[m::poignet_gauche]->move(make_rotationZ(-1));
		
	if(key_state('w')) // fait tourner la jambe droite sur Z dans le sens anti-horaire
		membres[m::boule_cuisse_droite]->move(make_rotationZ(1));
	if(key_state('x')) // fait tourner la jambe droite sur Z dans le sens horaire
		membres[m::boule_cuisse_droite]->move(make_rotationZ(-1));
		
	if(key_state('c')) // fait tourner le tibia droit sur Z dans le sens horaire
		membres[m::tibia_droit]->move(make_rotationZ(-1));
	if(key_state('v')) // fait tourner le tibia droit sur Z dans le sens anti-horaire
		membres[m::tibia_droit]->move(make_rotationZ(1));
		
	if(key_state('b')) // fait tourner le pied droit sur Z dans le sens anti-horaire
		membres[m::pied_droit]->move(make_rotationZ(1));
	if(key_state('n')) // fait tourner le pied droit sur Z dans le sens horaire
		membres[m::pied_droit]->move(make_rotationZ(-1));
		
	if(key_state('j')) // fait tourner le cou sur Y dans le sens anti-horaire
		membres[m::cou]->move(make_rotationY(1));
	if(key_state('k')) // fait tourner le cou sur Y dans le sens horaire
		membres[m::cou]->move(make_rotationY(-1));
		
	if(key_state('l')) //fait tourner la tete sur Z dans le sens horaire
		membres[m::tete]->move(make_rotationZ(-1));
	if(key_state('m')) //fait tourner la tete sur Z dans le sens anti-horaire
		membres[m::tete]->move(make_rotationZ(1));

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
