/* Naam: Mats Vink
   UvAnetID: 15874648
   Studie: BSc Informatica

   spel.c is verantwoordelijk voor de spelmechanica en interactie met de speler. 
   De belangrijkste onderdelen zijn:
    - Functie toon_rooster: Toont de huidige staat van het doolhof op het scherm met 
      behulp van ncurses.
    - Functie beweeg: Verwerkt de beweging van de speler door het doolhof op basis 
      van de invoer van de gebruiker.
    - Functie speel: Beheert de hoofdspelloop, waarin de speler kan bewegen 
      en de toestand van het spel (zoals winnen of verliezen) wordt gecontroleerd.
*/

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rooster.h"
#include <time.h>

int verzamelde_doelen = 0; // Houdt $-tekens bij
int vraag = 0; // Houdt bij bij welke vraag je bent

/* Stel een quizvraag en geef feedback.
 *
 * Retourneert:
 *   1 als het antwoord juist is, anders 0.
 */
int stel_quizvraag() {
    clear();
    printw("Beantwoord de volgende vraag:\n");

    // Selecteer de huidige quizvraag
    int nummer = vraag;
    int antwoord = 0;
    int correct = 0;

    switch (nummer) {
        case 0:
            printw("In welk jaar werd de Universiteit van Amsterdam opgericht?\n");
            printw("a) 1632\nb) 1812\nc) 1900\n");
            antwoord = getch();
            correct = (antwoord == 'a'); // Correcte antwoord is a
            break;
        case 1:
            printw("Wat is de naam van de faculteit die zich richt op bèta- en informatiewetenschappen?\n");
            printw("a) Faculteit der Geesteswetenschappen\n");
            printw("b) Faculteit der Natuurwetenschappen, Wiskunde en Informatica\n");
            printw("c) Faculteit der Maatschappij- en Gedragswetenschappen\n");
            antwoord = getch();
            correct = (antwoord == 'b'); // Correcte antwoord is b
            break;
        case 2:
            printw("Welke Nederlandse wetenschapper ontving de Nobelprijs voor Scheikunde in 1901?\n");
            printw("a) Christiaan Eijkman\nb) Pieter Zeeman\nc) Hendrik Lorentz\n");
            antwoord = getch();
            correct = (antwoord == 'b'); // Correcte antwoord is b
            break;
        case 3:
            printw("Wat is de naam van de bibliotheek van de UvA aan het Singel?\n");
            printw("a) De Universiteitsbibliotheek\n");
            printw("b) De Bijzondere Collecties\n");
            printw("c) De Maagdenhuisbibliotheek\n");
            antwoord = getch();
            correct = (antwoord == 'a'); // Correcte antwoord is a
            break;
        case 4:
            printw("Wat is de naam van de mascotte van de UvA?\n");
            printw("a) Magnus\n");
            printw("b) Vossius\n");
            printw("c) Amsterdaemus\n");
            antwoord = getch();
            correct = (antwoord == 'a'); // Correcte antwoord is a
            break;
        case 5:
            printw("In welk gebouw bevindt zich de aula van de UvA?\n");
            printw("a) Het Maagdenhuis\n");
            printw("b) De Oude Lutherse Kerk\n");
            printw("c) Het Allard Pierson Museum\n");
            antwoord = getch();
            correct = (antwoord == 'b'); // Correcte antwoord is b
            break;
        case 6:
            printw("Hoe heet het studenteninformatiesysteem van de UvA?\n");
            printw("a) Blackboard\n");
            printw("b) Canvas\n");
            printw("c) SIS\n");
            antwoord = getch();
            correct = (antwoord == 'c'); // Correcte antwoord is c
            break;
        case 7:
            printw("Wat is het motto van de Universiteit van Amsterdam?\n");
            printw("a) Veritas et Scientia\n");
            printw("b) Sapientia et Justitia\n");
            printw("c) Ex Scientia Veritas\n");
            antwoord = getch();
            correct = (antwoord == 'b'); // Correcte antwoord is b
            break;
    }

    // Toon feedback afhankelijk van of het antwoord juist was

    if (correct) {
        clear();
        printw("Goed gedaan! Je hebt het juiste antwoord gegeven.\n");
    } else {
        clear();
        printw("Helaas, dat was het verkeerde antwoord.\n");
    }
    refresh();
    napms(2000); // Wacht 2 seconden zodat de speler het bericht kan lezen
    return correct;
}

/* Verplaats een $ naar een willekeurige lege plek in het rooster.
 *
 * rp: Een pointer naar het rooster.
 */
void verplaats_dollar(rooster *rp) {
    int x = rand() % rooster_breedte(rp);
    int y = rand() % rooster_hoogte(rp);
    // Zoek een willekeurige lege plek in het rooster (een ' ' plek)
    while (rooster_kijk(rp, x, y) != ' ') {
        x = rand() % rooster_breedte(rp);
        y = rand() % rooster_hoogte(rp);
    }
    rooster_plaats(rp, x, y, '$');
}

/* Toont het gegeven rooster met ncurses.

   rp: een pointer naar het rooster.
*/
void toon_rooster(rooster *rp) {
    clear();
    for (int y = 0; y < rooster_hoogte(rp); y++) {
        for (int x = 0; x < rooster_breedte(rp); x++) {
            addch(rooster_kijk(rp, x, y));
        }
        addch('\n');
    }
    // Toon het aantal verzamelde doelen onder het doolhof
    printw("\nVerzamelde $: %d/8\n", verzamelde_doelen);
    refresh();
}


/* Toont het beginscherm met de handleiding */
void toon_beginscherm() {
    clear();

    // Miniatuur doolholf
    printw("Welkom bij het Doolhofspel!\n\n");
    printw("#########\n");
    printw("#     $ #\n");
    printw("# ### ###\n");
    printw("# #   # #\n");
    printw("### ### #\n");
    printw("#*      #\n");
    printw("#########\n\n");

    // Handleiding van het spel
    printw("Instructies:\n");
    printw("Gebruik de pijltjestoetsen om als * door het doolhof te navigeren.\n");
    printw("Probeer alle ($) te verzamelen zonder in een val te lopen (X).\n");
    printw("Je kan met de trap (/) of (\\) naar boven en beneden gaan\n");
    printw("En pas op voorde valhekken (H) daar kan je maar 1 keer door heen!\n\n");
    printw("Druk op een toets om te beginnen...\n");

    refresh();
    getch(); // Wacht tot de speler op een toets drukt
}

/* Toont het scherm wanneer de speler heeft gewonnen */
void toon_gewonnen_scherm() {
    clear();
    printw("Gefeliciteerd!\n");
    printw("Je hebt het doolhof voltooid!\n\n");
    printw("         |\n");
    printw("       $$$$$\n");
    printw("      $$ |  $$\n");
    printw("     $$  |\n");
    printw("      $$ |\n");
    printw("       $$$\n");
    printw("         $$$\n");
    printw("         | $$\n");
    printw("         |  $$\n");
    printw("     $$  | $$\n");
    printw("       $$$$$\n");
    printw("         |\n\n");
    printw("Druk op 'r' om opnieuw te spelen of 'q' om te stoppen.\n");
    refresh();
}

/* Toont het scherm wanneer de speler heeft verloren */
void toon_verloren_scherm() {
    clear();
    printw("Je bent in de val gelopen!\n");
    printw("Helaas, je hebt verloren...\n\n");
    printw("    X     X\n");
    printw("     X   X \n");
    printw("      X X  \n");
    printw("       X   \n");
    printw("      X X  \n");
    printw("     X   X \n");
    printw("    X     X\n\n");
    printw("Druk op 'r' om opnieuw te spelen of 'q' om te stoppen.\n");
    refresh();
}


/* Voert de benodigde veranderingen in het rooster door als de speler in een
   bepaalde richting probeert te bewegen.

   rp   : een pointer naar het rooster
   rh   : een pointer naar het 2e rooster voor de trappen
   dx,dy: de richting waarin de speler probeert te bewegen. De vier mogelijk-
          heden voor (dx,dy) zijn (-1,0), (1,0), (0,-1), (0,1) voor resp.
          links, rechts, omhoog en omlaag.

   Side effect: het rooster wordt aangepast op basis van de handeling van
                de speler.
*/
void beweeg(rooster *rp, rooster *rh, int dx, int dy) {
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
        rooster_zet_toestand(rh, VERLOREN);
    } else if (volgende == 'H') {
        rooster_plaats(rp, x + 2*dx, y + 2*dy, '*');
        rooster_plaats(rp, x + dx, y + dy, '#');
        rooster_plaats(rp, x, y, ' ');
    } else if (volgende == '$') {
        if (stel_quizvraag()) {
            verzamelde_doelen++;
            rooster_plaats(rp, x + dx, y + dy, '*');
            rooster_plaats(rp, x, y, ' ');
            vraag++;
        } else {
            rooster_plaats(rp, x + dx, y + dy, '*');
            rooster_plaats(rp, x, y, ' ');
            verplaats_dollar(rp);  // Verplaats het $-teken naar een random plek
        }
    }

    // Check of alle $-tekens verzameld zijn en of de speler naar boven of beneden gaat
    if (volgende == '/') {
        rooster_zet_toestand(rp, BOVEN);
    }
    if (volgende == '\\') {
        rooster_zet_toestand(rh, ONDER);
    }
    if (verzamelde_doelen == 8) {
        rooster_zet_toestand(rp, GEWONNEN);
        rooster_zet_toestand(rh, GEWONNEN);
        verzamelde_doelen = 0;
    }
}

/* Speelt het spel met een gegeven rooster tot de toestand niet langer
   AAN_HET_SPELEN is.
 */
void speel(rooster *rp, rooster *rh) {
    rooster_zet_toestand(rp, ONDER);

    // Toon de onderverdieping als rp ONDER is en andersom
    while (rooster_vraag_toestand(rp) == ONDER || rooster_vraag_toestand(rp) == BOVEN) {

        while (rooster_vraag_toestand(rp) == ONDER) {
            toon_rooster(rp);
            int toets = getch();
        
            switch (toets) {
                case KEY_UP:    beweeg(rp, rh, 0, -1); break;
                case KEY_DOWN:  beweeg(rp, rh, 0, 1);  break;
                case KEY_LEFT:  beweeg(rp, rh, -1, 0); break;
                case KEY_RIGHT: beweeg(rp, rh, 1, 0);  break;
            }
        }

        while (rooster_vraag_toestand(rp) == BOVEN) {
            toon_rooster(rh);
            int toets = getch();
        
            switch (toets) {
                case KEY_UP:    beweeg(rh, rp, 0, -1); break;
                case KEY_DOWN:  beweeg(rh, rp, 0, 1);  break;
                case KEY_LEFT:  beweeg(rh, rp, -1, 0); break;
                case KEY_RIGHT: beweeg(rh, rp, 1, 0);  break;
            }
        }
    }

    // Als de speler wint of verliest, toon een scherm en geef de optie om opnieuw te spelen
    if (rooster_vraag_toestand(rp) == GEWONNEN) {
        toon_gewonnen_scherm();
    } else if (rooster_vraag_toestand(rp) == VERLOREN) {
        toon_verloren_scherm();
    }

    // Wacht op invoer voor opnieuw spelen of stoppen
    int keuze = getch();
    if (keuze == 'r') {
        // Sluit het huidige rooster af
        rooster_klaar(rp);
        rooster_klaar(rh);
    
        // Heropen het doolhofbestand en lees het rooster opnieuw
        FILE *fh = fopen("assets/Onderverdieping.txt", "r");
        if (fh == NULL) {
            perror("main");
            exit(1);
        }
        rp = rooster_lees(fh);
        fclose(fh);

        FILE *fp = fopen("assets/Bovenverdieping.txt", "r");
        if (fh == NULL) {
            perror("main");
            exit(1);
        }
        rh = rooster_lees(fp);
        fclose(fp);

        // Reset verzamelde doelen
        verzamelde_doelen = 0;

        // Start het spel opnieuw
        speel(rp, rh);
    } else if (keuze == 'q') {
        return; // Stop het spel
    }
}

int main() {

    // 1. Open de onderverdieping en lees het rooster.
    FILE *fh = fopen("assets/Onderverdieping.txt", "r");
    if (fh == NULL) {
        perror("main");
        exit(1); 
    }
    rooster *rp = rooster_lees(fh);
    fclose(fh);

    // 2. Open de onderverdieping en lees het rooster.
    FILE *fp = fopen("assets/Bovenverdieping.txt", "r");
    if (fh == NULL) {
        perror("main");
        exit(1); 
    }
    rooster *rh = rooster_lees(fp);
    fclose(fp);

    // 3. Bepaal of het lezen van de roosters is gelukt.
    if (rp == NULL) {
        fprintf(stderr, "Kan rooster niet maken.\n"); 
        exit(1); 
    }
    if (rh == NULL) {
        fprintf(stderr, "Kan rooster niet maken.\n"); 
        exit(1); 
    }

    // Initialiseer random generator
    srand(time(NULL));

    // 4. Initialiseer ncurses
    initscr();
    cbreak();             
    keypad(stdscr, TRUE); 
    noecho();             

    // 5. Toon het beginscherm
    toon_beginscherm();

    // 6. Speel het spel
    speel(rp, rh);

    // 7. Sluit af.
    rooster_klaar(rp);
    rooster_klaar(rh);
    endwin();
    return 0;
}
