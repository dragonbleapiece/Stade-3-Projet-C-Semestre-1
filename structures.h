#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

/* dimension du monde en nombre de cases */
#define LONG 12
#define LARG 18
/* l'origine est en haut a gauche */

#define MAXCHAR 12 /*Nombre max de caractères pour les strings*/
#define MAXPERSO 2 /*On a envisagé de mettre plusieurs perso sur une même case*/
#define MIN_PVMAX_SERF 1
#define MAX_PVMAX_SERF 5
#define MIN_ATT_SERF 1
#define MAX_ATT_SERF 5
#define BONUS_BARDE 3
#define MALUS_BARDE -2
#define MAXEVO 3 /*Nombre max d'évolution par classe*/

/* les genres d'unites */

typedef enum Genre {
  SERF = 's',
  GUERRIER = 'g',
  MATRIARCHE = 'M',
  ASSASSIN = 'a',
  ARCHER = 'c',
  BARDE = 'b',
  DUC = 'd',
  ARCHIDUC = 'D',
  BASTION = 'B',
  CHAMPION = 'C',
  SORCIERE = 'S',
  SAINTE = 'o'
} Genre;

/*Forme des sélections*/

typedef enum Forme {
  CARRE,
  LOSANGE,
  CROIX
} Forme;

/*Couleurs possibles*/

typedef enum Couleur {
  ROUGE = 'R', /* identifiant du premier joueur */
  BLEU = 'B' /* identifiant du deuxième joueur */
} Couleur;

/*Etats possibles*/

typedef enum Etat {
  PARALYSIE = 'p', /*passe un tour*/
  DEFAUT = 'd', /*par défaut*/
  EVOLUTION = 'e', /*montée de classe*/
  ATTENTE = 'a' /*ordre d'attente*/
} Etat;

typedef struct Unite {
    int posX,posY;
    Couleur couleur;
    Genre genre;
    int tour; /*n°Tour où l'unité est apparue ou a été améliorée*/
    int kills; /*nombre de kills effectués*/
    int subis; /*nombre d'attaques subis*/
    int PVmax, B_PVmax, PV, B_att, att, portee, B_portee;
    /*B_PVmax non utilisé. Dans l'optique de pouvoir augmenter les PVmax si unites en duo*/
    Etat etat;
    int evo; /*indice pour toute évolution ou augmentation d'att du GUERRIER*/
    struct Unite *suiv;

} Unite;

typedef struct Coord {
  int x;
  int y;
} Coord;

/*J'ai changé le UListe, car chaque liste a une couleur. Lorsqu'une unité rentre dans une liste, il prend immédiatement la couleur de cette liste*/
typedef struct UListe{
  Unite *unites;
  Couleur couleur;
  int champion; /*Si le joueur a ou a eu un champion*/
} UListe;


typedef struct Monde{
  Unite *plateau[LONG][LARG];
  int tour;   /* Numero du tour */
  UListe *rouge, *bleu; /*Listes des deux joueurs*/
} Monde;

#endif /* STRUCTURES_H_INCLUDED */
