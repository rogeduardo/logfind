#include <stdio.h>
#include <stdbool.h>
#include <glob.h>
#include <string.h>
#include <stdlib.h>
#include "dbg.h"
#include "macro.h"

#define MAX_LINE 1024

typedef struct keyword_line
{
	char *keyword;
	int line;
} keyword_line;

void removeinvalidchar(char *str, char invchar)
{
	char *pos = strchr(str, invchar);
	if (pos)
	{
		debug("Found a invalid character in %s at %ld\n", str, (pos - str));
		*pos = '\0';
	}
}

void searchAND(char *pathfile, char **keywordsv, int keywordsc)
{
	FILE *fp = NULL;
	char readline[MAX_LINE], *token = NULL;
	// searchctl is the control to known if all keywords was found
	unsigned int searchctl[MAX_KEYWORDS], size_kl = 0;
	// Store the position where keywords was found
	keyword_line *kl = NULL;

	debug ("Search on this file %s\n", pathfile);
	fp = fopen(pathfile, "r");
	check (fp, "Failed to open %s\n", pathfile);
	int line = 1;

	memset(&searchctl, 0, sizeof(int)*MAX_KEYWORDS);
	while (fgets (readline, MAX_LINE, fp))
	{
		for (int i = 0; i < keywordsc; i++)
		{
			token = strtok(readline, " ");
			while (token)
			{
				debug("%s\n", token);
				if (!strcmp (token, keywordsv[i]))
				{
					size_kl++;
					kl = (keyword_line *) realloc (kl, sizeof(keyword_line)*size_kl);
					check_mem (kl);
					kl[size_kl - 1].keyword = keywordsv[i];
					kl[size_kl - 1].line = line;
					searchctl[i]++;
				}
				token = strtok(NULL, " ");
			}
			free(token);
			token = NULL;
		}
		line++;
	}

	for ( int i = 0; i < keywordsc; i++ )
	{
		//If any keyword was not found on the file, dont print nothing.
		if (!searchctl[i])
			goto error;
	}

	for ( int i = 0; i < size_kl; i++ )
	{
		printf("Found a keyword %s at %s, line %d!\n", kl[i].keyword, pathfile, kl[i].line);
	}

	error:
		if (fp)
			fclose (fp);
		if (token)
			free (token);
		if (kl)
			free (kl);
}

void searchOR(char *pathfile, char **keywordsv, int keywordsc)
{
	FILE *fp = NULL;
	char readline[MAX_LINE], *token = NULL;

	debug ("Search on this file %s\n", pathfile);
	fp = fopen(pathfile, "r");
	check (fp, "Failed to open %s\n", pathfile);
	int line = 1;

	while (fgets (readline, MAX_LINE, fp))
	{
		for (int i = 0; i < keywordsc; i++)
		{
			token = strtok(readline, " ");
			while (token)
			{
				debug("%s\n", token);
				if (!strcmp(token, keywordsv[i]))
					printf("Found a keyword %s at %s, line %d!\n", keywordsv[i], pathfile, line);

				token = strtok(NULL, " ");
			}
			free(token);
			token = NULL;
		}
		line++;
	}
	error:
		if (fp)
			fclose (fp);
		if (token)
			free (token);
}

void logfind(char **keywordsv, int keywordsc, bool operator)
{
	FILE *fp = NULL;
	char readline[MAX_LINE], *home;
	char invalid_character[3] = "\r\n";
	int out;
	glob_t ret_glob;

	home = getenv("HOME");
	fp = fopen(strcat(home, "/.logfind"), "r");
	check (fp, "Failed to open $HOME/.logfind");
	while (fgets(readline, MAX_LINE, fp))
	{
		/*Remove all invalid characters (\r or \n) from the string to be used on glob,
		 *because it take them in consider in a search */
		for (int i = 0; i < strlen(invalid_character); i++)
		{
			removeinvalidchar(readline, invalid_character[i]);
		}

		out = glob(readline, 0, NULL, &ret_glob);
		debug ("Return glob %d of pattern file %s and found %zu file(s)\n", out, readline, ret_glob.gl_pathc);

		if (!out)
		{
			for (int i = 0; i < ret_glob.gl_pathc; i++)
			{
				if (operator == OR)
					searchOR (ret_glob.gl_pathv[i], keywordsv, keywordsc);
				else if (operator == AND)
					searchAND (ret_glob.gl_pathv[i], keywordsv, keywordsc);
			}
		}
	}

error:
	if (fp)
		fclose(fp);
	globfree(&ret_glob);
}
