/**
 * \file dh_crypt.c
 * \brief Programme principal, appelant la fonction implementant la methode 1 (avec xor) et la methode 2 (les fonctions CBC de cryptage et decryptage).
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
#include <getopt.h>     //librairie permettant de traiter les arguments de la ligne de commande

typedef unsigned char byte; 
/**
 * @brief Chiffrer et dechiffrer un message caractere par caractere.
 * Le message en clair sera lu dans un fichier texte, et le message chiffré sera ecrit dans un autre fichier texte.
 * 
 * @param clef  Pour chiffrer le message avec xor (^)
 * @param pointeurSource Pointe vers le fichier ouvert en mode lecture
 * @param pointeurDestination Pointe vers le fichier ouvert/créé en mode ecriture
 */
void chiffrementXor (byte  * clef, FILE * pointeurSource, FILE * pointeurDestination) {

    int indexLecture = 0;
    int msg, sortie;
    int tailleClef = (int) strlen((const char*) clef);

    while ( (msg = fgetc(pointeurSource)) != EOF ){         // boucle s'executant tant que la fonction fgetc pointant vers le fichier source ne renvoit pas EOF (Fin de fichier), EOF correspond à la valeur -1.

        sortie = msg ^ clef[indexLecture%tailleClef];       //  affectation à sortie l'operation binaire xor entre les operandes msg (obtenu via f getc) et un caractere de clef.
        fputc(sortie, pointeurDestination);
        ++indexLecture;
    }
}
/**
 * @brief Fonction permettant de chiffrer avec la methode CBC (chiffrement par blocs). 
 *  Chaque bloc de 16 octets traité donne un bloc chiffré de même taille.
 * 
 * @param vecteurInitialisation Bloc saisi au depart par l'utilisateur pour le chiffrage avec le premier bloc (qui n'a pas de chiffré precedent)
 * @param clef  Pour chiffrer le bloc avec l'operateur xor (^)
 * @param nombreBloc Correspond au nombre de bloc total a traiter avec la methode CBC
 * @param pointeurSource Pointe vers le fichier clair à crypter
 * @param pointeurDestination Pointe vers le fichier destination (ouvert/créé en mode ecriture)
 */
void chiffrementCbc(byte *vecteurInitialisation, byte * clef, int nombreBloc, FILE * pointeurSource, FILE * pointeurDestination) {

    int compteurBlocLu = 0;
    int blocActuel[16], blocCrypte[16];
    byte *precedentBlocCrypte = vecteurInitialisation;        // initialisation au vecteur d'initialisation pour chiffrer le premier bloc

    while (compteurBlocLu < nombreBloc){
        
        for (int i = 0; i < 16; i++){

            if ((blocActuel[i] = fgetc(pointeurSource)) == EOF){    // condition permettant le padding avec des espaces en fin de lecture de fichier
                blocActuel[i] = (int) ' ';    }
        } 

        for (int n = 0; n < 16; n++){

            blocCrypte[n] = (blocActuel[n] ^ precedentBlocCrypte[n]) ^ clef[n % strlen((const char*) clef)];
            fputc(blocCrypte[n],pointeurDestination);
        }

        for (int n = 0; n < 16; n++){
            
            precedentBlocCrypte[n] = (byte) blocCrypte[n];      //  remplacement du precedent bloc crypté par le bloc venant d'etre crypté pour la prochaine itération du While
        }
        ++compteurBlocLu;
    }
}
/**
 * @brief Fonction permettant de dechiffrer avec la methode CBC (dechiffrement par blocs).
 *  Chaque bloc de 16 octets traité donne un bloc dechiffré de même taille.
 *  Il s'agit de renverser le processus de chiffrement en chaine par blocs.
 * 
 * @param vecteurInitialisation Bloc saisi au depart par l'utilisateur pour le chiffrage avec le premier bloc (qui n'a pas de chiffré precedent)
 * @param clef Pour chiffrer le bloc avec l'operateur xor (^)
 * @param nombreBloc Correspond au nombre de bloc total a traiter avec la methode CBC
 * @param pointeurSource Pointe vers le fichier chiffré à lire (ouvert en mode lecture)
 * @param pointeurDestination Pointe vers le fichier destination (ouvert/créé en mode ecriture)
 */
void dechiffrementCbc(byte *vecteurInitialisation, byte * clef, int nombreBloc, FILE * pointeurSource, FILE * pointeurDestination) {

    int compteurBlocLu = 0;
    int blocActuel[16], blocClair[16];
    byte *precedentBloc = vecteurInitialisation;

    while (compteurBlocLu < nombreBloc){ 

        for (int i = 0; i < 16; i++){

            if ((blocActuel[i] = fgetc(pointeurSource)) == EOF){
                blocActuel[i] = (int) ' ';    }
        }

        for (int n = 0; n < 16; n++){

            blocClair[n] = (clef[n % strlen((const char*) clef)] ^ blocActuel[n]) ^ precedentBloc[n];       // Chiffrage du bloc en cours
            fputc(blocClair[n],pointeurDestination);
        }

        for (int n = 0; n < 16; n++){

            precedentBloc[n] = (byte) blocActuel[n];       //  remplacement du precedent bloc clair par le bloc venant d'etre lu (encodé) pour la prochaine itération du While
        }
        ++compteurBlocLu;
    }
}
/**
 * @brief Fonction principale permettant d'appeler les fonctions chiffrementXor ; chiffrementCbc ; dechiffrement CBC
 * 
 *  En parametres les arguments de la ligne de commande et execute en fonction des options saisies les fonctions auxiliaires
 * 
 * @param argc Correspond au nombre d'arguments saisies dans la ligne de commande
 * @param argv Contient les arguments saisis
 */
int main (int argc, char *argv[]) {

    int balise;
    char *source, *destination;
    char *methode;
    byte *vecteurInitialisation;
    byte *clef;
    int nombreBloc;

    FILE *pointeurSource, *pointeurDestination;

    while ((balise = getopt(argc, argv, "i:o:m:k:h")) != -1) {          // le : permet de specifier que l'option qui le precede requiere un argument

        switch (balise) {
            case 'i':
                source = optarg;
                break;
            case 'o':
                destination = optarg;
                break;
            case 'm':
                methode = optarg;

                if (( !strcmp(methode, "cbc-crypt") || !strcmp(methode, "cbc-uncrypt")) && strlen(argv[optind]) == 16) {        // verification que le vecteur d'initialisation est conforme
                    vecteurInitialisation = (byte*) argv[optind];
                    optind++;
                } else {
                    vecteurInitialisation = (byte*) "";
                }
                break;
            case 'k':
                clef = (byte *) optarg;
                break;
            case 'h':
                printf("dh_crypt -i infile -o outfile -k key -m xor\n"
                       "dh_crypt -i infile -o outfile -k key -m cbc-crypt vi\n"
                       "dh_crypt -i infile -o outfile -k key -m cbc-uncrypt vi\n"
                       "\n"
                       "dh_xxx -h : help\n");
                exit(0);        //sortie avec le code 0 car l'option -h annule toutes les autres options
            case '?':
                printf("option inconnu: %c\n", optopt);
                break;
            default:
                break;
        }
    }

    pointeurSource = fopen(source, "r");
    pointeurDestination = fopen(destination, "w");

    if (pointeurSource == NULL || strcmp(destination,"") == 0) {        //  Verification que le fichier source existe et que l'utilisateur a bien saisie l'option -o
        printf("erreur: Fichier inexistant\n"
               "usage: [-i|-o] nomfichier.extension\n");
        exit(1);
    } //if ( strcmp(source,"") && strcmp(destination,"") && strcmp(clef,"")){}
    
    if (!strcmp(methode, "cbc-crypt") || !strcmp(methode, "cbc-uncrypt")){

        fseek(pointeurSource, 0L, SEEK_END);                //  positionne le curseur à la fin du fichier
        int tailleSource = ftell(pointeurSource);           //  recuperation de la position du curseur
        rewind(pointeurSource);                             //  remet le curseur au début du fichier
        nombreBloc = (tailleSource + 16 - 1) / 16;      // calcul du nombre de bloc a traiter
    }
    
    if (strcmp(methode, "xor") == 0 && clef != NULL){

        chiffrementXor(clef, pointeurSource, pointeurDestination);

    }
    else if (strcmp(methode, "cbc-crypt") == 0 && clef != NULL && vecteurInitialisation != NULL){

        chiffrementCbc(vecteurInitialisation, clef, nombreBloc, pointeurSource, pointeurDestination);

    }
    else if (strcmp(methode, "cbc-uncrypt") == 0 && clef != NULL && vecteurInitialisation != NULL){

        dechiffrementCbc(vecteurInitialisation, clef, nombreBloc, pointeurSource, pointeurDestination);

    }

    fclose(pointeurSource);
    fclose(pointeurDestination);
    return 0;
}