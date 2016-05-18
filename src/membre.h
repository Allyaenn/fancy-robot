#ifndef _MEMBRE_H_
#define _MEMBRE_H_

#include "window.h"
#include "vec.h"
#include "mat.h"
#include "color.h"
#include "mesh.h"
#include "orbiter.h"
#include "wavefront.h"
#include "draw.h"

#include <iostream>
#include <vector>
#include <string>

class Membre {
	
	private : 
		// le dessin de l'objet
		Mesh maillage; 
		// le décalage par rapport au membre père (ou à l'origine pour le corps)
		Transform t_offset; 
		// la transformation représentant les mouvements fait depuis le début (ou le dernier reset)
		Transform t_courante;
		// le père de l'objet
		Membre * pere; 

	public : 

		Membre(const char* file, Transform t, Membre * m = NULL);
		
		Mesh & getMaillage();
		
		Transform getTransform();
		
		void move(Transform t);
		
		void reset();
		
		~Membre();
	
};

#endif
