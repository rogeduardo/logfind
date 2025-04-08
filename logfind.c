#include <stdio.h>
#include <stdbool.h>
#include <glob.h>
#include <string.h>
#include <stdlib.h>
#include "dbg.h"
#include "macro.h"

/**
 * Store keyword, line (number) and the content line where keyword was found
 */
typedef struct keyword_line
{
	char *keyword;
	char *full_line;
	int line;
} keyword_line;

/**
 * Do the and search on a given file
 * pathfile - Complete path file
 * keywordsv - keyword value string array
 * keywordsc - keyword value count
 */
void searchAND(char *pathfile, char **keywordsv, int keywordsc)
{
	FILE *fp = NULL;
	char readline[MAX_LINE];
	// searchctl is the control to known if all keywords was found
	unsigned int searchctl[MAX_KEYWORDS], size_kl = 0;
	// Store the position where keywords was found
	keyword_line *kl = NULL;

	debug("Search on this file %s\n", pathfile);
	fp = fopen(pathfile, "r");
	check(fp, "Failed to open %s\n", pathfile);
	int line = 1;

	memset(&searchctl, 0, sizeof(int) * MAX_KEYWORDS);
	while (fgets(readline, MAX_LINE, fp))
	{
		for (int i = 0; i < keywordsc; i++)
		{

			debug("*%s* in *%s*\n", keywordsv[i], readline);
			if (strstr(readline, keywordsv[i]))
			{
				size_kl++;
				kl = (keyword_line *)realloc(kl, sizeof(keyword_line) * size_kl);
				check_mem(kl);
				kl[size_kl - 1].keyword = keywordsv[i];
				kl[size_kl - 1].full_line = strdup(readline);
				kl[size_kl - 1].line = line;
				searchctl[i]++;
			}
		}
		line++;
	}

	for (int i = 0; i < keywordsc; i++)
	{
		// If any keyword was not found on the file, dont print nothing.
		if (!searchctl[i])
			goto error;
	}

	for (int i = 0; i < size_kl; i++)
	{
		printf("%s at %s, %d : %s\n", kl[i].keyword, pathfile, kl[i].line, kl[i].full_line);
		free(kl[i].full_line);
		kl[i].full_line = NULL;
	}

error:
	if (fp)
		fclose(fp);
	if (kl)
	{
		for (int i = 0; i < size_kl; i++)
		{
			if (kl[i].full_line)
				free(kl[i].full_line);
		}
		free(kl);
	}
}

/**
 * Do the or search on a given file
 * pathfile - Complete path file
 * keywordsv - keyword value string array
 * keywordsc - keyword value count
 */
void searchOR(char *pathfile, char **keywordsv, int keywordsc)
{
	FILE *fp = NULL;
	char readline[MAX_LINE];

	debug("Search on this file %s\n", pathfile);
	fp = fopen(pathfile, "r");
	check(fp, "Failed to open %s\n", pathfile);
	int line = 1;

	while (fgets(readline, MAX_LINE, fp))
	{
		for (int i = 0; i < keywordsc; i++)
		{
			debug("*%s* in *%s*\n", keywordsv[i], readline);
			if (strstr(readline, keywordsv[i]))
				printf("%s at %s, %d : %s\n", keywordsv[i], pathfile, line, readline);
		}
		line++;
	}
error:
	if (fp)
		fclose(fp);
}

/**
 * Do the and search on a given file
 * keywordsv - keyword value string array
 * keywordsc - keyword value count
 * operator - OR or AND search to be performed on files
 */
void logfind(char **keywordsv, int keywordsc, bool operator)
{
	FILE *fp = NULL;
	char readline[MAX_LINE], *home;
	int rc;
	glob_t ret_glob;

	home = getenv("HOME");
	fp = fopen(strcat(home, "/.logfind"), "r");
	check(fp, "Failed to open $HOME/.logfind");
	while (fgets(readline, MAX_LINE, fp))
	{
		/*Remove invalid characters (\r or \n) from the string to be used on glob,
		 *because it take them in consider in a search */
		readline[ strlen(readline) - 1 ] = '\0';

		rc = glob(readline, 0, NULL, &ret_glob);
		debug("Return glob %d of pattern file %s and found %zu file(s)\n", rc, readline, ret_glob.gl_pathc);
		check(rc == 0, "Error while globbing");

		for (int i = 0; i < ret_glob.gl_pathc; i++)
		{
			if (operator== OR)
				searchOR(ret_glob.gl_pathv[i], keywordsv, keywordsc);
			else if (operator== AND)
				searchAND(ret_glob.gl_pathv[i], keywordsv, keywordsc);
		}
		globfree(&ret_glob);
	}

error:
	if (fp)
		fclose(fp);
	globfree(&ret_glob);
}
