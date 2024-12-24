/*
∗ Naam : M.Vink
∗ UvAnetID : 15874648
∗ S t u d i e : BSc I n f o r m a t i c a
∗
∗ deel1.c
*/

#include <stdio.h>

int main(void) {
    int totaal_rijen = 23;      // Totaal aantal rijen in de driehoek
    int midden_rij = 14;        // Rij waar het patroon omschakelt
    int rij = 0;                // Huidige rij
    int afname = 1;       // Teller voor het afnemende deel van de driehoek

    while (rij < totaal_rijen) {
        int spaties, punten;

        if (rij < midden_rij) {
            // Voor het bovenste deel van de driehoek
            spaties = rij * 2;
            punten = rij * 2;
        } else {
            // Voor het onderste deel van de driehoek
            spaties = rij * 2;
            punten = (midden_rij * 2 - 2) - (3 * afname); //weet niet of dit de juiste manier is
            afname++;
        }

        // Print de spaties
        while (spaties > 0) {
            printf(" ");
            spaties--;
        }

        // Print het eerste sterretje
        printf("*");

        // Print de punten
        while (punten > 0) {
            printf(".");
            punten--;
        }

        // Print het tweede sterretje, behalve op de laatste rij
        if (rij != totaal_rijen - 1) {
            printf("*");
        }

        // Ga naar de volgende rij
        printf("\n");
        rij++;
    }

    return 0;
}