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

		Membre(const char* file, Transform t, Membre * m = NULL);
		
		Mesh & getMaillage();
		
		Transform getTransform();
		
		void move(Transform t);
		
		void reset();
		
		~Membre();
	
};

#endif
