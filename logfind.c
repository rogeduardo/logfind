#include <stdio.h>
#include <stdbool.h>
#include <glob.h>
#include <string.h>
#include <stdlib.h>
#include "dbg.h"

#define MAX_LINE 1024

void logfind (char **keywords, int keywordsc, bool operator)
{
	FILE *flog=NULL, *fpattern=NULL;
	char lineflog[MAX_LINE], linefpattern[MAX_LINE], *token, *home;
	glob_t ret_glob;
	int out;

	home = getenv ("HOME");
	flog = fopen ( strcat(home, "/.logfind"), "r" );
	check (flog, "Failed to open $HOME/.logfind");
	while (fgets (lineflog, MAX_LINE, flog))
	{
		out = glob (lineflog, 0, NULL, &ret_glob);
		debug("Return glob %d of pattern file %s\n", out, lineflog);
		if (!out)
		{
			for (int j=0; j < ret_glob.gl_pathc; j++)
			{
				debug ("Search on this file %s\n", ret_glob.gl_pathv[j]);
				fpattern = fopen (ret_glob.gl_pathv[j], "r");
				for (int i=0; i < keywordsc; i++ )
				{
					token = strtok (keywords[i], " ");
					while (!token)
					{
						printf ("%s\n", token);
						token = strtok (NULL, " ");
					}
					free (token);
				}
			}
		}
	}

	error:
		if (flog)
			fclose (flog);
		if (fpattern)
			fclose (fpattern);
}
