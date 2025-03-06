#include <stdio.h>
#include <stdbool.h>

void logfind (char **keywords, int keywordsc, bool operator)
{
	for (int i=0; i < keywordsc; i++)
	{
		printf ("%s\n", keywords[i]);
	}
}
