/*
 * DNAMatch
 * -------------------------
 *
 * Auteur  : Mats Vink
 * Datum   : 30 september 2024
 * Versie  : 1.0
 *
 * inhoud:
 * Dit programma beheert een DNA-sequentie database en voert verschillende operaties uit, 
 * zoals het toevoegen van sequenties, vergelijken van sequenties, ophalen van overeenkomsten,
 * lezen van bestanden en het berekenen van verschillen met behulp van het Levenshtein-algoritme.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DB_SIZE 100
#define MAX_INPUT_SIZE 100

char **DATABASE;
int DB_SIZE = 0;

/* 
 * Functie: Deze functie geeft het geheugen vrij dat is toegewezen voor het Levenshtein-raster.
 * 
 * grid: Een pointer naar de array van rijen die vrijgegeven moeten worden.
 * rows: Het aantal rijen in het raster.
 */
void free_levenshtein_grid(int **grid, int rows) {
    for (int i = 0; i < rows; i++) {
        free(grid[i]);
    }
    free(grid);
}

/* 
 * Functie: implementeert het Levenshtein-afstandsalgoritme om de minimale bewerkingskosten te
 * berekenentussen twee strings.
 * 
 * s1: De eerste string.
 * s2: De tweede string.
 * 
 * Return: Een 2D-array (raster) van afstanden.
 * Bijeffecten: Geheugen wordt toegewezen voor het raster.
 */
int **levenshtein(char *s1, char *s2) {
    int len1 = (int) strlen(s1);
    int len2 = (int) strlen(s2);

    // Geheugen toewijzen voor het raster
    int **grid = malloc(((size_t)len1 + 1) * sizeof(int *));
    for (int i = 0; i <= len1; i++) {
        grid[i] = malloc(((size_t)len2 + 1) * sizeof(int));
    }

    // Raster initialiseren met afstanden
    for (int i = 0; i <= len1; i++) {
        grid[i][0] = i;
    }
    for (int j = 0; j <= len2; j++) {
        grid[0][j] = j;
    }

    // Raster vullen met het Levenshtein-algoritme
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            grid[i][j] = (grid[i - 1][j] + 1 < grid[i][j - 1] + 1) ? grid[i - 1][j] + 1 : grid[i][j - 1] + 1;
            grid[i][j] = (grid[i][j] < grid[i - 1][j - 1] + cost) ? grid[i][j] : grid[i - 1][j - 1] + cost;
        }
    }
    return grid;
}

/* 
 * Functie: Vergelijkt twee strings met behulp van de Levenshtein-afstand en print het
 * verschil en de rasterwaardes.
 * s1: De eerste string.
 * s2: De tweede string.
 */
void compare(char *s1, char *s2) {
    int len1 = (int) strlen(s1);
    int len2 = (int) strlen(s2);

    // Levenshtein-afstand berekenen
    int **grid = levenshtein(s1, s2);

    // Raster printen
    for (int i = 0; i <= len1; i++) {
        for (int j = 0; j <= len2; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
    printf("Difference = %d\n", grid[len1][len2]);

    // Geheugen vrijgeven
    free_levenshtein_grid(grid, len1 + 1);
}

/* 
 * Functie: Voegt een string toe aan de database als deze nog niet aanwezig is.
 * 
 * str: De string die toegevoegd moet worden.
 * 
 * Bijeffecten: Wijzigt de database, print naar de console.
 */
void add(char *str) {
    // String naar kleine letters omzetten
    for (int i = 0; str[i]; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }

    // Controleer of de string al in de database staat
    for (int i = 0; i < DB_SIZE; i++) {
        if (strcmp(DATABASE[i], str) == 0) {
            printf("\"%s\" not added; already in database\n", str);
            return;
        }
    }

    // String toevoegen als er ruimte is
    if (DB_SIZE < MAX_DB_SIZE) {
        DATABASE[DB_SIZE] = malloc((strlen(str) + 1) * sizeof(char));
        strcpy(DATABASE[DB_SIZE], str);
        DB_SIZE++;
        printf("\"%s\" added to database\n", str);
    } else {
        printf("Database is full\n");
    }
}

/* 
 * Functie: Verwijdert een string uit de database als deze aanwezig is.
 * 
 * str: De string die verwijderd moet worden.
 */
void remove_string(char *str) {
    int index = -1;

    // Zoek de string in de database
    for (int i = 0; i < DB_SIZE; i++) {
        if (strcmp(DATABASE[i], str) == 0) {
            index = i;
            break;
        }
    }

    // String verwijderen indien gevonden
    if (index != -1) {
        free(DATABASE[index]);
        for (int i = index; i < DB_SIZE - 1; i++) {
            DATABASE[i] = DATABASE[i + 1];
        }
        DB_SIZE--;
        printf("\"%s\" removed from database\n", str);
    } else {
        printf("\"%s\" not removed; not in database\n", str);
    }
}

/* 
 * Functie: Haalt de beste overeenkomst of een exacte match op uit de database en print de resultaten.
 * 
 * str: De string waar je naar wilt zoeken in de database.
 * 
 * bijeffecten: Print resultaten.
 */
void retrieve(char *str) {
    int best_distance = -1;
    int distances[MAX_DB_SIZE];
    char *matches[MAX_DB_SIZE];
    int match_count = 0;

    if (DB_SIZE == 0) {
        printf("No match found; database is empty\n");
        return;
    }

    int len1 = (size_t)strlen(str);

    for (int i = 0; i < DB_SIZE; i++) {
        int len2 = (int)strlen(DATABASE[i]);

        // Bereken de Levenshtein-afstand
        int **grid = levenshtein(str, DATABASE[i]);
        int distance = grid[len1][len2];

        // Voeg de match toe aan de lijst
        matches[match_count] = DATABASE[i];
        distances[match_count] = distance;
        match_count++;

        // Geef het Levenshtein-raster vrij
        free_levenshtein_grid(grid, len1 + 1);
    }

    // Sorteer de matches op basis van de afstand
    for (int i = 0; i < match_count - 1; i++) {
        for (int j = 0; j < match_count - i - 1; j++) {
            if (distances[j] > distances[j + 1]) {
                // Wissel de afstanden
                int temp_distance = distances[j];
                distances[j] = distances[j + 1];
                distances[j + 1] = temp_distance;

                // Wissel de bijbehorende strings
                char *temp_match = matches[j];
                matches[j] = matches[j + 1];
                matches[j + 1] = temp_match;
            }
        }
    }

    // Zoek de kleinste afstand voor een perfecte match
    for (int i = 0; i < match_count; i++) {
        if (best_distance == -1 || distances[i] < best_distance) {
            best_distance = distances[i];
        }
    }

    // Print de perfecte match als deze gevonden is
    if (best_distance == 0) {
        for (int i = 0; i < match_count; i++) {
            if (distances[i] == 0) {
                printf("Perfect match found; \"%s\" is in database\n", matches[i]);
                return;
            }
        }
    }

    // Print de beste matches zonder perfecte match
    printf("No perfect match found; \"%s\" is not in database\n", str);
    printf("Best matches:\n");
    printf("Distance  String\n");

    for (int i = 0; i < match_count && i < 3; i++) {
        printf("%d         %s\n", distances[i], matches[i]);
    }
}

/* 
 * Functie: Leest DNA-sequenties uit een bestand en voegt deze toe aan de database.
 * 
 * filename: De naam van het bestand dat gelezen moet worden.
 * 
 * Bijeffecten: Wijzigt de database, print fouten of succesmeldingen naar de console.
 */
void read_from_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("\"%s\" not found\n", filename);
        return;
    }

    char line[MAX_INPUT_SIZE];
    while (fgets(line, MAX_INPUT_SIZE, file)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) > 0) {
            add(line);
        }
    }

    fclose(file);
}

/* 
 * Functie: Drukt alle strings in de database af.
 */
void list_database(void) {
    if (DB_SIZE == 0) {
        printf("Database is empty\n");
    } else {
        for (int i = 0; i < DB_SIZE; i++) {
            printf("%s\n", DATABASE[i]);
        }
    }
}

/* 
 * Functie: Toont een lijst van beschikbare commando's en hun beschrijving.
 */
void help(void) {
    printf("LIST OF COMMANDS\n");
    printf("  add ....... add to database\n");
    printf("  compare ... compare two strings\n");
    printf("  help ...... print this text\n");
    printf("  list ...... print database\n");
    printf("  read ...... read from file and add to database\n");
    printf("  remove .... remove from database\n");
    printf("  retrieve .. find in database\n");
    printf("  size ...... print number of items in database\n");
    printf("  quit ...... stop\n");
}

/* 
 * Functie: Geeft al het geheugen vrij dat door de database in gebruik is.
 * 
 * Bijeffecten: Database wordt geleegd en het geheugen wordt vrijgegeven.
 */
void cleanup_database(void) {
    for (int i = 0; i < DB_SIZE; i++) {
        free(DATABASE[i]);
    }
    free(DATABASE);
}

/* 
 * Functie: Verwerkt de gebruikersinvoer en voert de corresponderende commando's uit.
 * 
 * Neveneffecten: Wijzigt de database en print naar de console.
 */
int main(void) {
    printf("Welcome to DNA Matcher v0.2\n");

    // Geheugen toewijzen voor de database en initialiseren
    DATABASE = malloc(MAX_DB_SIZE * sizeof(char *));
    for (int i = 0; i < MAX_DB_SIZE; i++) {
        DATABASE[i] = NULL;
    }
    
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("console> ");
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            printf("Error reading input\n");
            continue;
        }

        input[strcspn(input, "\n")] = 0;

        char *action = strtok(input, " ");
        char *arg1 = strtok(NULL, " ");
        char *arg2 = strtok(NULL, " ");

        if (action == NULL) {
            continue;
        }

        if (strcmp(action, "quit") == 0) {
            cleanup_database();
            exit(0);
        } else if (strcmp(action, "help") == 0) {
            help();
        } else if (strcmp(action, "list") == 0) {
            list_database();
        } else if (strcmp(action, "size") == 0) {
            printf("%d items in database\n", DB_SIZE);
        } else if (strcmp(action, "add") == 0) {
            if (arg1 != NULL) {
                add(arg1);
            } else {
                printf("Please enter a valid command\n");
            }
        } else if (strcmp(action, "remove") == 0) {
            if (arg1 != NULL) {
                remove_string(arg1);
            } else {
                printf("Please enter a valid command\n"); 
            }
        } else if (strcmp(action, "compare") == 0) {
            if (arg1 != NULL && arg2 != NULL) {
                compare(arg1, arg2);
            } else {
                printf("Please enter a valid command\n");
            }
        } else if (strcmp(action, "retrieve") == 0) { 
            if (arg1 != NULL) { 
                retrieve(arg1); 
            } else { 
                printf("Please enter a valid command\n"); 
            } 
        } else if (strcmp(action, "read") == 0) { 
            if (arg1 != NULL) { int
                read_from_file(arg1); 
            } else { 
                printf("Please enter a valid command\n"); 
            } 
        } else { 
            printf("Please enter a valid command\n"); 
        } 
    }
    return 0;
}
