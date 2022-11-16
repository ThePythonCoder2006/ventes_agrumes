#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>

#define FRUITS_FILE "commande_fruits.txt"
#define MIELS_AMANDES_FILE "commande_miels_amandes.txt"

typedef enum
{
    ORANGES,
    MANDARINES,
    PAMPLEMOUSSES,
    CITRONS,
    ORANGES_MANDARINES,
    ORANGES_PAMPLEMOUSES,
    ORANGES_CITRONS,
    MANDARINES_CITRONS,
    NB_TYPE_CAGETTES
} cagette;

const uint32_t prix[NB_TYPE_CAGETTES] = {240, 270, 270, 300, 265, 255, 275, 290};

const uint32_t prix_miel_orange = 115;
const uint32_t prix_miel_citron = 115;
const uint32_t prix_amandes = 125;

void bon_commande_miels(void);
void bon_commande_fruits(void);

int main(int argc, char **argv)
{
    --argc, ++argv;

    printf("commande de fruits ? ([O] (oui) / N (non)");
    char commande_type;
    scanf("%c", &commande_type);
    fflush(stdin);

    commande_type = toupper(commande_type);

    if (commande_type == 'O' || commande_type == '\n')
    {
        bon_commande_fruits();
    }
    else if (commande_type == 'N')
    {
        bon_commande_miels();
    }

    return EXIT_SUCCESS;
}

void bon_commande_miels(void)
{
    printf("BON DE COMMANDE DE MIELS ET D'AMANDES\n");

    FILE *f = fopen(MIELS_AMANDES_FILE, "a");

    // nom + prenom du client
    char prenom[64 + 1] = {0};
    char nom[64 + 1] = {0};
    do
    {
        printf("nom : ");
        scanf("%64[^ ^\n]", prenom);
        scanf("%64[^\n]", nom);
        fflush(stdin);
        if (prenom[0] == 0 || nom[0] == 0)
            printf("Il faut fournir un prenom et un nom pour le client.\n");
    } while (prenom[0] == 0 || nom[0] == 0);

    printf("le client s'appelle %s %s\n", prenom, nom);

    fprintf(f, "|%32s%-32s|  ", prenom, nom);

    // numero de telephone du client
    char tel[15] = {0};
    printf("tel : ");
    scanf("%14[^\n]", tel);
    fflush(stdin);

    if (tel[0] == 0)
        printf("pas de numero de telephone\n");
    else
        printf("le numero de telephone du client est : %s\n", tel);

    fprintf(f, "%14s", tel);

    // e-mail du client
    char mail[64 + 1] = {0};
    printf("e-mail : ");
    scanf("%64[^\n]", mail);
    fflush(stdin);

    if (mail[0] == 0)
        printf("pas d'e-mail\n");
    else
        printf("l'e-mail du client est %s\n", mail);

    char tmp;

    uint32_t amandes;
    printf("amandes : ");
    if (scanf("%1[\n]", &tmp) == 0)
        scanf("%u", &amandes);
    fflush(stdin);

    uint32_t miel_orange;
    printf("miel orange : ");
    if (scanf("%1[\n]", &tmp) == 0)
        scanf("%u", &miel_orange);
    fflush(stdin);

    uint32_t miel_citron;
    printf("miel citron : ");
    if (scanf("%1[\n]", &tmp) == 0)
        scanf("%u", &miel_citron);
    fflush(stdin);

    uint32_t tot_prix = miel_citron * prix_miel_citron + miel_orange * prix_miel_orange + amandes * prix_amandes;

    printf("le client doit payer %.1f", (double)tot_prix / 10);

    fclose(f);

    return;
}

void bon_commande_fruits(void)
{
    printf("BON DE COMMANDE DE FRUITS\n");

    FILE *f = fopen(FRUITS_FILE, "a");

    // nom + prenom du client
    char prenom[64 + 1] = {0};
    char nom[64 + 1] = {0};
    do
    {
        printf("nom : ");
        scanf("%64[^ ^\n]", prenom);
        scanf("%64[^\n]", nom);
        fflush(stdin);
        if (prenom[0] == 0 || nom[0] == 0)
            printf("Il faut fournir un prenom et un nom pour le client.\n");
    } while (prenom[0] == 0 || nom[0] == 0);

    printf("le client s'appelle %s %s\n", prenom, nom);

    fprintf(f, "|%32s%-32s|  ", prenom, nom);

    // numero de telephone du client
    char tel[15] = {0};
    printf("tel : ");
    scanf("%14[^\n]", tel);
    fflush(stdin);

    if (tel[0] == 0)
        printf("pas de numero de telephone\n");
    else
        printf("le numero de telephone du client est : %s\n", tel);

    fprintf(f, "%14s", tel);

    // e-mail du client
    char mail[64 + 1] = {0};
    printf("e-mail : ");
    scanf("%64[^\n]", mail);
    fflush(stdin);

    if (mail[0] == 0)
        printf("pas d'e-mail\n");
    else
        printf("l'e-mail du client est %s\n", mail);

    // prise de la commande en cagettes
    uint32_t commande[NB_TYPE_CAGETTES] = {0};

    char tmp;

    printf("oranges : ");
    if (scanf("%1[\n]", &tmp) == 0)
        scanf("%u", &commande[ORANGES]);
    fflush(stdin);

    printf("mandarines : ");
    if (scanf("%1[\n]", &tmp) == 0)
        scanf("%u", &commande[MANDARINES]);
    fflush(stdin);

    printf("pamplemousses : ");
    if (scanf("%1[\n]", &tmp) == 0)
        scanf("%u", &commande[PAMPLEMOUSSES]);
    fflush(stdin);

    printf("citrons : ");
    if (scanf("%1[\n]", &tmp) == 0)
        scanf("%u", &commande[CITRONS]);
    fflush(stdin);

    printf("oranges et mandarines : ");
    if (scanf("%1[\n]", &tmp) == 0)
        scanf("%u", &commande[ORANGES_MANDARINES]);
    fflush(stdin);

    printf("oranges et pamplemousses : ");
    if (scanf("%1[\n]", &tmp) == 0)
        scanf("%u", &commande[ORANGES_PAMPLEMOUSES]);
    fflush(stdin);

    printf("oranges et citrons : ");
    if (scanf("%1[\n]", &tmp) == 0)
        scanf("%u", &commande[ORANGES_CITRONS]);
    fflush(stdin);

    printf("mandarines et citrons : ");
    if (scanf("%1[\n]", &tmp) == 0)
        scanf("%u", &commande[MANDARINES_CITRONS]);
    fflush(stdin);

    uint32_t tot_cagettes = 0;
    uint32_t tot_prix = 0;

    for (uint8_t i = 0; i < NB_TYPE_CAGETTES; ++i)
    {
        tot_cagettes += commande[i];
        tot_prix += commande[i] * prix[i];
    }

    printf("le client a pris %u cagette%c et le total et de %.1f euro%c\n", tot_cagettes, tot_cagettes > 1 ? 's' : 0, (double)tot_prix / 10, tot_prix > 1 ? 's' : 0);

    // saving the data

    fprintf(f, "  |  %-62s|%9u|%9u|%9u|%9u|%9u|%9u|%9u|%9u|%9u|%9.1f|\n", mail, commande[0], commande[1], commande[2], commande[3], commande[4], commande[5], commande[6], commande[7], tot_cagettes, (double)tot_prix / 10);

    fprintf(f, "|----------------------------------------------------------------|------------------|----------------------------------------------------------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|\n");

    fclose(f);

    return;
}