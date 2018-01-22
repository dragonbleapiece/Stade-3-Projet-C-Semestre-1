#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "structures.h"
#include "functions.h"

int initialiserMonde(Monde *monde){
    monde->tour = 0;

    initialiserPlateau(monde->plateau);
    (monde->rouge) = calloc(1, sizeof(*(monde->rouge)));
    if((monde->rouge) == NULL) {
      return 0;
    }
    (monde->rouge)->couleur = ROUGE;
    (monde->rouge)->unites = NULL;
    (monde->bleu) = calloc(1, sizeof(*(monde->bleu)));
    if((monde->bleu) == NULL) {
      return 0;
    }
    (monde->bleu)->couleur = BLEU;
    (monde->bleu)->unites = NULL;
    return 1;
}

void initialiserPlateau(Unite *plateau[LONG][LARG]) {
  size_t i, j;
  for(i = 0; i < LONG; ++i) {
    for(j = 0; j < LARG; ++j) {
      plateau[i][j] = NULL;
    }
  }
}

Unite *creerUnite(Genre genre, UListe *uliste, int nTour) {
  Unite *temp;
  temp = calloc(1, sizeof(*(uliste->unites)));
  if(temp != NULL) {
    initialiseUnite(temp, genre, uliste->couleur, nTour);
    if(uliste->unites == NULL) {
      uliste->unites = temp;
    } else {
      temp->suiv = uliste->unites;
      uliste->unites = temp;
    }
  }

  return temp;

}

void initialiseUnite(Unite* unite, Genre genre, Couleur couleur, int nTour) {
  unite->genre = genre;
  unite->couleur = couleur;
  unite->suiv = NULL;
  unite->etat = DEFAUT;
  unite->posX = -1;
  unite->posY = -1;
  unite->kills = 0;
  unite->subis = 0;
  unite->evo = 0;
  unite->tour = nTour;
  initialiseStats(unite, genre);
}

void initialiseStats(Unite *unite, Genre genre) {
  int PVmax, att;

  PVmax = rand_a_b(MIN_PVMAX_SERF, MAX_PVMAX_SERF + 1);
  att = rand_a_b(MIN_ATT_SERF, MAX_ATT_SERF + 1);

  unite->B_PVmax = 0;
  unite->B_portee = 0;
  unite->B_att = 0;

  switch(genre) {
    case(GUERRIER):
      unite->PVmax = PVmax + 10;
      unite->att = att + 3;
      unite->portee = 1;
      break;
    case(SERF):
      unite->PVmax = PVmax;
      unite->att = att;
      unite->portee = 1;
      break;
    case(MATRIARCHE):
      unite->PVmax = 1;
      unite->att = 1;
      unite->portee = 3;
      break;
    case(ASSASSIN):
      unite->PVmax = PVmax;
      unite->att = 20;
      unite->portee = 1;
      break;
    case(ARCHER):
      unite->PVmax = PVmax + 3;
      unite->att = att + 1;
      unite->portee = 3;
      break;
    case(BARDE):
      unite->PVmax = PVmax + 6;
      unite->att = att + 6;
      unite->portee = 3;
      break;
    case(DUC):
      unite->PVmax = PVmax + 15;
      unite->att = att + 8;
      unite->portee = 1;
      break;
    case(ARCHIDUC):
      unite->PVmax = PVmax;
      unite->att = 1;
      unite->portee = 1;
      break;
    case(BASTION):
      unite->PVmax = PVmax + 30;
      unite->att = att;
      unite->portee = 1;
      break;
    case(CHAMPION):
      unite->PVmax = PVmax + 20;
      unite->att = att + 10;
      unite->portee = 2;
      break;
    case(SORCIERE):
      unite->PVmax = 10;
      unite->att = 6;
      unite->portee = 2;
      break;
    case(SAINTE):
      unite->PVmax = 15;
      unite->att = 3;
      unite->portee = 2;
      break;
    default:
      unite->PVmax = 0;
      unite->att = 0;
      unite->portee = 0;
      break;
  }

  unite->PV = unite->PVmax;
}

int rand_a_b(int a, int b) {
  return rand() % (b - a) + a;
}

Unite *dernier(UListe uliste) {
  Unite *unite;
  unite = uliste.unites;
  while(unite != NULL && unite->suiv != NULL) {
    unite = (unite->suiv);
  }
  return unite;
}

int placerAuMonde(Unite *unite, Monde *monde, size_t posX, size_t posY) {
  if(unite == NULL || monde->plateau[posY][posX] != NULL || posY >= LONG || posX >= LARG) {
    return 0;
  } else {
    unite->posX = posX;
    unite->posY = posY;
    monde->plateau[posY][posX] = unite;
    return 1;
  }
}

void affichePlateau(Monde monde) {
  int i, j;
  afficheNumH(LARG);
  ligne();
  for(i = 0; i < LONG; ++i) {
    printf("|");
    for(j = 0; j < LARG; ++j) {
      if(monde.plateau[i][j] == NULL) {
        printf("    ");
      } else {
        char *symbol = getSymbol(monde.plateau[i][j]->genre);
        printf(" %s%c", symbol, monde.plateau[i][j]->couleur);
        free(symbol);
      }
      printf("|");
    }
    printf("  %d", i);
    printf("\n");
    ligne();
  }
}

void afficheNumH(int n) {
  int i;
  for(i = 0; i < n; ++i) {
    printf("  %2d ", i);
  }
  printf("\n");
}

/*À libérer avec free*/
char *getSymbol(Genre genre) {
  char *string;
  string = malloc(3 * sizeof(*string));
  switch(genre) {
    case(GUERRIER):
      strcpy(string, "()");
      break;
    case(SERF):
      strcpy(string, "00");
      break;
    case(MATRIARCHE):
      strcpy(string, "MM");
      break;
    case(ASSASSIN):
      strcpy(string, "L)");
      break;
    case(ARCHER):
      strcpy(string, "}>");
      break;
    case(BARDE):
      strcpy(string, "o^");
      break;
    case(DUC):
      strcpy(string, "/)");
      break;
    case(ARCHIDUC):
      strcpy(string, "/}");
      break;
    case(BASTION):
      strcpy(string, "[]");
      break;
    case(CHAMPION):
      strcpy(string, "CH");
      break;
    case(SORCIERE):
      strcpy(string, "44");
      break;
    case(SAINTE):
      strcpy(string, "&&");
      break;
    default:
      strcpy(string, "xx");
      break;
  }
  return string;
}

void ligne() {
  int i;
  for(i = 0; i < LARG * 5 + 1; ++i) {
    printf("-");
  }
  printf("\n");

}

int deplacerUnite(Unite *unite, Monde *monde, int destX, int destY){
  if(monde->plateau[destY][destX] == NULL) {
    if(destX <= LARG
      && destY <= LONG
      && (abs(destX-(unite->posX)) + abs(destY-(unite->posY)) <= 1)) /* On verifie que la destination existe et est vide et que c'est un déplacement strictement adjacent */
    {
                    monde->plateau[unite->posY][unite->posX]=NULL;
                    unite->posX = destX;
                    unite->posY = destY;
                    monde->plateau[destY][destX] = unite;
                    return 1;
    } else {
      return 0;
    }
  } else {
    return -1;
  }

}

int deplacerUniteAuto(Unite *unite, Monde *monde, int destX, int destY, int *mouvements) {
    if(destX <= LARG
      && destY <= LONG
      && destX >= 0
      && destY >= 0
      && destX != unite->posX
      && destY != unite->posY
      && (abs(destX-(unite->posX)) + abs(destY-(unite->posY)) <= *mouvements)) /* On verifie que la destination existe et est vide et que c'est un déplacement strictement adjacent */
    {
      printf("Deplacement\n");
      Coord depart, dest;
      Coord *tab;
      int r;
      depart.x = unite->posX;
      depart.y = unite->posY;
      dest.x = destX;
      dest.y = destY;
      tab = findPath(depart, dest, *monde);
      if(tab == NULL) {
        return 2;
      }
      r = executePath(unite, tab, monde);
      if(r != 0) {
        if(unite->posX != destX) {
          r = -1; /*Si l'unité n'a pas atteint sa destination et si aucune erreur de déplacement n'a été signalée, cela veut dire qu'un obstacle a été rencontré. Cette instruction peut être optimisée en modifiant construireTab()*/
        }
        *mouvements -= nCoordTab(tab) - 1; /*Ne compte pas la position de départ du chemin*/
      }
      /*Deplacement reussi même si la destination donnee n'a pas ete atteinte*/
      free(tab);

      return r;

    } else {
      return 0;
    }
}

/*ne pas oublier de libérer avec free*/
Coord *findPath(Coord depart, Coord dest, Monde monde) {
  int lengthX = nbDeplacement(depart, dest) + 2;
  int lengthY;
  if(depart.x == dest.x || depart.y == dest.y) {
    lengthY = 1; /*un seul chemin possible*/
  } else {
    lengthY = (min(abs(depart.x - dest.x) + 1, abs(depart.y - dest.y) + 1) - 1) * max(abs(depart.x - dest.x) + 1, abs(depart.y - dest.y) + 1);
  }
  printf("nombre de chemins %d\n", lengthY);
  Coord **tab = calloc(lengthY, sizeof(*tab));
  Coord *g;

  int pasX = signe(dest.x - depart.x);
  int pasY = signe(dest.y - depart.y);

  if(tab != NULL) {
    if(initialiseTab(tab, lengthX, lengthY) == 0) {
      return NULL;
    }

    printf("findPath\n");
    construireTab(tab, depart, dest, 0, 0, pasX, pasY, monde, lengthY);
    g = goodPath(tab, lengthX, lengthY);
    free(tab);

    return g;
  }

  return NULL;
}

/*ne pas oublier de libérer avec free*/
int initialiseTab(Coord **tab, size_t lengthX, size_t lengthY) {
  size_t i;
  for(i = 0; i < lengthY; ++i) {
    tab[i] = calloc(lengthX, sizeof(**tab));
    if(tab[i] == NULL) {
      return 0;
    }
    initialiseTabCoord(tab[i], lengthX);
  }

  return 1;
}

void initialiseTabCoord(Coord *tab, size_t length) {
  size_t i;
  for(i = 0; i < length; ++i) {
    tab[i].x = -1;
    tab[i].y = -1;
  }
}

void construireTab(Coord **tab, Coord debut, Coord dest, int i, int j, int pasX, int pasY, Monde monde, int lengthY) {
  Coord coord = debut;
  int indice;
  printf("%d %d = %d,%d\n", i, j, coord.x, coord.y);
  for(indice = j; indice < lengthY; ++indice) {
    tab[j][i] = coord;
  }
  if(!(monde.plateau[coord.y + pasY][coord.x] != NULL && monde.plateau[coord.y][coord.x + pasX] != NULL)) {

    if(coord.x != dest.x && monde.plateau[coord.y][coord.x + pasX] == NULL) {
      coord.x += pasX;
      construireTab(tab, coord, dest, i + 1, j, pasX, pasY, monde, lengthY);
      coord.x = debut.x; /*revient à la valeur initiale*/
      j++;
    }
    if(coord.y != dest.y && monde.plateau[coord.y + pasY][coord.x] == NULL) {
      coord.y += pasY;
      construireTab(tab, coord, dest, i + 1, j, pasX, pasY, monde, lengthY);
    }
  }

}

Coord *goodPath(Coord **tab, int lengthX, int lengthY) {
  printf("GoodPath\n");
  int i, j;
  i = 1;
  while(i < lengthX && lengthY > 1) {
    j = 0;
    while(j < lengthY) {
      if(tab[j][i].x == -1) {
        free(tab[j]);
        lengthY = enleverTab(tab, j, lengthY, sizeof(*tab));
      }
      ++j;
    }
    ++i;
  }

  return *tab;
}

int min(int a, int b) {
  return (a < b) ? a : b;
}

int max(int a, int b) {
  return (a > b) ? a : b;
}

int nbDeplacement(Coord depart, Coord dest) {
  return (abs(depart.x - dest.x) + abs(depart.y - dest.y));
}

int nCoordTab(Coord *tab) {
  int n = 0;
  while(tab[n].x != -1 && tab[n].y != -1) {
    ++n;
  }
  return n;
}

int executePath(Unite *unite, Coord *tab, Monde *monde) {
  int length = nCoordTab(tab);
  int i = 1;
  int r;
  while(i < length && (r = deplacerUnite(unite, monde, tab[i].x, tab[i].y)) == 1) {
    printf("%d, %d\n", tab[i].x, tab[i].y);
    ++i;
  }
  return r;
}

int signe(int a) {
  if(a > 0) {
    return 1;
  } else if(a == 0) {
    return 0;
  } else {
    return -1;
  }
}

void enleverUnite(Unite *unite, Monde *monde) {
  UListe *uliste = getUListe(unite->couleur, monde);
  Unite *unitePrec = getUnitePrec(unite, uliste);
  if(unitePrec == unite) {
    uliste->unites = unite->suiv;
  } else if(unitePrec != NULL) {
    unitePrec->suiv = unite->suiv;
  }
  monde->plateau[unite->posY][unite->posX] = NULL;
  free(unite);
  unite = NULL; /*prévient de bugs*/
}

UListe *getUListe(Couleur couleur, Monde *monde) {
  if(couleur == (monde->rouge)->couleur) {
    return (monde->rouge);
  }
  else if(couleur == (monde->bleu)->couleur) {
    return (monde->bleu);
  }
  else {
    return NULL;
  }

}

Unite *getUnitePrec(Unite *unite, UListe *uliste) {
  Unite *search;
  search = uliste->unites;
  while(search != NULL && search->suiv != unite && search != unite) {
    search = search->suiv;
  }
  return search;
}

void gererTourJoueur(Couleur couleur, Monde *monde) {
  int selection;
  char cmd;
  UListe uliste = *getUListe(couleur, monde);
  int nUnite;
  Unite **uniteSelect = creerSelection(uliste, &nUnite);
  int mouvements = nombreGenre(uliste, SERF) + 1;
  int action;

  if(nUnite && uniteSelect != NULL) {
    affichePlateau(*monde);
    printf("Tour : %d | Joueur : %c\n", monde->tour, couleur);
    do {
      printf("Points de mouvement : %d\n", mouvements);
      selection = parcourirUniteSelect(uniteSelect, nUnite);
      if(selection > -1 && mouvements > 0) {
        action = actionUnite(uniteSelect[selection], monde, &mouvements);
        if(action == 4 || action == 3) {
          nUnite = enleverTab(uniteSelect, selection, nUnite, sizeof(*uniteSelect));
        }
        affichePlateau(*monde);
        printf("Voulez-vous arreter votre tour ? (o/n)\n");
        scanf(" %c", &cmd);
      } else {
        if(mouvements <= 0) {
          printf("Vous n'avez plus aucun ordre à donner.\n");
        }
        printf("Arret du tour !\n");
        cmd = 'o';
      }
    } while(cmd != 'o');
    free(uniteSelect);
  }
}

Unite **creerSelection(UListe uliste, int *n) {
  int i;
  Unite **tab;
  Unite *unite;
  *n = nombreUnite(uliste);
  tab = calloc(*n, sizeof(*tab));
  unite = uliste.unites;
  for(i = 0; i < *n && unite != NULL; ++i) {
    tab[i] = unite;
    unite = unite->suiv;
  }

  return tab;
}

int nombreUnite(UListe uliste) {
  Unite *unite;
  int n = 0;
  unite = uliste.unites;
  n += (unite != NULL);
  while(unite != NULL && unite->suiv != NULL) {
    unite = (unite->suiv);
    n++;
  }
  return n;
}

int nombreGenre(UListe uliste, Genre genre) {
  Unite *unite;
  int n = 0;
  unite = uliste.unites;
  if(unite != NULL) {
    n += (unite->genre == genre);
    while(unite->suiv != NULL) {
      unite = (unite->suiv);
      n += (unite->genre == genre);
    }
  }

  return n;
}

int parcourirUniteSelect(Unite **tab, int length) {
  char cmd = 'n';
  int i = -1;
  printf("--------LISTE UNITES--------\n");
  if(length <= 0) {
    printf("Plus aucune unite selectionnable\n");
    printf("----------------------------\n");
  } else {
    while(cmd != 'o') {
      if(i + 1 < length) {
        ++i;
      } else {
        i = 0;
      }
      afficherUnite(*tab[i]);
      printf("Voulez-vous le selectionner ? (o/n)\n");
      scanf(" %c", &cmd);
      printf("----------------------------\n");
    }
  }

  return i;
}


/*fonction générique*/
int enleverTab(void *tab, size_t indice, size_t length, size_t size) {
  if(indice < length - 1) {
    decaleTab(tab, indice, length, size);
  }
  return (length - 1);
}

void decaleTab(void *tab, size_t debut, size_t length, size_t size) {
  size_t j;
  char *p;
  char *pend = (char *)tab + (length - 1) * size;
  for(p = (char *)tab + debut * size; p < pend; p += size) {
    for(j = 0; j < size; ++j) {
      *(p + j) = *(p + size + j);
    }
  }
}

/* BACKUP
Unite *parcourirUnites(UListe uliste) {
  char cmd = 'n';
  Unite *selection;
  selection = uliste.unites;
  printf("--------LISTE UNITES--------\n");
  while(cmd != 'o') {
    if(selection->suiv == NULL) {
      selection = uliste.unites;
    } else {
      selection = selection->suiv;
    }
    afficherUnite(*selection);
    printf("Voulez-vous le selectionner ? (o/n)\n");
    scanf(" %c", &cmd);
    printf("----------------------------\n");
  }

  return selection;
}*/

int actionUnite(Unite *unite, Monde *monde, int* mouvements) {
  char c[MAXCHAR];
  int r;
  printf("Que voulez-vous faire ?\n");
  printf("deplacer | attaquer | attendre | evoluer\n");
  scanf(" %s", c);
  if(strcmp("deplacer", c) == 0) {

    printf("========DEPLACEMENT========\n");
    r = actionDeplacer(unite, monde, mouvements);

  } else if(strcmp("attaquer", c) == 0) {

    printf("==========ATTAQUE==========\n");
    r = actionAttaquer(unite, monde);

    if(r > 0) {
      (*mouvements)--;
    }



  } else if(strcmp("attendre", c) == 0) {

    printf("==========ATTENTE==========\n");
    r = actionAttendre(unite, monde);
    if(r > 0) {
      (*mouvements)--;
    }

  } else if(strcmp("evoluer", c) == 0) {

    printf("===CHANGEMENT DE CLASSE===\n");
    r = 4;

  } else {

    printf("==========RETOUR==========\n");
    r = 0;

  }

  return r;
}

int actionAttaquer(Unite *unite, Monde *monde) {
  Unite **uportee;
  int length, selection, r = 2;

    switch(unite->genre) {
      case(ARCHER):
        uportee = unitesAPortee(*unite, *monde, unite->portee + unite->B_portee, 0, CROIX, &length);
        if(uportee != NULL) {
          if(length > 0) {
            selection = parcourirUniteSelect(uportee, length);
            combat_Archer(unite, uportee[selection], monde);
          } else {
            printf("Pas d'ennemi proche...\n");
            r = 0;
          }
        } else {
          printf("Erreur mémoire !\n");
        }
        break;
      default:
        uportee = unitesAPortee(*unite, *monde, unite->portee + unite->B_portee, 0, LOSANGE, &length);
        if(uportee != NULL) {
          if(length > 0) {
            selection = parcourirUniteSelect(uportee, length);
            combat(unite, uportee[selection], monde);
          } else {
            printf("Pas d'ennemi proche...\n");
            r = 0;
          }
        } else {
          printf("Erreur mémoire !\n");
        }
        break;
    }


  free(uportee);

  return r;
}


void combat_Archer(Unite* exec, Unite *cible, Monde *monde) {
  int pasX = signe(cible->posX - exec->posX);
  int pasY = signe(cible->posY - exec->posY);
  int x, y;
  int portee = exec->portee + exec->B_portee;
  int degats = exec->att + exec->B_att;
  int reverse = 0;
  Unite *unite;

  x = 0;
  y = 0;

  while(((x < portee && y == 0) || (y < portee && x == 0)) && degats > 0) {
    unite = monde->plateau[y + exec->posY][x + exec->posX];
    if(unite != NULL && unite->couleur != exec->couleur) {
      degats = max(attaquer(exec, cible, degats, monde), 0);
      if(peutRiposter(*cible, *exec)) {
        reverse += cible->att + cible->B_att;
      }
    }
    x += pasX;
    y += pasY;
  }

  infligerDegats(exec, reverse, monde);
}

int infligerDegats(Unite* cible, int degats, Monde *monde) {
  int r = 0;
  printf("%c%c (%d,%d) recoit %d de degats !\n", cible->genre, cible->couleur, cible->posX, cible->posY, min(degats, cible->PV));
  cible->PV = max(cible->PV - degats, 0);
  if(cible->PV == 0) {
    printf("%c%c (%d,%d) disparait !\n", cible->genre, cible->couleur, cible->posX, cible->posY);
    enleverUnite(cible, monde);
    r = 1;
  }

  return r;
}

int attaquer(Unite* exec, Unite* cible, int d, Monde *monde) {
  int degats;
  int pv = cible->PV;
  if(seProtege(*cible)) {
    degats = ceil(d / 2);
  } else {
    degats = d;
  }

  cible->subis++;

  if(exec->genre == SORCIERE) {
    soigne(min(cible->PV, degats), exec);
  }

  if(infligerDegats(cible, degats, monde)) {
    exec->kills++;
  }


  return (degats - pv);
}

void combat(Unite* exec, Unite *cible, Monde *monde) {
  int fail = 0;
  int success = (attaquer(exec, cible, exec->att + exec->B_att, monde) >= 0);
  if(!success && peutRiposter(*cible, *exec)) {
    fail = (attaquer(cible, exec, cible->att + cible->B_att, monde) >= 0);
  }
  if(!fail && !success) {
    switch(exec->genre) {
      case(MATRIARCHE):
        exec->etat = PARALYSIE;
        cible->etat = PARALYSIE;
        break;
      case(SERF):
        infligerDegats(exec, 1, monde);
        break;
      default:
        break;
    }

  }
}

int seProtege(Unite cible) {
  return ((cible.genre == GUERRIER || cible.genre == BASTION) && cible.etat == ATTENTE);
}

int peutRiposter(Unite exec, Unite cible) {
  return (exec.genre == CHAMPION && exec.etat == DEFAUT && abs(cible.posX - exec.posX) + abs(cible.posY - exec.posX) <= exec.portee + exec.B_portee);

}

int actionAttendre(Unite *unite, Monde *monde) {
  int r = 3;

  switch(unite->genre) {
    case(SORCIERE):
      r = actionAttendre_Sorciere(unite, monde);
      break;
    case(SAINTE):
      r = actionAttendre_Sainte(unite, monde);
      break;
    default:
      unite->etat = ATTENTE;
      break;
  }

  return r;
}

int actionAttendre_Sorciere(Unite *unite, Monde *monde) {
  Unite **uportee;
  int length, selection;
  int r = 3;
  uportee = unitesAPortee(*unite, *monde, 1, 1, CARRE, &length);
  if(uportee != NULL && length > 0) {
    selection = parcourirUniteSelect(uportee, length);
    if (selection > -1) {
      intervertirPV(unite, uportee[selection]);
    } else {
      printf("Pas d'allié proche...\n");
      r = 0;
    }
  } else {
    printf("Erreur mémoire !\n");
  }

  free(uportee);

  return r;
}

int actionAttendre_Sainte(Unite *unite, Monde *monde) {
  Unite **uportee;
  int length, i;
  int r = 3;
  uportee = unitesAPortee(*unite, *monde, 1, 1, CARRE, &length);
  if(uportee != NULL && length > 0) {

    for(i = 0; i < length; ++i) {
      soigne(unite->att + unite->B_att, uportee[i]);
    }

  } else {
    printf("Erreur mémoire !\n");
  }

  free(uportee);

  return r;
}

void soigne(int soin, Unite *cible) {
  cible->PV = min(soin + cible->PV, cible->PVmax + cible->B_PVmax);
}

void intervertirPV(Unite *exec, Unite *cible) {
  int temp = exec->PV;
  exec->PV = min(cible->PV, exec->PVmax + exec->B_PVmax);
  cible->PV = min(temp, cible->PVmax + cible->B_PVmax);
}


/*À libérer avec free*/
Unite **unitesAPortee(Unite unite, Monde monde, int portee, int alliee, Forme forme, int *length) {
  int i, j, n, cond;
  Unite **select;
  *length = nbUnitesAPortee(unite, monde, portee, alliee, forme);
  select = calloc(*length, sizeof(*select));
  n = 0;

  if(alliee) {  /*Debug si alliee n'est pas 1 ou 0*/
    cond = 1;
  } else {
    cond = 0;
  }

  for(i = unite.posX - portee; i <= unite.posX + portee; ++i) {
    for(j = unite.posY - portee; j <= unite.posY + portee; ++j) {
      if(i >= 0 && i <= LARG && j >= 0 && j <= LONG
      && (i != unite.posX || j != unite.posY)
      && monde.plateau[j][i] != NULL
      && ((monde.plateau[j][i])->couleur == unite.couleur) == cond
      && rangeShape(i, j, unite.posX, unite.posY, portee, forme)) {
        select[n] = monde.plateau[j][i];
        n++;
      }
    }
  }

  return select;
}

int nbUnitesAPortee(Unite unite, Monde monde, int portee, int alliee, Forme forme) {
  int i, j, n, cond;
  n = 0;

  if(alliee) {  /*Debug si alliee n'est pas 1 ou 0*/
    cond = 1;
  } else {
    cond = 0;
  }

  for(i = unite.posX - portee; i <= unite.posX + portee; ++i) {
    for(j = unite.posY - portee; j <= unite.posY + portee; ++j) {
      if(i >= 0 && i <= LARG
        && j >= 0 && j <= LONG
        && (i != unite.posX || j != unite.posY)) {
        n += (monde.plateau[j][i] != NULL
          && ((monde.plateau[j][i])->couleur == unite.couleur) == cond
        && rangeShape(i, j, unite.posX, unite.posY, portee, forme));
      }
    }
  }

  return n;
}

int rangeShape(int x, int y, int centerX, int centerY, int portee, Forme forme) {
  int r;
  switch(forme) {
    /*case(CARRE):
      return (abs(x - centerX) <= portee && abs(y - centerY) <= portee);
      break;*/
    case(LOSANGE):
      return (abs(x - centerX) + abs(y - centerY) <= portee);
      break;
    case(CROIX):
      return ((abs(x - centerX) <= portee && y - centerY == 0) || (abs(y - centerY) <= portee && x - centerX == 0));
      break;
    default:
      r = 1;
      break;
  }

  return r;
}

int actionDeplacer(Unite *unite, Monde *monde, int *mouvements) {
  int posX, posY, r = 1;
  printf("Indiquer positions x,y : ");
  scanf("%d,%d", &posX, &posY);
  switch(deplacerUniteAuto(unite, monde, posX, posY, mouvements)) {
    case(-1):
      printf("Obstacle rencontre !\n");
      break;
    case(0):
      printf("Erreur dans le deplacement !\n");
      r = 0;
      break;
    case(1):
      printf("Deplacement reussi !\n");
      break;
    case(2):
      printf("Erreur mémoire !\n");
      break;
  }

  return r;
}

void afficherUnite(Unite unite) {
  printf("\tGenre : %c\tEtat : %c\n", unite.genre, unite.etat);
  printf("\tPositions x,y : %d,%d\n", unite.posX, unite.posY);
  printf("\tStats : PV %d/%d (%d)\tATT %d (%d)\t PORTEE %d (%d)\n", unite.PV, unite.PVmax + unite.B_PVmax, unite.B_PVmax, unite.att + unite.B_att, unite.B_att, unite.portee + unite.B_portee, unite.B_portee);
}

/*BACKUP
int attaquer(Unite *unite, Monde *monde, int destX, int destY){
    if(monde->plateau[destY][destX] !=NULL && unite->couleur!= monde->plateau[destY][destX]->couleur ){
                if (unite->genre==GUERRIER || unite->genre==monde->plateau[destY][destX]->genre){
                    enleverUnite(monde->plateau[destY][destX],monde);
                    deplacerUnite(unite,monde,destX,destY);
                    return 1;
                        }
                    enleverUnite(unite,monde);
                    return 0;
    }

   return 0;

}

int deplacerOuAttaquer(Unite *unite, Monde *monde, int destX, int destY){
    if( destX >= LONG || destY >= LARG){
        return -1;
    }

    if( destX <= LONG && destY <= LARG && abs(destX-(unite->posX))>=1 && abs(destY-(unite->posY))>=1 ){
        return -2;
    }

       if(monde->plateau[destY][destX]->genre == unite->genre){
        return -3;
    }

    if(monde->plateau[destY][destX] == NULL && destX <= LONG && destY <= LARG && abs(destX-(unite->posX))<=1 && abs(destY-(unite->posY))<=1 ) // On verifie que la destination existe et est vide et que c'est un déplacement adjacent
    {   deplacerUnite(unite,monde,destX,destY);

        return 1;
    }

    if(monde->plateau[destY][destX]->genre != unite->genre  && destX <= LONG && destY <= LARG && abs(destX-(unite->posX))<=1 && abs(destY-(unite->posY))<=1 ){
        attaquer(unite,monde,destX,destY);
        if ((attaquer(unite,monde,destX,destY))==1){
            return 2;
        }
        return 3;
    }
    return 0;
}*/

void viderMonde(Monde *monde) {
  viderUListe(monde->rouge);
  viderUListe(monde->bleu);
  initialiserPlateau(monde->plateau);
}

void viderUListe(UListe *uliste) {
  Unite *temp, *unite;
  temp = uliste->unites;
  if(temp != NULL) {
    while(temp->suiv != NULL) {
      unite = temp;
      temp = temp->suiv;
      free(unite);
    }
    free(uliste);
  }

}

void gererTour(Monde *monde) {

    ++(monde->tour);
    gererTourJoueur(ROUGE, monde);
    gererTourJoueur(BLEU, monde);

}

void placerUnite(Monde *monde, UListe *uliste, Genre genre){
    int posX, posY;
    int k = scanf("%d,%d", &posX, &posY);
    Unite *unite = creerUnite(genre, uliste, monde->tour);

    while ((k < 2) || !placerAuMonde(unite, monde, posX, posY)){
      printf("Position indisponible \n");
      k = scanf("%d,%d", &posX, &posY);
    }

}

void placementParJoueur(Monde *monde, Couleur couleur){
    UListe *uliste=getUListe(couleur,monde);
    printf("Ou voulez-vous positionner vos deux serfs ? \n ");
    placerUnite(monde,uliste,SERF);
    placerUnite(monde,uliste,SERF);
    /*printf(" Placez vos guerriers. \n");
    placerUnite(monde,uliste,GUERRIER);
    placerUnite(monde,uliste,GUERRIER);
    placerUnite(monde,uliste,GUERRIER);
    printf(" Placez votre Matriarche. \n");
    placerUnite(monde,uliste,MATRIARCHE);*/
}

void placementInitial(Monde *monde){
    char couleur;
    printf("Qui commence ? (R/B) \n");
    scanf(" %c",&couleur);
    while(couleur != ROUGE && couleur != BLEU) {
      printf("Veuillez utiliser les commandes indiquees !\n");
      scanf(" %c",&couleur);
    }
    placementParJoueur(monde,couleur);
    printf("À l'autre joueur de placer ses unités :) \n");
    if('B' == couleur){
    placementParJoueur(monde,ROUGE);
  } else {
    placementParJoueur(monde,BLEU);
  }
}

int arreterPartie(){
    char reponse;
    printf("Voulez vous quitter la partie ? (o/n)\n");
    scanf(" %c",&reponse);
    if('o' == reponse){

        printf("Merci d'avoir joué \n");
        return 1;
    }
    return 0;
}



void gererPartie(void){
    Monde mondejeu;
    int arret = 0;
    srand(time(NULL)); /*pour le rand*/
    initialiserMonde(&mondejeu);
    affichePlateau(mondejeu);
    /*3PIONS 1 GUERRIER 2 SERFS */
    placementInitial(&mondejeu);
    printf("Debut de la partie \n ");
    affichePlateau(mondejeu);
    while( !arret && (nombreUnite(*(mondejeu.rouge)) > 0 && nombreUnite(*(mondejeu.bleu)) > 0)) {
    gererTour(&mondejeu);
    arret = arreterPartie(mondejeu);
    }
        /*viderMonde(&mondejeu);*/
    if(!arret) {
      if (nombreUnite(*(mondejeu.bleu)) <= 0)
      {
          printf("Fin de la partie, le joueur ROUGE a gagne !");
      } else {
          printf("Fin de la partie, le joueur BLEU a gagne !");
      }
    }
    else{
      printf("Fin de la partie, pas de gagnants.");
    }
}
