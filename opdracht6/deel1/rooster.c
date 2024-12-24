/* Naam: Mats Vink
   UvAnetID: 15874648
   Studie: BSc Informatica

   Rooster.c bevat de implementatie van de datastructuur en de bijbehorende functies voor het beheren van een doolhof, dat wordt weergegeven als een rooster. 
   De belangrijkste componenten zijn:

   Struct rooster_data: Definieert de structuur van het doolhof met breedte, hoogte en een tweedimensionale array voor de inhoud van het doolhof.
   Functies voor bestandshantering: Leest een doolhof uit een bestand en initialiseert het rooster.
   Functies voor roosteroperaties: Biedt functionaliteit om karakters te bekijken en te plaatsen in het rooster, evenals het controleren van grenzen.  
   Zoekfunctie: Zoekt naar specifieke karakters (bijvoorbeeld de speler) in het rooster en retourneert hun coördinaten.
*/

#include <stdio.h>
#include <stdlib.h>
#include "rooster.h"

struct rooster_data {
    char **veld;
    int breedte, hoogte;
    toestand huidige_toestand;
};
typedef struct rooster_data rooster;

/* Bepaal de grootte van het bestand. */
long Check_grootte(FILE *fh) {
    if (fh == NULL) {
        perror("Invalid file handle");
        return -1;
    }
    // Verplaats naar het einde van het bestand om de grootte te bepalen
    if (fseek(fh, 0, SEEK_END) != 0) {
        perror("Fout bij fseek");
        return -1;
    }
    // Bepaal de grootte
    long grootte = ftell(fh);
    if (grootte == -1) {
        perror("Fout bij ftell");
        return -1;
    }
    // Zet de bestandspointer terug naar het begin
    rewind(fh);
    return grootte;
}

/* Maak een rooster op basis van de data in de gegeven stream. */
rooster *rooster_lees(FILE *fh) {
    if (fh == NULL) {
        perror("Invalid file handle");
        return NULL;
    }

    // Bestandsgrootte bepalen
    long grootte = Check_grootte(fh);
    if (grootte == -1) {
        return NULL;
    }

    // Geheugen alloceren voor doolhofinhoud (1e malloc)
    char *doolhof = malloc(grootte);
    if (doolhof == NULL) {
        perror("Fout bij geheugenallocatie");
        return NULL;
    }

    // Bestand inlezen in het doolhof
    size_t gelezen = fread(doolhof, sizeof(char), grootte, fh);
    if (gelezen != (size_t)grootte) {
        perror("Fout bij het lezen van het bestand");
        free(doolhof);
        return NULL;
    }

    // Breedte en hoogte bepalen
    int breedte = 0, hoogte = 0, huidige_breedte = 0;
    for (int i = 0; i < grootte; i++) {
        if (doolhof[i] == '\n') {
            hoogte++;
            if (huidige_breedte > breedte) {
                breedte = huidige_breedte;
            }
            huidige_breedte = 0;
        } else {
            huidige_breedte++;
        }
    }

    // Geheugen alloceren voor rooster_data struct (2e malloc)
    rooster *r = malloc(sizeof(rooster));
    if (r == NULL) {
        perror("Fout bij rooster-allocatie");
        free(doolhof);
        return NULL;
    }

    r->breedte = breedte;
    r->hoogte = hoogte;
    r->huidige_toestand = BEGIN;

    // Geheugen alloceren voor het 2D-veld (3e malloc)
    r->veld = malloc(hoogte * sizeof(char *));
    if (r->veld == NULL) {
        perror("Fout bij veld-allocatie");
        free(doolhof);
        free(r);
        return NULL;
    }

    for (int i = 0; i < hoogte; i++) {
        r->veld[i] = malloc(breedte * sizeof(char));
        if (r->veld[i] == NULL) {
            perror("Fout bij allocatie van veldrijen");
            // Vrijgeven van eerder gealloceerd geheugen
            for (int j = 0; j < i; j++) {
                free(r->veld[j]);
            }
            free(r->veld);
            free(doolhof);
            free(r);
            return NULL;
        }
    }

    // Vul het veld met de inhoud van het doolhof
    int rij = 0, kolom = 0;
    for (int i = 0; i < grootte; i++) {
        if (doolhof[i] == '\n') {
            kolom = 0;
            rij++;
        } else {
            r->veld[rij][kolom] = doolhof[i];
            kolom++;
        }
    }

    free(doolhof);  // Doolhofinhoud is nu gekopieerd, we kunnen het vrijgeven
    return r;
}


/* Vraag de huidige toestand van het spel op. */
toestand rooster_vraag_toestand(const rooster *rp) {
    return rp->huidige_toestand;
}

/* Verander de huidige toestand van het spel. */
void rooster_zet_toestand(rooster *rp, toestand t) {
    rp->huidige_toestand = t;
}

/* Geef alle resources vrij die zijn gealloceerd voor een rooster. */
void rooster_klaar(rooster *rp) {
    if (rp == NULL) return;

    // Vrijgeven van elke rij in het veld
    for (int i = 0; i < rp->hoogte; i++) {
        free(rp->veld[i]);
    }

    // Vrijgeven van het veld zelf
    free(rp->veld);

    // Vrijgeven van het rooster
    free(rp);
}

/* Vraag de breedte van het rooster op. */
int rooster_breedte(const rooster *rp) {
    return rp->breedte;
}

/* Vraag de hoogte van het rooster op. */
int rooster_hoogte(const rooster *rp) {
    return rp->hoogte;
}

/* Kijk of de gegeven positie binnen het rooster valt. */
int rooster_bevat(const rooster *rp, int x, int y) {
    return (y >= 0 && y < rp->hoogte && x >= 0 && x < rp->breedte);
}

/* Kijk welk object er staat op een bepaalde positie in het rooster. */
char rooster_kijk(const rooster *rp, int x, int y) {
    if (rooster_bevat(rp, x, y)) {
        return rp->veld[y][x]; // Kolom is x, rij is y
    }
    return '\0';
}

/* Schrijf een bepaald object op een bepaalde positie in het rooster. */
int rooster_plaats(rooster *rp, int x, int y, char c) {
    if (rooster_bevat(rp, x, y)) {
        rp->veld[y][x] = c; // Plaatst op kolom x en rij y
        return 1;
    }
    return 0;
}

/* Zoek een bepaald object in het rooster, en geef de coördinaten terug. */
void rooster_zoek(const rooster *rp, char c, int *x, int *y) {
    for (int i = 0; i < rp->hoogte; i++) {
        for (int j = 0; j < rp->breedte; j++) {
            if (rp->veld[i][j] == c) {
                *x = j; // x is de kolom
                *y = i; // y is de rij
                return;
            }
        }
    }
    *x = -1;
    *y = -1;
}
