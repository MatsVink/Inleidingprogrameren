/*
∗ Naam : M.Vink
∗ UvAnetID : 15874648
∗ S t u d i e : BSc I n f o r m a t i c a
∗
∗ deel1.c 
*/

#include <stdio.h>
#include <stdlib.h>

int main(void) {

    int bezig = 1;  // Variabele om de loop gaande te houden
    int max = 100;  // Maximale waarde voor de gok
    int min = 0;    // Minimale waarde voor de gok

    while (bezig == 1) {
        int gok;  // Gebruikersinvoer voor de gok

        // Vraag de gebruiker om een gok te doen
        printf("Doe een gok:\n");
        int code = scanf("%d", &gok);

        // Controleer of de invoer correct is en binnen het toegestane bereik ligt
        if (code != 1 || gok < 0 || gok > 100) { //ik heb || gevonden op google
            printf("Ik begrijp de invoer niet.\n");
            exit(1);
        }

        // Controleer of de gebruiker het juiste getal heeft geraden
        if (gok == min + 1 && gok == max - 1) { //ik heb && code gevonden op google
            printf("Je hebt het geraden!\n");
            bezig = 0;  // Stop de loop
        } else {
            // Geef aan of de gok te hoog of te laag is en pas de grenzen aan
            if (random() % 2 == 0) {
                printf("Te hoog.\n");
                max = gok;
            } else {
                printf("Te laag.\n");
                min = gok;
            }
        }
    }

    return 0;
}
