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
		Transform transform_att;
		Transform t_cours;
		Membre * pere;
		std::vector<Membre*> enfants;

	public : 
	
		Membre();
		
		Membre(const char* file, Transform t, Membre * m = NULL);
		
		Membre(const Membre & m);
		
		Membre & operator=(const Membre &);
		
		Mesh & getMaillage();
		
		Transform getTransform();
		
		const Membre & getPere();
		
		const std::vector<Membre*> & getEnfants();
	
		void setMaillage(Mesh & m);
		
		void setTransform(Transform & t);
		
		void setPere(Membre * p);
		
		bool addEnfant(Membre* m);
		
		bool aPourFils(const Membre * m);
		
		void move(Transform t);
	
		void afficherTransform();
		
		~Membre();
	
};

#endif
