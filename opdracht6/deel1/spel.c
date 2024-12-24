/* Naam: Mats Vink
   UvAnetID: 15874648
   Studie: BSc Informatica

   spel.c is verantwoordelijk voor de spelmechanica en interactie met de speler. 
   De belangrijkste onderdelen zijn:

    Functie toon_rooster: Toont het huidige staat van het doolhof op het scherm met 
    behulp van de ncurses bibliotheek voor tekstgebaseerde interfaces.
    Functie beweeg: Verwerkt de beweging van de speler door het doolhof op basis 
    van de invoer van de gebruiker 
    Functie speel: Beheert de hoofdspelloop, waarin de speler kan bewegen 
    en de toestand van het spel (zoals winnen of verliezen) wordt gecontroleerd.
*/

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "rooster.h"

/* Toont het gegeven rooster met ncurses.

   rp: een pointer naar het rooster.
*/
void toon_rooster(rooster *rp) {
    clear(); // begin met een nieuw ncurses scherm
    for (int y = 0; y < rooster_hoogte(rp); y++) {
        for (int x = 0; x < rooster_breedte(rp); x++) {
            addch(rooster_kijk(rp, x, y));
        }
        addch('\n');
    }
    refresh();
}

/* Voert de benodigde veranderingen in het rooster door als de speler in een
   bepaalde richting probeert te bewegen.

   rp   : een pointer naar het rooster
   dx,dy: de richting waarin de speler probeert te bewegen. De vier mogelijk-
          heden voor (dx,dy) zijn (-1,0), (1,0), (0,-1), (0,1) voor resp.
          links, rechts, omhoog en omlaag.

   Side effect: het rooster wordt aangepast op basis van de handeling van
                de speler.
*/
void beweeg(rooster *rp, int dx, int dy) {
    int x, y;
    rooster_zoek(rp, '*', &x, &y);
    char volgende = rooster_kijk(rp, x + dx, y + dy);
    
    if (volgende == ' ') {
        rooster_plaats(rp, x + dx, y + dy, '*');
        rooster_plaats(rp, x, y, ' ');
    } else if (volgende == '#') {
        return;
    } else if (volgende == 'X') {
        rooster_zet_toestand(rp, VERLOREN);
        printf("Je bent in de val gelopen.\n");
    } else if (volgende == '$') {
        rooster_zet_toestand(rp, GEWONNEN);
        printf("Je hebt gewonnen!\n");
    }
}

/* Speelt het spel met een gegeven rooster tot de toestand niet langer
   AAN_HET_SPELEN is.
 */
void speel(rooster *rp) {
    rooster_zet_toestand(rp, AAN_HET_SPELEN);
    while (rooster_vraag_toestand(rp) == AAN_HET_SPELEN) {
        toon_rooster(rp);
        int toets = getch();
        
        switch (toets) {
            case KEY_UP:    beweeg(rp, 0, -1); break;
            case KEY_DOWN:  beweeg(rp, 0, 1);  break;
            case KEY_LEFT:  beweeg(rp, -1, 0); break;
            case KEY_RIGHT: beweeg(rp, 1, 0);  break;
        }
    }
}

int main(int argc, char *argv[]) {

    // 1. Controleer dat er een doolhofbestand is opgegeven op de command line.
    if (argc != 2) {
        fprintf(stderr, "gebruik: ./spel assets/voorbeeld_doolhof.txt\n");
        exit(1); 
    }

    // 2. Open het doolhofbestand en lees het rooster.
    FILE *fh = fopen(argv[1], "r");
    if (fh==NULL) {
        perror("main");
        exit(1); 
    }
    rooster *rp = rooster_lees(fh);
    fclose(fh);

    // 3. Bepaal of het lezen van het rooster is gelukt.
    if (rp==NULL) {
        fprintf(stderr, "Kan rooster niet maken.\n"); 
        exit(1); 
    }

    // 4. Initialiseer ncurses
    initscr();
    cbreak();             // zodat je kunt onderbreken met Ctrl+C
    keypad(stdscr, TRUE); // luister ook naar extra toetsen zoals pijltjes
    noecho();             // druk niet de letters af die je intypt

    // 5. Speel het spel.
    speel(rp);

    // 6. Sluit af.
    getch();
    rooster_klaar(rp);
    endwin();
    return 0;
}
