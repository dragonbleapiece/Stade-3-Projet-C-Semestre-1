#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
  unite->evoSpe = 0;
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
    if(monde->plateau[destY][destX] == NULL && destX <= LONG && destY <= LARG && abs(destX-(unite->posX))<=1 && abs(destY-(unite->posY))<=1 ) /* On verifie que la destination existe et est vide et que c'est un déplacement adjacent */
    {
                    monde->plateau[unite->posY][unite->posX]=NULL;
                    unite->posX = destX;
                    unite->posY = destY;
                    monde->plateau[destY][destX] = unite;
                    return 1;
    } else {
      return 0;
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
  int nUnite = nombreUnite(uliste);
  Unite **uniteSelect = creerSelection(uliste);

  if(nUnite) {
    affichePlateau(*monde);
    printf("Tour : %d | Joueur : %c\n", monde->tour, couleur);
    do {
      selection = parcourirUniteSelect(uniteSelect, nUnite);
      if(selection != -1) {
        actionUnite(uniteSelect[selection], monde);
        nUnite = enleverSelect(uniteSelect, selection, nUnite);
        affichePlateau(*monde);
        printf("Voulez-vous arreter votre tour ? (o/n)\n");
        scanf(" %c", &cmd);
      } else {
        printf("Arret du tour !\n");
        cmd = 'o';
      }
    } while(cmd != 'o');
    free(uniteSelect);
  }
}

Unite **creerSelection(UListe uliste) {
  int n = nombreUnite(uliste);
  int i;
  Unite **tab = calloc(n, sizeof(*tab));
  Unite *unite = uliste.unites;
  for(i = 0; i < n && unite != NULL; ++i) {
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

int enleverSelect(Unite **tab, size_t indice, size_t length) {
  if(indice < length - 1) {
    decaleSelect(tab, indice, length);
  }
  return (length - 1);
}

void decaleSelect(Unite **tab, size_t debut, size_t length) {
  size_t i;
  for(i = debut; i < length - 1; ++i) {
    tab[i] = tab[i + 1];
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

void actionUnite(Unite *unite, Monde *monde) {
  char c[MAXCHAR];
  printf("Que voulez-vous faire ?\n");
  printf("deplacer | attaquer | attendre\n");
  scanf(" %s", c);
  if(strcmp("deplacer", c) == 0) {
    int posX, posY;
    printf("Indiquer positions x,y : ");
    scanf("%d,%d", &posX, &posY);
    deplacerUnite(unite, monde, posX, posY);
  } else if(strcmp("attaquer", c) == 0) {
    int posX, posY;
    printf("Indiquer positions x,y : ");
    scanf("%d,%d", &posX, &posY);
    attaquer(unite, monde, posX, posY);
  }
}

void afficherUnite(Unite unite) {
  printf("\tGenre : %c\n", unite.genre);
  printf("\tPositions x,y : %d,%d\n", unite.posX, unite.posY);
  printf("\tStats : PV %d/%d (%d)\tATT %d (%d)\t PORTEE %d (%d)\n", unite.PV, unite.PVmax + unite.B_PVmax, unite.B_PVmax, unite.att + unite.B_att, unite.B_att, unite.portee + unite.B_portee, unite.B_portee);
}

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

    if(monde->plateau[destY][destX] == NULL && destX <= LONG && destY <= LARG && abs(destX-(unite->posX))<=1 && abs(destY-(unite->posY))<=1 ) /* On verifie que la destination existe et est vide et que c'est un déplacement adjacent */
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
}

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
    printf(" Placez vos guerriers. \n");
    placerUnite(monde,uliste,GUERRIER);
    placerUnite(monde,uliste,GUERRIER);
    placerUnite(monde,uliste,GUERRIER);
    printf(" Placez votre Matriarche. \n");
    placerUnite(monde,uliste,MATRIARCHE);
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
