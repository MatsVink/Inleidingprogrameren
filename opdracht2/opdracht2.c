/* Naam: Mats Vink
   UvAnetID: 15874648
   Studie: BSc Informatica

   Dit programma bevat verschillende functies waarmee de Collatz-reeks
   onderzocht kan worden. Zo bereken we de collatzreeks tot 1, 
   welke stoptijd dit geeft en welk maximum getal hier in zit. 
   Daarnaast bereken we het cijfer met de maximale stoptijd en getal tussen (a, b).
   En we berekenen alle getallen die niet correct door dit programma gerunt kunnen worden.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

/* Berekent de opvolger in de Collatz-reeks voor een gegeven getal.
 * Input: Een positief geheel getal.
 * Output: Het volgende getal in de reeks. Bij fout (negatieve of te grote input): -1.
 * Side effects: Geen.
 */
int collatz_opvolger(int getal) {
    if(getal <= 0) {
        return -1;  // Ongeldige invoer
    }

    // Controleer of het getal even of oneven is
    if(getal % 2 == 0) {
        getal = getal / 2;  // Deel door 2 als het even is
    } else {
        if (getal > INT_MAX/3) {
            return -1;  // Vermijd overflow voor hele grote getallen
        }
        getal = getal * 3 + 1;  // Vermenigvuldig met 3 en tel 1 op als het oneven is
    }
    return getal;
}

/* Berekent de stoptijd van een Collatz-reeks.
 * Input: Een positief geheel getal dat de start van de reeks aangeeft.
 * Output: Het aantal stappen tot het getal 1 bereikt is, of -1 bij foutieve invoer.
 * Side effects: Geen.
 */
int collatz_stoptijd(int reeks) {
    int stop = 1; // Minimaal 1 stap voor reeks van 1

    if (reeks <= 0) {
        return -1;  // Ongeldige invoer
    }

    // Iteratief de reeks volgen tot het getal 1 wordt
    while(reeks != 1) {
        reeks = collatz_opvolger(reeks);
        stop++;
    }

    return stop;
}

/* Berekent het maximum van een Collatz-reeks.
 * Input: Een positief geheel getal dat de start van de reeks aangeeft.
 * Output: Het grootste getal dat voorkomt in de reeks tot en met 1, of -1 bij foutieve invoer.
 * Side effects: Geen.
 */
int collatz_maximum(int reeks) {
    int max = 1; // Startwaarde van max is minimaal 1

    if (reeks <= 0) {
        return -1;  // Ongeldige invoer
    }

     // Voor reeksen die beginnen met 1 of 2 is het maximum 4
    if (reeks == 1 || reeks == 2) {
        max = 4;
    } else{ 
    // Zoek naar het maximale getal in de reeks tot het 1 bereikt
        while(reeks != 1) {
            if(reeks > max) {
                max = reeks;
            }
        reeks = collatz_opvolger(reeks);
        }
    }

    return max;
}

/* Toont de volledige Collatz-reeks van een begingetal.
 * Input: Een positief geheel getal dat de start van de reeks aangeeft.
 * Output: Geen (resultaten worden afgedrukt naar de console).
 * Side effects: Print de reeks naar de console.
 */
void collatz_toon_reeks(int reeks) {
    if(reeks <= 0) {
        return;  // Ongeldige invoer
    }

    // Druk de getallen af tot het getal 1
    while(reeks != 1) {
        printf("%d, ", reeks);
        reeks = collatz_opvolger(reeks);
    }
    printf("1\n"); // Print het laatste getal in de reeks
}

/* Toont informatie over de Collatz-reeks, inclusief de stoptijd en het maximum.
 * Input: Een positief geheel getal dat de start van de reeks aangeeft.
 * Output: Geen (resultaten worden afgedrukt naar de console).
 * Side effects: Print de reeks, stoptijd en maximum naar de console.
 */
void collatz_info(int reeks) {
    if(reeks <= 0) {
        return;  // Ongeldige invoer
    }

    printf("Reeks: ");
    collatz_toon_reeks(reeks);
    printf("Stoptijd: %d\n", collatz_stoptijd(reeks));
    printf("Maximum: %d\n", collatz_maximum(reeks));
}

/* Zoekt het begingetal met de langste stoptijd tussen a en b.
 * Input: Twee positieve gehele getallen a en b.
 * Output: Het getal met de langste stoptijd in het bereik [a, b], of -1 bij foutieve invoer.
 * Side effects: Geen.
 */
int collatz_zoek_langste(int a, int b) {
    if (a <= 0 || b <= 0) {
        return -1;  // Ongeldige invoer
    }

    int getal = a;
    int getalmax = a; // Houdt het getal met de langste stoptijd bij

    // Zoek naar het getal met de langste stoptijd tussen a en b
    while(getal <= b) {
        int stoptijd = collatz_stoptijd(getal);
        if(stoptijd > collatz_stoptijd(getalmax)) {
            getalmax = getal; // Update als deze stoptijd langer is
        }
        getal++;
    }

    return getalmax;
}

/* Zoekt het begingetal met het grootste maximum in de reeks tussen a en b.
 * Input: Twee positieve gehele getallen a en b.
 * Output: Het getal met het grootste maximum in het bereik [a, b], of -1 bij foutieve invoer.
 * Side effects: Geen.
 */
int collatz_zoek_grootste(int a, int b) {
    if (a <= 0 || b <= 0 || b < a) {
        return -1;  // Ongeldige invoer
    }

    int getal = a;
    int getalmax = a; // Houdt het getal met het grootste maximum bij

    // Zoek naar het getal met het grootste maximum tussen a en b
    while (getal <= b) {
        int maximum = collatz_maximum(getal);
        if (maximum > collatz_maximum(getalmax)) {
            getalmax = getal; // Update als dit maximum groter is
        }
        getal++;
    }

    return getalmax;
}

/* Voert een onderzoek uit naar de langste stoptijd en het grootste maximum tussen a en b.
 * Input: Twee positieve gehele getallen a en b.
 * Output: Geen (resultaten worden afgedrukt naar de console).
 * Side effects: Print de resultaten van de onderzoeken naar de console.
 */
void collatz_onderzoek(int a, int b) {
    if (a <= 0 || b <= 0) {
        return;  // Ongeldige invoer
    }

    printf("Langste Collatz reeks tussen %d en %d:\n", a, b);
    collatz_info(collatz_zoek_langste(a, b));

    printf("\nCollatz reeks met de grootste waarde tussen %d en %d:\n", a, b);
    collatz_info(collatz_zoek_grootste(a, b));
}

/* Controleert welke startgetallen foutief zijn tussen 1 en 1 miljoen.
 * Input: Geen.
 * Output: Geen (resultaten worden afgedrukt naar de console).
 * Side effects: Print foutieve getallen naar de console.
 */
void collatz_incorrect(void) {
    int start_getal;
    for (start_getal = 1; start_getal <= 1000000; start_getal++) {
        int getal = start_getal;
        while (getal != 1) {
            if(collatz_opvolger(getal) == -1) {
                printf("%d\n", start_getal);  // Print foutief startgetal
                break;
            }
            getal = collatz_opvolger(getal);
        }
    }
}

#ifndef COMPILE_MAIN
int main(int argc, char **argv) {

    if (argc == 1) {
        // Programma is aangeroepen zonder argumenten
        collatz_incorrect();
    } else if (argc == 2) {
        // Programma is aangeroepen met 1 argument.
        collatz_info(atoi(argv[1]));
    } else if (argc == 3) {
        // Programma is aangeroepen met 2 argumenten.
        collatz_onderzoek(atoi(argv[1]), atoi(argv[2]));
    } else {
        printf("gebruik: opdracht2 [getal [tweede getal]]\n");
        return 1;
    }

    return 0;
}
#endif
