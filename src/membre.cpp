#include "membre.h"

/**
 * Constructeur
 */
Membre::Membre(const char* file, Transform t, Membre * p){

	maillage = read_mesh(file);
	pere = p;
	if (pere !=  NULL){ // autre element que le corps
		t_offset = t;
		t_courante = make_identity();
	}
	else // le corps
		t_courante = make_identity();
		t_offset = t;
}

/**
 * ***************************
 * Getters
 * ***************************
 */
Mesh & Membre::getMaillage(){
	return maillage;
}

Transform  Membre::getTransform(){
	
	if (pere == NULL)
		// le corps renvoie que sa propre transformation
		// on applique d'abord le mouvement puis on place l'objet
		return t_offset *  t_courante; 
	else
		// les autres elements on besoin de la transformation de leur père
		// on applique d'abord le mouvement puis on place l'objet par rapport à son père
		// et enfin on applique la transformation du père
		return pere->getTransform() * t_offset *  t_courante; 
}

/**
 * Applique une Transformation t au membre
 */
void Membre::move(Transform t)
{
	// sauvegarde de la transformation courante (pour les enfants)
	t_courante = t_courante * t; 
}

/*
 * Remplace le membre à sa position d'origine 
 */
void Membre::reset()
{
	t_courante = make_identity();
}
