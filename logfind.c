#include <stdio.h>
#include <stdbool.h>
#include <glob.h>
#include <string.h>
#include <stdlib.h>
#include "dbg.h"

#define MAX_LINE 1024

void removeinvalidchar (char *str, char invchar)
{
	char *pos = strchr( str, invchar );
	if (pos)
	{
		debug("Found a invalid character in %s at %ld\n", str, (pos - str) );
		*pos = '\0';
	}
}

void logfind (char **keywords, int keywordsc, bool operator)
{
	FILE *flog=NULL, *fpattern=NULL;
	char lineflog[MAX_LINE], linefpattern[MAX_LINE], *token, *home;
	char invalid_character[3] = "\r\n";
	int out;
	glob_t ret_glob;

	home = getenv ("HOME");
	flog = fopen (strcat(home, "/.logfind"), "r");
	check (flog, "Failed to open $HOME/.logfind");
	while (fgets (lineflog, MAX_LINE, flog))
	{
		/*Remove all invalid characters (\r or \n) from the string to be used on glob,
		 *because it take them in consider in a search */
		for (int i=0; i < strlen (invalid_character); i++)
		{
			removeinvalidchar (lineflog, invalid_character[i]);
		}

		out = glob (lineflog, 0, NULL, &ret_glob);
		debug("Return glob %d of pattern file %s and found %zu file(s)\n", out, lineflog, ret_glob.gl_pathc);
		free (token);

		if (!out)
		{
			for (int i=0; i < ret_glob.gl_pathc; i++)
			{
				debug ("Search on this file %s\n", ret_glob.gl_pathv[i]);
				fpattern = fopen (ret_glob.gl_pathv[i], "r");
				check (fpattern, "Failed to open %s\n", ret_glob.gl_pathv[i]);
				int line = 1;
				while( fgets (linefpattern, MAX_LINE, fpattern) )
				{
					for (int j=0; j < keywordsc; j++ )
					{
						token = strtok (linefpattern, " ");
						while (token)
						{
							debug("%s\n", token);
							if ( !strcmp ( token, keywords[j] ) )
								printf ("Found a keyword %s at %s, line %d!\n", keywords[j], ret_glob.gl_pathv[i], line );

							token = strtok (NULL, " ");
						}
						free (token);
					}
					line++;
				}
			}
		}
		globfree (&ret_glob);
	}

	error:
		if (flog)
			fclose (flog);
		if (fpattern)
			fclose (fpattern);
}
