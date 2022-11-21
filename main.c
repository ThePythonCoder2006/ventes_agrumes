#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>

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

char cagette_names[NB_TYPE_CAGETTES][32] = {"oranges", "mandarines", "pamplemousses", "citrons", "oranges et mandarines", "oranges et pamplemousses", "oranges et citrons", "mandarines et citrons"};

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
uint8_t get_client_id(char name[65], char adr[65], char tel[15], char mail[65]);
void get_list(void);
void help(void);

int main(int argc, char **argv)
{
	--argc, ++argv;

	uint8_t run = 1;

	while (run)
	{
		printf("[LISTE DES COMMANDES]\n"
					 "c : nouvelles commande\n"
					 "s : statistiques\n"
					 "l : liste des commandes non-payee\n"
					 "h : aide\n"
					 "q : quitter\n");

		printf("> ");

		char action;
		scanf("%c", &action);

		if (toupper(action) == 'Q')
		{
			run = 0;
			continue;
		}

		if (toupper(action) == 'C')
		{
		start:
			printf("commande d'agrumes ? (O (oui) ou N (non))");
			scanf("%c", &action);
			if (action == '\n')
				scanf("%c", &action);
			fflush(stdin);

			if (toupper(action) == 'O' || toupper(action) == 'A')
				bon_commande_fruits();
			else if (toupper(action) == 'N' || toupper(action) == 'M')
				bon_commande_miels();
			else if (toupper(action) == 'Q')
				continue;
			else
			{
				printf("'%s%c' n'est pas un reponse valide, il faut repondre par 'o' (oui) ou 'n' (non) ou alors 'a' (agrumes) ou 'm' (miels et amandes) ou 'q' (quitter)\n", action == '\n' ? "[enter]" : "", action != '\n' ? action : 0);
				goto start;
			}
		}
		else
		{
			fflush(stdin);

			switch (toupper(action))
			{
			case 'S':
				stats();
				break;

			case 'L':
				get_list();
				break;

			case 'H':
				help();
				break;

			default:
				printf("'%s%c', n'est pas une commande valide, faire 'h' pour avoir de l'aide sur les commandes\n", action == '\n' ? "[enter]" : "", action != '\n' ? action : 0);
				break;
			}
		}
	}

	return EXIT_SUCCESS;
}

void bon_commande_miels(void)
{
	printf("\nBON DE COMMANDE DE MIELS ET D'AMANDES\n");

	FILE *f = fopen(MIELS_AMANDES_FILE, "a");

	char name[64 + 1], adr[64 + 1], tel[14 + 1], mail[64 + 1];

	if (get_client_id(name, adr, tel, mail))
		return;

	char tmp = 0;

	uint32_t tmp2 = 0;

	uint32_t amandes = 0;
	printf("amandes : ");
	if (scanf("%1[\n]", &tmp) == 0)
	{
		scanf("%u", &tmp2);
		amandes = tmp2;
	}
	fflush(stdin);

	uint32_t miel_orange = 0;
	printf("miel oranger : ");
	if (scanf("%1[\n]", &tmp) == 0)
	{
		scanf("%u", &tmp2);
		miel_orange = tmp2;
	}
	fflush(stdin);

	uint32_t miel_citron = 0;
	printf("miel citronnier : ");
	if (scanf("%1[\n]", &tmp) == 0)
	{
		scanf("%u", &tmp2);
		miel_citron = tmp2;
	}
	fflush(stdin);

	uint32_t tot_prix = miel_citron * prix_miel_citron + miel_orange * prix_miel_orange + amandes * prix_amandes;

	if (tot_prix == 0)
	{
		printf("le client n'a pas passe commande, elle n'a pas ete enregistree.\n");
		return;
	}

	printf("le client doit payer %.1f\n", (double)tot_prix / 10);

	char pay;
	printf("le client a-t-il paye ? (O (oui) ou N (non))");
	scanf("%c", &pay);
	fflush(stdin);
	while (toupper(pay) != 'O' && toupper(pay) != 'N' && pay != '\n')
	{
		printf("Il faut que vous repondiez soit 'O' soit 'N' soit rien\n");
		printf("le client a-t-il paye ? (O (oui) ou N (non))");
		scanf("%c", &pay);
	}

	if (pay == '\n')
		pay = 'N';
	else
		pay = toupper(pay);

	fprintf(f, "|  %-62s" // name
						 "|  %-64s" // adresse
						 "|%-14s  " // telephone
						 "|  %-62s" // e-mail
						 "|%-12u"
						 "|%-12u"
						 "|%-12u"
						 "|%-12.1f"
						 "|  %c  |\n",
					name, adr, tel, mail, amandes, miel_orange, miel_citron, (double)tot_prix / 10, pay);
	fprintf(f, "|----------------------------------------------------------------|------------------------------------------------------------------|----------------|----------------------------------------------------------------|------------|------------|------------|------------|-----|\n");

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
	printf("\nBON DE COMMANDE D'AGRUMES\n");

	FILE *f = fopen(FRUITS_FILE, "a");

	char name[64 + 1], adr[64 + 1], tel[14 + 1], mail[64 + 1];

	if (get_client_id(name, adr, tel, mail))
		return;

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

	if (tot_prix == 0)
	{
		printf("le client n'a pas passe commande, elle n'a pas ete enregistree.\n");
		return;
	}

	printf("le client a pris %u cagette%c et le total et de %.1f euro%c\n", tot_cagettes, tot_cagettes > 1 ? 's' : 0, (double)tot_prix / 10, tot_prix > 1 ? 's' : 0);

	char pay;
	printf("le client a-t-il paye ? (O (oui) ou N (non))");
	scanf("%c", &pay);
	fflush(stdin);
	while (toupper(pay) != 'O' && toupper(pay) != 'N' && pay != '\n')
	{
		printf("Il faut que vous repondiez soit 'O' soit 'N' soit rien\n");
		printf("le client a-t-il paye ? (O (oui) ou N (non))");
		scanf("%c", &pay);
	}

	if (pay == '\n')
		pay = 'n';
	else
		pay = toupper(pay);

	// saving the data

	fprintf(f, "|  %-62s" // nom
						 "|  %-64s" // adresse
						 "|%-14s  " // telephone
						 "|  %-62s" // e-mail
					,
					name, adr, tel, mail);

	for (uint8_t i = 0; i < NB_TYPE_CAGETTES; ++i)
		fprintf(f, "|%-9u", commande[i]);

	fprintf(f, "|%-9u"
						 "|%-9.1f"
						 "|  %c  |\n",
					tot_cagettes, (double)tot_prix / 10, pay);

	fprintf(f, "|----------------------------------------------------------------|------------------------------------------------------------------|----------------|----------------------------------------------------------------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|-----|\n");

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

	if (svd_amandes > svd_miel_citron && svd_amandes > svd_miel_orange)
		printf("le produit le plus vendu est les amandes\n");
	// amandes <= miel citron et amandes <= miel orange
	else if (svd_amandes == svd_miel_citron)
		if (svd_amandes == svd_miel_orange)
			if (svd_amandes > 0)
				printf("les produits les plus vendus sont les amandes, le miel d'orangier et le miel de citronnier\n");
			else
				printf("il n'y a pas eu de vente de meils et d'amandes pour l'instant :(\n");
		else
			printf("les produits les plus vendus sont les amandes et le miel de citronnier\n");
	else if (svd_amandes == svd_miel_orange)
		printf("les produits les plus vendus sont les amandes et le miel d'orangier\n");
	// amandes < miel citron et amandes < miel orange
	else if (svd_miel_citron > svd_miel_orange)
		printf("le produit le plus vendu est le miel de citronnier\n");
	else if (svd_miel_citron == svd_miel_orange)
		printf("les produits les plus vendus sont le miel d'orangier et le miel de citronnier\n");
	else if (svd_miel_orange > svd_miel_citron)
		printf("le produit le plus vendu est le miel d'orangier\n");
	else
		printf("[ERROR] unreachable path\n");

	printf("appuyer sur [enter] pour continuer...");
	scanf("%*c"); // attendre que l'utilisateur ait lu les donnees
	fflush(stdin);

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

	uint8_t high[NB_TYPE_CAGETTES] = {0};
	uint32_t max = 0;

	for (uint8_t i = 0; i < NB_TYPE_CAGETTES; ++i)
	{
		if (svd_fruits[i] >= max)
		{
			if (svd_fruits[i] == max)
				high[i] = 1;
			else
			{
				memset(high, 0, NB_TYPE_CAGETTES * sizeof(high[0]));
				high[i] = 1;
				max = svd_fruits[i];
			}
		}
	}

	if (max == 0)
		printf("il n'y a pas encore eu de vente d'agrumes pour l'instant :(\n");
	else
	{
		cagette high_cagette[NB_TYPE_CAGETTES];
		uint8_t max_high_cagette = 0;

		for (uint8_t i = 0; i < NB_TYPE_CAGETTES; ++i)
		{
			if (high[i])
				high_cagette[max_high_cagette++] = i;
		}

		if (max_high_cagette > 1)
		{
			printf("les produits les plus vendus sont la cagette d%s%s", cagette_names[high_cagette[0]][0] == 'o' ? "'" : "e ", cagette_names[high_cagette[0]]);
			for (uint8_t i = 1; i < max_high_cagette - 1; ++i)
				printf(", la cagette d%s%s", cagette_names[high_cagette[i]][0] == 'o' ? "'" : "e ", cagette_names[high_cagette[i]]);
			printf(" et la cagette de %s\n", cagette_names[high_cagette[max_high_cagette - 1]]);
		}
		else
			printf("le produit le plus vendu est la cagette de %s\n", cagette_names[high_cagette[0]]);
	}

	printf("appuyer sur [enter] pour revenir au menu principal...");
	scanf("%*c"); // attendre que l'utilisateur ait lu les donnees
	fflush(stdin);

	return;
}

uint8_t get_client_id(char name_ptr[65], char adr_ptr[65], char tel_ptr[15], char mail_ptr[65])
{
	char tmp;

	// nom du client
	char nom[64 + 1] = {0};
	do
	{
		printf("nom : ");
		scanf("%64[^\n]", nom);
		fflush(stdin);
		if (nom[0] == 0)
			printf("Il faut fournir un nom pour le client.\n");
		if (toupper(nom[0]) == 'Q' && strlen(nom) == 1)
		{
			printf("voulez vous vraiment quitter la commande en cours (rien ne sera enregistre) ? (O (oui) ou [N] (non)");
			scanf("%c", &tmp);
			fflush(stdin);
			if (toupper(tmp) == 'O')
				return 1;
			else if (toupper(tmp) != 'N' && tmp != '\n')
				printf("vous avez repondu '%c', ce qui n'est pas une reponse valide ('O'(oui) ou 'N' (non)), la commande va donc continuer...\n", tmp);
		}
	} while (nom[0] == 0);

	// printf("le client s'appelle %s %s\n", prenom, nom);

	// fprintf(f, "|  %-62s|  ", nom);

	strcpy(name_ptr, nom);

	// adresse du client
	char adr[64 + 1] = {0};
	printf("adresse : ");
	scanf("%64[^\n]", adr);
	fflush(stdin);
	if (adr[0] == 0)
		printf("pas d'adresse\n");
	if (toupper(adr[0]) == 'Q' && strlen(adr) == 1)
	{
		printf("voulez vous vraiment quitter la commande en cours (rien ne sera enregistre) ? (O (oui) ou [N] (non)");
		scanf("%c", &tmp);
		fflush(stdin);
		if (toupper(tmp) == 'O')
			return 1;
		else if (toupper(tmp) != 'N' && tmp != '\n')
			printf("vous avez repondu '%c', ce qui n'est pas une reponse valide ('O'(oui) ou 'N' (non)), la commande va donc continuer...\n", tmp);
	}

	// fprintf(f, "%-64s|", adr);

	strcpy(adr_ptr, adr);

	// numero de telephone du client
	char tel[14 + 1] = {0};
	printf("tel : ");
	scanf("%*[ ]");
	scanf("%14[^\n]", tel);
	fflush(stdin);
	if (tel[0] == 0)
		printf("pas de numero de telephone\n");
	if (toupper(tel[0]) == 'Q' && strlen(tel) == 1)
	{
		printf("voulez vous vraiment quitter la commande en cours (rien ne sera enregistre) ? (O (oui) ou [N] (non)");
		scanf("%c", &tmp);
		fflush(stdin);
		if (toupper(tmp) == 'O')
			return 1;
		else if (toupper(tmp) != 'N' && tmp != '\n')
			printf("vous avez repondu '%c', ce qui n'est pas une reponse valide ('O'(oui) ou 'N' (non)), la commande va donc continuer...\n", tmp);
	}
	// else
	//     printf("le numero de telephone du client est : %s\n", tel);

	// fprintf(f, "%-14s", tel);

	strcpy(tel_ptr, tel);

	// e-mail du client
	char mail[64 + 1] = {0};
	printf("e-mail : ");
	scanf("%64[^\n]", mail);
	fflush(stdin);
	if (mail[0] == 0)
		printf("pas d'e-mail\n");
	if (toupper(mail[0]) == 'Q' && strlen(mail) == 1)
	{
		printf("voulez vous vraiment quitter la commande en cours (rien ne sera enregistre) ? (O (oui) ou [N] (non)");
		scanf("%c", &tmp);
		fflush(stdin);
		if (toupper(tmp) == 'O')
			return 1;
		else if (toupper(tmp) != 'N' && tmp != '\n')
			printf("vous avez repondu '%c', ce qui n'est pas une reponse valide ('O'(oui) ou 'N' (non)), la commande va donc continuer...\n", tmp);
	}
	// else
	//     printf("l'e-mail du client est %s\n", mail);

	// fprintf(f, "  |  %-62s", mail);

	strcpy(mail_ptr, mail);

	return 0;
}

void get_list(void)
{
	char choice;
	printf("le client a-t-il commande des agrumes ? [O] oui / N (non)");
	scanf("%c", &choice);
	fflush(stdin);

	char name[62 + 1], tel[14 + 1], mail[62 + 1];
	char prix_str[9 + 1];
	char prix_end, pay, tmp;

	if (toupper(choice) == 'O' || toupper(choice) == '\n')
	{
		printf("client n'ayant pas paye leur commande d'agrumes :\n");

		FILE *f = fopen(FRUITS_FILE, "r");

		if (f == NULL)
		{
			printf("erreur lors de l'ouverture du fichier \"" FRUITS_FILE "\"");
			return;
		}

		uint32_t cagettes;

		if (fscanf(f, "%*[^\n]%*c%*[^\n]%*c") == EOF)
		{
			fclose(f);
			return;
		}

		do
		{
			if (fscanf(f, "%*[^\n]%*c%*c") == EOF) // first line + first '|'
			{
				fclose(f);
				return;
			}

			if (fscanf(f, "%*2c"				// remove "  "
										"%62c"				// get name
										"%*68c"				// remove adress
										"%14c"				// get phone number
										"%*5c"				// remove "  |  "
										"%62c"				// get e-mail
										"%*81c"				//
										"%u"					// get cagettes
										"%*[^|]%*c"		//
										"%8[^.]"			// get prix_str
										"%*c"					// remove the '.'
										"%c"					// get prix_end
										"%*[^|]%*3c"	//
										"%c"					// get pay
										"%*[^\n]%*c", // finish the line and remove '\n'
								 name, tel, mail, &cagettes, prix_str, &prix_end, &pay) == EOF)
			{
				fclose(f);
				return;
			}

			if (pay == 'N')
			{
				uint8_t i = 62;

				while (name[i] == ' ' || name[i] == '\0')
					--i;

				name[i + 1] = '\0';

				i = 62;

				while (mail[i] == ' ' || name[i] == '\0')
				{
					--i;
					if (i == 0)
					{
						mail[0] = '\0';
						break;
					}
				}

				mail[i + 1] = '\0';

				if (tel[0] == ' ')
					tel[0] = '\0';

				printf("%s  "
							 "%s%s%s"
							 "%s%s%s"
							 "|  %u cagettes  |  %s.%c",
							 name,
							 tel[0] != '\0' ? "|  " : "", tel, tel[0] != '\0' ? "  " : "",
							 mail[0] != '\0' ? "|  " : "", mail, mail[0] != '\0' ? "  " : "",
							 cagettes, prix_str, prix_end);
				scanf("%c", &tmp);
				fflush(stdin);
			}
			else
				continue;
		} while (toupper(tmp) != 'Q');

		fclose(f);
	}
	else if (toupper(choice) == 'N')
	{
		printf("client n'ayant pas paye leur commande de miels et d'amandes\n");

		FILE *f = fopen(MIELS_AMANDES_FILE, "r");

		if (f == NULL)
		{
			printf("erreur lors de l'ouverture du fichier \"" MIELS_AMANDES_FILE "\"");
			return;
		}

		if (fscanf(f, "%*[^\n]%*c%*[^\n]%*c") == EOF)
		{
			fclose(f);
			return;
		}

		do
		{
			if (fscanf(f, "%*[^\n]%*c%*c") == EOF) // first line + first '|'
			{
				fclose(f);
				return;
			}

			if (fscanf(f, "%*2c"				// remove "  "
										"%62c"				// get name
										"%*68c"				// remove adresse
										"%14c"				// get phone number
										"%*5c"				// remove "  |  "
										"%62c"				// get e-mail
										"%*40c"				//
										"%8[^.]"			// get prix_str
										"%*c"					// remove the '.'
										"%c"					// get prix_end
										"%*[^|]%*3c"	//
										"%c"					// get pay
										"%*[^\n]%*c", // finish the line and remove '\n'
								 name, tel, mail, prix_str, &prix_end, &pay) == EOF)
			{
				fclose(f);
				return;
			}

			if (pay == 'N')
			{
				uint8_t i = 62;

				while (name[--i] == ' ' || name[i] == '\0')
					;

				name[i + 1] = '\0';

				i = 62;

				while (mail[--i] == ' ' || name[i] == '\0')
					if (i == 0)
					{
						mail[0] = '\0';
						break;
					}

				mail[i + 1] = '\0';

				if (tel[0] == ' ')
					tel[0] = '\0';

				printf("%s  "
							 "%s%s%s"
							 "%s%s%s"
							 "|  %s.%c",
							 name,
							 tel[0] != '\0' ? "|  " : "", tel, tel[0] != '\0' ? "  " : "",
							 mail[0] != '\0' ? "|  " : "", mail, mail[0] != '\0' ? "  " : "",
							 prix_str, prix_end);
				scanf("%c", &tmp);
				fflush(stdin);
			}
			else
				continue;

		} while (toupper(tmp) != 'Q');

		fclose(f);
	}
	else if (toupper(choice) == 'Q')
		return;
	else
		printf("'%c' n'est pas une reponse valide, il faut repondre 'O' ou 'N' ou rien\n", choice);

	return;
}

void help(void)
{
	char action;
	printf("Sur quelle commande souhaitez vous de l'aide ? (c, s, l, h ou q)\n> ");
	scanf("%c", &action);
	fflush(stdin);

	switch (toupper(action))
	{
	case 'C':
		printf("entrer une commande :\n"
					 "vous permet d'entrer un commande, elle sera enregistree si le client a commande quelque chose. il est possible de quitter la commande en cours e nrepondant 'q'. il est possible de se rendre directement dans un des bon de commande depuis le menu principal en tapant \"ca\" (ou \"co\") pour remplir une commande d'agrumes et en tapant \"cm\" (ou \"cn\") pour remplir une commande de miels et d'amandes\n"
					 "champs a remplir : (les champs avec un asteriques (*) sont obligatoires)\n"
					 "\t- quel type de commande le client passe-t-il ? (miel et amandes ou agrumes) (*)\n"
					 "\t- nom du client (*)\n"
					 "\t- adresse du client\n"
					 "\t- numero de telephone de client\n"
					 "\t- adresse e-mail (courriel) du client\n"
					 "il faudra ensuite repondre par un chiffre ou ne pas repondre (ce qui correspond a repondre 0) a tout les type de produit que le client pourrait vouloir, en fonction de quel type de commande a ete choisi (miels et amandes ou agrumes)\n");
		break;

	case 'S':
		printf("statistiques :\n"
					 "affiche des statistique sur les ventes qui on deja eu lieu. Ces statistiques sont separer par type de commande (meils et amandes ou agrumes)\n"
					 "les statistiques afficher sont :\n"
					 "\t- le total vendu en poids par categorie de produit\n"
					 "\t- le total vendu en poids\n"
					 "\t- la recette total par categorie de produit\n"
					 "\t- la recette total \n");
		break;

	case 'L':
		printf("liste des commandes non-payee :\n"
					 "affiche la liste des commandes non-payee. Il faut appuyer sur la touche [enter] pour voir la commande suivant, si la dernière commande a ete atteinte, la commande se termine et vous etes renvoyer au menu principal, vous pouvez a ce moment la ecrire 'q' et appuyer sur la touche [enter] pour revenir au menu principal sans continuer ni voir les commandes suivantes\n"
					 "les informations presentes sont : (les champs marquees par un asterisque (*) ne seront pas presents s'il n'ont pas ete renseigne lors de la prise de la commande)"
					 "\t- le nom et prenom du client\n"
					 "\t- le numero de telephone du client (*)\n"
					 "\t- l'adresse e-mail (couriel) du client (*)\n"
					 "\t- le prix de la commande\n");
		break;

	case 'H':
		printf("affiche ce menu d'aide...");
		break;

	case 'Q':
		printf("quitter : \n"
					 "permet de quitter la procedure actuelle.\n"
					 "La commande 'q' peut etre utiliser dans plusieurs endroit different : \n"
					 "\t- dans le menu principal pour quitter le logiciel\n"
					 "\t- dans les bon de commande, du debut et durant tout la phase de prise des information du client\n"
					 "\t- dans la commande 'l' pour quitter et revenir au menu principal\n");
		break;

	default:
		printf("'%s%c' n'est pas une commande valide, il faut repondre par 'c', 's', 'l', 'h' ou 'q'", action == '\n' ? "[enter]" : "", action != '\n' ? action : 0);
		break;
	}

	printf("appuyer sur [enter] pour continuer...");
	scanf("%*c");
	fflush(stdin);

	return;
}