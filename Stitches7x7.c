#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// Définition des constantes
#define MAX_GRILLES 10
#define MAX_SIZE 7

// Structure de cellule représentant chaque point dans la grille du jeu
typedef struct {
    char stitch; // Type de point ('>', 'v', '<', '^', ou ' ' pour vide)
    int zone;    // Identifiant de la zone à laquelle la cellule appartient
} Cell;

// Structure de bloc représentant le nombre de trous dans une ligne ou une colonne
typedef struct {
    int numHoles;
} Block;

// Structure de lien entre deux zones
typedef struct {
    int zone1;
    int zone2;
    int stitched; // Indique si les deux zones sont liées par un point de couture
} ZoneLink;

// Structure principale du jeu représentant la grille
typedef struct {
    int size;               // Taille de la grille
    Cell grid[MAX_SIZE][MAX_SIZE];  // Grille de cellules
    Block rowBlocks[MAX_SIZE];      // Nombre de trous dans chaque ligne
    Block colBlocks[MAX_SIZE];      // Nombre de trous dans chaque colonne
    ZoneLink zoneLinks[MAX_SIZE * MAX_SIZE]; // Liens entre les zones
    int numZoneLinks;        // Nombre de liens entre les zones
} GameGrid;

void red() {
    printf("\033[1;31m");
}

void yellow() {
    printf("\033[1;33m");
}

void blue() {
    printf("\033[1;34m");
}

void magenta() {
    printf("\033[1;35m");
}

void green() {
    printf("\033[1;32m");
}

void cyan() {
    printf("\033[1;36m");
}

void grey() {
    printf("\033[1;30m");
}

void white() {
    printf("\033[1;37m");
}

void reset() {
    printf("\033[0m");
}



void initializeGrid(GameGrid* game, char* array[MAX_SIZE + 1][MAX_SIZE + 1]) {
    // Parcours de chaque cellule dans la grille
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            // Initialisation du point de couture vide
            game->grid[i][j].stitch = ' ';
            // Attribution de la zone à partir de la valeur dans le tableau importé
            game->grid[i][j].zone = atoi(array[i][j]);
        }
    }
    // Attribution de la taille de la grille
    game->size = MAX_SIZE;
    // Initialisation du nombre de liens entre les zones à zéro
    game->numZoneLinks = 0;
}



void displayGrid(GameGrid* game, char* array[MAX_SIZE + 1][MAX_SIZE + 1]) {
    // Affichage des titres des colonnes
    printf("      ");
    for (int j = 1; j <= game->size; ++j) {
        printf("%c       ", array[0][j][0]);
    }
    printf("\n  +");
    // Affichage des lignes horizontales entre les lignes
    for (int j = 1; j <= MAX_SIZE; ++j) {
        printf("-------+");
    }
    printf("\n");

    // Parcours de chaque ligne de la grille
    for (int i = 0; i < game->size; ++i) {
        // Affichage du titre de la ligne
        printf("%c |", array[i + 1][0][0]);

        // Parcours de chaque colonne de la grille
        for (int j = 0; j < game->size; ++j) {
            // Affichage du point de couture dans la cellule actuelle
            printf("   %c   |", game->grid[i][j].stitch);
        }

        // Affichage de la ligne horizontale entre les lignes
        printf("\n  +");
        for (int j = 1; j <= MAX_SIZE; ++j) {
            printf("-------+");
        }
        printf("\n");
    }
}



void displayImportedGrid(char* array[MAX_SIZE + 1][MAX_SIZE + 1]) {
    // Affichage des titres des colonnes
    printf("      ");
    for (int j = 1; j <= MAX_SIZE; ++j) {
        printf("%c       ", array[0][j][0]);
    }
    printf("\n  +");
    // Affichage des lignes horizontales entre les lignes
    for (int j = 1; j <= MAX_SIZE; ++j) {
        printf("-------+");
    }
    printf("\n");

    // Parcours de chaque ligne de la grille
    for (int i = 1; i <= MAX_SIZE; ++i) {
        // Affichage du titre de la ligne
        printf("%c |", array[i][0][0]);

        // Parcours de chaque colonne de la grille
        for (int j = 1; j <= MAX_SIZE; ++j) {
            // Attribution de couleurs en fonction des valeurs dans la grille
            if (array[i][j][0] == '1') yellow();
            if (array[i][j][0] == '2') red();
            if (array[i][j][0] == '3') blue();
            if (array[i][j][0] == '4') magenta();
            if (array[i][j][0] == '5') green();
            if (array[i][j][0] == '6') cyan();
            if (array[i][j][0] == '7') grey();
            if (array[i][j][0] == '8') white();
            if (array[i][j][0] == '9') yellow();
            if (array[i][j][0] == '0') red();

            // Affichage du contenu de la cellule avec la couleur attribuée
            printf("   %c   ", array[i][j][0]);
            reset();
            printf("|");
        }

        // Affichage de la ligne horizontale entre les lignes de la grille
        printf("\n  +");
        for (int j = 1; j <= MAX_SIZE; ++j) {
            printf("-------+");
        }
        printf("\n");
    }
}


void removeLink(GameGrid* game, int zone1, int zone2) {
    // Parcours des liens entre les zones
    for (int i = 0; i < game->numZoneLinks; ++i) {
        // Vérification si le lien correspond aux zones spécifiées
        if ((game->zoneLinks[i].zone1 == zone1 && game->zoneLinks[i].zone2 == zone2) ||
            (game->zoneLinks[i].zone1 == zone2 && game->zoneLinks[i].zone2 == zone1)) {
            
            // Suppression du lien en décalant les éléments suivants dans le tableau
            for (int j = i; j < game->numZoneLinks - 1; ++j) {
                game->zoneLinks[j] = game->zoneLinks[j + 1];
            }
            // Réduction du nombre total de liens
            game->numZoneLinks--;
            // Sortie de la boucle une fois le lien supprimé
            break;
        }
    }
}

void addStitch(GameGrid* game, int row1, int col1, int row2, int col2,   char* array[MAX_SIZE + 1][MAX_SIZE + 1]) {
    // Vérifie si les points spécifiés sont adjacents horizontalement
    if (row1 == row2 && col1 + 1 == col2) {
        game->grid[row1 - 1][col1 - 1].stitch = '>'; // Point de couture à gauche du premier point
        game->grid[row2 - 1][col2 - 1].stitch = '<'; // Point de couture à droite du deuxième point
    }

    // Vérifie si les points spécifiés sont adjacents verticalement
    if (row1 + 1 == row2 && col1 == col2) {
        game->grid[row1 - 1][col1 - 1].stitch = 'v'; // Point de couture en haut du premier point
        game->grid[row2 - 1][col2 - 1].stitch = '^'; // Point de couture en bas du deuxième point
    }

    // Obtient les identifiants de zone des deux points
    int zone1 = game->grid[row1 - 1][col1 - 1].zone;
    int zone2 = game->grid[row2 - 1][col2 - 1].zone;

    // Ajoute un nouveau lien entre les zones dans la structure de données GameGrid
    game->zoneLinks[game->numZoneLinks].zone1 = zone1;
    game->zoneLinks[game->numZoneLinks].zone2 = zone2;
    game->zoneLinks[game->numZoneLinks].stitched = 1; // Indique que les zones sont reliées par une stitch
    game->numZoneLinks++; // Incrémente le nombre total de liens dans la grille
}

void removeStitch(GameGrid* game, int row, int col, char* array[MAX_SIZE + 1][MAX_SIZE + 1]) {
    int zone1;
    int zone2;

    // Vérifie si les coordonnées spécifiées sont valides
    if (row >= 1 && row <= game->size && col >= 1 && col <= game->size) {

        // Supprime la couture horizontale (>)
        if (game->grid[row - 1][col - 1].stitch == '>') {
            game->grid[row - 1][col - 1].stitch = ' ';
            game->grid[row - 1][col].stitch = ' ';
            zone1 = game->grid[row - 1][col - 1].zone;
            zone2 = game->grid[row - 1][col].zone;
            removeLink(game, zone1, zone2); // Appelle la fonction pour supprimer le lien entre les zones
        }
        // Supprime la couture verticale (v)
        else if (game->grid[row - 1][col - 1].stitch == 'v') {
            game->grid[row - 1][col - 1].stitch = ' ';
            game->grid[row][col - 1].stitch = ' ';
            zone1 = game->grid[row - 1][col - 1].zone;
            zone2 = game->grid[row][col - 1].zone;
            removeLink(game, zone1, zone2); // Appelle la fonction pour supprimer le lien entre les zones
        }
        // Supprime la couture horizontale (<)
        else if (game->grid[row - 1][col - 1].stitch == '<') {
            game->grid[row - 1][col - 2].stitch = ' ';
            game->grid[row - 1][col - 1].stitch = ' ';
            zone1 = game->grid[row - 1][col - 1].zone;
            zone2 = game->grid[row - 1][col - 2].zone;
            removeLink(game, zone1, zone2); // Appelle la fonction pour supprimer le lien entre les zones
        }
        // Supprime la couture verticale (^)
        else if (game->grid[row - 1][col - 1].stitch == '^') {
            game->grid[row - 1][col - 1].stitch = ' ';
            game->grid[row - 2][col - 1].stitch = ' ';
            zone1 = game->grid[row - 1][col - 1].zone;
            zone2 = game->grid[row - 2][col - 1].zone;
            removeLink(game, zone1, zone2); // Appelle la fonction pour supprimer le lien entre les zones
        }
    }
}

int isValid(GameGrid* game, int row1, int col1, int row2, int col2, char* array[MAX_SIZE + 1][MAX_SIZE + 1]) {
    // Vérifie si les coordonnées spécifiées sont valides
    if (row1 >= 1 && row1 <= game->size && col1 >= 1 && col1 <= game->size &&
        row2 >= 1 && row2 <= game->size && col2 >= 1 && col2 <= game->size) {
        
        // Vérifie si les emplacements spécifiés n'ont pas déjà une couture
        if (game->grid[row1 - 1][col1 - 1].stitch == ' ' && game->grid[row2 - 1][col2 - 1].stitch == ' ') {

            // Vérifie si les zones sont valides et différentes
            if ((game->grid[row1 - 1][col1 - 1].zone && game->grid[row2 - 1][col2 - 1].zone) && (array[row1][col1][0] != array[row2][col2][0])) {
                
                // Vérifie s'il y a déjà un lien entre les deux zones
                int zone1 = game->grid[row1 - 1][col1 - 1].zone;
                int zone2 = game->grid[row2 - 1][col2 - 1].zone;
                for (int i = 0; i < game->numZoneLinks; ++i) {
                    if ((game->zoneLinks[i].zone1 == zone1 && game->zoneLinks[i].zone2 == zone2) ||
                        (game->zoneLinks[i].zone1 == zone2 && game->zoneLinks[i].zone2 == zone1)) {
                        return 1; // Il existe déjà un lien entre les deux zones
                    }
                }
                return 1; // Pas de lien existant, la couture est valide
            }
        }
    }
    return 0; // Les conditions ci-dessus n'ont pas été remplies, la couture n'est pas valide
}

int checkRow(GameGrid* game, int rowIndex, char* array[MAX_SIZE + 1][MAX_SIZE + 1]) {
    // Récupère le nombre attendu de coutures pour la ligne spécifiée
    int sr = atoi(array[0][rowIndex + 1]);
    
    // Initialise le compteur de coutures
    int stitchCount = 0;

    // Parcourt la ligne spécifiée dans la grille de jeu
    for (int j = 0; j < game->size; j++) {
        // Vérifie si la case de la grille a une couture
        if (game->grid[j][rowIndex].stitch != ' ') {
            stitchCount++;
        }
    }

    // Compare le nombre attendu de coutures avec le nombre réel de coutures dans la ligne
    return sr == stitchCount;
}

int checkColumn(GameGrid* game, int colIndex, char* array[MAX_SIZE + 1][MAX_SIZE + 1]) {
    // Récupère le nombre attendu de coutures pour la colonne spécifiée
    int sr = atoi(array[colIndex + 1][0]);
    
    // Initialise le compteur de coutures
    int stitchCount = 0;

    // Parcourt la colonne spécifiée dans la grille de jeu
    for (int i = 0; i < game->size; i++) {
        // Vérifie si la case de la grille a une couture
        if (game->grid[colIndex][i].stitch != ' ') {
            stitchCount++;
        }
    }

    // Compare le nombre attendu de coutures avec le nombre réel de coutures dans la colonne
    return sr == stitchCount;
}

int checkSolution(GameGrid* game, char* array[MAX_SIZE + 1][MAX_SIZE + 1], int i) {
    int check;
    
    // Vérifie si i est inférieur à la taille de la grille
    if (i < game->size) {
        // Vérifie si la ligne i et la colonne i satisfont les contraintes
        check = checkRow(game, i, array) && checkColumn(game, i, array);

        // Appelle récursivement la fonction pour la ligne suivante (i+1)
        return check && checkSolution(game, array, i + 1);
    }
    // Si i est égal à la taille de la grille, retourne 1 (true)
    return 1;
}



int solve(GameGrid* game, int row, int col, char* array[MAX_SIZE + 1][MAX_SIZE + 1]) {
    // Vérifie si la solution actuelle est valide
    if (checkSolution(game, array, 0)) {
        return 1;
    }

    // Vérifie si on a parcouru toutes les lignes
    if (row > game->size) {
        // Vérifie si la solution après avoir parcouru toutes les lignes est valide
        return checkSolution(game, array, 0);
    }

    // Vérifie si on a parcouru toutes les colonnes dans la ligne actuelle
    if (col > game->size) {
        // Passe à la ligne suivante
        return solve(game, row + 1, 0, array);
    }

    // Essaie d'ajouter un point de suture horizontal entre la colonne col et col+1
    if (isValid(game, row, col, row, col + 1, array)) {
        addStitch(game, row, col, row, col + 1, array);
        
        // Appelle récursivement la fonction pour la colonne suivante (col + 1)
        if (solve(game, row, col + 1, array)) {
            return 1;
        }

        // Si la solution n'est pas trouvée, annule l'ajout du point de suture et retire le lien
        game->grid[row - 1][col - 1].stitch = ' ';
        game->grid[row - 1][col].stitch = ' ';
        removeLink(game, game->grid[row - 1][col - 1].zone, game->grid[row - 1][col].zone);
    }

    // Essaie d'ajouter un point de suture vertical entre la ligne row et row+1
    if (isValid(game, row, col, row + 1, col, array)) {
        addStitch(game, row, col, row + 1, col, array);
        
        // Appelle récursivement la fonction pour la colonne suivante (col + 1)
        if (solve(game, row, col + 1, array)) {
            return 1;
        }

        // Si la solution n'est pas trouvée, annule l'ajout du point de suture et retire le lien
        game->grid[row - 1][col - 1].stitch = ' ';
        game->grid[row][col - 1].stitch = ' ';
        removeLink(game, game->grid[row - 1][col - 1].zone, game->grid[row][col - 1].zone);
    }

    // Si aucune solution n'est trouvée pour la configuration actuelle, passe à la colonne suivante
    return solve(game, row, col + 1, array);
}

void readGridFromFile(char* filename, char* array[MAX_SIZE + 1][MAX_SIZE + 1]) {
    // Ouvre le fichier en mode lecture
    FILE* file = fopen(filename, "r+");
    char character;

    // Vérifie si l'ouverture du fichier a réussi
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Parcourt le tableau et lit chaque caractère du fichier
    for (int i = 0; i <= MAX_SIZE; ++i) {
        for (int j = 0; j <= MAX_SIZE; ++j) {
            // Lit un caractère du fichier
            fscanf(file, " %c", &character);

            // Alloue de la mémoire pour stocker le caractère dans le tableau
            array[i][j] = malloc(4 * sizeof(char));
            
            // Stocke le caractère dans le tableau
            array[i][j][0] = character;
        }
    }

    // Ferme le fichier après la lecture
    fclose(file);
}

void readSizeFromFile(char* filename, int* size) {
    // Ouvre le fichier en mode lecture
    FILE* file = fopen(filename, "r");

    // Vérifie si l'ouverture du fichier a réussi
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Lit la taille depuis le fichier
    fscanf(file, " %d", size);

    // Ferme le fichier après la lecture
    fclose(file);
}

void Player(GameGrid* gamePlayer, GameGrid* game, char* array[MAX_SIZE + 1][MAX_SIZE + 1]){
    int row1, col1, row2, col2;
    int choix;
    int stop = checkSolution(game,array,0);
    
    // Boucle tant que la solution n'est pas atteinte
    while(!stop){
        printf("\n");
        printf("Que voulez-vous faire ? \n");
        printf("1- Placer\n");
        printf("2- Retirer\n");
        printf("3- Solution\n");
        printf("Votre choix : ");
        
        // Demande à l'utilisateur de faire un choix
        scanf("%d", &choix);
        printf("\n");

        // Affiche la grille importée et la grille du joueur
        displayImportedGrid(array);
        displayGrid(gamePlayer, array);

        // Vérifie si le choix de l'utilisateur est valide
        if (choix < 1 || choix > 3) {
            fprintf(stderr, "Choix invalide.\n");
            return;
        }

        // Si le choix est 1, le joueur souhaite placer une liaison
        if (choix == 1){
            printf("\n");
            printf("Veuillez entrer les coordonnées du premier point à ajouter (ligne colonne) : ");
            scanf("%d %d", &row1, &col1);
            printf("\n");
            printf("Veuillez entrer les coordonnées du deuxième point à ajouter (ligne colonne) : ");
            scanf("%d %d", &row2, &col2);
            printf("\n");

            // Ajoute la liaison et affiche les grilles mises à jour
            addStitch(gamePlayer, row1, col1, row2, col2, array);
            displayImportedGrid(array);
            displayGrid(gamePlayer, array);
        }

        // Si le choix est 2, le joueur souhaite retirer une liaison
        if (choix == 2){
            printf("\n");
            printf("Veuillez entrer les coordonnées du premier point à retirer (ligne colonne) : ");
            scanf("%d %d", &row1, &col1);
            printf("\n");

            // Retire la liaison et affiche les grilles mises à jour
            removeStitch(gamePlayer, row1, col1, array);
            displayImportedGrid(array);
            displayGrid(gamePlayer, array);
        }

        // Si le choix est 3, le joueur souhaite voir la solution
        if (choix == 3){
            // Résout la grille d'origine, affiche la grille mise à jour et retourne
            solve(game, 0, 0, array);
            stop = checkSolution(game, array, 0);
            displayGrid(game, array);
            return;
        }

        // Vérifie si la solution du joueur a été atteinte
        stop = checkSolution(gamePlayer, array, 0);
    }
}

void displayMenu() {
    printf("Choisissez une grille :\n");

    printf("1- grille 7x7\n");
    printf("2- grille 7x7\n");
    printf("3- grille 7x7\n");
    printf("4- grille 7x7 - difficile \n");
    printf("5- quitter\n");
}

int main() {
    // Variables pour mesurer le temps d'exécution
    clock_t start_t, stop_t;
    double d_time;

    // Boucle principale pour l'interface utilisateur
    while (1) {
        // Déclaration de deux structures GameGrid pour le jeu et le joueur
        GameGrid game;
        GameGrid gamePlayer;

        // Noms des fichiers contenant les grilles
        char* grilles[MAX_GRILLES] = {"7x7-1.txt", "7x7-2.txt", "7x7-3.txt", "7x7-4.txt"};

        int choix;

        // Affichage du menu principal
        printf("\n\n");
        displayMenu();
        printf("Entrez le numéro de la grille que vous souhaitez utiliser : ");
        scanf("%d", &choix);
        printf("\n\n");

        // Vérification de la validité du choix
        if (choix < 1 || choix > MAX_GRILLES + 1) {
            fprintf(stderr, "Choix invalide.\n");
            return EXIT_FAILURE;
        }

        // Sortie du programme si l'utilisateur choisit l'option de quitter
        if (choix == MAX_GRILLES + 1) {
            return 0;
        }

        // Tableau pour stocker le contenu de la grille à partir du fichier
        char* aText[MAX_SIZE + 1][MAX_SIZE + 1];
        readGridFromFile(grilles[choix - 1], aText);

        // Initialisation des grilles du jeu et du joueur
        initializeGrid(&game, aText);
        initializeGrid(&gamePlayer, aText);
        displayImportedGrid(aText);

        // Menu pour choisir entre jouer, afficher la solution, ou revenir en arrière
        printf("\n\n");
        printf("Solution ? \n");
        printf("1- Jouer\n");
        printf("2- Solution\n");
        printf("3- Retour\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        printf("\n\n");

        // Vérification de la validité du choix
        if (choix < 1 || choix > 3) {
            fprintf(stderr, "Choix invalide.\n");
            return EXIT_FAILURE;
        }

        // Exécution de l'option choisie
        if (choix == 1) {
            Player(&gamePlayer, &game, aText);
        }

        if (choix == 2) {
            // Mesure du temps d'exécution pour la résolution automatique
            start_t = clock();
            solve(&game, 0, 0, aText);
            stop_t = clock();
            d_time = (double)(stop_t - start_t) / CLOCKS_PER_SEC;
            displayGrid(&game, aText);
            printf("Temps d'execution : %f secondes\n", d_time);
        }

        // Libération de la mémoire allouée pour le tableau aText
        for (int i = 0; i <= MAX_SIZE; ++i) {
            for (int j = 0; j <= MAX_SIZE; ++j) {
                free(aText[i][j]);
            }
        }
    }
    return 0;
}
