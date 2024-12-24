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

    int a, b;  // Gebruikersinvoer voor de waarden a en b
    int x, y;  // Resultaten van de berekeningen
    int code;  // Code om te controleren of scanf succesvol is

    // Vraag de gebruiker om de waarde voor a
    printf("Geef de waarde voor a: \n");
    code = scanf("%d", &a);
    if (code != 1) { // heb != gevonden op google
        printf("Het lezen van een getal is niet goed gelukt. Ik stop ermee.\n");
        exit(1);
    }

    // Vraag de gebruiker om de waarde voor b
    printf("Geef de waarde voor b: \n");
    code = scanf("%d", &b);
    if (code != 1) {
        printf("Het lezen van een getal is niet goed gelukt. Ik stop ermee.\n");
        exit(1);
    }

    // Berekeningen van x en y
    y = (a - b) / 2;
    x = (a + b) / 2;

    // Toon de resultaten
    printf("x = %d\n", x);
    printf("y = %d\n", y);

    return 0;
}

