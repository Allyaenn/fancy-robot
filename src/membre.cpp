#include "membre.h"

/**
 * Constructeur
 */
Membre::Membre(const char* file, Transform t, Membre * p){

	this->maillage = read_mesh(file);
	this->pere = p;
	if (this->pere !=  NULL){ //autre element que le torse
		this->t_offset = t;
		this->t_courante = make_identity();
	}
	else // le torse
		this->t_courante = make_identity();
		this->t_offset = t;
}

/**
 * ***************************
 * Getters
 * ***************************
 */
Mesh & Membre::getMaillage(){
	return this->maillage;
}

Transform  Membre::getTransform(){
	
	if (pere == NULL)
		return this->t_offset *  t_courante; // je suis le torse, je ne renvoie que ma propre transformation
	else
		return this->pere->getTransform() * this->t_offset *  t_courante;
		//	   transformation du père	  * tr courante * décalage	
}

/**
 * Applique une Transformation t au membre
 */
void Membre::move(Transform t)
{
	this->t_courante = t_courante * t; // sauvegarde de la transformation courante (pour les enfants)
	//this->transform = this->transform * t;
}

/*
 * Remplace le membre à sa position d'origine 
 */
void Membre::reset()
{
	this->t_courante = make_identity();
}
