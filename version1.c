/**
 * @file version1.c
 * @brief Programme Snake
 * 
 * Ce fichier contient l'implémentation d'un jeu de serpent simple en C.
 * Le serpent se déplace automatiquement et le programme se termine lorsque
 * l'utilisateur appuie sur la touche 'A' ou 'a'.
 * 
 * @author Elouan Passereau
 * @date 24/10/2024
 */

/**
 * @brief Affiche un caractère à une position donnée dans le terminal.
 * 
 * @param x La position en x (colonne).
 * @param y La position en y (ligne).
 * @param c Le caractère à afficher.
 */
void afficher(int x, int y, char c);

/**
 * @brief Efface un caractère à une position donnée dans le terminal.
 * 
 * @param x La position en x (colonne).
 * @param y La position en y (ligne).
 */
void effacer(int x, int y);

/**
 * @brief Dessine le serpent dans le terminal.
 * 
 * @param lesX Tableau des positions en x des segments du serpent.
 * @param lesY Tableau des positions en y des segments du serpent.
 */
void dessinerSerpent(int lesX[], int lesY[]);

/**
 * @brief Initialise les positions du serpent.
 * 
 * @param lesX Tableau des positions en x des segments du serpent.
 * @param lesY Tableau des positions en y des segments du serpent.
 */
void initialiserSerpent(int lesX[], int lesY[]);

/**
 * @brief Déplace le serpent d'une position vers la droite.
 * 
 * @param lesX Tableau des positions en x des segments du serpent.
 * @param lesY Tableau des positions en y des segments du serpent.
 */
void deplacerSerpent(int lesX[], int lesY[]);

/**
 * @brief Configure le terminal pour désactiver l'affichage canonique et l'écho.
 */
void configTerminal();

/**
 * @brief Vérifie si la touche 'A' ou 'a' a été pressée.
 * 
 * @return 1 si la touche 'A' ou 'a' a été pressée, 0 sinon.
 */
int checkForA();

/**
 * @brief Point d'entrée principal du programme.
 * 
 * Initialise le serpent, configure le terminal et lance la boucle principale
 * du jeu où le serpent se déplace automatiquement jusqu'à ce que l'utilisateur
 * appuie sur la touche 'A' ou 'a'.
 * 
 * @return 0 si le programme se termine correctement.
 */
int main();

/* Bibliotèques */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
/* Fin Bibliotèques */

#define TAILLE_SERPENT 10

void afficher(int x, int y, char c) {
    printf("\033[%d;%dH%c", y, x, c);
    fflush(stdout);
}
void effacer(int x, int y) {
    afficher(x, y, ' ');
}
void dessinerSerpent(int lesX[], int lesY[]) {
    for (int i = 0; i < TAILLE_SERPENT; i++) {
        if (i == 0) {
            afficher(lesX[i], lesY[i], 'O'); // Tête du serpent
        } else {
            afficher(lesX[i], lesY[i], 'X'); // Corps du serpent
        }
    }
}
void initialiserSerpent(int lesX[], int lesY[]) {
    for (int i = 0; i < TAILLE_SERPENT; i++) {
        lesX[i] = 10 - i;
        lesY[i] = 10;
    }
}
void deplacerSerpent(int lesX[], int lesY[]) {
    for (int i = TAILLE_SERPENT - 1; i > 0; i--) {
        lesX[i] = lesX[i - 1];
        lesY[i] = lesY[i - 1];
    }
    lesX[0]++;
}
void configTerminal() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
int checkForA() {
    struct termios oldt, newt;
    int charactere;
    int old_flags;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    old_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, old_flags | O_NONBLOCK);

    charactere = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, old_flags);

    if(charactere == 'A' || charactere == 'a') {
        return 1;
    }
    return 0;
}
int main() {
    int lesX[TAILLE_SERPENT], lesY[TAILLE_SERPENT];
    initialiserSerpent(lesX, lesY);
    configTerminal();
    
    while (1) {
        if (checkForA()) {
            printf("\n");
            break;
        } for (int i = 0; i < TAILLE_SERPENT; i++) {
            effacer(lesX[i], lesY[i]);
        }
        deplacerSerpent(lesX, lesY);
        dessinerSerpent(lesX, lesY);
        usleep(200000); // Pause de 200ms
    }
    return 0;
}