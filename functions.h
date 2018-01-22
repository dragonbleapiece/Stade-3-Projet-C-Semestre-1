#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "structures.h"
#include <stdio.h>

/*Initialise Monde *monde donné en paramètre en lui attribuant les valeurs de départ à ses attributs.
La fonction alloue de la mémoire pour les UListe et donne à *monde les adresses d'allocation.
Elle renvoie 1 si les allocations ont été réussies, renvoie 0 sinon.
  / monde : le pointeur vers le Monde que l'on veut initialiser
*/
int initialiserMonde(Monde *monde);

/*Initialise le plateau donné en paramètre en attribuant à toutes ses cases la valeur NULL
  / plateau : pointeur vers la grille que l'on veut initialiser
*/
void initialiserPlateau(Unite *plateau[LONG][LARG]);

/*Crée et rajoute dans UListe uliste une Unite au genre voulu. Elle retourne l'adresse de l'allocation si celle-ci a été réussie, NULL sinon
  / genre : caractère définissant le genre de l'Unite
  / uliste : pointeur vers UListe à laquelle on ajoute l'Unite créée
  / nTour : numero du tour où est crée l'Unite
*/
Unite* creerUnite(Genre genre, UListe *uliste, int nTour);

/*Place au monde l'Unite pointée par *unite sur le plateau de *monde aux coordonnées posX, posY. Si les positions sont valides, la fonction renvoie 1. Sinon, renvoie 0.
  / unite : pointeur de l'Unite que l'on veut placer
  / monde : pointeur vers le monde dans lequel on place l'Unite
  / posX : position X du plateau de monde
  / posY : position Y du plateau de monde
*/
int placerAuMonde(Unite *unite, Monde *monde, size_t posX, size_t posY);

/*Affiche dans la console le plateau du Monde monde.
  / monde: pointeur vers le Monde
*/
void affichePlateau(Monde monde);

/*Renvoie l'adresse d'allocation mémoire du symbole du genre de l'unité.
  / genre : genre de l'unité
  /!\ Penser à libérer avec free
*/
char *getSymbol(Genre genre);

/*Affiche dans la console une ligne de numérotations de 0 à n - 1 suivie d'un retour chariot.
  / n : nombre de numérotations
*/
void afficheNumH(int n);

/*Affiche dans la console une ligne de caractères '-' suivie d'un retour chariot.
*/
void ligne();

/*Renvoie l'adresse de la dernière Unite d'une liste chaînée dans une UListe.
  / uliste : UListe dans laquelle on cherche la dernière Unite de la liste chaînée
*/
Unite *dernier(UListe uliste);

/*Supprime l'Unite de la liste et du plateau en libérant la mémoire.
  / unite : pointeur de l'Unite à supprimer
  / monde : pointeur du Monde dans lequel l'Unite réside
*/
void enleverUnite(Unite *unite, Monde* monde);

/*Renvoie l'adresse de l'Unite précédant celle de Unite unite dans l'UListe uliste si elle existe. Si unite est en première position, renvoie son adresse. Si rien n'est trouvé, renvoie NULL.
  / unite : pointeur vers l'Unite dont on cherche le précédent
  / uliste : pointeur vers la UListe dans lequel se trouverait l'Unite unite
*/
Unite *getUnitePrec(Unite *unite, UListe *uliste);

/*Renvoie l'adresse de la UListe en fonction de la couleur recherchée si elle existe, sinon renvoie NULL.
  / couleur : couleur recherchée
  / monde : pointeur vers le monde où on cherche l'UListe
*/
UListe *getUListe(Couleur couleur, Monde *monde);

/*Si la destination du déplacement est vide et respecte les limites du plateau, alors l'Unite est déplacée et la fonction renvoie 1. Sinon renvoie 0.
  / *unite : pointeur vers l'Unite que l'on cherche à déplacer
  / *monde : pointeur vers le monde dans lequel se déplace l'Unite
  / destX : destination en X
  / destY : destination en Y
*/
int deplacerUnite(Unite *unite, Monde *monde, int destX, int destY);

/*Permet d'afficher dans la console des propositions et permet l'interaction avec le joueur pour connaître ses choix durant son tour. Son tour se termine dès que tous ses pions ont été joués ou dès qu'il l'a décidé.
  / couleur : couleur du joueur
  / monde : pointeur vers le monde dans lequel le joueur joue
*/
void gererTourJoueur(Couleur couleur, Monde *monde);

/*Si la destination de l'attaque est valide, l'Unite unite attaque l'Unite qui s'y trouve. L'Unite unite gagne si elle a l'avantage et la fonctions supprime le pion défait. Sinon la fonctions supprime l'Unite attaquante.
  / unite : pointeur vers l'Unite qui initie l'attaque
  / monde : pointeur vers le monde où se trouve l'Unite
  / destX : destination de l'attaque en X
  / destY : destination de l'attaque en Y
*/
/*int attaquer(Unite *unite, Monde *monde, int destX, int destY); BACKUP*/

/**/
/*int deplacerOuAttaquer(Unite *unite, Monde *monde, int destX, int destY); BACKUP*/

/*Renvoie l'adresse d'un tableau d'Unite* à partir d'une UListe.
  / uliste : UListe à partir de laquelle on veut créer un tableau
  / *n : pointeur auquel on attribue la longueur du tableau
*/
Unite **creerSelection(UListe uliste, int *n);

/*Renvoie le nombre d'Unite dans une UListe.
  / uliste : UListe contenant une liste chaînée d'Unite
*/
int nombreUnite(UListe uliste);

/*Parcourt le tableau d'Unite* tant que le joueur le souhaite. Si on arrive à la dernière Unite*, la suivante se retrouve être la première. Renvoie l'indice de l'Unite selectionnée s'il y en a. Retourne -1 si le tableau est vide.
  / tab : tableau des Unite* selectionnables
  / length : taille du tableau tab
*/
int parcourirUniteSelect(Unite **tab, int length);

/*Enleve une unite du tableau à l'indice voulu.
  / tab : tableau des Unite* selectionnables
  / indice : indice de l'unite que l'on veut enlever
  / length : taille du tableau
*/
int enleverTab(void *tab, size_t indice, size_t length, size_t size);

/*Decale les Unite* selectionnables vers size_t debut.
  / tab : tableau des Unite* selectionnables
  / debut : indice vers laquelle les Unite* suivantes sont déplacées
  / length : taille du tableau
*/
void decaleTab(void *tab, size_t debut, size_t length, size_t size);
/*Unite *parcourirUnites(UListe uliste); BACKUP*/

/*Affiche dans la console les informations sur l'Unite.
  / unite : Unite dont on veut afficher les informations
*/
void afficherUnite(Unite unite);

/*Affiche dans la console les choix d'action pour l'Unite et demande au joueur ce qu'il veut. 'attendre' et toute autre réponse que 'deplacer' ou 'attaquer' reviennent au même choix
  / unite : pointeur vers l'Unite qui fait l'action
  / monde : pointeur vers le Monde dans lequel se trouve l'Unite
*/
int actionUnite(Unite *unite, Monde *monde, int *mouvements);

/*Libère l'allocation mémoire de Monde et de certains de ses attributs.
  / monde : pointeur vers le Monde que l'on veut libérer
*/
void viderMonde(Monde *monde);

/*Libère l'allocation mémoire de la UListe et de certains de ses attributs.
  / uliste : pointeur vers la UListe que l'on veut libérer
*/
void viderUListe(UListe *uliste);

/*Permet d'augmenter le nombre de tour du Monde et de passer aux tours des joueurs.
  / monde : pointeur vers le Monde où les joueurs jouent
*/
void gererTour(Monde *monde);

/*Place une nouvelle Unite au Monde d'après les positions indiquées par le joueur.
  / monde : pointeur vers le Monde dans lequel le jeu se joue
  / uliste : pointeur vers la UListe dans lequel on veut ajouter la nouvelle Unite
  / genre : genre de la nouvelle Unite
*/
void placerUnite(Monde *monde, UListe *uliste, Genre genre);

/*Demande à placer les pions de départ au le joueur indiqué.
  / monde : pointeur vers le Monde dans lequel le jeu se joue
  / couleur : couleur du joueur
*/
void placementParJoueur(Monde *monde, Couleur couleur);

/*Demande aux joueurs lequel souhaite placer en premier ses pions.
  / monde : pointeur vers le Monde dans lequel le jeu se joue
*/
void placementInitial(Monde *monde);

/*Demande aux joueurs s'ils souhaitent arrêter la partie. Renvoie 1 si ils acceptent, renvoie 0 sinon.
*/
int arreterPartie();

/*Démarre la partie.
*/
void gererPartie(void);

void initialiseUnite(Unite* unite, Genre genre, Couleur couleur, int nTour);

void initialiseStats(Unite *unite, Genre genre);

int rand_a_b(int a, int b);

int nombreGenre(UListe uliste, Genre genre);

int deplacerUniteAuto(Unite *unite, Monde *monde, int destX, int destY, int *mouvements);

Coord *findPath(Coord depart, Coord dest, Monde monde);

int nbDeplacement(Coord depart, Coord dest);

int nCoordTab(Coord *tab);

int executePath(Unite *unite, Coord *tab, Monde *monde);

int signe(int a);

int initialiseTab(Coord **tab, size_t lengthX, size_t lengthY);

void construireTab(Coord **tab, Coord debut, Coord dest, int i, int j, int pasX, int pasY, Monde monde, int lengthY);

Coord *goodPath(Coord **tab, int lengthX, int lengthY);

int min(int a, int b);

int max(int a, int b);

void initialiseTabCoord(Coord *tab, size_t length);

int actionDeplacer(Unite *unite, Monde *monde, int *mouvements);

int actionAttaquer(Unite *unite, Monde *monde);

void combat_Archer(Unite* exec, Unite *cible, Monde *monde);

int infligerDegats(Unite* cible, int degats, Monde *monde);

int attaquer(Unite* exec, Unite* cible, int d, Monde *monde);

void combat(Unite* exec, Unite *cible, Monde *monde);

int seProtege(Unite cible);

int peutRiposter(Unite exec, Unite cible);

int actionAttendre(Unite *unite, Monde *monde);

int actionAttendre_Sorciere(Unite *unite, Monde *monde);

int actionAttendre_Sainte(Unite *unite, Monde *monde);

void soigne(int soin, Unite *cible);

void intervertirPV(Unite *exec, Unite *cible);

Unite **unitesAPortee(Unite unite, Monde monde, int portee, int alliee, Forme forme, int *length);

int nbUnitesAPortee(Unite unite, Monde monde, int portee, int alliee, Forme forme);

int rangeShape(int x, int y, int centerX, int centerY, int portee, Forme forme);



#endif /* FUNCTIONS_H_INCLUDED */
