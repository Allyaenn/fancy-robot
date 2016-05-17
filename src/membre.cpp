#include "membre.h"

Membre::Membre(){}

/**
 * Constructeur tous arguments
 */
Membre::Membre(const char* file, Transform t, Membre * p){

	this->maillage = read_mesh(file);
	this->pere = p;
	if (this->pere !=  NULL){ //autre element que le torse
		this->t_offset = t;
		this->t_courante = make_identity();
	}
	else // le torse
		this->t_courante = t;
		this->t_offset = t;
}

/**
 * Constructeur par copie
 * attention à la copie, deux membres identiques on le même père mais pas les mêmes enfants
 */
Membre::Membre(const Membre & m){

	this->maillage = m.maillage;
	//this->transform = m.transform;
	this->pere = m.pere;
}


/**
 * Redéfinition de l'affectation
 */
Membre & Membre::operator=(const Membre & m){
	if(this != &m){

		this->maillage = m.maillage;
		//this->transform = m.transform;
		this->pere = m.pere;
	}
	return *this;
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
		return this->t_courante; // je suis le torse, je ne renvoie que ma propre transformation
	else
		return this->pere->getTransform() * this->t_offset *  t_courante;
		//	   transformation du père	  * tr courante * décalage	
}

const Membre & Membre::getPere(){
	return *(this->pere);
}

/**
 * ****************************
 * Setters
 * ****************************
 */
void Membre::setMaillage(Mesh & m){
	this->maillage = m;
}
void Membre::setTransform(Transform & t){
	//this->transform = t;
}
void Membre::setPere(Membre* p){
	this->pere = p;
}

/**
 * Applique une Transformation t au membre
 */
void Membre::move(Transform t)
{
	this->t_courante = t_courante * t; // sauvegarde de la transformation courante (pour les enfants)
	//this->transform = this->transform * t;
}

void Membre::reset()
{
	this->t_courante = make_identity();
}

/* affiche la matrice de transformation*/
void Membre::afficherTransform(){

	for(int i = 0; i< 4; i++){
		for(int j = 0; j < 4; j++){
			//printf(" %f ", this->transform.m[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
