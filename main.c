/* This tool is a specialized version fo another tool called grep, but designed for log files on a system.
 * The idea is that I can type:
 *
 *	logfind rogerio
 *
 * And, it will search all the common places that log files are stored, and print out every file that has
 * the word "rogerio" in it.
 * The logfind tool have these basic features:
 *    1. This tool takes any sequence of words and assumes I mean "and" for them. So $logfind rogerio
 *       smart guy will find all files that have rogerio and smart and guy in them.
 *    2. It takes an optional argument of -o if the parameters are ment to be or logic.
 *    3. It loads the list of allowed log files from ~/.logfind
 *    4. The list of file names can be anything that the glob function allows.
 *    5. It output the matching lines as scan.
 *
 *This project was suggested at Learn C the Hard Way: Practical Exercises on the Computational Subjects You Keep Avoiding (Like C)*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dbg.h"
#include "logfind.h"
#include "macro.h"

int main (int argc, char *argv[])
{
	//Operador will be used at the search: false means AND, true means OR.
	bool operator = AND;
	char **keywordsv;
	int keywordc = 0;

	check (argc == 1, "Correct usage: logfind [-o] keywords ...\n");

	for (int i = 1; i < argc; i++)
	{
		if (strcmp ("-o", argv[i]) == 0)
			operator = OR;
	}

	if (operator == OR)
	{
		int j = 0;
		keywordc = argc - 2;

		check (keywordc == 0, "Correct usage: logfind [-o] keywords ...\n");

		check (keywordc < MAX_KEYWORDS, "The maximum keywords to be search is %d", MAX_KEYWORDS);

		debug ("OR operator");
		keywordsv = (char **) calloc ((argc - 2), sizeof (char *));
		//Get only keywords not program name and the -o parameter
		for (int i = 1; i < argc; i++)
		{
			if (strcmp("-o", argv[i]) != 0)
				keywordsv[j++] = argv[i];
		}
		check_mem (keywordsv);
		logfind ( keywordsv, keywordc, operator );
	}
	else
	{
		debug ("AND operator");
		logfind ( (&argv[0] + 1), (argc-1), operator );
	}
	error:
		if (keywordsv)
			free (keywordsv);

	return 0;
}
