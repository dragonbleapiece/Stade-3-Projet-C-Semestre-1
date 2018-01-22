#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

/* dimension du monde en nombre de cases */
#define LONG 12
#define LARG 18
/* l'origine est en haut a gauche */

#define MAXCHAR 12 /*Nombre max de caractères pour les strings*/
#define MAXPERSO 2
#define MIN_PVMAX_SERF 1
#define MAX_PVMAX_SERF 5
#define MIN_ATT_SERF 1
#define MAX_ATT_SERF 5
#define BONUS_BARDE 3
#define MALUS_BARDE -2
#define MAXEVO 3

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

typedef enum Forme {
  CARRE,
  LOSANGE,
  CROIX
} Forme;

typedef enum Couleur {
  ROUGE = 'R', /* identifiant du premier joueur */
  BLEU = 'B' /* identifiant du deuxième joueur */
} Couleur;

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
    int tour; /*nTour où l'unité est apparue ou a été améliorée*/
    int kills; /*nombre de kills effectués*/
    int subis; /*nombre d'attaques subis*/
    int PVmax, B_PVmax, PV, B_att, att, portee, B_portee;
    Etat etat;
    int evo; /*indice pour toute évolution*/
    struct Unite *suiv;

} Unite;

typedef struct Coord {
  int x;
  int y;
} Coord;

/*J'ai changé le Uliste, car chaque liste a une couleur. Lorsqu'une unité rentre dans une liste, il prend immédiatement la couleur de cette liste*/
typedef struct UListe{
  Unite *unites;
  Couleur couleur;
} UListe;


typedef struct Monde{
  Unite *plateau[LONG][LARG];
  int tour;   /* Numero du tour */
  UListe *rouge, *bleu; /*Listes des deux joueurs*/
} Monde;

#endif /* STRUCTURES_H_INCLUDED */
