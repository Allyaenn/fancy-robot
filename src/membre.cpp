#include "membre.h"

Membre::Membre(){}

/**
 * Constructeur tous arguments
 */
Membre::Membre(const char* file, Transform t, Membre * p){

	this->maillage = read_mesh(file);
	this->pere = p;
	if (this->pere !=  NULL){
		this->transform = this->pere->getTransform() * t ;
		p->addEnfant(this);
	}
	else
		this->transform = t;
}

/**
 * Constructeur par copie
 * attention à la copie, deux membres identiques on le même père mais pas les mêmes enfants
 */
Membre::Membre(const Membre & m){

	this->maillage = m.maillage;
	this->transform = m.transform;
	this->pere = m.pere;
	for(std::vector<Membre *>::const_iterator it = m.enfants.begin(); it != m.enfants.end(); it++){
		this->addEnfant(new Membre(**it)); // on copie les enfants
	}
}


/**
 * Redéfinition de l'affectation
 */
Membre & Membre::operator=(const Membre & m){
	if(this != &m){

		this->maillage = m.maillage;
		this->transform = m.transform;
		this->pere = m.pere;
		for(std::vector<Membre *>::const_iterator it = m.enfants.begin(); it != m.enfants.end(); it++){
			this->addEnfant(new Membre(**it)); // on copie les enfants
		}
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

Transform & Membre::getTransform(){
	return this->transform;
}

const Membre & Membre::getPere(){
	return *(this->pere);
}

const std::vector<Membre*> & Membre::getEnfants(){
	return this->enfants;
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
	this->transform = t;
}
void Membre::setPere(Membre* p){
	this->pere = p;
	p->addEnfant(this);
}

/**
 * Fonction d'ajout d'enfant
 * Renvoi vrai si déjà présent
 * faux si il faut l'ajouter
 */
bool Membre::addEnfant(Membre* m){
	
	if(this->aPourFils(m))
		return true;
	else {
		this->enfants.push_back(m);
		return false;
	}
}

/**
 * Fonction de vérification de la présence d'un enfant
 * renvoi vrai si m est un enfant de this
 */
bool Membre::aPourFils(const Membre * m){
	
	for(std::vector<Membre *>::iterator it = enfants.begin(); it != enfants.end(); it++){
		if(*it == m)
			return true;
	}
	return false;
}

/**
 * Applique une Transformation t au membre et ses enfants
 */
void Membre::move(Transform t)
{
	this->transform = t * this->transform ;
	for(std::vector<Membre *>::iterator it = enfants.begin(); it != enfants.end(); it++){
		(*it)->move(t);
	}
}

/**
 * Applique une transformation t au membre sans la propager a ses enfant
 */
void Membre::transformWithoutSpreading(Transform t)
{
	this->transform = this->transform * t;
}

/* affiche la matrice de transformation*/
void Membre::afficherTransform(){

	for(int i = 0; i< 4; i++){
		for(int j = 0; j < 4; j++){
			printf(" %f ", this->transform.m[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
