/* Naam: Mats Vink
   UvAnetID: 15874648
   Studie: BSc Informatica
  
   Dit programma implementeert de Caesar en Vigenere geheimschriften, en code
   om cryptogrammen te kraken.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_GROOTTE 500000
#define MAX_SLEUTEL 16

/* Lees een tekstbestand in de gegeven char array.

   fn:  de bestandsnaam van het bestand dat moet worden gelezen.
   str: een pointer naar een char array met grootte tenminste MAX_GROOTTE
        waar het bestand moet worden gelezen.
*/
void lees_bestand(const char *fn, char *str) {
    FILE *fh = fopen(fn, "r");
    if (fh==NULL) { 
        perror("Kan invoer niet lezen"); 
        exit(EXIT_FAILURE);
    }
    size_t grootte = fread(str, 1, MAX_GROOTTE-1, fh);
    fclose(fh);
    if (grootte==MAX_GROOTTE-1) {
        fprintf(stderr, "De invoer was te lang.\n");
        exit(1);
    }
    str[grootte] = '\0';
}


/* Verwijder niet-alfabetische karakters uit een string
 * en zet alle karakters om naar kleine letters.
 *
 * str: de string die moet worden versimpeld.
 */
void versimpel(char *str) {
    int j = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (isalpha(str[i])) {
            str[j] = (char)tolower(str[i]);
            j++;
        }
    }
    str[j] = '\0';
}

/* Versleutel een bericht met het Caesar-cijfer.
 *
 * bericht: het bericht dat moet worden versleuteld.
 */
void caesar_versleutel(char *bericht) {
    int j = 0;
    for (int i = 0; bericht[i] != '\0'; i++) {
        if (bericht[i] == 'x' || bericht[i] == 'y' || bericht[i] == 'z') {
            bericht[j] = (char)(bericht[i] - 23);
        } else {
            bericht[j] = (char)(bericht[i] + 3);
        }
        j++;
    }
    bericht[j] = '\0';
}

/* Voer Caesar-versleuteling uit op de inhoud van een bestand.
 *
 * fn: de bestandsnaam
 */
void opdracht_caesar_versleutel(const char *fn) {
    char bericht[MAX_GROOTTE];
    lees_bestand(fn, bericht);
    versimpel(bericht);
    caesar_versleutel(bericht);
    printf("Caesar versleuteld: %s\n", bericht);
}

/* Ontsleutel een bericht dat is versleuteld met het Caesar-cijfer.
 *
 * bericht: het bericht dat moet worden ontsleuteld.
 */
void caesar_ontsleutel(char *bericht) {
    int j = 0;
    for (int i = 0; bericht[i] != '\0'; i++) {
        if (bericht[i] == 'a' || bericht[i] == 'b' || bericht[i] == 'c') {
            bericht[j] = (char)(bericht[i] + 23);
        } else {
            bericht[j] = (char)(bericht[i] - 3);
        }
        j++;
    }
    bericht[j] = '\0';
}

/* Voer Caesar-ontsleuteling uit op de inhoud van een bestand.
 *
 * fn: de bestandsnaam
 */
void opdracht_caesar_ontsleutel(const char *fn) {
    char bericht[MAX_GROOTTE];
    lees_bestand(fn, bericht);
    versimpel(bericht);
    caesar_ontsleutel(bericht);
    printf("Caesar ontsleuteld: %s\n", bericht);
}

/* Versleutel een bericht met het Vigenère-cijfer.
 *
 * bericht: het bericht dat moet worden versleuteld.
 * sleutel: de sleutel om het bericht mee te versleutelen.
 */
void vigenere_versleutel(char *bericht, const char *sleutel) {
    size_t sleutel_lengte = strlen(sleutel);

    for (int i = 0; bericht[i] != '\0'; i++) {
        int verschuiving = sleutel[(size_t)i % sleutel_lengte] - 'a';
        bericht[i] = (char)(((bericht[i] - 'a' + verschuiving) % 26) + 'a');
    }
}

/* Voer Vigenère-versleuteling uit op de inhoud van een bestand.
 *
 * fn: de bestandsnaam
 * sleutel: de sleutel om het bericht mee te versleutelen.
 */
void opdracht_vigenere_versleutel(const char *fn, const char *sleutel) {
    char bericht[MAX_GROOTTE];
    lees_bestand(fn, bericht);
    versimpel(bericht);
    vigenere_versleutel(bericht, sleutel);
    printf("Vigenere versleuteld met '%s':\n    %s\n", sleutel, bericht);
}

/* Ontsleutel een bericht dat is versleuteld met het Vigenère-cijfer.
 *
 * bericht: het bericht dat moet worden ontsleuteld.
 * sleutel: de sleutel waarmee het bericht is versleuteld.
 */
void vigenere_ontsleutel(char *bericht, const char *sleutel) {
    size_t sleutel_lengte = strlen(sleutel);

    for (int i = 0; bericht[i] != '\0'; i++) {
        int verschuiving = sleutel[(size_t)i % sleutel_lengte] - 'a';
        bericht[i] = (char)(((bericht[i] - 'a' - verschuiving + 26) % 26) + 'a');
    }
}

/* Voer Vigenère-ontsleuteling uit op de inhoud van een bestand.
 *
 * fn: de bestandsnaam
 * sleutel: de sleutel om het bericht mee te ontsleutelen.
 */
void opdracht_vigenere_ontsleutel(const char *fn, const char *sleutel) {
    char bericht[MAX_GROOTTE];
    lees_bestand(fn, bericht);
    versimpel(bericht);
    vigenere_ontsleutel(bericht, sleutel);
    printf("Vigenere ontsleuteld met '%s':\n    %s\n", sleutel, bericht);
}

/* Bepaal de frequenties van letters in een tekst.
 *
 * tekst: de tekst waarvan de frequenties moeten worden bepaald.
 * p: een array waarin de frequenties worden opgeslagen.
 */
void bepaal_frequenties(const char *tekst, double *p) {
    int letters[26] = {0};
    int totaal_letters = 0;
    for (int i = 0; tekst[i] != '\0'; i++) {
        letters[tekst[i] - 'a']++;
        totaal_letters++;
    }

    for (int i = 0; i < 26; i++) {
        p[i] = (double)letters[i] / totaal_letters;
    }
}

/* Lees een tekstbestand in en bepaal de frequenties van de letters.
 *
 * fn: de bestandsnaam van het bestand.
 */
void opdracht_frequenties(const char *fn) {
    char bericht[MAX_GROOTTE];
    lees_bestand(fn, bericht);
    versimpel(bericht);
    double p[26] = {0};
    bepaal_frequenties(bericht, p);
    for (int i = 0; i < 26; i++) {
        printf("%c: %.3f\n", 'a' + i, p[i]);
    }
}

/* Bereken de gelijkenis tussen twee frequentiearrays.
 *
 * p, q: de twee frequentiearrays die worden vergeleken.
 *
 * Retourneert: de gelijkenis tussen p en q.
 */
double gelijkenis(const double *p, const double *q) {
    double resultaat = 0.0;
    for (int i = 0; i < 26; i++) {
        resultaat += p[i] * q[i];
    }
    return resultaat;
}

/* Vergelijk de letterfrequenties van twee teksten.
 *
 * fn_tekst1, fn_tekst2: de bestandsnamen van de teksten die vergeleken moeten worden.
 */
void opdracht_vergelijk(const char fn_tekst1[], const char fn_tekst2[]) {
    char bericht1[MAX_GROOTTE];
    char bericht2[MAX_GROOTTE];
    lees_bestand(fn_tekst1, bericht1);
    lees_bestand(fn_tekst2, bericht2);
    versimpel(bericht1);
    versimpel(bericht2);
    double p[26] = {0};
    double q[26] = {0};
    bepaal_frequenties(bericht1, p);
    bepaal_frequenties(bericht2, q);

    printf("Gelijkenis is %.3f\n", gelijkenis(p, q));
}

/* Zoek de beste sleutel voor een gegeven cijfertekst en frequenties.
 *
 * cijfertekst: de versleutelde tekst.
 * sleutel: een buffer waarin de gevonden sleutel wordt opgeslagen.
 * slen: de lengte van de sleutel.
 * frequenties_taal: de referentiefrequenties van de taal.
 *
 * Retourneert: de beste gevonden gelijkenisscore.
 */
double zoek_sleutel(const char *cijfertekst, char *sleutel, size_t slen, const double *frequenties_taal) {
    for (size_t i = 0; i < slen; i++) {
        sleutel[i] = 'a';
    }
    sleutel[slen] = '\0';

    double beste_score = 0.0;
    char beste_sleutel[slen + 1];
    strcpy(beste_sleutel, sleutel);

    char ontsleuteld[MAX_GROOTTE];

    for (size_t positie = 0; positie < slen; positie++) {
        for (char letter = 'a'; letter <= 'z'; letter++) {
            sleutel[positie] = letter;
            strcpy(ontsleuteld, cijfertekst);
            vigenere_ontsleutel(ontsleuteld, sleutel);

            double frequenties_bericht[26] = {0};
            bepaal_frequenties(ontsleuteld, frequenties_bericht);

            double score = gelijkenis(frequenties_bericht, frequenties_taal);
            if (score > beste_score) {
                beste_score = score;
                strcpy(beste_sleutel, sleutel);
            }
        }
        strcpy(sleutel, beste_sleutel);
    }

    strcpy(sleutel, beste_sleutel);
    return beste_score;
}

/* Kraak een Vigenère-versleuteld bericht door de beste sleutel te zoeken.
 *
 * fn_cijfertekst: de bestandsnaam van de versleutelde tekst.
 * fn_taal: de bestandsnaam van de referentietekst voor frequenties.
 */
void opdracht_kraak(const char *fn_cijfertekst, const char *fn_taal) {
    char cijfertekst[MAX_GROOTTE];
    lees_bestand(fn_cijfertekst, cijfertekst);
    versimpel(cijfertekst);

    double frequenties_taal[26] = {0};
    char taal_tekst[MAX_GROOTTE];
    lees_bestand(fn_taal, taal_tekst);
    versimpel(taal_tekst);
    bepaal_frequenties(taal_tekst, frequenties_taal);

    double beste_score = 0.0;
    char beste_sleutel[20];

    for (int lengte = 1; lengte <= 15; lengte++) {
        char sleutel[16] = {0};
        double score = zoek_sleutel(cijfertekst, sleutel, (size_t)lengte, frequenties_taal);
        printf("Sleutel %s: score=%.3f\n", sleutel, score);
        if (score > beste_score) {
            beste_score = score;
            strcpy(beste_sleutel, sleutel);
        }
    }

    printf("sleutel: %s\n", beste_sleutel);
    vigenere_ontsleutel(cijfertekst, beste_sleutel);
    printf("bericht: %s\n", cijfertekst);
}

/* Drukt gebruiksinformatie 
af en beeindigt het programma met een foutcode.
 */
void gebruik(void) {
    fprintf(stderr, "gebruik: vigenere caesar_versleutel <bericht>\n"
                    "         vigenere caesar_ontsleutel <bericht>\n"
                    "         vigenere versleutel <bericht> <sleutel>\n"
                    "         vigenere ontsleutel <cijfertekst> <sleutel>\n"
                    "         vigenere frequenties <tekst>\n"
                    "         vigenere vergelijk <tekst1> <tekst2>\n"
                    "         vigenere kraak <cijfertekst> <voorbeeld>\n");
    exit(EXIT_FAILURE);
}


#ifndef COMPILE_MAIN
int main(int argc, char *argv[]) {

    if (argc < 2 || argc > 4) {
        gebruik();
    }

    const char *opdr = argv[1];
    if (strcmp(opdr, "caesar_versleutel")==0 && argc==3) {
        opdracht_caesar_versleutel(argv[2]);
    } else if (strcmp(opdr, "caesar_ontsleutel")==0 && argc==3) {
        opdracht_caesar_ontsleutel(argv[2]);
    } else if (strcmp(opdr, "versleutel")==0 && argc==4) {
        opdracht_vigenere_versleutel(argv[2], argv[3]);
    } else if (strcmp(opdr, "ontsleutel")==0 && argc==4) {
        opdracht_vigenere_ontsleutel(argv[2], argv[3]);
    } else if (strcmp(opdr, "frequenties")==0 && argc==3) {
        opdracht_frequenties(argv[2]);
    } else if (strcmp(opdr, "vergelijk")==0 && argc==4) {
        opdracht_vergelijk(argv[2], argv[3]);
    } else if (strcmp(opdr, "kraak")==0 && argc==4) {
        opdracht_kraak(argv[2], argv[3]);
    } else {
        gebruik();
    }

    return EXIT_SUCCESS;
}
#endif
