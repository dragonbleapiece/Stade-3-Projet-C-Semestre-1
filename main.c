#include "functions.h"
#include "structures.h"

#include <stdio.h>
#include <stdlib.h>


int main(void) {
  Monde monmonde;
  if(!initialiserMonde(&monmonde)) {
    printf("Erreur Mémoire\n");
    return EXIT_FAILURE;
  }

    gererPartie();

  return EXIT_SUCCESS;
}
