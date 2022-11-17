#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>
#include <locale.h>
#include <wchar.h>

#define FRUITS_FILE "commande_fruits.txt"
#define MIELS_AMANDES_FILE "commande_miels_amandes.txt"
#define VALUES_AGRUMES_FILE "values_agrumes.bin"
#define VALUES_MIELS_FILE "values_miels_amandes.bin"

#define MIELS_AMANDES_READ_DATA                             \
	f = fopen(VALUES_MIELS_FILE, "r");                        \
                                                            \
	if (f != NULL)                                            \
	{                                                         \
		fread(&svd_amandes, sizeof(svd_amandes), 1, f);         \
		fread(&svd_miel_orange, sizeof(svd_miel_orange), 1, f); \
		fread(&svd_miel_citron, sizeof(svd_miel_citron), 1, f); \
		fread(&svd_tot_miels, sizeof(svd_tot_miels), 1, f);     \
	}                                                         \
                                                            \
	fclose(f);

#define MIELS_AMANDES_SAVE_DATA                            \
	f = fopen(VALUES_MIELS_FILE, "w");                       \
                                                           \
	fwrite(&svd_amandes, sizeof(svd_amandes), 1, f);         \
	fwrite(&svd_miel_orange, sizeof(svd_miel_orange), 1, f); \
	fwrite(&svd_miel_citron, sizeof(svd_miel_citron), 1, f); \
	fwrite(&svd_tot_miels, sizeof(svd_tot_miels), 1, f);     \
                                                           \
	fclose(f);

#define AGRUMES_READ_DATA                                   \
	f = fopen(VALUES_AGRUMES_FILE, "r");                      \
                                                            \
	if (f != NULL)                                            \
		for (uint8_t i = 0; i < NB_TYPE_CAGETTES; ++i)          \
			fread(&(svd_fruits[i]), sizeof(svd_fruits[i]), 1, f); \
                                                            \
	fread(&svd_tot_agrumes, sizeof(svd_tot_agrumes), 1, f);   \
                                                            \
	fclose(f);

#define AGRUMES_SAVE_DATA                                  \
	f = fopen(VALUES_AGRUMES_FILE, "w");                     \
                                                           \
	for (uint8_t i = 0; i < NB_TYPE_CAGETTES; ++i)           \
		fwrite(&(svd_fruits[i]), sizeof(svd_fruits[i]), 1, f); \
	fwrite(&svd_tot_agrumes, sizeof(svd_tot_agrumes), 1, f); \
                                                           \
	fclose(f);

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

static uint32_t svd_tot_agrumes = 0;
static uint32_t svd_fruits[NB_TYPE_CAGETTES];

static uint32_t svd_tot_miels;
static uint32_t svd_amandes;
static uint32_t svd_miel_orange;
static uint32_t svd_miel_citron;

void bon_commande_miels(void);
void bon_commande_fruits(void);
void stats(void);
void get_client_id(FILE *f);

int main(int argc, char **argv)
{
	--argc, ++argv;

	setlocale(LC_ALL, "C.UTF-8");

	uint8_t run = 1;

	while (run)
	{
		printf("commande d'agrummes ? ([O] (oui) / N (non)");
		char commande_type;
		scanf("%c", &commande_type);
		fflush(stdin);

		commande_type = toupper(commande_type);

		if (commande_type == 'Q')
		{
			run = 0;
			continue;
		}

		if (commande_type == 'O' || commande_type == '\n')
			bon_commande_fruits();
		else if (commande_type == 'N')
			bon_commande_miels();
		else if (commande_type == 'S')
			stats();
		else
		{
			printf("'%c', n'est pas une commande valide, les commandes valides sont: 'O' (oui), 'N'(non), 'S' (statistiques) et 'Q' (quitter)\n", commande_type);
		}
	}

	return EXIT_SUCCESS;
}

void bon_commande_miels(void)
{
	printf("BON DE COMMANDE DE MIELS ET D'AMANDES\n");

	FILE *f = fopen(MIELS_AMANDES_FILE, "a");

	get_client_id(f);

	char tmp = 0;

	uint32_t tmp2 = 0;
	printf("amandes : ");
	if (scanf("%1[\n]", &tmp) == 0)
		scanf("%u", &tmp2);
	fflush(stdin);
	uint32_t amandes = tmp2;

	printf("miel oranger : ");
	if (scanf("%1[\n]", &tmp) == 0)
		scanf("%u", &tmp2);
	fflush(stdin);
	uint32_t miel_orange = tmp2;

	printf("miel citronnier : ");
	if (scanf("%1[\n]", &tmp) == 0)
		scanf("%u", &tmp2);
	fflush(stdin);
	uint32_t miel_citron = tmp2;

	uint32_t tot_prix = miel_citron * prix_miel_citron + miel_orange * prix_miel_orange + amandes * prix_amandes;

	printf("le client doit payer %.1f\n", (double)tot_prix / 10);

	fprintf(f, "|%12u|%12u|%12u|%12.1f|\n", amandes, miel_orange, miel_citron, (double)tot_prix / 10);
	fprintf(f, "|----------------------------------------------------------------|------------------------------------------------------------------|----------------|----------------------------------------------------------------|------------|------------|------------|------------|\n");

	fclose(f);

	MIELS_AMANDES_READ_DATA;

	svd_amandes += amandes;
	svd_miel_orange += miel_orange;
	svd_miel_citron += miel_citron;
	svd_tot_miels += tot_prix;

	MIELS_AMANDES_SAVE_DATA

	return;
}

void bon_commande_fruits(void)
{
	printf("BON DE COMMANDE DE FRUITS\n");

	FILE *f = fopen(FRUITS_FILE, "a");

	get_client_id(f);

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

	fprintf(f, "|%-9u|%-9u|%-9u|%-9u|%-9u|%-9u|%-9u|%-9u|%-9u|%-9.1f|\n", commande[0], commande[1], commande[2], commande[3], commande[4], commande[5], commande[6], commande[7], tot_cagettes, (double)tot_prix / 10);

	fprintf(f, "|----------------------------------------------------------------|------------------------------------------------------------------|----------------|----------------------------------------------------------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|\n");

	fclose(f);

	AGRUMES_READ_DATA;

	for (uint8_t i = 0; i < NB_TYPE_CAGETTES; ++i)
		svd_fruits[i] += commande[i];

	svd_tot_agrumes += tot_prix;

	AGRUMES_SAVE_DATA;

	return;
}

void stats(void)
{
	FILE *f;

	MIELS_AMANDES_READ_DATA;

	AGRUMES_READ_DATA;

	printf("STATISTIQUES : \n");

	printf("Ventes de miels et d'amandes : \n");
	printf("|-------------------------------------------------------------------------------|\n");
	printf("|                    | amandes | miel d'oranger | miel de citronnier | total    |\n");
	printf("|--------------------|---------|----------------|--------------------|----------|\n");
	printf("| prix unitaire      |%-9.1f|%-16.1f|%-20.1f|          |\n", (double)prix_amandes / 10, (double)prix_miel_orange / 10, (double)prix_miel_citron / 10);
	printf("|--------------------|---------|----------------|--------------------|----------|\n");
	printf("|total achats (x500g)|%-9u|%-16u|%-20u|%-10u|\n", svd_amandes, svd_miel_orange, svd_miel_citron, svd_amandes + svd_miel_orange + svd_miel_citron);
	printf("|--------------------|---------|----------------|--------------------|----------|\n");
	printf("|total ventes        |%-9.1f|%-16.1f|%-20.1f|%-10.1f|\n", (double)(prix_amandes * svd_amandes) / 10, (double)(prix_miel_orange * svd_miel_orange) / 10,
				 (double)(prix_miel_citron * svd_miel_citron) / 10, (double)((prix_amandes * svd_amandes) + (prix_miel_orange * svd_miel_orange) + (prix_miel_citron * svd_miel_citron)) / 10);
	printf("|-------------------------------------------------------------------------------|\n");

	const uint8_t width[NB_TYPE_CAGETTES] = {9, 12, 15, 9, 23, 26, 20, 23};

	printf("\n\nVentes d'agrumes : \n");
	printf("|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
	printf("|                    | oranges | mandarines | pamplemousses | citrons | oranges et mandarines | oranges et pamplemousses | oranges et citrons | mandarines et citrons | total |\n");
	printf("|--------------------|---------|------------|---------------|---------|-----------------------|--------------------------|--------------------|-----------------------|-------|\n");
	printf("|prix unitaires      |");
	for (uint8_t i = 0; i < NB_TYPE_CAGETTES; ++i)
		printf("%-*.1f|", width[i], (double)prix[i] / 10);
	printf("       |\n");
	printf("|--------------------|---------|------------|---------------|---------|-----------------------|--------------------------|--------------------|-----------------------|-------|\n");
	printf("|total achats (x10kg)|");
	uint32_t tot = 0;
	for (uint8_t i = 0; i < NB_TYPE_CAGETTES; ++i)
	{
		printf("%-*u|", width[i], svd_fruits[i]);
		tot += svd_fruits[i];
	}
	printf("%-7u|\n", tot);
	printf("|--------------------|---------|------------|---------------|---------|-----------------------|--------------------------|--------------------|-----------------------|-------|\n");
	printf("|total ventes        |");
	tot = 0;
	for (uint8_t i = 0; i < NB_TYPE_CAGETTES; ++i)
	{
		printf("%-*.1f|", width[i], (double)(prix[i] * svd_fruits[i]) / 10);
		tot += prix[i] * svd_fruits[i];
	}
	printf("%-7.1f|\n", (double)tot / 10);
	printf("|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");

	return;
}

void get_client_id(FILE *f)
{
	// nom du client
	char nom[64 + 1] = {0};
	do
	{
		printf("nom : ");
		scanf("%64[^\n]", nom);
		fflush(stdin);
		if (nom[0] == 0)
			printf("Il faut fournir un nom pour le client.\n");
	} while (nom[0] == 0);

	// printf("le client s'appelle %s %s\n", prenom, nom);

	fprintf(f, "|%-64s|  ", nom);

	// adresse du client
	char adr[64 + 1] = {0};
	printf("adresse : ");
	scanf("%64[^\n]", adr);
	fflush(stdin);

	if (adr[0] == 0)
		printf("pas d'adresse\n");

	fprintf(f, "%-64s|", adr);

	// numero de telephone du client
	char tel[15] = {0};
	printf("tel : ");
	scanf("%14[^\n]", tel);
	fflush(stdin);

	if (tel[0] == 0)
		printf("pas de numero de telephone\n");
	// else
	//     printf("le numero de telephone du client est : %s\n", tel);

	fprintf(f, "%14s", tel);

	// e-mail du client
	char mail[64 + 1] = {0};
	printf("e-mail : ");
	scanf("%64[^\n]", mail);
	fflush(stdin);

	if (mail[0] == 0)
		printf("pas d'e-mail\n");
	// else
	//     printf("l'e-mail du client est %s\n", mail);

	fprintf(f, "  |  %-62s", mail);

	return;
}
