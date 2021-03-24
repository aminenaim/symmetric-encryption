#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>

/**
 * @brief Fonction basique initialisant les lignes du tableau de 0 à 9. 
 * 
 * @param array
 * @param nombreLigne 
 */
void remplirTableau(int **tableau, int nombreLigne){
   
   for (int ligne = 0; ligne < nombreLigne; ligne++){

      for (int colonne = 0; colonne < 10; colonne++){

         tableau[ligne][colonne] = colonne;
      }
   }
   tableau[0][0] = -1;
}
/**
 * @brief Fonction basique pour l'impression des résultats
 * 
 * @param tableau tableau à deux dimensions de caracteres traités 
 * @param nombreLigne 
 */
void afficherTableau(int **tableau, int nombreLigne){
   
   for (int ligne = 0; ligne < nombreLigne; ligne++){
      
      fprintf(stdout,"[");
      for (int colonne = 0; colonne < 10; colonne++){

         if (tableau[ligne][colonne] != -1){
            fprintf(stdout,"%d,",tableau[ligne][colonne]);
         }
      }
      fprintf(stdout,"]\n");
   }
   fprintf(stdout,"\n\nEverything is under control my friend.\n\n");
}
/**
 * @brief Fonction créant des combinaisons avec les valeurs possibles pour la clef
 * 
 * @param tableau 
 * @param colonne 
 * @param tailleColonne dimension maximale de la colonne
 * @param nombreEnConstruction 
 */
void creerCombinaison(int **tableau, int colonne, int tailleColonne, int nombreEnConstruction){

    for (int i = 0; i < 10; i++){
        
        if ( tableau[colonne][i] != -1){
           nombreEnConstruction *= 10; 
           nombreEnConstruction += tableau[colonne][i];     // construction de la combinaison
        }
        else{
           continue;
        }

        if (colonne == tailleColonne-1){
           fprintf(stdout,"%d ",nombreEnConstruction);
        }
        else{
            creerCombinaison(tableau, colonne + 1, tailleColonne, nombreEnConstruction);     // recursivite en passant à la colonne suivante
        }
         nombreEnConstruction -= (nombreEnConstruction%10);    
         nombreEnConstruction /= 10;      // permet d'effacer le dernier chiffre pour creer la combinaison suivante
    }
}


/**
 * @brief Fonction d'attaque par emondage des caracteres possible pour la cle
 * 
 * @param pointeurSource Pointe vers le fichier crypté à lire
 * @param tailleClef 
 */
void crack_c1(FILE *pointeurSource, int **tableau, int tailleClef){

   int caractereCrypte, caractereClair, indexLecture = 0;

   while ((caractereCrypte = fgetc(pointeurSource)) != EOF){

      for (int colonne = 0; colonne < 10; colonne ++){

         if (tableau[indexLecture%tailleClef][colonne]  == -1){      // saut de l'itération actuelle dans le cas ou le caractere a deja été testé incorrect
            continue;
            }
         caractereClair = caractereCrypte ^ ( '0' + tableau[indexLecture%tailleClef][colonne]);

         if (!ispunct(caractereClair) && !isalnum(caractereClair) && !isspace(caractereClair) && (caractereClair != '\n')){     // condition eliminant le caractere testé si le caractere obtenu au xor est incorrect

            tableau[indexLecture%tailleClef][colonne] = -1;
         }         
      }  ++indexLecture;
   }
}