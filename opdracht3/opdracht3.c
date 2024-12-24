/*
   Naam: Mats Vink
   UvAnetID: 15874648
   Studie: BSc Informatica
 * opdracht3.c:
 * − In deze opdracht spelen we het spel "the game of life". Hierin maken we een speelveld met
 * HOOGTE en BREEDTE. Daarin is elke cel dood behalve een paar vaarvan we zelf het patroon er
 * in hebben gezet. Als een levende cel meer dan 3 buren heeft sterft hij aan overpopulatie.
 * En als een levende cel minder dan 2 buren heeft sterft hij aan eenzaamheid. EN als een dode
 * cel precies 3 levende celen naast zich heeft dan wordt het een levende cel.
 * Zo kan het spel ondeindig lang door gaan.
 * 
 */


#include <stdio.h>
#include <stdlib.h>

#define MAX_OPDRACHT_LEN 100
#define BREEDTE 79
#define HOOGTE 40
#define LEVEND 1
#define DOOD 0

/*
 * Deze funtie initialiseerterafaef het grid met een bepaald patroon.
 * Input:
 * - grid: een 2D array met afmetingen HOOGTE x BREEDTE.
 * - patroon: een integer die aangeeft welk patroon moet worden ingesteld.
 * 
 * Output: Geen directe return waarde.
 * 
 * Side effects: Wijzigt het grid op basis van het gekozen patroon.
 */
void initialiseer_grid(int grid[HOOGTE][BREEDTE], int patroon) {
    // Zet elke cel in het grid op "DOOD"
    for (int i = 0; i < HOOGTE; i++) {
        for (int j = 0; j < BREEDTE; j++) {
            grid[i][j] = DOOD;
        }
    }

    // Stel de patronen in op basis van de waarde van patroon
    if (patroon == 1) {
        grid[1][2] = LEVEND;
        grid[2][2] = LEVEND;
        grid[2][3] = LEVEND;
        grid[3][3] = LEVEND;
        grid[3][2] = LEVEND;
        grid[4][3] = LEVEND;
    } else if (patroon == 2) {
        grid[1][2] = LEVEND;
        grid[3][1] = LEVEND;
        grid[3][2] = LEVEND;
        grid[3][3] = LEVEND;
        grid[2][3] = LEVEND;
    } else if (patroon == 3) {
        grid[5][1] = LEVEND;
        grid[6][1] = LEVEND;
        grid[5][2] = LEVEND;
        grid[6][2] = LEVEND;
        grid[5][11] = LEVEND;
        grid[6][11] = LEVEND;
        grid[7][11] = LEVEND;
        grid[8][12] = LEVEND;
        grid[9][13] = LEVEND;
        grid[9][14] = LEVEND;
        grid[4][12] = LEVEND;
        grid[3][13] = LEVEND;
        grid[3][14] = LEVEND;
        grid[4][16] = LEVEND;
        grid[5][17] = LEVEND;
        grid[6][17] = LEVEND;
        grid[6][18] = LEVEND;
        grid[6][15] = LEVEND;
        grid[8][16] = LEVEND;
        grid[7][17] = LEVEND;
        grid[5][21] = LEVEND;
        grid[5][22] = LEVEND;
        grid[4][21] = LEVEND;
        grid[4][22] = LEVEND;
        grid[3][21] = LEVEND;
        grid[3][22] = LEVEND;
        grid[6][23] = LEVEND;
        grid[6][25] = LEVEND;
        grid[7][25] = LEVEND;
        grid[2][23] = LEVEND;
        grid[2][25] = LEVEND;
        grid[1][25] = LEVEND;
        grid[4][35] = LEVEND;
        grid[4][36] = LEVEND;
        grid[3][35] = LEVEND;
        grid[3][36] = LEVEND;
    }
}


/*
 * Functie: Toon het huidige grid en de generatie op het scherm.
 * 
 * Input:
 * - grid: een 2D array met de huidige toestand van het grid.
 * - generatie: een integer die de huidige generatie aangeeft.
 * 
 * Output: Geen directe return waarde.
 * 
 * Side effects: Print het grid en de generatie naar de console.
 */
void toon(const int grid[HOOGTE][BREEDTE], int generatie) {
    printf("\nGeneratie %d:\n", generatie);

    // Loop door het grid en print elke cel
    for (int i = 0; i < HOOGTE; i++) {
        for (int j = 0; j < BREEDTE; j++) {
            if (grid[i][j] == LEVEND) {
                printf("*");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

/*
 * Functie: Kopieer de inhoud van het bron_grid naar het doel_grid.
 * 
 * Input:
 * - bron_grid: een 2D array die de bron is.
 * - doel_grid: een 2D array die het doel is.
 * 
 * Output: Geen directe return waarde.
 * 
 * Side effects: Wijzigt doel_grid zodat het identiek is aan bron_grid.
 */
void kopie(const int bron_grid[HOOGTE][BREEDTE], int doel_grid[HOOGTE][BREEDTE]) {
    // Kopieer elke cel van bron_grid naar doel_grid
    for (int i = 0; i < HOOGTE; i++) {
        for (int j = 0; j < BREEDTE; j++) {
            doel_grid[i][j] = bron_grid[i][j];
        }
    }
}

/*
 * Functie: Tel het aantal levende buren van een specifieke cel in het grid.
 * 
 * Input:
 * - grid: een 2D array met de huidige toestand van het grid.
 * - x, y: de coördinaten van de cel waarvan we de buren tellen.
 * 
 * Output: Return het aantal levende buren van de cel.
 */
int buren(const int grid[HOOGTE][BREEDTE], int x, int y) {
    int aantal_buren = 0;

    // Controleer de 8 omringende cellen
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            // Sla de cel zelf over
            if (dx == 0 && dy == 0) {
                continue;
            }    

            int buur_x = x + dx;
            int buur_y = y + dy;

            // Controleer of de buur binnen het grid valt
            if (buur_x >= 0 && buur_x < BREEDTE && buur_y >= 0 && buur_y < HOOGTE) {
                if (grid[buur_y][buur_x] == LEVEND) {
                    aantal_buren++;
                }
            }
        }
    }

    return aantal_buren;
}

/*
 * Functie: Bereken de volgende generatie van het grid.
 * 
 * Input:
 * - oud_grid: een 2D array met de huidige toestand van het grid.
 * - nieuw_grid: een 2D array waar de nieuwe generatie in wordt opgeslagen.
 * 
 * Output: Geen directe return waarde.
 * 
 * Side effects: Wijzigt nieuw_grid op basis van de regels van het spel.
 */
void stap(const int oud_grid[HOOGTE][BREEDTE], int nieuw_grid[HOOGTE][BREEDTE]) {
    // Loop door elke cel van het grid
    for (int i = 0; i < HOOGTE; i++) {
        for (int j = 0; j < BREEDTE; j++) {
            int levende_buren = buren(oud_grid, j, i);

            // Toepassing van de regels van het spel
            if (oud_grid[i][j] == LEVEND) {
                if (levende_buren == 2 || levende_buren == 3) {
                    nieuw_grid[i][j] = LEVEND;
                } else {
                    nieuw_grid[i][j] = DOOD;
                }
            } else {
                if (levende_buren == 3) {
                    nieuw_grid[i][j] = LEVEND;
                } else {
                    nieuw_grid[i][j] = DOOD;
                }
            }
        }
    }
}

/* Hoofdprogramma.

   Het initialiseert en toont eerst de begintoestand van het grid. (Je kunt
   eventueel als je het programma opstart opgeven welk beginpatroon moet
   worden gebruikt.)

   Vervolgens kun je telkens opgeven hoeveel generaties de computer moet
   doorberekenen, door het aantal generaties in te typen dat er moet worden
   doorberekend.

   Als je op Enter drukt zonder een getal in te vullen zal de computer de
   eerstvolgende generatie tonen.

   Druk op Ctrl+D om het programma te beeindigen.
*/
#ifndef COMPILE_MAIN
int main(int argc, char *argv[]) {
    char buffer[MAX_OPDRACHT_LEN];
    int grid[HOOGTE][BREEDTE];
    int nieuw_grid[HOOGTE][BREEDTE];

    int patroon = argc == 1 ? 1 : atoi(argv[1]);
    initialiseer_grid(grid, patroon);
    int generatie = 0;

    printf("Conway's Game of life\n\n");
    toon(grid, generatie);
    while (1) {
        printf("\nGeef een opdracht:\n");
        char *res = fgets(buffer, MAX_OPDRACHT_LEN, stdin);
        if (res == NULL) {
            // Er is waarschijnlijk op Ctrl+D gedrukt.
            printf("Programma wordt beëindigd.\n");
            break;
        }
        int aantal_generaties = buffer[0] == '\n' ? 1 : atoi(buffer);
        for (int i = 0; i < aantal_generaties; i++) {
            stap(grid, nieuw_grid);
            kopie(nieuw_grid, grid);
        }
        generatie += aantal_generaties;
        toon(grid, generatie);
    }
    return 0;
}
#endif
