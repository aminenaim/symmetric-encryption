/**
 * \file dh_crack.c
 * \brief Programme principal, appelant la fonction implementant c1 de crack.
 * \author Amine NAIM
 * \version Finale
 * \date 03 Janvier 2021
 *
 * 
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include "dh_crack_c1.c"

typedef unsigned char byte;

/**
 * @brief Fonction principale permettant d'appeler les fonctions chiffrementXor ; chiffrementCbc ; dechiffrement CBC.
 * 
 *  En parametres les arguments de la ligne de commande et execute en fonction des options saisies les fonctions auxiliaires
 * 
 * @param argc Correspond au nombre d'arguments saisies dans la ligne de commande
 * @param argv Contient les arguments saisis
 */
int main (int argc, char *argv[]) {

    int balise;
    char *source, *methodeCrack;
    int tailleClef;

    FILE *pointeurSource;

    while ((balise = getopt(argc, argv, "i:l:m:k:d:h")) != -1) {
        switch (balise) {
            case 'i':
                source = optarg;
                break;
            case 'm':
                methodeCrack = optarg;
                break;
            case 'k':
                tailleClef = atoi(optarg);
                break;
            case 'd':
                  printf("option non disponible.   (C2 et C3 non choisi par l'étudiant)\n");
            case 'h':
                printf("dh_crack -i infile -m c1 -k length\n"
                       "dh_crack -i infile -m all -k length -d dicofile\n"
                       "\n"
                       "dh_xxx -h : help\n");
                exit(0);        //sortie avec le code 0 car l'option -h annule toutes les autres options
            case '?':
                printf("option inconnue: %c \n", optopt);
                break;
            default:
               break;
        }
    }

   pointeurSource = fopen(source, "r");

   if (pointeurSource == NULL) {
        printf("erreur: Fichier inexistant\n"
               "usage: -i nomfichier.extension\n");
        exit(1);
   }

   if (strcmp(methodeCrack,"c1") == 0 && tailleClef > 0){ // condition Si vérifiant si la methodeCracke saisie est 'xor'

         int **clef = malloc(tailleClef * sizeof *clef);
         int combinaison = 0;

         for (int i = 0; i < tailleClef; i++){
            clef[i] = malloc(10 * sizeof *clef[i]);
         }
         remplirTableau(clef, tailleClef);

         crack_c1(pointeurSource, clef, tailleClef);
         afficherTableau(clef,tailleClef);
         creerCombinaison(clef, 0, tailleClef, combinaison);
         fprintf(stdout,"\n");       // retour chariot esthetique pour afficher l'invite de commande correctement
   
         for(int i = 0; i < tailleClef; i++){
            free(clef[i]);
         }
         free(clef);

    }
   else if (strcmp(methodeCrack,"all") == 0){

        printf("methodeCracke 'all' non disponible. (module C2 et C3 non choisis par l'étudiant)\n");

    }
   else {
       printf("methodeCracke non valide.\n"
              "usage: -m c1 -k longueur-de-la-clef\n"
              "       -m all -k longueur-de-la-clef\n");
    }

   fclose(pointeurSource);
   
   return 0;
}