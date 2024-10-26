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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define TAILLE_SERPENT 20
#define DELAY 200000 // Delay in microseconds
#define MAX_SIZE_XY 40
#define MIN_SIZE_XY 1

void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int lesX[], int lesY[]);
void gotoXY(int x, int y);
int kbhit();

int main() {
    system("clear");

    int lesX[TAILLE_SERPENT] = {0};
    int lesY[TAILLE_SERPENT] = {0};

    // Demander les coordonnées initiales de la tête du serpent
    int x_initial = -1, y_initial = -1;
    printf("Entrez les coordonnées initiales de la tête du serpent (x y) (%d <= x, y <= %d): ", MIN_SIZE_XY,MAX_SIZE_XY);
    while (scanf("%d %d", &x_initial, &y_initial) != 2 || x_initial < MIN_SIZE_XY || x_initial > MAX_SIZE_XY || y_initial < MIN_SIZE_XY || y_initial > MAX_SIZE_XY) {
        printf("Coordonnées invalides. Veuillez réessayer (%d <= x, y <= %d): ", MIN_SIZE_XY,MAX_SIZE_XY);
        while (getchar() != '\n'); // Clear l'enter buffer
    }
    
    system("clear");

    for (int i = 0; i < TAILLE_SERPENT; i++) {
        lesX[i] = x_initial - i;
        lesY[i] = y_initial;
    }

    while (1) {
        if (kbhit()) {
            unsigned char ch = (unsigned char)getchar();
            if (ch == 'a') {
                system("clear");
                break;
            }
        }

        effacer(lesX[0], lesY[0]);
        for (int i = 0; i < TAILLE_SERPENT - 1; i++) {
            lesX[i] = lesX[i + 1];
            lesY[i] = lesY[i + 1];
        }
        lesX[TAILLE_SERPENT - 1]++;
        dessinerSerpent(lesX, lesY);

        usleep(DELAY);
    }

    return EXIT_SUCCESS;
}

void afficher(int x, int y, char c) {
    if (x >= MIN_SIZE_XY && y >= MIN_SIZE_XY) {
        gotoXY(x, y);
        putchar(c);
    }
}

void effacer(int x, int y) {
    if (x >= MIN_SIZE_XY && y >= MIN_SIZE_XY) {
        gotoXY(x, y);
        putchar(' ');
    }
}

void dessinerSerpent(int lesX[], int lesY[]) {
    for (int i = TAILLE_SERPENT - 1; i >= 0; i--) {
        if (i == TAILLE_SERPENT - 1) {
            afficher(lesX[i], lesY[i], 'O'); // Tête du serpent
        } else {
            afficher(lesX[i], lesY[i], 'x'); // Corps du serpent
        }
    }
}

void gotoXY(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

int kbhit(){
    // la fonction retourne :
    // 1 si un caractere est present
    // 0 si pas de caractere present
    
    int unCaractere=0;
    struct termios oldt, newt;
    int ch;
    int oldf;

    // mettre le terminal en mode non bloquant
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= (tcflag_t)~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
    ch = getchar();

    // restaurer le mode du terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
 
    if(ch != EOF){
        ungetc(ch, stdin);
        unCaractere=1;
    } 
    return unCaractere;
}
