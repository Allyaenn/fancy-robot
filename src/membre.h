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
		Mesh maillage;
		Transform t_offset;
		Transform t_courante;
		Membre * pere;

	public : 
	
		Membre();
		
		Membre(const char* file, Transform t, Membre * m = NULL);
		
		//Membre(Transform t, Membre * m = NULL);
		
		Membre(const Membre & m);
		
		Membre & operator=(const Membre &);
		
		Mesh & getMaillage();
		
		Transform getTransform();
		
		const Membre & getPere();
	
		void setMaillage(Mesh & m);
		
		void setTransform(Transform & t);
		
		void setPere(Membre * p);
		
		void move(Transform t);
		
		void reset();
	
		void afficherTransform();
		
		~Membre();
	
};

#endif
