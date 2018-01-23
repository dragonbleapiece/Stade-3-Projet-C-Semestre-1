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
    (monde->rouge)->champion = 0;
    (monde->bleu) = calloc(1, sizeof(*(monde->bleu)));
    if((monde->bleu) == NULL) {
      return 0;
    }
    (monde->bleu)->couleur = BLEU;
    (monde->bleu)->unites = NULL;
    (monde->bleu)->champion = 0;
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

  unite->PVmax = PVmax;
  unite->att = att;
  unite->portee = 1;

  switch(genre) {
    case(GUERRIER):
      upGuerrier(unite);
      break;
    case(SERF):
      unite->PV = unite->PVmax;
      break;
    case(MATRIARCHE):
      upMatriarche(unite);
      break;
    case(ASSASSIN):
      upAssassin(unite);
      break;
    case(ARCHER):
      upGuerrier(unite);
      upArcher(unite);
      break;
    case(BARDE):
      upGuerrier(unite);
      upArcher(unite);
      upBarde(unite);
      break;
    case(DUC):
      upGuerrier(unite);
      upDuc(unite);
      break;
    case(ARCHIDUC):
      upGuerrier(unite);
      upDuc(unite);
      upArchiduc(unite);
      break;
    case(BASTION):
      upGuerrier(unite);
      upBastion(unite);
      break;
    case(CHAMPION):
      upGuerrier(unite);
      upDuc(unite);
      unite->att -= 10;
      upChampion(unite);
      break;
    case(SORCIERE):
      upMatriarche(unite);
      upSorciere(unite);
      break;
    case(SAINTE):
      upMatriarche(unite);
      upSainte(unite);
      break;
    default:
      unite->PVmax = 0;
      unite->att = 0;
      unite->portee = 0;
      unite->PV = unite->PVmax;
      break;
  }
}

void upGuerrier(Unite *unite) {
  unite->PVmax += 10;
  unite->PV = unite->PVmax;
  unite->att += 3;
  unite->portee = 1;
  unite->genre = GUERRIER;
}

void upAssassin(Unite *unite) {
  unite->PV = unite->PVmax;
  unite->att = 20;
  unite->portee = 1;
  unite->genre = ASSASSIN;
}

void upArcher(Unite *unite) {
  unite->PVmax += -7;
  unite->PV = unite->PVmax;
  unite->att += -2;
  unite->portee = 3;
  unite->genre = ARCHER;
}

void upBarde(Unite *unite) {
  unite->PVmax += 3;
  unite->PV = unite->PVmax;
  unite->att += 5;
  unite->portee = 3;
  unite->genre = BARDE;
}

void upBastion(Unite *unite) {
  unite->PVmax += 20;
  unite->PV = unite->PVmax;
  unite->att += -3;
  unite->portee = 1;
  unite->genre = BASTION;
}

void upDuc(Unite *unite) {
  unite->PVmax += 5;
  unite->PV = unite->PVmax;
  unite->att += 5;
  unite->portee = 1;
  unite->genre = DUC;
}

void upArchiduc(Unite *unite) {
  unite->PVmax += -15;
  unite->PV = unite->PVmax;
  unite->att = 1;
  unite->portee = 1;
  unite->genre = ARCHIDUC;
}

void upChampion(Unite *unite) {
  unite->PVmax += 5;
  unite->PV = unite->PVmax;
  unite->att += 12;
  unite->portee = 2;
  unite->genre = CHAMPION;
}

void upSorciere(Unite *unite) {
  unite->PVmax = 10;
  unite->PV = unite->PVmax;
  unite->att = 6;
  unite->portee = 2;
  unite->genre = SORCIERE;
}

void upSainte(Unite *unite) {
  unite->PVmax = 15;
  unite->PV = unite->PVmax;
  unite->att = 3;
  unite->portee = 2;
  unite->genre = SAINTE;
}

void upMatriarche(Unite *unite) {
  unite->PVmax = 1;
  unite->PV = unite->PVmax;
  unite->att = 1;
  unite->portee = 3;
  unite->genre = MATRIARCHE;
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
    if((unite->genre == BASTION && nbUnitesAPortee(*unite, *monde, 1, 0, CROIX) == 0) || unite->genre != BASTION) {
      if(destX < LARG
        && destY < LONG
        && destX >= 0
        && destY >= 0
        && !(destX == unite->posX && destY == unite->posY))
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

  } else {
    return -1;
  }

}

int deplacerUniteAuto(Unite *unite, Monde *monde, int destX, int destY, int *mouvements) {
    if(destX < LARG
      && destY < LONG
      && destX >= 0
      && destY >= 0
      && !(destX == unite->posX && destY == unite->posY)
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

void enleverUnite(Unite **unite, Monde *monde) {
  UListe *uliste = getUListe((*unite)->couleur, monde);
  Unite *unitePrec = getUnitePrec(*unite, uliste);
  if(unitePrec == *unite) {
    uliste->unites = (*unite)->suiv;
  } else if(unitePrec != NULL) {
    unitePrec->suiv = (*unite)->suiv;
  }
  monde->plateau[(*unite)->posY][(*unite)->posX] = NULL;
  free(*unite);
  *unite = NULL;
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

UListe *getUListeAdverse(Couleur couleur, Monde *monde) {
  if(couleur == (monde->rouge)->couleur) {
    return (monde->bleu);
  }
  else if(couleur == (monde->bleu)->couleur) {
    return (monde->rouge);
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
  UListe adverse = *getUListeAdverse(couleur, monde);
  int nUnite;
  Unite **uniteSelect;
  int action, mouvements;

  attaqueAutoBastion(uliste, monde);
  evolution(uliste, *monde);
  attente(uliste, monde);

  mouvements = nombreGenre(uliste, SERF) + 1;
  uniteSelect = creerSelection(uliste, DEFAUT, &nUnite);

  if(nUnite && uniteSelect != NULL) {
    affichePlateau(*monde);
    printf("Tour : %d | Joueur : %c\n", monde->tour, couleur);

    debutTourJoueur(uliste, monde, uniteSelect, nUnite);


    do {
      printf("Points de mouvement : %d\n", mouvements);
      selection = parcourirUniteSelect(uniteSelect, &nUnite);
      if(selection > -1 && mouvements > 0) {
        action = actionUnite(&(uniteSelect[selection]), monde, &mouvements);
        if(action == 4 || action == 3) {
          nUnite = enleverTab(uniteSelect, selection, nUnite, sizeof(*uniteSelect));
        }
        affichePlateau(*monde);

        if(nombreUnite(adverse) > 0 && !aPerduChampion(adverse)) {
          printf("Voulez-vous arreter votre tour ? (o/n)\n");
          scanf(" %c", &cmd);
        } else {
          cmd = 'o';
        }

      } else {
        if(mouvements <= 0) {
          printf("Vous n'avez plus aucun ordre à donner.\n");
        }
        printf("Arret du tour !\n");
        cmd = 'o';
      }
    } while(cmd != 'o');

    /*finTourJoueur*/paralysie(uliste);

    free(uniteSelect);
  }
}

void debutTourJoueur(UListe uliste, Monde *monde, Unite **tab, int length) {
  Unite *mater = getUniteByGenre(uliste, MATRIARCHE);
  renforceGuerriers(tab, length);
  reinitialiseBonusMalus(tab, length);
  if(mater != NULL) {
    augmenteEvoMatriarche(mater, *monde);
  }
}

void attaqueAutoBastion(UListe uliste, Monde *monde) {
  int length, i;
  Unite **bastions = creerSelectionGenre(uliste, BASTION, &length);
  for(i = 0; i < length; ++i) {
    attaqueZone(&(bastions[i]), monde, 1, CARRE);
  }
  free(bastions);
}

void attaqueZone(Unite **exec, Monde *monde, int portee, Forme forme) {
  int length;
  int i, d, reverse;
  Unite **zone = unitesAPortee(**exec, *monde, portee, 0, forme, &length);
  reverse = 0;
  for(i = 0; i < length; ++i) {
    d = attaquer(exec, &(zone[i]), (*exec)->att + (*exec)->B_att, monde);
    if(d < 0 && peutRiposter(*zone[i], **exec)) {
      reverse += zone[i]->att + zone[i]->B_att;
    }
  }
  if(seProtege(**exec)) {
    reverse = ceil(reverse / 2);
  }
  infligerDegats(exec, reverse, monde);
  free(zone);
}

void renforceGuerriers(Unite **tab, int length) {
  int i;
  Unite *unite;
  for(i = 0; i < length; ++i) {
    unite = tab[i];
    if(unite->genre == GUERRIER && unite->evo == 0 && unite->PV <= 5) {
      unite->att += 5;
      unite->evo = 1;
    }
  }
}

void augmenteEvoMatriarche(Unite *unite, Monde monde) {
  int nUnite = nbUnitesAPortee(*unite, monde, 1, 1, CROIX);
  if(nUnite > 0) {
    unite->evo = min(unite->evo + 1, 10);
  } else {
    unite->evo = 0;
  }
}

Unite *getUniteByGenre(UListe uliste, Genre genre) {
  Unite* unite = uliste.unites;
  while(unite != NULL && unite->genre != genre) {
    unite = unite->suiv;
  }

  return unite;
}

void reinitialiseBonusMalus(Unite **tab, int length) {
  int i;
  for(i = 0; i < length; ++i) {
    tab[i]->B_att = 0;
    tab[i]->B_PVmax = 0;
    tab[i]->PV = min(tab[i]->PV, tab[i]->PVmax);
    tab[i]->B_portee = 0;
  }
}



void evolution(UListe uliste, Monde monde) {
  int nUnite, i;
  Unite **tab = creerSelection(uliste, EVOLUTION, &nUnite);
  if(tab != NULL) {
    reinitialiseBonusMalus(tab, nUnite);
    for(i = 0; i < nUnite; ++i) {
      evoluer(tab[i], monde);
    }
  }

  free(tab);
}

void evoluer(Unite *unite, Monde monde) {
  switch(unite->evo) {
    case(GUERRIER):
      upGuerrier(unite);
      break;
    case(ARCHER):
      upArcher(unite);
      break;
    case(BARDE):
      upBarde(unite);
      break;
    case(BASTION):
      upBastion(unite);
      break;
    case(DUC):
      upDuc(unite);
      break;
    case(ARCHIDUC):
      upArchiduc(unite);
      break;
    case(CHAMPION):
      upChampion(unite);
      getUListe(unite->couleur, &monde)->champion = 1;
      break;
    case(SORCIERE):
      upSorciere(unite);
      break;
    case(SAINTE):
      upSainte(unite);
      break;
    case(ASSASSIN):
      upAssassin(unite);
      break;
    default:
      unite->att = 0;
      unite->PVmax = 0;
      unite->PV = 0;
      unite->portee = 0;
      break;
  }
  unite->evo = 0;
  unite->kills = 0;
  unite->subis = 0;
  unite->tour = monde.tour;
  unite->etat = DEFAUT;
}

void attente(UListe uliste, Monde *monde) {
  int nUnite, i;
  Unite **tab = creerSelection(uliste, ATTENTE, &nUnite);
  if(tab != NULL) {
    reinitialiseBonusMalus(tab, nUnite);
    for(i = 0; i < nUnite; ++i) {
      attendre(tab[i], monde);
    }
  }

  free(tab);
}

void attendre(Unite *unite, Monde *monde) {
  switch(unite->genre) {
    case(CHAMPION):
      soigne(5, unite);
      break;
    case(MATRIARCHE):
      if(rand_a_b(0,100) < 50) {
        production(*unite, monde, SERF, CROIX, 1);
      }
      break;
    default:
      break;
  }
  unite->etat = DEFAUT;
}

void production(Unite unite, Monde *monde, Genre genre, Forme forme, int portee) {
  int length, random;
  Coord coord;
  Coord *places = placesAPortee(unite, *monde, portee, forme, &length);

  if(places != NULL && length > 0) {
    random = rand_a_b(0, length);
    coord = places[random];
    placerAuMonde(creerUnite(genre, getUListe(unite.couleur, monde), monde->tour), monde, coord.x, coord.y);
  }

  free(places);
}



void paralysie(UListe uliste) {
  int nUnite, i;
  Unite **tab = creerSelection(uliste, PARALYSIE, &nUnite);
  if(tab != NULL) {
    reinitialiseBonusMalus(tab, nUnite);
    for(i = 0; i < nUnite; ++i) {
      tab[i]->etat = DEFAUT;
    }
  }

  free(tab);
}

Unite **creerSelection(UListe uliste, Etat etat, int *n) {
  int i;
  Unite **tab;
  Unite *unite;
  *n = nombreEtat(uliste, etat);
  tab = calloc(*n, sizeof(*tab));
  unite = uliste.unites;
  i = 0;
  while(i < *n && unite != NULL) {
    if(unite->etat == etat) {
      tab[i] = unite;
      ++i;
    }
    unite = unite->suiv;
  }

  return tab;
}

Unite **creerSelectionGenre(UListe uliste, Genre genre, int *n) {
  int i;
  Unite **tab;
  Unite *unite;
  *n = nombreGenre(uliste, genre);
  tab = calloc(*n, sizeof(*tab));
  unite = uliste.unites;
  i = 0;
  while(i < *n && unite != NULL) {
    if(unite->genre == genre) {
      tab[i] = unite;
      ++i;
    }
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

int nombreEtat(UListe uliste, Etat etat) {
  Unite *unite;
  int n = 0;
  unite = uliste.unites;
  if(unite != NULL) {
    n += (unite->etat == etat);
    while(unite->suiv != NULL) {
      unite = (unite->suiv);
      n += (unite->etat == etat);
    }
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

int parcourirUniteSelect(Unite **tab, int *length) {
  char cmd = 'n';
  int i = -1;
  printf("--------LISTE UNITES--------\n");
  while(cmd != 'o' && *length > 0) {
    if(i + 1 < *length) {
      ++i;
    } else {
      i = 0;
    }
    if(tab[i] == NULL) {
      *length = enleverTab(tab, i, *length, sizeof(*tab));
    } else {
      afficherUnite(*tab[i]);
      printf("Voulez-vous le selectionner ? (o/n)\n");
      scanf(" %c", &cmd);
      printf("----------------------------\n");
    }
  }

  if(*length <= 0) {
    printf("Plus aucune unite selectionnable\n");
    printf("----------------------------\n");
    i = -1;
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

int actionUnite(Unite **unite, Monde *monde, int* mouvements) {
  char c[MAXCHAR];
  int r;
  printf("Que voulez-vous faire ?\n");
  printf("deplacer | attaquer | attendre | evoluer\n");
  scanf(" %s", c);
  if(strcmp("deplacer", c) == 0) {

    printf("========DEPLACEMENT========\n");
    r = actionDeplacer(*unite, monde, mouvements);

  } else if(strcmp("attaquer", c) == 0) {

    printf("==========ATTAQUE==========\n");
    r = actionAttaquer(unite, monde);

    if(r > 0) {
      (*mouvements)--;
    }



  } else if(strcmp("attendre", c) == 0) {

    printf("==========ATTENTE==========\n");
    r = actionAttendre(*unite, monde);
    if(r > 0) {
      (*mouvements)--;
    }

  } else if(strcmp("evoluer", c) == 0) {

    printf("===CHANGEMENT DE CLASSE===\n");

    r = actionEvoluer(*unite, monde);

    if(r > 0) {
      (*mouvements)--;
    }
  } else {

    printf("==========RETOUR==========\n");
    r = 0;

  }

  return r;
}

int actionEvoluer(Unite *unite, Monde *monde) {
  int r = 4;
  int i = 0;
  char c;

  char *evo = checkEvolutions(*unite, *monde);
  while(evo[i] != '\0') {
    switch(evo[i]) {
      case(ASSASSIN):
        printf("Assassin (%c)\n", ASSASSIN);
        break;
      case(SORCIERE):
        printf("Sorciere (%c)\n", SORCIERE);
        break;
      case(SAINTE):
        printf("Sainte (%c)\n", SAINTE);
        break;
      case(GUERRIER):
        printf("Guerrier (%c)\n", GUERRIER);
        break;
      case(ARCHER):
        printf("Archer (%c)\n", ARCHER);
        break;
      case(BARDE):
        printf("Barde (%c)\n", BARDE);
        break;
      case(DUC):
        printf("Duc (%c)\n", DUC);
        break;
      case(ARCHIDUC):
        printf("ARCHIDUC (%c)\n", ARCHIDUC);
        break;
      case(CHAMPION):
        printf("Champion (%c)\n", CHAMPION);
        break;
      default:
        break;
    }
    ++i;
  }


  if(i == 0) {
    printf("Votre unite ne peut pas evoluer...\n");
    r = 0;
  } else {
    printf("Inscrivez un caractere correspondant a la classe vers laquelle evoluer :\n");
    while(!scanf(" %c", &c) || strchr(evo, c) == NULL) {
      printf("Veuillez inscrire un caractere valide...\n");
    }
    unite->evo = c;
    unite->etat = EVOLUTION;
  }

  free(evo);
  return r;
}

/*À libérer avec free*/
char *checkEvolutions(Unite unite, Monde monde) {
  char *evo = calloc(MAXEVO + 1, sizeof(*evo));
  int i = 0;
  switch(unite.genre) {
    case(SERF):
      if(unite.evo == 1) {
        evo[i] = ASSASSIN;
        ++i;
      }
      if(monde.tour - unite.tour >= 3) {
        evo[i] = GUERRIER;
        ++i;
      }
      break;
    case(MATRIARCHE):
      if(unite.evo >= 10) {
        evo[i] = SAINTE;
        ++i;
      }
      if(unite.kills > 0) {
        evo[i] = SORCIERE;
        ++i;
      }
      break;
    case(GUERRIER):
      evo[i] = ARCHER;
      ++i;
      if(unite.subis >= 3) {
        evo[i] = BASTION;
        ++i;
      }
      if(unite.kills >= 3) {
        evo[i] = DUC;
        ++i;
      }
      break;
    case(ARCHER):
      if(monde.tour - unite.tour >= 7) {
        evo[i] = BARDE;
        ++i;
      }
      break;
    case(DUC):
      if(nombreGenre(*getUListe(unite.couleur, &monde), CHAMPION) < 1 && unite.kills >= 5) {
        evo[i] = CHAMPION;
        ++i;
      }
      if(unite.att <= 5) {
        evo[i] = ARCHIDUC;
        ++i;
      }
      break;
    default:
      break;
  }

  evo[i] = '\0';
  return evo;
}

int actionAttaquer(Unite **unite, Monde *monde) {
  Unite **uportee;
  int length, selection, r = 2;

    switch((*unite)->genre) {
      case(ARCHER):
        uportee = unitesAPortee(**unite, *monde, (*unite)->portee + (*unite)->B_portee, 0, CROIX, &length);
        if(uportee != NULL) {
          if(length > 0) {
            selection = parcourirUniteSelect(uportee, &length);
            combat_Archer(unite, &(uportee[selection]), monde);
          } else {
            printf("Pas d'ennemi proche...\n");
            r = 0;
          }
        } else {
          printf("Erreur mémoire !\n");
        }
        break;
      default:
        uportee = unitesAPortee(**unite, *monde, (*unite)->portee + (*unite)->B_portee, 0, LOSANGE, &length);
        if(uportee != NULL) {
          if(length > 0) {
            selection = parcourirUniteSelect(uportee, &length);
            combat(unite, &(uportee[selection]), monde);
          } else {
            printf("Pas d'ennemi proche...\n");
            r = 0;
          }
        } else {
          printf("Erreur mémoire !\n");
        }
        break;
    }
  if(*unite == NULL || (*unite)->etat == PARALYSIE) {
    r = 0;
  }

  free(uportee);

  return r;
}


void combat_Archer(Unite** exec, Unite **cible, Monde *monde) {
  int pasX = signe((*cible)->posX - (*exec)->posX);
  int pasY = signe((*cible)->posY - (*exec)->posY);
  int x, y;
  int portee = (*exec)->portee + (*exec)->B_portee;
  int degats = (*exec)->att + (*exec)->B_att;
  int reverse = 0;
  Unite *unite;

  x = 0;
  y = 0;

  while(((x < portee && y == 0) || (y < portee && x == 0)) && degats > 0) {
    unite = monde->plateau[y + (*exec)->posY][x + (*exec)->posX];
    if(unite != NULL && unite->couleur != (*exec)->couleur) {
      degats = max(attaquer(exec, cible, degats, monde), 0);
      if(!degats && peutRiposter(**cible, **exec)) {
        reverse += (*cible)->att + (*cible)->B_att;
      }
    }
    x += pasX;
    y += pasY;
  }

  infligerDegats(exec, reverse, monde);
}

int infligerDegats(Unite** cible, int degats, Monde *monde) {
  int r = 0;
  printf("%c%c (%d,%d) recoit %d de degats !\n", (*cible)->genre, (*cible)->couleur, (*cible)->posX, (*cible)->posY, min(degats, (*cible)->PV));
  (*cible)->PV = max((*cible)->PV - degats, 0);
  if((*cible)->PV == 0) {
    printf("%c%c (%d,%d) disparait !\n", (*cible)->genre, (*cible)->couleur, (*cible)->posX, (*cible)->posY);
    enleverUnite(cible, monde);
    r = 1;
  }

  return r;
}

int attaquer(Unite **exec, Unite **cible, int d, Monde *monde) {
  int degats;
  Genre genreCible = (*cible)->genre;
  int pv = (*cible)->PV;
  if(seProtege(**cible)) {
    degats = ceil((float) d / 2.0);
  } else {
    degats = d;
  }

  (*cible)->subis++;

  if((*exec)->genre == SORCIERE) {
    soigne(min((*cible)->PV, degats), *exec);
  }

  if(infligerDegats(cible, degats, monde)) {
    (*exec)->kills++;
    if((*exec)->genre == SERF && genreCible == MATRIARCHE) {
      (*exec)->evo = 1;
    }
  }


  return (degats - pv);
}

void combat(Unite **exec, Unite **cible, Monde *monde) {
  int fail = 0;
  int degats = (*exec)->att + (*exec)->B_att;

  if((*exec)->genre == ARCHIDUC) {
    degats += nombreGenre(*getUListe((*exec)->couleur, monde), SERF);
  }

  int success = (attaquer(exec, cible, degats, monde) >= 0);
  if(!success && peutRiposter(**cible, **exec)) {
    fail = (attaquer(cible, exec, (*cible)->att + (*cible)->B_att, monde) >= 0);
  }


  if(!fail && !success) {
    switch((*exec)->genre) {
      case(MATRIARCHE):
        (*exec)->etat = PARALYSIE;
        (*cible)->etat = PARALYSIE;
        break;
      case(SERF):
        infligerDegats(exec, 1, monde);
        break;
      case(BARDE):
        infligeMalusZone(*cible, *monde, 1, CROIX, MALUS_BARDE);
        infligeBonusMalus(*cible, MALUS_BARDE);
        break;
      case(DUC):
        production(**exec, monde, SERF, CROIX, 1);
        (*exec)->att = min((*exec)->att - 1, 1);
        break;
      default:
        break;
    }

  }
}

void infligeMalusZone(Unite *cible, Monde monde, int portee, Forme forme, int malus) {
  int length, i;
  Unite **uporte;
  uporte = unitesAPortee(*cible, monde, portee, 1, forme, &length);
  if(uporte != NULL) {
    for(i = 0; i < length; ++i) {
      infligeBonusMalus(uporte[i], malus);
    }
  }
  free(uporte);
}

void infligeBonusMalus(Unite *cible, int bm) {
  if(bm > 0 && cible->B_att <= 0) {
    cible->B_att += bm;
  } else if (bm < 0 && cible->B_att % BONUS_BARDE == 0) {
    /*max bonus == 3 && max malus == -2*/
    cible->B_att += bm;
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
      unite->etat = ATTENTE;
      break;
    case(SAINTE):
      r = actionAttendre_Sainte(unite, monde);
      unite->etat = ATTENTE;
      break;
    case(BARDE):
      r = actionAttendre_Barde(unite, monde);
      unite->etat = ATTENTE;
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
    selection = parcourirUniteSelect(uportee, &length);
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

int actionAttendre_Barde(Unite *unite, Monde *monde) {
  Unite **uportee;
  int length, selection;
  int r = 3;
  uportee = unitesAPortee(*unite, *monde, unite->portee + unite->B_portee, 1, CROIX, &length);
  if(uportee != NULL && length > 0) {
    selection = parcourirUniteSelect(uportee, &length);
    if (selection > -1) {
      infligeBonusMalus(uportee[selection], BONUS_BARDE);
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
      if(i >= 0 && i < LARG && j >= 0 && j < LONG
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

/*À libérer avec free*/
Coord *placesAPortee(Unite unite, Monde monde, int portee, Forme forme, int *length) {
  int i, j, n;
  Coord coord;
  Coord *select;
  *length = nbPlacesAPortee(unite, monde, portee, forme);
  select = calloc(*length, sizeof(*select));
  n = 0;


  for(i = unite.posX - portee; i <= unite.posX + portee; ++i) {
    for(j = unite.posY - portee; j <= unite.posY + portee; ++j) {
      if(i >= 0 && i < LARG && j >= 0 && j < LONG
      && (i != unite.posX || j != unite.posY)
      && monde.plateau[j][i] == NULL
      && rangeShape(i, j, unite.posX, unite.posY, portee, forme)) {
        coord.x = i;
        coord.y = j;
        select[n] = coord;
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
      if(i >= 0 && i < LARG
        && j >= 0 && j < LONG
        && (i != unite.posX || j != unite.posY)) {
        n += (monde.plateau[j][i] != NULL
          && ((monde.plateau[j][i])->couleur == unite.couleur) == cond
        && rangeShape(i, j, unite.posX, unite.posY, portee, forme));
      }
    }
  }

  return n;
}

int nbPlacesAPortee(Unite unite, Monde monde, int portee, Forme forme) {
  int i, j, n;
  n = 0;

  for(i = unite.posX - portee; i <= unite.posX + portee; ++i) {
    for(j = unite.posY - portee; j <= unite.posY + portee; ++j) {
      if(i >= 0 && i < LARG
        && j >= 0 && j < LONG
        && (i != unite.posX || j != unite.posY)) {
        n += (monde.plateau[j][i] == NULL
        && rangeShape(i, j, unite.posX, unite.posY, portee, forme));
      }
    }
  }

  return n;
}

int rangeShape(int x, int y, int centerX, int centerY, int portee, Forme forme) {
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
      return 1;
      break;
  }

}

int actionDeplacer(Unite *unite, Monde *monde, int *mouvements) {
  int posX, posY, r = 1;
  Unite copie = *unite;
  printf("Indiquer positions x,y : ");
  scanf("%d,%d", &posX, &posY);
  copie.posX = posX;
  copie.posY = posY;
  if((unite->genre == MATRIARCHE || unite->genre == SORCIERE || unite->genre == SAINTE) && nbUnitesAPortee(copie, *monde, 1, 1, CROIX) > 0) {

    r = deplacerUnite(unite, monde, posX, posY);

  } else if (unite->genre == ASSASSIN && nbUnitesAPortee(copie, *monde, 1, 0, CROIX) > 0){

    r = deplacerUnite(unite, monde, posX, posY);

  } else {

    r = deplacerUniteAuto(unite, monde, posX, posY, mouvements);

  }
  switch(r) {
    case(-1):
      printf("Obstacle rencontre !\n");
      break;
    case(0):
      printf("Erreur dans le deplacement !\n");
      break;
    case(1):
      printf("Deplacement reussi !\n");
      break;
    case(-2):
      printf("Erreur mémoire !\n");
      break;
    default:
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
    if(nombreUnite(*(monde->bleu)) > 0 && nombreUnite(*(monde->rouge)) > 0 && !aPerduChampion(*(monde->bleu)) && !aPerduChampion(*(monde->rouge))) {
      gererTourJoueur(BLEU, monde);
    }

}

int aPerduChampion(UListe uliste) {
  return (nombreGenre(uliste, CHAMPION) == 0 && uliste.champion == 1);
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
    int i = 0;
    placerAuMonde(creerUnite(MATRIARCHE, uliste, monde->tour), monde, abs((couleur == BLEU) * (LARG - 1) - i), (couleur == BLEU) * (LONG - 1));
    i++;
    placerAuMonde(creerUnite(SERF, uliste, monde->tour), monde, abs((couleur == BLEU) * (LARG - 1) - i), (couleur == BLEU) * (LONG - 1));
    i++;
    placerAuMonde(creerUnite(SERF, uliste, monde->tour), monde, abs((couleur == BLEU) * (LARG - 1) - i), (couleur == BLEU) * (LONG - 1));
    i++;
    placerAuMonde(creerUnite(GUERRIER, uliste, monde->tour), monde, abs((couleur == BLEU) * (LARG - 1) - i), (couleur == BLEU) * (LONG - 1));
    i++;
    placerAuMonde(creerUnite(GUERRIER, uliste, monde->tour), monde, abs((couleur == BLEU) * (LARG - 1) - i), (couleur == BLEU) * (LONG - 1));
    i++;
    placerAuMonde(creerUnite(GUERRIER, uliste, monde->tour), monde, abs((couleur == BLEU) * (LARG - 1) - i), (couleur == BLEU) * (LONG - 1));
}

void placementInitial(Monde *monde){
    placementParJoueur(monde, ROUGE);
    placementParJoueur(monde, BLEU);
}

int arreterPartie(Monde monde){
    char reponse;
    if((nombreUnite(*(monde.rouge)) > 0
    && nombreUnite(*(monde.bleu)) > 0)
    && !aPerduChampion(*(monde.rouge))
    && !aPerduChampion(*(monde.bleu))) {
      printf("Voulez vous continuer la partie ? (o/n)\n");
      scanf(" %c",&reponse);
      if('n' == reponse){

          printf("Merci d'avoir joué \n");
          return 1;
      }
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
    while( !arret
      && (nombreUnite(*(mondejeu.rouge)) > 0
      && nombreUnite(*(mondejeu.bleu)) > 0)
      && !aPerduChampion(*(mondejeu.rouge))
      && !aPerduChampion(*(mondejeu.bleu))) {
    gererTour(&mondejeu);
    arret = arreterPartie(mondejeu);
    }
        /*viderMonde(&mondejeu);*/
    if(!arret) {
      if (nombreUnite(*(mondejeu.bleu)) <= 0 || aPerduChampion(*(mondejeu.bleu)))
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
