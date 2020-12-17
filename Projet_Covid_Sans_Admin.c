/*!
 * \file Projet_Covid_Sans_Admin.c 
 * \author Bapth-R
 * \version 0.57
 * \date 5 décembre 2020
 * 
 * Programme permettant de répertorier toutes les personnes étant allées à differentes fêtes.
 * 
 * 
*/

/**
 * 
 * \mainpage 
 *  \brief     Ce programme permet de pouvoir enregistrer les différentes personnes que nous avons croisé dans les fêtes que nous avons faites. \n
 *             Nous pouvons pour cela enregistrer les différentes personnes que nous avons rencontré et les différents lieux que nous avons visités. \n
 *             Nous pouvons aussi enregistrer les informations rentrées dans des fichiers afin de les sauvegardées même après l’arrêt de notre programme. \n \n
 * 
 * \b Image \b d'accueil
 * \image html Accueil.png width=400px
 *  
 * \b Menu : 
 *  \image html Menu.png width=400px
 *  \author Bapth-R
 *
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h> //Jsp a quoi ca sert :(
#include <windows.h>
#include <dirent.h> //Pour les repertoire
#include <dirent.h> //Pour repertoire aussi



/*!
 * \struct Sdate
 * \brief Permet de caractériser la date 
 * 
 * Sdate permet de pouvoir différencier le jour, le mois et l'année.
 * 
*/
struct Sdate {
    int day;
    int month;
    int year;
};

/*!
 * \struct Citoyen
 * \brief Permet de caractériser un citoyen (Doublement chainée)
 * 
 * Permet de caractériser un citoyen en donnant son nom et son prénom. \n
 * Contient 2 pointeurs : \n
 *  -   \b pnext : Permet d'avancer dans la liste \n
 *  -   \b pprevious : Permet de reculer dans la liste
 * 
*/
typedef struct Citoyen {
	char nom[40];
	char prenom[40];
	struct Citoyen * pnext;
	struct Citoyen * pprevious;
} Tcitoyen;
typedef Tcitoyen * ptCitoyen;

/*!
 * \struct Participant
 * \brief Permet de caractériser un participant (Simplement chainée)
 * 
 * - Permet de caractériser un participant en donnant la date de la fête où il est allé grâce à Sdate. \n
 * - Permet de faire le lien avec grâce au pointeur ppersonne avec la structure Citoyen. \n
 * - Permet d'avancer dans la liste grâce au pointeur \b psuivant. 
 *  
*/
typedef struct Participant {
	struct Sdate date;
	Tcitoyen * ppersonne;
	struct Participant * psuivant;
} Tparticipant;
typedef Tparticipant * ptParticipant;

/*!
 * \struct Lieu
 * \brief Permet de caractériser un lieu (Doublement chainée)
 * 
 * - Permet de caractériser un lieu en donnant le nom du lieu. \n
 * - Permet de faire un lien à la structure Participant grâce au pointeur pliste. \n
 * - Permet d'avancer dans la liste grâce au pointeur \b pavant
 * - Permet de reculer dans la liste grâce au pointeur \b pprecedent
 *  
*/
typedef struct Lieu {
	char nomLieu [40];
	Tparticipant * pliste;
	struct Lieu * pavant;
	struct Lieu * pprecedent;
} Tlieu;
typedef Tlieu * ptLieu;

/**
 * \fn void insertion_Citoyen (Tcitoyen * p)
 * \brief Fonction permettant d'ajouter un citoyen dans la structure Citoyen
 *
 * \param p Permet de donner l'emplacement à créer pour l'ajout du citoyen.
 * 
 */
void insertion_Citoyen (Tcitoyen * p){

    p->pnext->pprevious= (Tcitoyen *)malloc(sizeof(Tcitoyen));
	p->pnext->pprevious->pnext= p->pnext;
	p->pnext->pprevious->pprevious =p;
	p->pnext = p->pnext->pprevious;
}

/**
 * \fn void insertion_Lieu (Tlieu * p)
 * \brief Fonction permettant d'ajouter un lieu dans la structure Lieu
 *
 * \param p Permet de donner l'emplacement à créer pour l'ajout du lieu.
 * 
 */
void insertion_Lieu (Tlieu * p){
    Tlieu * py;

    py = (Tlieu *)malloc(sizeof(Tlieu));
    py -> pavant = p -> pavant;
    py -> pprecedent = p;
    p -> pavant -> pprecedent = py;
    p -> pavant = py;
}

/**
 * \fn void gotoligcol( int lig, int col )
 * \brief Fonction permettant l'écriture dans le terminal à une place précise en fonction des paramètres \a lig et \a col  
 *
 * \param lig \e int  Permet de donner l'emplacement \a x.
 * \param col \e int  Permet de donner l'emplacement \a y.
 * 
 */
void gotoligcol( int lig, int col ){
    COORD mycoord;
    
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
}

/**
 * \fn void Color(int couleur_Du_Texte,int couleur_De_Fond)
 * \brief Fonction permettant de mettre le texte que nous voulons écrire en couleur
 * 
 * \param couleur_Du_Texte \e int    Permet de changer la couleur du texte.
 * \param couleur_De_Fond  \e int     Permet de changer la couleur de fond.
 * 
 */
void Color(int couleur_Du_Texte,int couleur_De_Fond) {

    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,couleur_De_Fond*16+couleur_Du_Texte);
    /*
    0 : Noir
    1 : Bleu foncé
    2 : Vert foncé
    3 : Turquoise
    4 : Rouge foncé
    5 : Violet
    6 : Vert caca d'oie
    7 : Gris clair
    8 : Gris foncé
    9 : Bleu fluo
    10 : Vert fluo
    11 : Turquoise
    12 : Rouge fluo
    13 : Violet 2
    14 : Jaune
    15 : Blanc
    */
}



/**
 * \fn void afficher_nombre_cas()
 * \brief Fonction permettant de faire une requête sur internet pour télécharger le fichier \b fichier_coronavirus.csv qui contient des informations sur le coronavirus
 * \brief Le fichier est mis à jour vers 21h  \n
 *  
 * \warning la ligne \b system("script_corona_racc.lnk"); demande les droits d'administrateur 
 * \warning Le fichier \b script_cornona.bat ne doit pas être lancer ! 
 * 
 */
void afficher_nombre_cas(){

    char lettre[]="f"; char chaine1[50];
    char chaine[100]; char chaine_modif[100]; char chaine_final[100];
    int i = 28; int j = 0;
    
    system("script_corona_racc.lnk"); /*Apelle le raccourcit du script */
    FILE * fichier_corona = fopen("fichier_coronavirus.csv", "r"); /*Ouvre le fichier telecharger */
    FILE * fichier_enregister_nb_cas = fopen("Fichier/Nombre_cas/nombre_cas.txt","w"); /* Ouvre le fichier ou l'on va stocker le nombre de cas de coronavirus */


    fseek(fichier_corona, 0, SEEK_SET); /* Pour etre sur que l'on lit bien le ficher a partir du debut */
    fgets(chaine,100,fichier_corona); /* Saute les 3 1ere lignes qui ne correspondent pas aux autres */
    fgets(chaine,100,fichier_corona);
    fgets(chaine,100,fichier_corona);
    


    while(chaine[19] == lettre[0]){ /* Boucle tant que le fichier donne le cas en France */
        strcpy(chaine_modif , chaine);
        fgets(chaine,100,fichier_corona);
        
    }

    strncpy(chaine1, chaine_modif , 35);  /* Garde que les 35 1er caracteres */
  
    while(i<strlen(chaine1)){ /* Stock caractere par caractere */
        
        chaine_final[j] = chaine1[i];
        i = i + 1; j = j + 1;
    }
    
    fprintf(fichier_enregister_nb_cas, "Nombre de cas en France : %c %c%c%c %c%c%c", chaine_final[0], chaine_final[1], chaine_final[2], chaine_final[3] , chaine_final[4], chaine_final[5], chaine_final[6], chaine_final[7]);

    fclose(fichier_corona); fclose(fichier_enregister_nb_cas);
    
}



/**
 * \fn void afficheMenu(HANDLE hConsole)
 * \brief Fonction permettant d'afficher le menu et le nombre de cas de coronavirus.
 * 
 *  \remark Si le fichier n'est pas trouvé, affiche un message : "Echec du chargement des cas de coronavirus".
 */
void afficheMenu(HANDLE hConsole){

    FILE * fichier_enregister_nb_cas = fopen("Fichier/Nombre_cas/nombre_cas.txt","r"); /* Apelle le fichier d'enregistrement de cas de coronavirus */
    char nb_cas[40];

    system("cls");
    
    Color(12, 0);
    gotoligcol(5,35);printf(" /$$      /$$"                              );
    gotoligcol(6,35);printf("| $$$    /$$$"                              );
    gotoligcol(7,35);printf("| $$$$  /$$$$  /$$$$$$  /$$$$$$$  /$$   /$$");
    gotoligcol(8,35);printf("| $$ $$/$$ $$ /$$__  $$| $$__  $$| $$  | $$");
    gotoligcol(9,35);printf("| $$  $$$| $$| $$$$$$$$| $$  \\ $$| $$  | $$");
    gotoligcol(10,35);printf("| $$\\  $ | $$| $$_____/| $$  | $$| $$  | $$");
    gotoligcol(11,35);printf("| $$ \\/  | $$|  $$$$$$$| $$  | $$|  $$$$$$/");
    gotoligcol(12,35);printf("|__/     |__/ \\_______/|__/  |__/ \\______/ ");
                                          
    Color(15,0);		
    gotoligcol(17,9);printf("1 - Afficher la liste des citoyens");	
    gotoligcol(18,9);printf("2 - Afficher la liste des lieux de fetes");
    gotoligcol(19,9);printf("3 - Ajouter un citoyen");
    gotoligcol(20,9);printf("4 - Ajouter un lieu de fetes");
    gotoligcol(21,9);printf("5 - Remplir les participants a une fete");
    gotoligcol(22,9);printf("6 - Afficher tous ceux qui ont rencontre un citoyen");
    gotoligcol(23,9);printf("7 - Enregistrer un fichier situation");
    gotoligcol(24,9);printf("8 - Ouvrir un ficher situation");
    gotoligcol(25,9);printf("9 - Supprimer tous les evenements anterieur a une date");
    gotoligcol(26,9);printf("0 - Quit");		

    if (fichier_enregister_nb_cas != NULL){
        
        fgets(nb_cas,40,fichier_enregister_nb_cas);
        Color(1,15);
        gotoligcol(29,48); printf("%s",nb_cas);
        Color(15,0);
        fclose(fichier_enregister_nb_cas);
    }
    else{
        Color(12,15);
        gotoligcol(29,48); printf("Echec du chargement des cas de coronavirus");
        Color(15,0);
    }
}


/**
 * \fn void affichage_aucun_lieu()
 * \brief Fonction affichage aucun lieu dans la base.
 */
void affichage_aucun_lieu(){

    gotoligcol(15,23);printf("  ___                           _     _                  _                  ");
    gotoligcol(16,23);printf(" / _ \\                         | |   (_)                | |                 ");
    gotoligcol(17,23);printf("/ /_\\ \\_   _  ___ _   _ _ __   | |    _  ___ _   _    __| | __ _ _ __  ___  ");
    gotoligcol(18,23);printf("|  _  | | | |/ __| | | | '_ \\  | |   | |/ _ \\ | | |  / _` |/ _` | '_ \\/ __| ");
    gotoligcol(19,23);printf("| | | | |_| | (__| |_| | | | | | |___| |  __/ |_| | | (_| | (_| | | | \\__ \\ ");
    gotoligcol(20,23);printf("\\_| |_/\\__,_|\\___|\\__,_|_| |_| \\_____/_|\\___|\\__,_|  \\__,_|\\__,_|_| |_|___/ ");
    gotoligcol(21,23);printf("                                                                            ");
    gotoligcol(22,23);printf("                                                                            ");
    gotoligcol(23,23);printf("                  _         _                                              ");
    gotoligcol(24,23);printf("                 | |       | |                                             ");
    gotoligcol(25,23);printf("                 | | __ _  | |__   __ _ ___  ___                            ");
    gotoligcol(26,23);printf("                 | |/ _` | | '_ \\ / _` / __|/ _ \\                           ");
    gotoligcol(27,23);printf("                 | | (_| | | |_) | (_| \\__ \\  __/                           ");
    gotoligcol(28,23);printf("                 |_|\\__,_| |_.__/ \\__,_|___/\\___|                           ");
}

/**
 * \fn void affichage_smiley_triste()
 * \brief Fonction affichage Smiley
 */
void affichage_smiley_triste(){

    gotoligcol(20,7);printf(".-''''''-.");
    gotoligcol(21,5);printf(".'          '.");
    gotoligcol(22,4);printf("/   O      O   \\");
    gotoligcol(23,3);printf(":           `    :");
    gotoligcol(24,3);printf("|           `    |");
    gotoligcol(25,3);printf(":    .------.    :");
    gotoligcol(26,4);printf("\\  '        '  /");
    gotoligcol(27,5);printf("'.          .'");
    gotoligcol(28,7);printf("'-......-'");    
}

/**
 * \fn void affichage_titre_participant()
 * \brief Fonction affichage Participant
 */
void affichage_titre_participant(){
    
    gotoligcol(1,1);printf("                                   .''.                                                                               ");
    gotoligcol(2,1);printf("       .''.      .        *''*    :_\\/_:     .            ______          _   _      _                   _            ");
    gotoligcol(3,1);printf("      :_\\/_:   _\\(/_  .:.*_\\/_*   : /\\ :  .'.:.'.         | ___ \\        | | (_)    (_)                 | |           ");
    gotoligcol(4,1);printf("  .''.: /\\ :   ./)\\   ':'* /\\ * :  '..'.  -=:o:=-         | |_/ /_ _ _ __| |_ _  ___ _ _ __   __ _ _ __ | |_          ");
    gotoligcol(5,1);printf(" :_\\/_:'.:::.    ' *''*    * '.\'/.' _\\(/_'.':'.'          |  __/ _` | '__| __| |/ __| | '_ \\ / _` | '_ \\| __|         ");
    gotoligcol(6,1);printf(" : /\\ : :::::     *_\\/_*     -= o =-  /)\\    '  *         | | | (_| | |  | |_| | (__| | |_) | (_| | | | | |_          ");
    gotoligcol(7,1);printf("  '..'  ':::'     * /\\ *     .'/.\'.   '                   \\_|  \\__,_|_|   \\__|_|\\___|_| .__/ \\__,_|_| |_|\\__|         ");
    gotoligcol(8,1);printf("      *            *..*         :                                                      | |                            ");
    gotoligcol(9,1);printf("        *                                                                              |_|                            ");  
    gotoligcol(9,1);printf("_______________________________________________________________________________________________________________________");
}

/**
 * \fn void affichage_titre_recherche()
 * \brief Fonction affichage Recherche participant
 */
void affichage_titre_recherche(){

    gotoligcol(1,1);printf(" _____           _                   _                _        _____           _   _      _                   _        ");
    gotoligcol(2,1);printf("|  __ \\         | |                 | |              | |      |  __ \\         | | (_)    (_)                 | |       ");
    gotoligcol(3,1);printf("| |__) |___  ___| |__   ___ _ __ ___| |__   ___    __| | ___  | |__) |_ _ _ __| |_ _  ___ _ _ __   __ _ _ __ | |_ ___  ");
    gotoligcol(4,1);printf("|  _  // _ \\/ __| '_ \\ / _ \\ '__/ __| '_ \\ / _ \\  / _` |/ _ \\ |  ___/ _` | '__| __| |/ __| | '_ \\ / _` | '_ \\| __/ __| ");
    gotoligcol(5,1);printf("| | \\ \\  __/ (__| | | |  __/ | | (__| | | |  __/ | (_| |  __/ | |  | (_| | |  | |_| | (__| | |_) | (_| | | | | |_\\__ \\ ");
    gotoligcol(6,1);printf("|_|  \\_\\___|\\___|_| |_|\\___|_|  \\___|_| |_|\\___|  \\__,_|\\___| |_|   \\__,_|_|   \\__|_|\\___|_| .__/ \\__,_|_| |_|\\__|___/ ");
    gotoligcol(7,1);printf("                                                                                           | |                         ");
    gotoligcol(8,1);printf("                                                                                           |_|                         ");
    gotoligcol(9,1);printf("_______________________________________________________________________________________________________________________"); 
}

/**
 * \fn void afficher_titre_fichier_situation_enregistrer()
 * \brief Fonction affichage Enregistrement de fichier situation
 */
void afficher_titre_fichier_situation_enregistrer(){
    
    gotoligcol(1,1);printf("___________                           .__          __                           _____.__       .__    .__              ");
    gotoligcol(2,1);printf("\\_   _____/ ___________   ____   ____ |__| _______/  |________   ___________  _/ ____\\__| ____ |  |__ |__| _________   ");
    gotoligcol(3,1);printf(" |    __)_ /    \\_  __ \\_/ __ \\ / ___\\|  |/  ___/\\   __\\_  __ \\_/ __ \\_  __ \\ \\   __\\|  |/ ___\\|  |  \\|  |/ __ \\_  __ \\");
    gotoligcol(4,1);printf(" |        \\   |  \\  | \\/\\  ___// /_/  >  |\\___ \\  |  |  |  | \\/\\  ___/|  | \\/  |  |  |  \\  \\___|   Y  \\  \\  ___/|  | \\/");
    gotoligcol(5,1);printf("/_______  /___|  /__|    \\___  >___  /|__/____  > |__|  |__|    \\___  >__|     |__|  |__|\\___  >___|  /__|\\___  >__|   ");
    gotoligcol(6,1);printf("        \\/     \\/            \\/_____/         \\/                    \\/                       \\/     \\/        \\/       ");
    gotoligcol(7,1);printf("                           _________.__  __                __  .__                                                     ");      
    gotoligcol(8,1);printf("                          /   _____/|__|/  |_ __ _______ _/  |_|__| ____   ____                                        ");    
    gotoligcol(9,1);printf("                          \\_____  \\ |  \\   __\\  |  \\__  \\   __\\|  |/  _ \\ /    \\                                       ");    
    gotoligcol(10,1);printf("                          /        \\|  ||  | |  |  // __ \\|  | |  (  <_> )   |  \\                                      ");    
    gotoligcol(11,1);printf("                         /_______  /|__||__| |____/(____  /__| |__|\\____/|___|  /                                      ");   
    gotoligcol(12,1);printf("                                \\/                     \\/                    \\/                                        ");

}

/**
 * \fn void afficher_titre_supprimer_participants()
 * \brief Fonction affichage Supprimer
 */
void afficher_titre_supprimer_participants(){

gotoligcol(1,1);printf("                                _________                          __                                                 ");
gotoligcol(2,1);printf("                               /   _____/__ ________ _____________|__| _____   ___________                            ");
gotoligcol(3,1);printf("                               \\____   \\ |  |  \\____ \\____ \\_  __ \\  |/     \\_/ __ \\_  __ \\                           ");
gotoligcol(4,1);printf("                                /       \\  |  /  |_> >  |_> >  | \\/  |  Y Y  \\  ___/|  | \\/                           ");   
gotoligcol(5,1);printf("                               /_______  /____/|   __/|   __/|__|  |__|__|_|  /\\___  >__|                             ");
gotoligcol(6,1);printf("                                       \\/      |__|   |__|                  \\/     \\/                                 \n");
}

/**
 * \fn void afficher_titre_fichier_situation_ouvrir()
 * \brief Fonction affichage Ouvrir un fichier Situation
 */
void afficher_titre_fichier_situation_ouvrir(){
    gotoligcol(1,1);printf("                      ____                   _                       __ _      _     _                                ");
    gotoligcol(2,1);printf("                     / __ \\                 (_)                     / _(_)    | |   (_)                               ");
    gotoligcol(3,1);printf("                    | |  | |_   ___   ___ __ _ _ __   _   _ _ __   | |_ _  ___| |__  _  ___ _ __                      ");
    gotoligcol(4,1);printf("                    | |  | | | | \\ \\ / / '__| | '__| | | | | '_ \\  |  _| |/ __| '_ \\| |/ _ \\ '__|                     ");
    gotoligcol(5,1);printf("                    | |__| | |_| |\\ V /| |  | | |    | |_| | | | | | | | | (__| | | | |  __/ |                        ");
    gotoligcol(6,1);printf("                     \\____/ \\__,_| \\_/ |_|__|_|_|     \\__,_|_| |_| |_| |_|\\___|_| |_|_|\\___|_|                        ");
    gotoligcol(7,1);printf("                                         _____ _ _               _   _                                                ");
    gotoligcol(8,1);printf("                                        / ____(_) |             | | (_)                                               ");
    gotoligcol(9,1);printf("                                       | (___  _| |_ _   _  __ _| |_ _  ___  _ __                                     ");
    gotoligcol(10,1);printf("                                        \\___ \\| | __| | | |/ _` | __| |/ _ \\| '_ \\                                    ");
    gotoligcol(11,1);printf("                                        ____) | | |_| |_| | (_| | |_| | (_) | | | |                                   ");
    gotoligcol(12,1);printf("                                       |_____/|_|\\__|\\__,_|\\__,_|\\__|_|\\___/|_| |_|                                   ");
}

/**
 * \fn void tete_affiche_1(int x,int y)
 * \brief Fonction affichage Tete1
 * 
 * \param x \e int Permet de positionner la tête en fonction de \a x
 * \param y \e int Permet de positionner la tête en fonction de \a y 
 * 
 */
void tete_affiche_1(int x,int y){

    gotoligcol(x ,y+3);printf(".------\\ /------.");
    gotoligcol(x+1,y+3);printf("|       -       |");
    gotoligcol(x+2,y+3);printf("|               |");
    gotoligcol(x+3,y+3);printf("|               |");
    gotoligcol(x+4,y+3);printf("|               |");
    gotoligcol(x+5,y);printf("_______________________");
    gotoligcol(x+6,y);printf("===========.===========");
    gotoligcol(x+7,y+2);printf("/ ~~~~~     ~~~~~ \\");
    gotoligcol(x+8,y+1);printf("/|     |     |\\   | \\");
    gotoligcol(x+9,y+1);printf("W   ---  / \\  ---   W");
    gotoligcol(x+10,y+1);printf("\\.      |o o|      ./");
    gotoligcol(x+11,y+2);printf("|                 |");
    gotoligcol(x+12,y+2);printf("\\    #########    /");
    gotoligcol(x+13,y+3);printf("\\  ## ----- ##  /");
    gotoligcol(x+14,y+4);printf("\\##         ##/");
    gotoligcol(x+15,y+5);printf("\\_____v_____/");
}

/**
 * \fn void tete_affiche_2(int x,int y)
 * \brief Fonction affichage Tete2
 * 
 * \param x \e int Permet de positionner la tête en fonction de \a x
 * \param y \e int Permet de positionner la tête en fonction de \a y 
 * 
 */
void tete_affiche_2(int x,int y){

    gotoligcol(x+2,y+9);printf("#############");
    gotoligcol(x+3,y+9);printf("##         ##");
    gotoligcol(x+4,y+9);printf("#  ~~   ~~  #");
    gotoligcol(x+5,y+9);printf("#  ()   ()  #");
    gotoligcol(x+6,y+9);printf("(     ^     )");
    gotoligcol(x+7,y+9);printf("|           |");
    gotoligcol(x+8,y+9);printf("|   {===}   |");
    gotoligcol(x+9,y+10);printf("\\         /");
    gotoligcol(x+10,y+10);printf("/  -----  \\");
    gotoligcol(x+11,y+6);printf("---   |%%\\  /%%|  ---");
    gotoligcol(x+12,y+4);printf("/       |%%%%%%|         \\");
    gotoligcol(x+13,y+12);printf("|%%/  \\%%|");
}

/**
 * \fn void lieu_header()
 * \brief Fonction affichage Lieu de fête
 * 
 */
void lieu_header(){
            Color(1,15);
            gotoligcol(2,1);printf("                                                                                             .   *   ..  . *  *       ");
            gotoligcol(3,1);printf("                                                                                               *  * @()Ooc()*   o  .  ");
            gotoligcol(4,1);printf("      `'::.                                                                                         (Q@*0CG*O()  ___  ");
            gotoligcol(5,1);printf("    _________H ,%%&%%,               _     _                  _       ______   _                     |\\_________/|/ _ \\ ");
            gotoligcol(6,1);printf("   /\\     _   \\%%&&%%&%%             | |   (_)                | |      |  ___| | |                    |  |  |  |  | / | |");
            gotoligcol(7,1);printf("  /  \\___/^\\___\\%%&%%&&             | |    _  ___ _   _    __| | ___  | |_ ___| |_ ___               |  |  |  |  | | | |");
            gotoligcol(8,1);printf("  |  | []   [] |%%\\Y&%%'            | |   | |/ _ \\ | | |  / _` |/ _ \\ |  _/ _ \\ __/ _ \\              |  |  |  |  | | | |");
            gotoligcol(9,1);printf("  |  |   .-.   | ||               | |___| |  __/ |_| | | (_| |  __/ | ||  __/ ||  __/              |  |  |  |  | | | |");
            gotoligcol(10,1);printf("~~@._|@@_|||_@@|~||~~~~~          \\_____/_|\\___|\\__,_|  \\__,_|\\___| \\_| \\___|\\__\\___|              |  |  |  |  | | | |");
            gotoligcol(11,1);printf("     `'''') )''''`                                                                                 |  |  |  |  | \\_| |");
            gotoligcol(12,1);printf("                                                                                                   |  |  |  |  |\\___/ ");
            gotoligcol(13,1);printf("                                                                                                   |\\_|__|__|_/|      ");
            gotoligcol(14,1);printf("                                                                                                    \\_________/       ");
            Color(15,0);
}


/**
 * \fn void affichage_telechargement(int x)
 * \brief Fonction permettant d'afficher la barre de télechargement si fichier existe 
 * 
 * \param x \e int permet de positionner la barre de télechargement en fonction de \a x
 * 
 */
void affichage_telechargement(int x){

    int test1 = 0;
    int nb_alea=0;

    while(test1 < 50){

        nb_alea = rand()%40+1;
        gotoligcol(x,20+test1); printf("|");
        Color(12,0);
        gotoligcol(x,80); printf(" %d %%", test1*2+2);
        Color(15,0);
        Sleep(nb_alea);
        test1 = test1 + 1;
    }

    Color(10,0);
    gotoligcol(x,80); printf(" 100 %%");
    Color(15,0);
}

/**
 * \fn void affichage_telechargement_fail(int x)
 * \brief Fonction permettant d'afficher la barre de télechargement si fichier n'existe pas
 * 
 * \param x \e int permet de positionner la barre de télechargement en fonction de \a x
 * 
 */
void affichage_telechargement_fail(int x){

    int test1 = 0;
    int nb_alea=0;

    while(test1 < 10){

        nb_alea = rand()%40+1;
        gotoligcol(x,20+test1); printf("|");
        Color(12,0);
        gotoligcol(x,80); printf(" %d %%", test1*2+2);
        Color(15,0);
        Sleep(nb_alea);
        test1 = test1 + 1;
    }
    Color(12,0);
    gotoligcol(x,80); printf(" 20 %%");
    Color(15,0);
}

/**
 * \fn int int_modif(char phrase[])
 * \brief Fonction qui permet de forcer la saisie d'un chiffre 
 * 
 * \param phrase \e char Permet de stocker une phrase a réafficher si la saisie n'est pas un chiffre
 * 
 * \return Retrun le nombre voulu
 * 
 */
int int_modif(char phrase[]){

    int valeur = 0;  int n; 

    while (scanf("%d",&valeur )!= 1){ /* Si le "mot" rentré est un nombre ca return 1 sinon ca return 0 */ 
        
        Color(12,0);
        printf("erreur\n ");
        Color(15,0);
        printf("Recommencer ...\n ");
        printf("%s",phrase);
        fflush(stdin); /* Clear la valeur entrante du clavier */
    }
    return valeur;
} 

/**
 * \fn void fonction_date(int* jour_modif, int* mois_modif, int* annee_modif)
 * \brief Fonction permettant de rentrer la date et verifie la veracite de celle-ci 
 * 
 * \param jour_modif  \e  int  Permet de créer un pointeur jour pour pouvoir "retrun" un pointeur 
 * \param mois_modif  \e  int  Permet de créer un pointeur mois pour pouvoir "retrun" un pointeur 
 * \param annee_modif \e  int  Permet de créer un pointeur année pour pouvoir "retrun" un pointeur 
 * 
 * 
 */
void fonction_date(int* jour_modif, int* mois_modif, int* annee_modif){
        
    int jour = 1, mois = 1, annee = 0;
    
    /* Permet d eviter de mettre une date fausse. */

    printf("Quel date : \n"); 
    printf("Jour : "); jour = int_modif("Jour :");
    
    while (jour > 31){
        
        Color(12,0);
        printf("Il y a 28 ou 29 ou 30 ou 31 jours dans 1 mois \n"); printf("Recommence \n"); Color(15,0);
        printf("Jour : "); jour = int_modif("Jour : ");
    }
    
    printf("Mois : "); mois = int_modif("Mois : ");

    while (mois > 12){

        Color(12,0);
        printf("Il y a 12 mois dans une annee \n Recommence ...\n"); Color(15,0);
        printf("Mois : "); mois = int_modif("Mois : ");
    }
    
    if (jour  == 30 || jour == 31){

        while ( mois == 02 ) { 
        Color(12,0);
        printf("Il y a maximum 29 jours dans le mois de fevrier \n Recommencer ...\n"); Color(15,0);
        printf("Mois : "); mois = int_modif("Mois : ");
        }
    }


    if (jour == 31){      
    
        while(mois == 04 || mois == 06 || mois == 9 || mois == 11){

        Color(12,0);
        printf("Impossible il ne peut pas y avoir 31 jours dans ce mois \n Recommence .... \n"); Color(15,0);
        printf("Mois : "); mois = int_modif("Mois : ");
        }
    }

    printf("Annee : "); annee = int_modif("Annee : ");
    
    while (annee > 2021){
    
        Color(12,0);
        printf("Euh pas trop vite... Recommence ... \n"); Color(15,0);
        printf("Annee : "); annee = int_modif("Annee : ");
    }

    while (annee < 2019){
    
        Color(12,0);
        printf("Le coronavirus n'existait pas ... Recommence ... \n"); Color(15,0);
        printf("Annee : "); annee = int_modif("Annee : ");
    }

	printf("Date = %d/%d/%d\n", jour , mois , annee);
    *jour_modif = jour; 
    *mois_modif = mois; 
    *annee_modif = annee;
}

/**
 * \fn int Compte_ligne(FILE *fichier)
 * \brief Fonction permettant de nous renvoyer le nombre de ligne d'un fichier
 * 
 * \param fichier  \e  FILE  Permet de selectionner le fichier en question pour compter son nombre de ligne 
 * 
 * \return nLignes \e int  Retourne le nombre de ligne
 * 
 */
int Compte_ligne(FILE *fichier){

	int compte_ligne = 0;
	int nLignes = 0;
 
	while((compte_ligne=fgetc(fichier)) != EOF){ 
	
    	if(compte_ligne=='\n'){
		
        	nLignes = nLignes + 1;
        }    
	}
    
    compte_ligne = 1;
    /*On se remet au début du fichier */
    fseek(fichier, 0, SEEK_SET);
 
	return nLignes;
}

/**
 * \fn void affichier_dossier(char dossier[])
 * \brief Fonction permettant d'afficher le contenu d'un dossier
 * 
 * \param dossier[]  \e  char  Permet séléctionner le dossier 
 * 
 */
void affichier_dossier(char dossier[]){

    int nombre = 1;
    DIR * repertoire_situation = opendir(dossier); /* Selection du repertoire Fichier */

    if (repertoire_situation != NULL){  /* Si le répertoire existe on rentre dans la boucle */

        struct dirent * afficher; /* Permet de creer une structure pour l'affichage des fichiers */
        readdir(repertoire_situation); readdir(repertoire_situation);
        while ((afficher = readdir(repertoire_situation)) != NULL){  /* Tant que il y a un fichier alors rentre dans la boucle pour l'afficher */

            printf(  "Fichier n%d : %s\n", nombre , afficher->d_name);  /* Affiche le nom du fichier */
            nombre = nombre + 1;
        } 

        closedir(repertoire_situation); /* Ferme le dossier */ 
    } /* Fin de l'utilisation du dossier */
}

//1// : Pour la partie 1 :

/**
 * \details Partie 1
 * \fn void pointeur_Citoyen_ajout(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen)
 * \brief Fonction permettant l'ajout de citoyen
 * 
 * \param pdebut_citoyen  \e  ptCitoyen Pointeur pointant le 1er élément de la liste de Citoyen
 * \param pfin_citoyen  \e  ptCitoyen Pointeur pointant le dernier élément de la liste de Citoyen
 * 
 */
void pointeur_Citoyen_ajout(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen){  

    char Citoyen_nom_entree[40], Citoyen_prenom_entree[40];

    /* Info du citoyen a ajouter */
    gotoligcol(20,2);printf("Nom du citoyen a Ajouter : "); scanf("%s",&Citoyen_nom_entree);
    gotoligcol(22,2);printf("Prenom du participant a Ajouter : "); scanf("%s",&Citoyen_prenom_entree);


    ptCitoyen px_citoyen;
    px_citoyen = pdebut_citoyen ;
    int existe = 0;

    /* On se positionne a la fin de Ptcitoyen en verifiant si le citoyen existe dans la base. S'il existe existe == 1 sinon existe == 0 */
    while(px_citoyen -> pnext != pfin_citoyen){
        
        /* Si px_citoyen = px_citoyen -> pnext est a la fin de la boucle prend pas la deniere valeur */
        px_citoyen = px_citoyen -> pnext;
        if (strcmp(px_citoyen -> nom , Citoyen_nom_entree) == 0 && strcmp(px_citoyen -> prenom , Citoyen_prenom_entree) == 0){
            existe = 1;
        }
    }

    /* Si citoyen existe pas : */
    if (existe == 0){

        insertion_Citoyen(px_citoyen);  
        px_citoyen = px_citoyen -> pnext;
        strcpy(px_citoyen -> nom , Citoyen_nom_entree);
        strcpy(px_citoyen -> prenom , Citoyen_prenom_entree);
        Color(10,0); gotoligcol(17,49);printf("Nom du citoyen ajouter\n"); Color(15,0);
    }

    else {

        Color(12,0); gotoligcol(17,30);printf(" / ! \\  Erreur : Le citoyen existe deja dans la base / ! \\  "); Color(15,0); 
    }

}

//2// : Pour la partie 2 :

/**
 * \details Partie 2
 * \fn pointeur_Citoyen_affiche(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen)
 * \brief Fonction permettant d'afficher les citoyens
 * 
 * \param pdebut_citoyen  \e  ptCitoyen Pointeur pointant le 1er élément de la liste de Citoyen
 * \param pfin_citoyen  \e  ptCitoyen Pointeur pointant le dernier élément de la liste de Citoyen
 * 
 */
void pointeur_Citoyen_affiche(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen){ 

    ptCitoyen px_citoyen;

    int compteur = 1; 
    int i = 13; 
    int afficher = 15;

    /* On se place sur la 2eme case memoire car la 1ere est vide */
    px_citoyen = pdebut_citoyen -> pnext;

    /* Pour l'affichage */
    gotoligcol(13,1);printf("|"); gotoligcol(13,14);printf("Nom"); gotoligcol(13,42); printf("Prenom\n"); gotoligcol(13,60);printf("|");
    gotoligcol(14,1);printf("|__________________________________________________________|\n");

    while (px_citoyen != pfin_citoyen){

        printf(" %d.\t%s",compteur , px_citoyen ->nom); gotoligcol(afficher,30);printf("\t %s\n", px_citoyen -> prenom);
        px_citoyen = px_citoyen -> pnext;
        compteur = compteur + 1; afficher = afficher + 1;
    }

    compteur = compteur + 14; /* Permet de se mettre au "meme" niveux que le i */

    /* Pour l'affichage */
    while (i < compteur){

        gotoligcol(i,30);printf("|");
        i = i + 1;
    }
}

//3// : Pour la partie 3 :

/**
 * \details Partie 3
 * \fn void pointeur_Lieu_ajout(ptLieu pdebut_lieu, ptLieu pfin_lieu)
 * \brief Fonction permettant l'ajout d'un lieu
 * 
 * \param pdebut_lieu  \e  ptLieu  Pointeur pointant le 1er élément de la liste de Lieu
 * \param pfin_lieu  \e  ptLieu    Pointeur pointant le dernier élément de la liste de Lieu
 * 
 */
void pointeur_Lieu_ajout(ptLieu pdebut_lieu, ptLieu pfin_lieu){

    ptLieu px_lieu;

    int existe = 0; /* Permet de savoir si le lieu eixte deja. S'il existe deja existe = 1 */
    char Lieu_entree[40]; /* Permet de stcker le lieu entré */

    printf("Nom du Lieu : "); fflush(stdin); gets(Lieu_entree); /*Permet de pouvoir saisir des espaces */

    px_lieu = pdebut_lieu;

    /* On se positionne a la fin de PtLieu en verifiant si le lieu existe dans la base. S'il existe existe == 1 sinon existe == 0 */
    while(px_lieu -> pavant != pfin_lieu){

        /* Si px_lieu = px_lieu -> pavant est a la fin de la boucle prend pas la deniere valeur */
        px_lieu = px_lieu -> pavant;
        if(strcmp(px_lieu-> nomLieu , Lieu_entree) == 0){

            existe = 1;
        }
    }

    /* Si le lieu n'existe pas */
    if (existe == 0){

        insertion_Lieu(px_lieu);  
        px_lieu = px_lieu -> pavant;
        strcpy(px_lieu -> nomLieu , Lieu_entree);
        Color(10,0); gotoligcol(8,49);printf("Ajout du lieu dans la base"); Color(15,0);

        /*Creation d'un case memoire (ca en crera une a chaque lieu creer). Puis mise de la suivante a NULL */
        px_lieu -> pliste = (Tparticipant *)malloc(sizeof(Tparticipant));
        px_lieu -> pliste -> psuivant = NULL;
    }

    if(existe == 1){

        Color(12,0); gotoligcol(8,33);printf(" / ! \\  Erreur : Le lieu existe deja dans la base / ! \\  "); Color(15,0);
    }
}

//4// : Pour la partie 4 :

/**
 * \details Partie 4.1
 * \fn void pointeur_Lieu_participant_affiche(ptLieu pdebut_lieu, ptLieu pfin_lieu)
 * \brief Fonction permettant l'affichage des lieux avec les participants associés
 * 
 * \param pdebut_lieu  \e  ptLieu  Pointeur pointant le 1er élément de la liste des Lieu
 * \param pfin_lieu  \e  ptLieu    Pointeur pointant le dernier élément de la liste des Lieu
 * 
 */
void pointeur_Lieu_participant_affiche(ptLieu pdebut_lieu, ptLieu pfin_lieu){  

    ptLieu px_lieu;
    ptParticipant px_participant;

    int place = 0;          /* Pour permettre d'afficher les nom de lieu au milieu de la page */
    int affichage_y = 15;   /* affichage en x */
    int i = 0;              /* Pour l'incrémentation */
    int aucun = 1;          /* S'il n'y a pas de lieu aucun passe à 0 */
    char caractere = 32;    /* Permet de changer de page (comme avec le menu )*/

    /* On se place sur la 2eme case de memoire car la 1ere est vide */
    px_lieu = pdebut_lieu -> pavant ; 

    /* Si aucun lieu sont enregistrer alors rentre dans la boucle (que le l'affichage) */
    if (px_lieu == pfin_lieu){
        
        Color(12,0); affichage_aucun_lieu(); Color(14,0);
        Color(0,14); affichage_smiley_triste(); Color(14,0);
        aucun = 0;
    }

    if (aucun == 1){

        /* Permet d'afficher les lieux avec les participants */
        while (px_lieu != pfin_lieu){

            affichage_y = 15; i = 0; /* Réinitialise les valeurs */

            place = 60 - (strlen(px_lieu -> nomLieu)/2); /* Permet de mettre le nom du lieu au milieu de la page*/
            gotoligcol(affichage_y, place);printf("~~~ %s ~~~ \n",px_lieu -> nomLieu); 

            px_participant = px_lieu -> pliste;

            /* Permet d'afficher les participants */
            while(px_participant -> psuivant != NULL){ 

                affichage_y = affichage_y + 1;
                px_participant = px_participant -> psuivant;

                gotoligcol(affichage_y, 2); printf("Participant : %d/%d/%d \n",px_participant -> date.day ,px_participant -> date.month, px_participant -> date.year );
                affichage_y = affichage_y + 1;
                gotoligcol(affichage_y, 2); printf("Participant :  ### NOM  :  %s \t\t  ### PRENOM  : %s  \n",px_participant -> ppersonne -> nom , px_participant -> ppersonne -> prenom);  
                affichage_y = affichage_y + 1; 

                i = i + 1;
                /*S'il y a plus de 4 participants alors ln change de page pour affichier les suivants */
                if(i > 4){

                    Color(12,0);gotoligcol(20,116); printf("===>"); Color(15,0);
                    caractere= getch();
                    system("cls");
                    lieu_header();
                    affichage_y = 15;
                    gotoligcol(affichage_y, place);printf("~~~ %s ~~~ \n",px_lieu -> nomLieu); 
                    i = 0;
                }
            }

            Color(12,7);gotoligcol(30,79); printf("Appuyer sur une touche pour continuer ..."); Color(15,0);
            caractere= getch();
            system("cls");
            lieu_header();
            px_lieu = px_lieu -> pavant; 
        }

        gotoligcol(21,50);printf("Retour au MENU");
    }
}

/**
 * \details Partie 4.2
 * \fn void pointeur_Lieu_affiche(ptLieu pdebut_lieu, ptLieu pfin_lieu)
 * \brief Fonction affichant seulement les lieux 
 * 
 * \param pdebut_lieu  \e  ptLieu  Pointeur pointant le 1er élément de la liste des Lieu
 * \param pfin_lieu  \e  ptLieu    Pointeur pointant le dernier élément de la liste des Lieu
 * 
 */
void pointeur_Lieu_affiche(ptLieu pdebut_lieu, ptLieu pfin_lieu){  

    ptLieu px_lieu;

    int place = 0;          /* Pour permettre d'afficher les nom de lieu au milieu de la page */
    int affichage_y = 15;   /* affichage en x */
    int i = 0;              /* Pour l'affichage */
    int nombre = 1;         /* Permet d'afficher le nombre de lieu */

    /* On se place sur la 2eme case de memoire car la 1ere est vide */
    px_lieu = pdebut_lieu -> pavant ; 

    /* Si aucun lieu sont enregistrer alors rentre dans la boucle (que le l'affichage) */
    if (px_lieu == pfin_lieu){
        
        Color(12,0); affichage_aucun_lieu(); Color(14,0);
        Color(0,6); affichage_smiley_triste(); Color(14,0);
    }

    while (px_lieu != pfin_lieu){

        /* Affichage des lieux */
        place = 60 - (strlen(px_lieu -> nomLieu)/2);
        gotoligcol(affichage_y, 45);printf("%d .",nombre);
        gotoligcol(affichage_y, place);printf("%s\n",px_lieu -> nomLieu); 
        
        /* Pour afficher des flèches (affichage) */
        i = 50;
        while(i < (place - 4)){

            Color(8,0); gotoligcol(affichage_y, i); printf("->");
            i = i + 2;
        }

        Color(15,0);
        px_lieu = px_lieu -> pavant; 
        affichage_y = affichage_y + 1;
        nombre = nombre + 1;
    }
}

//5// : Pour la partie 5 :

/**
 * \details Partie 5
 * \fn ptLieu affichage_lieu_pour_participant(ptLieu pdebut_lieu, ptLieu pfin_lieu)
 * \brief Fonction permettant d'afficher les lieux pour en sélectionner 1 avec une fleche puis return le px_lieu
 * 
 * \param pdebut_lieu  \e  ptLieu  Pointeur pointant le 1er élément de la liste des Lieu
 * \param pfin_lieu  \e  ptLieu    Pointeur pointant le dernier élément de la liste des Lieu
 * 
 * \return Return \b px_lieu qui permet d'indiquer la positon du lieu dans le pointeur Lieu
 * 
 */
ptLieu affichage_lieu_pour_participant(ptLieu pdebut_lieu, ptLieu pfin_lieu){

    ptLieu px_lieu;

    int i = 10, touche = 0, move = 11;

    px_lieu = pdebut_lieu -> pavant;

    /* Affichage des lieux */
    while (px_lieu != pfin_lieu){

        gotoligcol(i+1,50);printf("%s\n",px_lieu -> nomLieu);
        px_lieu = px_lieu -> pavant; i = i + 1;
    }

    gotoligcol(move,42); printf("->");
    px_lieu = pdebut_lieu -> pavant;

    system("color 0f" ); /* Permet de passer toute la fenetre en une couleur précise (ici couleur de base) */
    Color(6,0);
    gotoligcol(move,50); printf("%s", px_lieu ->nomLieu);
    gotoligcol(move,42); printf("->");
    Color(15,0);  

    while (touche != 13){
            
        touche = getch();
        if(touche==72){ //haut

            if(px_lieu != pdebut_lieu -> pavant){
                px_lieu = px_lieu -> pprecedent;
                move = move - 1;
            }
        }
        
        if (touche==80) {    //Bas 
        
            if (px_lieu -> pavant != pfin_lieu){
                
                px_lieu = px_lieu -> pavant;
                move = move + 1;
            } // fleche bas
        }

        if (touche==13) {    //entree 

            printf("\n\n %s \n\n", px_lieu -> nomLieu);
            return (px_lieu);
        }

        system("color 0f" ); 
        gotoligcol(move,50); printf("%s", px_lieu -> nomLieu);
        gotoligcol(move - 1,42); printf("    ");
        gotoligcol(move + 1,42); printf("    ");
        gotoligcol(move,42); printf("->");
        Color(6,0);
        gotoligcol(move,50); printf("%s", px_lieu -> nomLieu);
        gotoligcol(move,42); printf("->");
    }
}

/**
 * \details Partie 5
 * \fn void affichage_citoyen_pour_participant(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, ptLieu px_lieu, int jour , int mois , int annee, int fin)
 * \brief Fonction permettant d'afficher les citoyen pour en séléctionner 1 ou plusieurs pour les ajouter dans la liste de Participant
 * \brief Cette fonction crée donc 1 ou plusieurs participants
 * 
 * \param pdebut_citoyen  \e  ptCitoyen Pointeur pointant le 1er élément de la liste de Citoyen
 * \param pfin_citoyen  \e  ptCitoyen Pointeur pointant le dernier élément de la liste de Citoyen
 * \param pdebut_lieu  \e  ptLieu  Pointeur pointant le 1er élément de la liste des Lieu
 * \param pfin_lieu  \e  ptLieu    Pointeur pointant le dernier élément de la liste des Lieu
 * \param jour \e int Correspond au jour de la fête
 * \param mois \e int Correspond au mois de la fête
 * \param annee \e int Correspond à l'année de la fête
 * 
 * 
 */
void affichage_citoyen_pour_participant(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, ptLieu px_lieu, int jour , int mois , int annee, int fin){
    	
    ptCitoyen px_citoyen;
    ptCitoyen py_citoyen;
    ptParticipant px_participant;
    ptParticipant py_participant;
   
    int i = 10; int touche = 0; int test = 0;
    int move = 11;
    int ajout = 0;

    px_citoyen = pdebut_citoyen -> pnext;  
    py_citoyen = pdebut_citoyen -> pnext;
    
    px_participant = px_lieu -> pliste;
    py_participant = px_lieu -> pliste;
    
    while(px_participant -> psuivant != NULL && px_participant != NULL){
        
        px_participant = px_participant -> psuivant;
    }
    
    while (px_citoyen != pfin_citoyen){

        gotoligcol(i+1,40); printf(" %s \t %s ", px_citoyen ->nom , px_citoyen -> prenom);
        px_citoyen = px_citoyen -> pnext;
        i = i + 1;
    }
    
    
    
    gotoligcol(move,32); printf("->");
    px_citoyen = pdebut_citoyen -> pnext;
    system("color 0f" );  
    Color(6,0);
    gotoligcol(move,40); printf(" %s \t %s ", px_citoyen ->nom , px_citoyen -> prenom);
    gotoligcol(move,32); printf("->");
    Color(15,0);  

    while (fin != 0){
        
        touche = getch();
        if(touche==72){ //haut
        
            if(px_citoyen != pdebut_citoyen -> pnext){
                px_citoyen = px_citoyen -> pprevious;
                move = move - 1;
            }
        }
        
        if (touche==80) {    //Bas 
    
            if (px_citoyen -> pnext != pfin_citoyen){
            
                px_citoyen = px_citoyen -> pnext;
                move = move + 1;
            } // fleche bas
        }
        
        if (touche==13) {    //entree 
      
            px_participant -> psuivant = (Tparticipant *)malloc(sizeof(Tparticipant)); /* Cree une case memoire */
            px_participant = px_participant -> psuivant; /* On se positionne sur la case que nous venons de creer */

            /* Ajout de la date */
            px_participant -> date.day = jour; px_participant -> date.month = mois; px_participant -> date.year = annee;
        
            /*Ajout de nom-participant et prenom_participant dans le pointeur */
            px_participant -> ppersonne = px_citoyen;
            ajout = 1; fin = fin - 1;
        }

        /*
        if (touche==77) {}     //droite
        if (touche==75) {}     // Gauche
        if(touche == 113 ){} //q
        if(touche == 27 ){} //echap  
        */
        system("color 0f" ); 
        gotoligcol(move,40); printf(" %s \t %s ", px_citoyen ->nom , px_citoyen -> prenom);
        gotoligcol(move - 1,32); printf("    ");
        gotoligcol(move + 1,32); printf("    ");
        Color(6,0);
        gotoligcol(move,40); printf(" %s \t %s ", px_citoyen ->nom , px_citoyen -> prenom);
        gotoligcol(move,32); printf("->");
        Color(15,0);
        if (ajout == 1){

            gotoligcol(move,32); printf("    ");
            ajout = 0;
        }
        gotoligcol(move,32); printf("->");
    
    }
    
    px_participant -> psuivant = NULL;
}


/**
 * \details Partie 5
 * \fn void pointeur_Participant_ajout(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, ptLieu pdebut_lieu, ptLieu pfin_lieu)
 * \brief Fonction permettant l'ajout des participants en réunissant les 2 fonctions : 
 *  - affichage_lieu_pour_participant(ptLieu pdebut_lieu, ptLieu pfin_lieu) 
 *  - affichage_citoyen_pour_participant(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, ptLieu px_lieu, int jour , int mois , int annee, int fin) 

 * 
 * \param pdebut_citoyen  \e  ptCitoyen Pointeur pointant le 1er élément de la liste de Citoyen
 * \param pfin_citoyen  \e  ptCitoyen Pointeur pointant le dernier élément de la liste de Citoyen
 * \param pdebut_lieu  \e  ptLieu  Pointeur pointant le 1er élément de la liste des Lieu
 * \param pfin_lieu  \e  ptLieu    Pointeur pointant le dernier élément de la liste des Lieu
 * 
 * 
 */
void pointeur_Participant_ajout(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, ptLieu pdebut_lieu, ptLieu pfin_lieu){
    
    ptCitoyen px_citoyen; 
    ptLieu px_lieu;
    ptParticipant px_participant;
    
    px_lieu = pdebut_lieu -> pavant; /* 1er lieu sur la 2eme case car 1ere case vide */
    
    int personne = 1; /* Permet de savoir si on a trouve le lieu */
    int jour = 0, mois = 0, annee = 0;
    
    if(px_lieu == pfin_lieu){
        personne = 0;
    }
    if(pdebut_citoyen -> pnext == pfin_citoyen){
        personne = 2;
    }
    if (personne == 1){    
        
        px_lieu = affichage_lieu_pour_participant(pdebut_lieu, pfin_lieu);
        system("color 0f" ); 
        system("cls");
        affichage_titre_participant();
        fonction_date(&jour, &mois, &annee);

        system("cls");
        affichage_titre_participant();
        /* Demande combien de participants  */
        int nombre_participant = 0;
        printf("Combien de personnes etaient presentes a la fete ? "); nombre_participant = int_modif("Combien de personnes etaient presentes a la fete ? ");

        system("cls");
        affichage_titre_participant();

        gotoligcol(30,20);
        affichage_citoyen_pour_participant(pdebut_citoyen, pfin_citoyen, px_lieu, jour, mois , annee, nombre_participant);  
    }
    else{
        printf("ERREUR");
    }   
}


// 6 // : Pour la partie 6 :

/**
 * \details Partie 6
 * \fn ptCitoyen affichage_citoyen_pour_recherche(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen)
 * \brief Fonction permettant d'afficher les citoyen pour une selection avec une fleche pour les ajouter dans ptParticipant
 * 
 * \param pdebut_citoyen  \e  ptCitoyen Pointeur pointant le 1er élément de la liste de Citoyen
 * \param pfin_citoyen  \e  ptCitoyen Pointeur pointant le dernier élément de la liste de Citoyen
 * 
 * Return \b px_citoyen qui permet d'indiquer la positon du citoyen séléctionner dans le pointeur Citoyen 
 * 
 */
ptCitoyen affichage_citoyen_pour_recherche(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen){
    	
    ptCitoyen px_citoyen;

    int i = 10; int touche = 0; int move = 11; int ajout = 0;

    px_citoyen = pdebut_citoyen -> pnext;  

    while (px_citoyen != pfin_citoyen){ /* Affiche citoyen */

        gotoligcol(i+1,40); printf(" %s \t %s ", px_citoyen ->nom , px_citoyen -> prenom);
        px_citoyen = px_citoyen -> pnext;
        i = i + 1;
    }
    
    px_citoyen = pdebut_citoyen -> pnext; /* remet px_citoyen au début */
    gotoligcol(move,32); printf("->");
    system("color 0f" );  
    Color(6,0);
    gotoligcol(move,40); printf(" %s \t %s ", px_citoyen ->nom , px_citoyen -> prenom);
    gotoligcol(move,32); printf("->");
    Color(15,0);  

    while (touche != 13){ //Touche entree
        
        touche = getch();
        if(touche==72){ //haut
        
            if(px_citoyen != pdebut_citoyen -> pnext){
                px_citoyen = px_citoyen -> pprevious;
                move = move - 1;
            }
        }
        
        if (touche==80) {    //Bas 
    
            if (px_citoyen -> pnext != pfin_citoyen){
            
                px_citoyen = px_citoyen -> pnext;
                move = move + 1;
            }
        }
        
        if (touche==13) {    //entree 
    
            return (px_citoyen);
        }

        system("color 0f" ); 
        gotoligcol(move,40); printf(" %s \t %s ", px_citoyen ->nom , px_citoyen -> prenom);
        gotoligcol(move - 1,32); printf("    ");
        gotoligcol(move + 1,32); printf("    ");
        Color(6,0);
        gotoligcol(move,40); printf(" %s \t %s ", px_citoyen ->nom , px_citoyen -> prenom);
        gotoligcol(move,32); printf("->");
        Color(15,0);
    }
}  

/**
 * \details Partie 6
 * \fn void rencontre_Citoyen(ptLieu pdebut_lieu, ptLieu pfin_lieu, ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen)
 * \brief Affiche pas le dernier et le 1er participant et affiche aussi la personne recherché
 * 
 * \param pdebut_lieu  \e  ptLieu  Pointeur pointant le 1er élément de la liste des Lieu
 * \param pfin_lieu  \e  ptLieu    Pointeur pointant le dernier élément de la liste des Lieu
 * \param pdebut_citoyen  \e  ptCitoyen Pointeur pointant le 1er élément de la liste de Citoyen
 * \param pfin_citoyen  \e  ptCitoyen Pointeur pointant le dernier élément de la liste de Citoyen
 * 
 */
void rencontre_Citoyen(ptLieu pdebut_lieu, ptLieu pfin_lieu, ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen){ 

    ptLieu  px_lieu;
    ptParticipant  px_participant;
    ptParticipant  py_participant;
    ptCitoyen px_citoyen;

    px_citoyen = pdebut_citoyen -> pnext;

    if (px_citoyen != pfin_citoyen){

        char nom_recherche[40], prenom_recherche[40];

        affichage_titre_recherche();
        px_citoyen = affichage_citoyen_pour_recherche( pdebut_citoyen,  pfin_citoyen);
        system("cls");
        affichage_titre_recherche();
        /* On se place sur la case non vide */
        px_lieu = pdebut_lieu -> pavant;
        
        while(px_lieu -> pavant != NULL){
            
            px_participant = px_lieu -> pliste;
            py_participant = px_lieu -> pliste;
        
            /* On positionne px et py au debut de la liste */
            
            while(px_participant -> psuivant!= NULL && py_participant -> psuivant != NULL){ 

                px_participant = px_participant -> psuivant; 
                

                if( strcmp(px_participant -> ppersonne -> nom , px_citoyen -> nom) == 0 && strcmp(px_participant -> ppersonne -> prenom , px_citoyen -> prenom ) == 0){
                    
                    /* Py va se positionner sur la 1ere personne qui est allée a la fete le meme jour que la personne malade */ 
                    
                    while(py_participant -> date.day != px_participant -> date.day || py_participant -> date.month != px_participant -> date.month || px_participant -> date.year != px_participant -> date.year){

                        py_participant = py_participant -> psuivant;
                    }
                    
                    if( strcmp(py_participant -> ppersonne -> nom , px_citoyen -> nom) != 0  || strcmp(py_participant -> ppersonne -> prenom , px_citoyen -> prenom) != 0 ){
                        
                        printf(" Nom : %s \t\t Prenom : %s \n", py_participant -> ppersonne -> nom , py_participant -> ppersonne -> prenom);
                    }
                    /* Affiche tous les personnes étant allé a la fete */
                    while( (py_participant -> date.day == px_participant -> date.day || py_participant -> date.month == px_participant -> date.month || px_participant -> date.year == px_participant -> date.year) && (py_participant -> psuivant != NULL )){
                        py_participant = py_participant -> psuivant;

                        if( strcmp(py_participant -> ppersonne -> nom , px_citoyen -> nom) != 0  || strcmp(py_participant -> ppersonne -> prenom , px_citoyen -> prenom) != 0){
                            
                            printf("Date : %d/%d/%d \n", py_participant -> date.day,  py_participant -> date.month ,py_participant -> date.year);
                            printf("Nom : %s \t\t Prenom : %s \n", py_participant -> ppersonne -> nom , py_participant -> ppersonne -> prenom);
                        }   
                    }
                    /* py est possitionne sur le dernier participant de la date en question */
                    px_participant = py_participant ;
                }    
            }

            px_lieu = px_lieu -> pavant;
        }
    }
}


// 7 // : Pour la 7 :

/**
 * \details Partie 7
 * \fn void enregistrer_fichier_citoyen(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, char nom_fichier1_citoyen[])
 * \brief Fonction permettant l'enregistrement des citoyens dans un fichier
 * 
 * \param pdebut_citoyen  \e  ptCitoyen Pointeur pointant le 1er élément de la liste de Citoyen
 * \param pfin_citoyen  \e  ptCitoyen Pointeur pointant le dernier élément de la liste de Citoyen
 * \param nom_fichier1_citoyen[] \e char Variable contennant le nom du fichier Citoyen à analyser 
 * 
 */
void enregistrer_fichier_citoyen(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, char nom_fichier1_citoyen[]){

    ptCitoyen px_citoyen;

    char final[200]; /* Ligne d'enregistrement */
    printf("Enregistrement des citoyens\n");

    FILE * enregistrer_fichier_citoyen = fopen(nom_fichier1_citoyen, "w");

    fputs("$$Debut_fichier_situation_citoyen$$\n",enregistrer_fichier_citoyen); /* Permet de montrer que ce fichier est bien un fichier situation */

    /* Début enregistrement dans fichier pour les citoyens */
    px_citoyen = pdebut_citoyen -> pnext; 
    while(px_citoyen != pfin_citoyen){
        
        strcpy(final,""); /* Initialise final */
        strcat(final , px_citoyen -> nom); strcat(final , ":"); /* Sous la forme Rajon: */
        strcat(final , px_citoyen -> prenom); strcat(final , ":\n"); /* Sous forme Rajon:Baptiste: */
        
        fputs(final, enregistrer_fichier_citoyen); /* Met l'info dans le fichier */
        
        px_citoyen = px_citoyen -> pnext;
    }
    fclose(enregistrer_fichier_citoyen);
}

//Fonction permettant l'enregistrement des lieux dans un fichier
/**
 * \details Partie 7
 * \fn void enregistrer_fichier_lieu(ptLieu pdebut_lieu, ptLieu pfin_lieu, char nom_fichier1_lieu[])
 * \brief Fonction permettant l'enregistrement de Lieu dans un fichier
 * 
 * \param pdebut_lieu  \e  ptLieu  Pointeur pointant le 1er élément de la liste des Lieu
 * \param pfin_lieu  \e  ptLieu    Pointeur pointant le dernier élément de la liste des Lieu
 * \param nom_fichier1_lieu[] \e char Variable contennant le nom du fichier Lieu à analyser 
 * 
 */
void enregistrer_fichier_lieu(ptLieu pdebut_lieu, ptLieu pfin_lieu, char nom_fichier1_lieu[]){

    ptLieu px_lieu;
    printf("\n\nEnregistrement des Lieux\n" );

    FILE * enregistrer_fichier_lieu = fopen(nom_fichier1_lieu, "w");  
    fputs("$$Debut_fichier_situation_lieu$$\n",enregistrer_fichier_lieu); /* Balise montrant que c'est bien un fichier situation */

    px_lieu = pdebut_lieu -> pavant;
    while(px_lieu != pfin_lieu){

        fputs(px_lieu -> nomLieu, enregistrer_fichier_lieu);
        fputs("\n",enregistrer_fichier_lieu);
        px_lieu = px_lieu -> pavant;
    }
    fclose(enregistrer_fichier_lieu);
}

/**
 * \details Partie 7
 * \fn void enregistrer_fichier_participant(ptLieu pdebut_lieu, ptLieu pfin_lieu, char nom_fichier_participant[])
 * \brief Fonction permettant l'enregistrement de Participant dans un fichier
 * 
 * \param pdebut_lieu  \e  ptLieu  Pointeur pointant le 1er élément de la liste des Lieu
 * \param pfin_lieu  \e  ptLieu    Pointeur pointant le dernier élément de la liste des Lieu
 * \param nom_fichier_participant[] \e char Variable contennant le nom du fichier Participant à analyser 
 * 
 */
void enregistrer_fichier_participant(ptLieu pdebut_lieu, ptLieu pfin_lieu, char nom_fichier_participant[]){

    ptLieu px_lieu;
    ptParticipant px_participant;
    
    printf("\n\nEnregistrement des participants\n");
    FILE * enregistrer_fichier_participant = fopen(nom_fichier_participant, "w");
    
    char final_participant[300];
    fputs("$$Fichier_situation_participants\n",enregistrer_fichier_participant); /* Balise */

    px_lieu = pdebut_lieu -> pavant;
    while(px_lieu != pfin_lieu){
        
        px_participant = px_lieu -> pliste;
        while(px_participant -> psuivant != NULL){

            px_participant = px_participant -> psuivant;
                                    
            strcpy(final_participant , ""); /* Initialise */
            strcat(final_participant, px_participant -> ppersonne -> nom); strcat(final_participant , ":"); /* Rajon: */
            strcat(final_participant, px_participant -> ppersonne -> prenom); strcat(final_participant , ":"); /* Rajon:Baptiste: */
            fputs(final_participant, enregistrer_fichier_participant); /* Enregistrement */
            fprintf(enregistrer_fichier_participant,"%d" , px_participant -> date.day); fputs("/", enregistrer_fichier_participant); /* Rajon:Baptiste:16/ */
            fprintf(enregistrer_fichier_participant,"%d" , px_participant -> date.month); fputs("/", enregistrer_fichier_participant); /* Rajon:Baptiste:16/11/ */
            fprintf(enregistrer_fichier_participant,"%d" , px_participant -> date.year); fputs(":", enregistrer_fichier_participant); /* Rajon:Baptiste:16/11/2020: */
            fputs(px_lieu -> nomLieu, enregistrer_fichier_participant); /* Rajon:Baptiste:16/11/2020:ENSIBS */
            fputs("\n",enregistrer_fichier_participant); /* Rajon:Baptiste:16/11/2020:ENSIBS\n */
        }
        px_lieu = px_lieu -> pavant;
    }
    fclose(enregistrer_fichier_participant); 
}

/**
 * \details Partie 7
 * \fn void pointeur_Participant_ajout(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, ptLieu pdebut_lieu, ptLieu pfin_lieu)
 * \brief Fonction enregistrant les données des structures Citoyen, Lieu et Participant dans des fichiers. Cette fonction requert 3 autre fonctions : 
 *  - enregistrer_fichier_citoyen(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, char nom_fichier1_citoyen[])
 *  - enregistrer_fichier_lieu(ptLieu pdebut_lieu, ptLieu pfin_lieu, char nom_fichier1_lieu[]) 
 *  - enregistrer_fichier_participant(ptLieu pdebut_lieu, ptLieu pfin_lieu, char nom_fichier_participant[])

 * 
 * \param pdebut_citoyen  \e  ptCitoyen Pointeur pointant le 1er élément de la liste de Citoyen
 * \param pfin_citoyen  \e  ptCitoyen Pointeur pointant le dernier élément de la liste de Citoyen
 * \param pdebut_lieu  \e  ptLieu  Pointeur pointant le 1er élément de la liste des Lieu
 * \param pfin_lieu  \e  ptLieu    Pointeur pointant le dernier élément de la liste des Lieu
 * 
 * 
 */
void enregistrer_fichier_situation(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, ptLieu pdebut_lieu, ptLieu pfin_lieu){
    
    ptCitoyen px_citoyen;
    ptLieu px_lieu;
    ptParticipant px_participant;

    px_lieu = pdebut_lieu -> pavant;

    char save[20]; char dossier[] = "Fichier/Affichage/.";
    char nom_fichier[20];
    char nom_fichier1_citoyen[] = "Fichier/citoyen_"; char nom_fichier1_lieu[] = "Fichier/lieu_"; char nom_fichier_participants[] = "Fichier/participant_"; char affichage[] = "Fichier/Affichage/";
    
    printf("\n\nVoulez vous enregistrer les informations dans un fichier ? [y/n] :");scanf("%s", &save);

    if ( strcmp(save,"y") == 0 || strcmp(save,"Y") == 0 || strcmp(save,"o") == 0 || strcmp(save,"O") == 0 || strcmp(save,"yes") == 0 || strcmp(save,"Yes") == 0 || strcmp(save,"YES") == 0 || strcmp(save,"oui") == 0 || strcmp(save,"Oui") == 0 || strcmp(save,"OUI") == 0){
        system("cls");
        Color(1,15); afficher_titre_fichier_situation_enregistrer(); Color(15,0);
        affichier_dossier(dossier);

        printf("Vous voulez enregistrer votre fichier sous quel nom ? (pas besoin de mettre l'extension (deja .txt))"); fflush(stdin); gets(nom_fichier); /*Permet de pouvoir saisir des espaces */

        strcpy(save,nom_fichier); /* Permet de copier le nom du fichier dans la variable save (qu'on a utiliser pour la verification mais la elle ne sert plus donc on peut l'utiliser pour autre chose) */
        strcat(nom_fichier1_citoyen, nom_fichier); strcat(nom_fichier1_citoyen, ".txt"); /* Permet d'avoir Fichier/nom_fichier.txt */

        /* Pour l'affichage cree un fichier dans un autre dossier*/
        strcat(affichage , save); strcat(affichage, ".txt");
        FILE * affichage1 = fopen(affichage,"w"); fclose(affichage1);
        /* Fin affichage */

        system("cls");

        Color(1,15);
        afficher_titre_fichier_situation_enregistrer();
        Color(15,0);

        enregistrer_fichier_citoyen(pdebut_citoyen, pfin_citoyen, nom_fichier1_citoyen);
        affichage_telechargement(15);
        
        strcat(nom_fichier1_lieu, save); strcat(nom_fichier1_lieu, ".txt"); 
        
        enregistrer_fichier_lieu(pdebut_lieu, pfin_lieu, nom_fichier1_lieu);
        affichage_telechargement(18);
        
        strcat(nom_fichier_participants , save); strcat(nom_fichier_participants, ".txt");
        
        enregistrer_fichier_participant(pdebut_lieu, pfin_lieu, nom_fichier_participants);
        affichage_telechargement(22);
    }

    else if (strcmp(save,"n") == 0 || strcmp(save,"N") == 0 || strcmp(save,"no") == 0 || strcmp(save,"NO") == 0 || strcmp(save,"non") == 0 || strcmp(save,"Non") == 0 || strcmp(save,"NON") == 0)
    {
        printf ("Ok aucun enregistrement ...");
    }

   else{
       printf("Retour au MENU ...");
   }
}


// 8 // : Pour la 8 :

/**
 * \details Partie 8
 * \fn int ouvrir_fichier_citoyen(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, char open_file_citoyen[])
 * \brief Fonction permettant l'enregistrement de citoyens d'un fichier à la liste Citoyen
 * 
 * \param pdebut_citoyen  \e  ptCitoyen Pointeur pointant le 1er élément de la liste de Citoyen
 * \param pfin_citoyen  \e  ptCitoyen Pointeur pointant le dernier élément de la liste de Citoyen
 * \param open_file_citoyen[] \e char Variable contennant le nom du fichier Citoyen à analyser 
 * 
 * \return Return 1 si le fichier existe et 0 s'il n'existe pas
 * 
 */
int ouvrir_fichier_citoyen(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, char open_file_citoyen[]){

    ptCitoyen px_citoyen;
    px_citoyen = pdebut_citoyen; 

    int ligne = 1; int existe = 1; /* Existe = dit si le fichier existe ~~ 1 : existe  2 : Existe pas*/
    char chaine_citoyen[80];

    FILE * final_file_citoyen = fopen(open_file_citoyen,"r");

    if(final_file_citoyen != NULL){

        int nLignes = Compte_ligne(final_file_citoyen);
        while (ligne < nLignes){

            fgets(chaine_citoyen, 80, final_file_citoyen); 
            insertion_Citoyen(px_citoyen);
            px_citoyen = px_citoyen -> pnext;
            fscanf(final_file_citoyen, "%[^:]:%[^:]%[$\n]", px_citoyen -> nom , px_citoyen -> prenom ); /* Forme : Nom:Prenom:\n */
            ligne = ligne + 1;
        }
        fclose(final_file_citoyen);
    }
    else{

        existe = 0;
    }
    return existe; 
}

/**
 * \details Partie 8
 * \fn int ouvrir_fichier_citoyen(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, char open_file_citoyen[])
 * \brief Fonction permettant l'enregistrement des lieux d'un fichier à la liste Lieu
 * 
 * \param pdebut_lieu  \e  ptLieu  Pointeur pointant le 1er élément de la liste des Lieu
 * \param pfin_lieu  \e  ptLieu    Pointeur pointant le dernier élément de la liste des Lieu
 * \param open_file_lieu[] \e char Variable contennant le nom du fichier Lieu à analyser 
 * 
 * \return Return 1 si le fichier existe et 0 s'il n'existe pas
 * 
 */
int ouvrir_fichier_lieu(ptLieu pdebut_lieu, ptLieu pfin_lieu, char open_file_lieu[]){

    ptLieu px_lieu;
    px_lieu = pdebut_lieu;

    int ligne = 1; int existe = 1; /* Existe = dit si le fichier existe ~~ 1 : existe  2 : Existe pas*/
    char chaine_lieu[80];

    FILE * final_file_lieu = fopen(open_file_lieu,"r");

        if(final_file_lieu != NULL){

            ligne = 1;
            int nLignes_lieu = Compte_ligne(final_file_lieu);
            while (ligne < nLignes_lieu){

                fgets(chaine_lieu, 80, final_file_lieu); 
                insertion_Lieu(px_lieu);
                px_lieu = px_lieu -> pavant;
                fscanf(final_file_lieu, "%s", px_lieu -> nomLieu);
                px_lieu -> pliste = (Tparticipant *)malloc(sizeof(Tparticipant));
                px_lieu -> pliste -> psuivant = NULL;
                 
                ligne = ligne + 1;
            }
            fclose(final_file_lieu);
        }
        else{

            existe = 0;
        }
    return (existe);
}

/**
 * \details Partie 8
 * \fn int ouvrir_fichier_citoyen(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, char open_file_citoyen[])
 * \brief Fonction permettant l'enregistrement des participants d'un fichier au pointeur participant. 
 *  Cette fonction Requert 2 autres fonctions : 
 *   - ouvrir_fichier_citoyen(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, char open_file_citoyen[])
 *   - ouvrir_fichier_lieu(ptLieu pdebut_lieu, ptLieu pfin_lieu, char open_file_lieu[])
 * 
 * \param pdebut_citoyen  \e  ptCitoyen Pointeur pointant le 1er élément de la liste de Citoyen
 * \param pfin_citoyen  \e  ptCitoyen Pointeur pointant le dernier élément de la liste de Citoyen
 * \param pdebut_lieu  \e  ptLieu  Pointeur pointant le 1er élément de la liste des Lieu
 * \param pfin_lieu  \e  ptLieu    Pointeur pointant le dernier élément de la liste des Lieu
 * \param open_file_participant[] \e char Variable contennant le nom du fichier Participant à analyser 
 * 
 * \return Return 1 si le fichier existe et 0 s'il n'existe pas
 * 
 */
int ouvrir_fichier_participant(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, ptLieu pdebut_lieu, ptLieu pfin_lieu, char open_file_participant[]){

    ptCitoyen px_citoyen;
    ptLieu px_lieu;
    ptParticipant px_participant;

    char chaine_participant[300];
    char nom[40], prenom[40], lieu_participant[40], jour[10] , mois[10], annee[10];
    int jour_modif = 0, mois_modif = 0, annee_modif = 0; /* Permet de transformer la date (char) ne int */
    int ligne_participant = 1; /* nb de ligne dans le fichier des participants */
    int boucle = 1;
    int existe = 1; /* Existe = dit si le fichier existe ~~ 1 : existe  2 : Existe pas*/

    px_citoyen = pdebut_citoyen -> pnext;
    px_lieu = pdebut_lieu -> pavant;

    FILE * final_file_participant = fopen(open_file_participant,"r");
        
    if(final_file_participant != NULL){
        
        int nLignes_participant = Compte_ligne(final_file_participant);

        while(ligne_participant < nLignes_participant){

            fgets(chaine_participant,300,final_file_participant);
            fscanf(final_file_participant, "%[^:]:%[^:]:%[^/]/%[^/]/%[^:]:%[^\n]", &nom , &prenom , &jour , &mois , &annee , &lieu_participant ); /* Forme : Nom:Prenom:Jour/Mois/Annee:Lieu\n */
            jour_modif = atof(jour); mois_modif = atof(mois); annee_modif = atof(annee); /* Passe de char en int */
            
            if(boucle == 1){ /* Permet de faire rentrer les participants du 1er mot dans le fichier participant */
                
                px_lieu = pdebut_lieu -> pavant;
                while(px_lieu != pfin_lieu){

                    if( strcmp(px_lieu -> nomLieu , lieu_participant) == 0){
                        
                        break;
                    }
                    px_lieu = px_lieu -> pavant;
                }

                px_participant = px_lieu -> pliste; 
            
                while(px_participant -> psuivant != NULL){

                    px_participant = px_participant -> psuivant;
                }  
                boucle = 0;
            }

            if (strcmp( px_lieu -> nomLieu , lieu_participant) != 0 ){

                px_participant -> psuivant = NULL; /* Met la valeur du px_participant -> psuivant  à NULL au cas ou il n'y ait plus de participant */
                px_lieu = pdebut_lieu -> pavant; 

                while(px_lieu != pfin_lieu){

                    if( strcmp(px_lieu -> nomLieu , lieu_participant) == 0){
                    
                        break;
                    }
                    px_lieu = px_lieu -> pavant;
                }
                px_participant = px_lieu -> pliste; 
            
                while(px_participant -> psuivant != NULL){

                    px_participant = px_participant -> psuivant;
                }  
            }

            px_citoyen = pdebut_citoyen -> pnext; /*Remet px_citoyen au début */

            while(px_citoyen != pfin_citoyen){
                
                if( strcmp(px_citoyen -> nom , nom) == 0 && strcmp(px_citoyen -> prenom , prenom) == 0){

                    px_participant -> psuivant = (Tparticipant *)malloc(sizeof(Tparticipant)); /* Cree une case memoire */
                    px_participant = px_participant -> psuivant; /* On se positionne sur la case que nous venons de creer */
                    /* Ajout de la date */
                    px_participant -> date.day = jour_modif; px_participant -> date.month = mois_modif; px_participant -> date.year = annee_modif;
                    /*Ajout de nom-participant et prenom_participant dans le pointeur */
                    px_participant -> ppersonne = px_citoyen;

                    px_citoyen = px_citoyen -> pnext;   
                }
                else{

                    px_citoyen = px_citoyen -> pnext;
                }
            }

            ligne_participant = ligne_participant + 1; /* Avance d'une ligne dans la variable permettant de compte les lignes*/
        }
        px_participant -> psuivant = NULL;
    }

    else{
        existe = 0;
    }
    return existe;
}

/**
 * \details Partie 8
 * \fn void ouvrir_fichier_situation(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, ptLieu pdebut_lieu, ptLieu pfin_lieu)
 * \brief Fonction enregistrant les données des fihciers Citoyen, Lieu et Participant dans des listes. 
 * Cette fonction requert 3 autre fonctions : 
 *   - ouvrir_fichier_citoyen(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, char open_file_citoyen[])
 *   - ouvrir_fichier_lieu(ptLieu pdebut_lieu, ptLieu pfin_lieu, char open_file_lieu[])
 *   - ouvrir_fichier_participant(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, ptLieu pdebut_lieu, ptLieu pfin_lieu, char open_file_participant[])
 * 
 * \param pdebut_citoyen  \e  ptCitoyen Pointeur pointant le 1er élément de la liste de Citoyen
 * \param pfin_citoyen  \e  ptCitoyen Pointeur pointant le dernier élément de la liste de Citoyen
 * \param pdebut_lieu  \e  ptLieu  Pointeur pointant le 1er élément de la liste des Lieu
 * \param pfin_lieu  \e  ptLieu    Pointeur pointant le dernier élément de la liste des Lieu
 * 
 * 
 */
void ouvrir_fichier_situation(ptCitoyen pdebut_citoyen, ptCitoyen pfin_citoyen, ptLieu pdebut_lieu, ptLieu pfin_lieu){

    ptCitoyen px_citoyen;
    ptCitoyen py_citoyen;
    ptLieu px_lieu;
    ptParticipant px_participant;
 
    char save[20];
    int existe = 0;

    char dossier[] =  "Fichier/Affichage/.";
    char open_file[20]; char open_file1[]="Fichier/"; char open_file_citoyen[]="Fichier/citoyen_"; char open_file_lieu[]="Fichier/lieu_"; char open_file_participant[]="Fichier/participant_";

    printf("\nVoulez vous ouvrir un fichier situation ? [y/n] :");scanf("%s", &save);

    if ( strcmp(save,"y") == 0 || strcmp(save,"Y") == 0 || strcmp(save,"o") == 0 || strcmp(save,"O") == 0 || strcmp(save,"yes") == 0 || strcmp(save,"Yes") == 0 || strcmp(save,"YES") == 0 || strcmp(save,"oui") == 0 || strcmp(save,"Oui") == 0 || strcmp(save,"OUI") == 0){
        
        system("cls");
        Color(1,15); afficher_titre_fichier_situation_ouvrir(); Color(15,0);
        affichier_dossier(dossier);        

        printf("\nQuel fichier voulez vous ouvrir ? (pas besoin de mettre l'extension )"); fflush(stdin); gets(open_file); /*Permet de pouvoir saisir des espaces */
        strcpy(save, open_file);

        system("cls");
        Color(1,15); afficher_titre_fichier_situation_ouvrir(); Color(15,0);

        /* Citoyen */
        strcat(open_file_citoyen, open_file); strcat(open_file_citoyen, ".txt"); //Pour avoir sous la forme Fichier/le_nom_du_fichier_a_ouvrir.txt
        existe = ouvrir_fichier_citoyen(pdebut_citoyen, pfin_citoyen, open_file_citoyen);
        printf("\nEnregistrement Citoyen\n");     
        if (existe != 0){   

            affichage_telechargement(15);
        }
        else{
            
            affichage_telechargement_fail(15);
            Color(12,0); printf (" \tLe fichier n'existe pas"); Color(15,0);
        }

        /* Lieu */
        strcat(open_file_lieu, save); strcat(open_file_lieu, ".txt"); //Pour avoir sous la forme Fichier/le_nom_du_fichier_a_ouvrir.txt

        existe = ouvrir_fichier_lieu(pdebut_lieu, pfin_lieu, open_file_lieu);    
        printf("\n\nEnregistrement Lieux\n");
        if (existe != 0){   

            affichage_telechargement(19);
        }
        else{
           
            affichage_telechargement_fail(19);
            Color(12,0); printf (" \tLe fichier n'existe pas"); Color(15,0);
        }

        /* Participants */
        strcat(open_file_participant, save); strcat(open_file_participant, ".txt"); //Pour avoir sous la forme Fichier/le_nom_du_fichier_a_ouvrir.txt

        existe = ouvrir_fichier_participant(pdebut_citoyen, pfin_citoyen, pdebut_lieu, pfin_lieu, open_file_participant);
        printf("\n\nEnregistrement Participants\n");
        
        if (existe != 0){   
            affichage_telechargement(23);
        }
        else{
            
            affichage_telechargement_fail(23);
            Color(12,0); printf (" \tLe fichier n'existe pas"); Color(15,0);
        } 
    }
    else {
        printf ("Ok retour au MENU ...");
    }
}

// 9 // Fonction qui permet de supprimer des participants en fonction de la date de la fete
/**
 * \details Partie 9
 * \fn void supprimer_evenement(ptLieu pdebut_lieu, ptLieu pfin_lieu)
 * \brief Fonction qui permet de supprimer des évènements ayant une date antérieur à une date saisie par l'utilisateur 
 
 * 
 * \param pdebut_lieu  \e  ptLieu  Pointeur pointant le 1er élément de la liste des Lieu
 * \param pfin_lieu  \e  ptLieu    Pointeur pointant le dernier élément de la liste des Lieu
 * 
 * \todo Faire un fichier log pour voir les participants supprimer
 * 
 */
void supprimer_evenement(ptLieu pdebut_lieu, ptLieu pfin_lieu){ 

    ptLieu px_lieu;
    ptParticipant px_participant;

    int jour = 0, mois = 0, annee = 0;
    int personne = 1;

    if(pdebut_lieu -> pavant == pfin_lieu){ /* Si aucun lieu enregistrer */
        personne = 0;
    }

    if (personne == 1){

        printf("\n\nA partir de quelle date voulez vous supprimer les evenements anterieur ? \n");
        fonction_date(&jour, &mois, &annee);
        system("cls");

        Color(1,15); afficher_titre_supprimer_participants(); Color(15,0);
        
        /* 1ere casse de PtLieu vide on passe donc a la suivante */ 
        px_lieu = pdebut_lieu -> pavant;

        while(px_lieu != pfin_lieu){
            
            px_participant = px_lieu -> pliste;

            while(px_participant -> psuivant != NULL){

                if(px_participant -> psuivant -> date.year < annee){

                    px_participant->psuivant->date.day = 0; px_participant->psuivant->date.month = 0; px_participant->psuivant->date.year = 0; px_participant->psuivant->ppersonne = 0;
                    px_participant -> psuivant = px_participant -> psuivant -> psuivant;
                }
                
                else if(px_participant -> psuivant -> date.year == annee && px_participant -> psuivant -> date.month < mois){

                    px_participant->psuivant->date.day = 0; px_participant->psuivant->date.month = 0; px_participant->psuivant->date.year = 0; px_participant->psuivant->ppersonne = 0;
                    px_participant -> psuivant = px_participant -> psuivant -> psuivant;
                }

                else if (px_participant -> psuivant -> date.year == annee && px_participant -> psuivant -> date.month == mois && px_participant -> psuivant -> date.day < jour){

                    px_participant->psuivant->date.day = 0; px_participant->psuivant->date.month = 0; px_participant->psuivant->date.year = 0; px_participant->psuivant->ppersonne = 0;
                    px_participant -> psuivant = px_participant -> psuivant -> psuivant;
                }

                else{
                    px_participant = px_participant -> psuivant;
                }
                
            }
            px_lieu = px_lieu -> pavant;
                    
        }
        px_participant -> psuivant = NULL;
        Color(10,0); gotoligcol(20,45); printf("Modification appliquees"); Color(15,0);
    }
    else{
        Color(12,0); gotoligcol(20,45); printf("ERREUR Aucun citoyen dans la base"); Color(15,0);
    }
}


int main (void) {

///////////////////

//Pour graphique //
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
///////////////////

///// Definiton pointeurs : /////

//Citoyen
    ptCitoyen pdebut_citoyen;
	ptCitoyen pfin_citoyen;  
    ptCitoyen px_citoyen; 
    
    pdebut_citoyen = (Tcitoyen *)malloc(sizeof(Tcitoyen));
	pfin_citoyen = (Tcitoyen *)malloc(sizeof(Tcitoyen));
	pdebut_citoyen->pprevious = NULL;
	pfin_citoyen->pnext = NULL;
	pdebut_citoyen->pnext = pfin_citoyen;
	pfin_citoyen->pprevious = pdebut_citoyen;

//Participant
    ptParticipant px_participant;
    ptParticipant py_participant;


//Lieu 
    ptLieu pdebut_lieu;
	ptLieu pfin_lieu;  
    ptLieu px_lieu;

    
    pdebut_lieu = (Tlieu *)malloc(sizeof(Tlieu));
	pfin_lieu = (Tlieu *)malloc(sizeof(Tlieu));
	pdebut_lieu->pprecedent = NULL;
	pfin_lieu->pavant = NULL;
	pdebut_lieu->pavant = pfin_lieu;
	pfin_lieu->pprecedent = pdebut_lieu;

/* Ecran d'accueil */
    system("cls"); 
	Color(2,0);
    gotoligcol(10,11);printf("   ______                                                           ________  ________  __    __"); 
    gotoligcol(11,11);printf("  /      \\                                                         /        |/        |/  \\  /  |");
    gotoligcol(12,11);printf(" /$$$$$$  |  ______    ______    ______   _______    ______        $$$$$$$$/ $$$$$$$$/ $$  \\ $$ |");
    gotoligcol(13,11);printf(" $$ |  $$/  /      \\  /      \\  /      \\ /       \\  /      \\           /$$/  $$ |__    $$$  \\$$ |");
    gotoligcol(14,11);printf(" $$ |      /$$$$$$  |/$$$$$$  |/$$$$$$  |$$$$$$$  | $$$$$$  |         /$$/   $$    |   $$$$  $$ |");
    gotoligcol(15,11);printf(" $$ |   __ $$ |  $$ |$$ |  $$/ $$ |  $$ |$$ |  $$ | /    $$ |        /$$/    $$$$$/    $$ $$ $$ |");
    gotoligcol(16,11);printf(" $$ \\__/  |$$ \\__$$ |$$ |      $$ \\__$$ |$$ |  $$ |/$$$$$$$ |       /$$/____ $$ |_____ $$ |$$$$ |");
    gotoligcol(17,11);printf(" $$    $$/ $$    $$/ $$ |      $$    $$/ $$ |  $$ |$$    $$ |      /$$      |$$       |$$ | $$$ |");
    gotoligcol(18,11);printf("  $$$$$$/   $$$$$$/  $$/        $$$$$$/  $$/   $$/  $$$$$$$/       $$$$$$$$/ $$$$$$$$/ $$/   $$/ ");
    gotoligcol(30,50);system("echo Date : %date%");
    //gotoligcol(9,80); afficher_nombre_cas();                                                                                                                                                                                                                                                                                                                 


    char caractere = 32;                                                                                                                                                                                                                                                                                                                     
    caractere = getch();
    int boucle = 0; 
    while(boucle != 1){

        afficheMenu(hConsole);
        caractere = getch();
        if (caractere == '1' ){

            system("cls");
            Color(1,15);
            gotoligcol(1,1);printf("                                                                                                      _____           ");       
            gotoligcol(2,1);printf("      ***                                                                                           _/ _ _ \\_         ");                                                                 
            gotoligcol(3,1);printf("    *******                                                                                        (o / | \\ o)        ");
            gotoligcol(4,1);printf("   *********                           ____ _ _                                                     || o|o ||         ");
            gotoligcol(5,1);printf("/\\* ### ### */\\                       / ___(_) |_ ___  _   _  ___ _ __                              | \\_|_/ |         ");  
            gotoligcol(6,1);printf("|    @ / @    |                       | |   | | __/ _ \\| | | |/ _ \\ '_ \\                            |  ___  |         ");
            gotoligcol(7,1);printf("\\/\\    ^    /\\/                       | |___| | || (_) | |_| |  __/ | | |                           | (___) |         ");  
            gotoligcol(8,1);printf("   \\  ===  /                           \\____|_|\\__\\___/ \\__, |\\___|_| |_|                           |\\_____/|         ");
            gotoligcol(9,1);printf("    \\_____/                                             |___/                                       | \\___/ |         "); 
            gotoligcol(10,1);printf("     _|_|_                                                                                          \\       /         ");          
            gotoligcol(11,1);printf("  *$$$$$$$$$*                                                                                        \\__ __/          ");
            gotoligcol(12   ,1);printf("                                                                                                        U             \n");
            Color(15,0);
            pointeur_Citoyen_affiche(pdebut_citoyen, pfin_citoyen);
        }
        if (caractere == '2' ){
            system("cls");
            char choix[4];
            printf("Voulez vous afficher les personnes etant alle dans les lieux ? [y/n] "); scanf("%s",&choix);
            system("cls");
            lieu_header();

            /* Affiche lieu avec participants */
            if ( strcmp(choix,"y") == 0 || strcmp(choix,"Y") == 0 || strcmp(choix,"o") == 0 || strcmp(choix,"O") == 0 || strcmp(choix,"yes") == 0 || strcmp(choix,"Yes") == 0 || strcmp(choix,"YES") == 0 || strcmp(choix,"oui") == 0 || strcmp(choix,"Oui") == 0 || strcmp(choix,"OUI") == 0){
                
                pointeur_Lieu_participant_affiche(pdebut_lieu, pfin_lieu);
            }
            else  if ( strcmp(choix,"n") == 0 || strcmp(choix,"N") == 0 || strcmp(choix,"no") == 0 || strcmp(choix,"No") == 0 || strcmp(choix,"NO") == 0 || strcmp(choix,"non") == 0 || strcmp(choix,"Non") == 0 || strcmp(choix,"NON") == 0){

                pointeur_Lieu_affiche(pdebut_lieu, pfin_lieu);
            }
            else{
                gotoligcol(21,50);printf("Retour au MENU");
            }
            
        }
        if (caractere == '3' ){
            
            system("cls");
            tete_affiche_1(1,1);
            
            Color(1,15);
            Color(3,5);gotoligcol(18,2);printf("                                                                                                                ");Color(15,0);
            gotoligcol(6,28);printf("           _             _          _ _                         ");
            gotoligcol(7,28);printf("     /\\   (_)           | |        (_) |                        ");
            gotoligcol(8,28);printf("    /  \\   _  ___  _   _| |_    ___ _| |_ ___  _   _  ___ _ __  ");
            gotoligcol(9,28);printf("   / /\\ \\ | |/ _ \\| | | | __|  / __| | __/ _ \\| | | |/ _ \\ '_ \\ ");
            gotoligcol(10,28);printf("  / ____ \\| | (_) | |_| | |_  | (__| | || (_) | |_| |  __/ | | |");
            gotoligcol(11,28);printf(" /_/    \\_\\ |\\___/ \\__,_|\\__|  \\___|_|\\__\\___/ \\__, |\\___|_| |_|");
            gotoligcol(12,28);printf("         _/ |                                   __/ |           ");
            gotoligcol(13,28);printf("        |__/                                   |___/            ");
            Color(15,0);
            
            tete_affiche_2(1,91);
            Color(1,0);gotoligcol(18,2);printf("____________________________________________________________________________________________________________________");Color(15,0);

            pointeur_Citoyen_ajout(pdebut_citoyen, pfin_citoyen);
        }
        if (caractere == '4' ){
            system("cls");
            Color(1,15);
            gotoligcol(1,1);printf("   ____||____                          _    _             _       __ _                                   []_____      ");
            gotoligcol(2,1);printf("  ///////////\\                        /_\\  (_) ___  _   _| |_    / /(_) ___ _   _                        /\\      \\    ");
            gotoligcol(3,1);printf(" ///////////  \\                      //_\\\\ | |/ _ \\| | | | __|  / / | |/ _ \\ | | |                   ___/  \\__/\\__\\__ ");
            gotoligcol(4,1);printf(" |    _    |  |                     /  _  \\| | (_) | |_| | |_  / /__| |  __/ |_| |                  /\\___\\ |''''''|__\\");
            gotoligcol(5,1);printf(" |[] | | []|[]|                     \\_/ \\_// |\\___/ \\__,_|\\__| \\____/_|\\___|\\__,_|                  ||'''| |''||''|''|");
            gotoligcol(6,1);printf(" |   | |   |  |                         |__/                                                         '''''''''))''''''");
            gotoligcol(7,1);printf("                                                                                                                      ");       
            gotoligcol(8,1);printf( "____________________________________________________________________________________________________________________  ");   
            Color(15,0);
            gotoligcol(9,2);pointeur_Lieu_ajout(pdebut_lieu, pfin_lieu);
        }
        if (caractere == '5' ){
            system("cls");
            affichage_titre_participant();            
            pointeur_Participant_ajout(pdebut_citoyen, pfin_citoyen, pdebut_lieu, pfin_lieu);
        }
        if (caractere == '6' ){
            system("cls");
            rencontre_Citoyen(pdebut_lieu, pfin_lieu,pdebut_citoyen,  pfin_citoyen);
        }
        if (caractere == '7' ){
            system("cls");
            Color(1,15);
            afficher_titre_fichier_situation_enregistrer();
            Color(15,0);
            enregistrer_fichier_situation(pdebut_citoyen, pfin_citoyen, pdebut_lieu, pfin_lieu);
            
        }
        if (caractere == '8' ){
            system("cls");
            Color(1,15);
            afficher_titre_fichier_situation_ouvrir();
            Color(15,0);
            ouvrir_fichier_situation(pdebut_citoyen, pfin_citoyen, pdebut_lieu, pfin_lieu);
            
        }
        if (caractere == '9' ){
            system("cls");
            Color(1,15);
            afficher_titre_supprimer_participants();
            Color(15,0);
            supprimer_evenement(pdebut_lieu, pfin_lieu);
        }
        if (caractere == '0' )
            break;
	        Color(12,7);gotoligcol(30,79); printf("Appuyer sur une touche pour continuer ..."); Color(15,0);
	        caractere= getch();
	}


    return 0;
}

/**
 * \fn main(void) 
 * 
 * \todo - Faire une fonctionnalité permettant de supprimer directement un Citoyen ou un Lieu 
 * \todo - Faire une fonctionnalité permettant de revenir au Menu principale en abandonnant la tâches que nous étions en train de faire
 * \todo - Faire une fonctionnalité permettant,au premier démarrage du programme, de demander si nous souhaitons ou non utiliser \n
 *       les droits d’administrateurs et stocker l’information dans un fichier
 * \todo - Rajouter des catégories pour mieux décrire les Citoyen et les Lieu
 * \todo - Modulariser afin de mieux pouvoir relir le code
 * 
*/




/**
 * \page "1 : Introduction"
 * 
 * \brief                                                                                     
 * 
 * \brief      Ce programme permet de pouvoir enregistrer les différentes personnes que nous avons croisé dans les fêtes que nous avons faites. \n
 *             Nous pouvons pour cela enregistrer les différentes personnes que nous avons rencontré et les différents lieux que nous avons visités. \n
 *             Nous pouvons aussi enregistrer les informations rentrées dans des fichiers afin de les sauvegardées même après l’arrêt de notre programme. \n \n
 * 
*/

/**
 * 
 * \page "2 : Simplification "
 * 
 * \brief                                                                                                                                                                   
 * 
 * \brief      Afin de simplifier le programme, je pourrai modifier la fonction permettant de rentrer la date choisisant de la date avec les flèches comme avec les citoyens et les lieux. 
 * 
 * 
 * 
*/
/**
 * 
 * \page "3 : Portee du projet"
 * 
 * \brief                  
 * \brief             
 * \brief             
 * \brief  L’objectif de ce projet est de réaliser un programme en langage C étant capable de nous dire les différentes personnes que nous avons rencontré dans un délais de 7 jours. \n
 *         Pour cela, nous avons du créer plusieurs structures nous permettant de pouvoir faire correspondre les différentes informations entre elles. C’est-à-dire pouvoir ajouter des \n
 *         personnes à un lieu à une date précise sans aucun problème. Nous avons été contrains d’enregistrer les informations saisis dans des fichiers afin de les sauvegardés. \n 
 *         Nous avons du aussi faire un programme qui les exploites afin de réexploiter nos sauvegardes. \n
 *         Nous avons eu qu'une seule contrainte qui a été le temps. Nous avons eu 1 mois et demi pour faire ce projet.
 * 
*/

/**
 * 
 * \page "4 : Limites" 
 * 
 * \brief  
 * \brief 
 * \brief   -    Pouvoir exploiter plus les données du coronavirus que nous pouvons trouver sur internet afin de faire des graphiques pour voir l’évolution de la maladie jour par jour.
 * \brief   -    Rajouter une classe malade dans la liste  Citoyen. Nous pourrions envoyer un message automatiquement aux différentes personnes que le malade a rencontré les 7 derniers jours.
 * 
*/




/**
 * 
 * \page "5 : Utilisation" 
 * 
 * \brief                                                                                        
 * 
 * \brief Afin de bien naviguer dans le programme, il faut suivre les indications que donne le programme. C'est à dire :
 *      - Afin de sélectionner une rubrique du Menu, il faut juste taper le chiffre correspondant à la rubrique 
 * \brief \n
 *      - Afin d'ajouter un Citoyen, il suffit de se rentre dans la partie d'ajout de citoyen puis de rentrer le nom. Afin de dire au programme que vous avez fini, \n
 *        il faut appuyer sur la touche entrée. C'est la même manière pour l'ajout du prénom. 
 * \brief \n
 *      - Afin d'ajouter un Lieu, il faut se rendre dans la partie d'ajout de lieu. Il faut ensuite rentrer le nom du lieu (qui peut être une suite de plusieurs mots ) en finissant par la touche entrée symbolisant la fin du nom du lieu.
 * \brief \n
 *      - Afin d'ajouter un Participant, il faut sélectionner un lieu grâce aux flèches (flèche haute pour monter / basse pour descendre). \n
 *        Il faut après sélectionner la date de la fête qui demande de saisir un nombre (appuyer sur entrée pour valider la sélection)
 *        Il faut après choisir de la même manière que les lieux les Citoyen ayant participés à la fête
 * \brief \n
 *      - Afin d'enregistrer les informations dans un fichier, il suffit d'aller dans la catégorie en question (7) et de suivre les informations. A la fin, il faudra rentrer le nom du fichier \b SANS mettre l'extension. Elle est mise automatiquement.
 * \brief \n
 *      - Afin de récupérer les informations que nous avons enregistrer, il suffit d'aller dans la catégorie en question (8) et de suivre les informations. A la fin, il faudra rentrer le nom du fichier que vous voulez charger \b SANS mettre l'extension. Elle est mise automatiquement.
 * \brief \n
 *      - Afin de supprimer des fêtes trop anciennes, il suffit d'aller dans la catégorie en question (9) et de suivre les informations. Tous les évènements ayant une date inférieur à la date rentrée seront supprimer.  
*/

/**
 * 
 *  \page "6 : Conclusion" 
 * 
 * \brief                                                                                                                                           
 * 
 * \brief Ce projet m’a permis de bien mieux comprendre le langage C en me faisant travailler sur les pointeurs, les listes simplement et doublement chainées, les structures … \n
 *        Cela m’a aussi permis de voir que pour faire des projet de programmation, il faut bien se pauser et se faire un plan clair et précis pour ne pas se perdre et devoir tout recommencer. \n
 *        J’ai trouvé ce projet intéressant mais je trouve que dès que nous voulons faire des choses qui sortent de l’ordinaire, le C devient très rapidement complexe. 
 *
*/
